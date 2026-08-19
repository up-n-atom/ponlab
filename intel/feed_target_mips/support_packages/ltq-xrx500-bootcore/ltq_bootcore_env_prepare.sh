#!/usr/bin/env bash

BASEDIR="$1/build_dir/xRX500_BootCore"
CURDIR="$1"
PKG_BUILD_DIR="$2"

error() {
	echo "$0: ERROR: $*"
	exit 1
}

symlink_tree() {
	local FILES="
		config
		Config.in
		dl
		feeds
		feeds.conf.default
		include
		LICENSE
		Makefile
		package
		README
		rules.mk
		scripts
		target
		toolchain
		tools
		version
		build_dir/host
		staging_dir/host
		"

	local OPTIONAL_FILES="
		.git
		feeds.conf
		other_config_path
		ugw_version
		docs
		"

	# if already existing silently return
	[ -e "$1" ] && return 0

	pushd $CURDIR

	mkdir -p dl "$1/build_dir" "$1/staging_dir"
	for file in $FILES; do
		[ -e "$CURDIR/$file" ] || error "ERROR: $file does not exist in the current tree" >&2
		#echo ln -sn "$CURDIR/$file" "$1/$file"
		ln -sn "$CURDIR/$file" "$1/$file"
	done
	for file in $OPTIONAL_FILES; do
		[ -e "$CURDIR/$file" ] && ln -sn "$CURDIR/$file" "$1/$file"
	done

	popd

	return 0
}


fix_config_options() {
	local TOS_SIZE=`grep "UBOOT_CONFIG_TOS.*=y$" $CURDIR/.config`
	local DDR_SIZE=`grep "UBOOT_CONFIG_DDR.*=y$" $CURDIR/.config`

	sed -E '/(CONFIG_NEED_TOOLCHAIN|CONFIG_TOOLCHAIN|CONFIG_EXTERNAL_TOOLCHAIN|CONFIG_TARGET_NAME|_ROOT_DIR=|CONFIG_BINUTILS_USE|CONFIG_GCC_USE|CONFIG_LIBC_USE|CONFIG_GCC_DEFAULT|CONFIG_GDB)/d' -i $BASEDIR/.config

	local TOOLCHAIN_SET="`grep 'CONFIG_EXTERNAL_TOOLCHAIN=y' $CURDIR/.config`";
	if [ -n "$TOOLCHAIN_SET" -a "$TOOLCHAIN_SET" = "CONFIG_EXTERNAL_TOOLCHAIN=y" ]; then
		echo "$TOOLCHAIN_SET" >> $BASEDIR/.config
		local VAR2="`grep -E 'CONFIG_TOOLCHAIN_ROOT|CONFIG_TARGET_NAME|CONFIG_TOOLCHAIN_PREFIX' $CURDIR/.config`"
		echo "$VAR2" >> $BASEDIR/.config
	else
		echo "CONFIG_NEED_TOOLCHAIN=y" >> $BASEDIR/.config
		echo "# CONFIG_EXTERNAL_TOOLCHAIN is not set" >> $BASEDIR/.config
		local VAR2="`grep CONFIG_TOOLCHAINOPTS $CURDIR/.config`"
		echo "$VAR2" >> $BASEDIR/.config
		if [ -n "$VAR2" -a "$VAR2" = "CONFIG_TOOLCHAINOPTS=y" ]; then
			local VAR2="`grep -E 'CONFIG_BINUTILS_USE|CONFIG_GCC_USE|CONFIG_LIBC_USE|CONFIG_GCC_DEFAULT|CONFIG_GDB' $CURDIR/.config`"
			echo "$VAR2" >> $BASEDIR/.config
		fi
	fi

	# override kernel location with expanded strings from parent
	local VAR=`grep CONFIG_EXTERNAL_KERNEL_TREE $BASEDIR/.config |cut -d '=' -f2|sed "s/\"//g"`
	if [ "$VAR" != "" ]; then
		local VAR2=${VAR/\$\(TOPDIR\)/$CURDIR}
		sed -i '/CONFIG_EXTERNAL_KERNEL_TREE/d' $BASEDIR/.config
		echo CONFIG_EXTERNAL_KERNEL_TREE=\"$VAR2\" >> $BASEDIR/.config
	fi

	local VAR=`grep CONFIG_KERNEL_GIT_CLONE_URI $BASEDIR/.config |cut -d '=' -f2|sed "s/\"//g"`
	if [ "$VAR" != "" ]; then
		local VAR2=${VAR/\$\(TOPDIR\)/$CURDIR}
		sed -i '/CONFIG_KERNEL_GIT_CLONE_URI/d' $BASEDIR/.config
		echo CONFIG_KERNEL_GIT_CLONE_URI=\"$VAR2\" >> $BASEDIR/.config
	fi

	local VAR=`grep 'CONFIG_INTEL_MIPS_KEYS_PROVISION=y' $CURDIR/.config`;
	if [ "$VAR" != "" ]; then
		# remove existing config
		sed -i '/CONFIG_INTEL_MIPS_/d' $BASEDIR/.config
		# copy other secboot options
		echo "CONFIG_INTEL_MIPS_KEYS_PROVISION=y" >> $BASEDIR/.config
		awk '/^(#[[:space:]]+)?CONFIG_XRX500_BOOTCORE/{sub("CONFIG_XRX500_BOOTCORE_","CONFIG_INTEL_MIPS_");print}' $CURDIR/.config >> $BASEDIR/.config
	fi

#
# To be reviewed, how the UBOOT configs should be handled
#
	if [ "$TOS_SIZE" = "CONFIG_UBOOT_CONFIG_TOS_16M=y" ];then
		sed -i s/"[#].*CONFIG_.*-tos-size-.*"/"CONFIG_PACKAGE_kmod-lantiq-tos-size-16=y"/   $BASEDIR/.config
	fi
	if [ "$TOS_SIZE" = "CONFIG_UBOOT_CONFIG_TOS_32M=y" ];then
		sed -i s/"[#].*CONFIG_.*-tos-size-.*"/"CONFIG_PACKAGE_kmod-lantiq-tos-size-32=y"/   $BASEDIR/.config
	fi
	if [ "$TOS_SIZE" = "CONFIG_UBOOT_CONFIG_TOS_64M=y" ];then
		sed -i s/"[#].*CONFIG_.*-tos-size-.*"/"CONFIG_PACKAGE_kmod-lantiq-tos-size-64=y"/   $BASEDIR/.config
	fi
	if [ "$TOS_SIZE" = "CONFIG_UBOOT_CONFIG_TOS_128M=y" ];then
		sed -i s/"[#].*CONFIG_.*-tos-size-.*"/"CONFIG_PACKAGE_kmod-lantiq-tos-size-128=y"/   $BASEDIR/.config
	fi
	sed -i s/"CONFIG_UBOOT_CONFIG_TOS.*=y$"/"$TOS_SIZE"/   $BASEDIR/.config
	sed -i s/"CONFIG_UBOOT_CONFIG_DDR.*=y$"/"$DDR_SIZE"/  $BASEDIR/.config
}

env_link_config() {
	local MODEL=`grep CONFIG_XRX500_BOOTCORE_MODEL $CURDIR/.config|cut -d '=' -f2|sed "s/\"//g"`
	local CONFIG_PATH
	[ -e "$CURDIR/other_config_path" ] && CONFIG_PATH=`cat $CURDIR/other_config_path`
	[ -z "$CONFIG_PATH" ] && CONFIG_PATH="$CURDIR/../config"
	local NAME=`find $CONFIG_PATH/ -name $MODEL -type d -print0`
	[ -f "$NAME/.config" ] || error "$NAME or $BASEDIR/invalid environment directory"

	echo "Apply config $NAME"
	rm -f "$BASEDIR/.config"
	rm -Rf "$BASEDIR/files"
	cp "$NAME/.config" "$BASEDIR/.config" || error "Failed to copy environment configuration"
	fix_config_options

	OPENWRT_BUILD=0
	export OPENWRT_BUILD
	unset LD_LIBRARY_PATH

	pushd $BASEDIR
	make -s staging_dir/host/.prereq-build defconfig
	popd

	[ -d "$NAME/files" ] && {
		cp -Rf "$NAME/files" "$BASEDIR/files" || error "Failed to copy environment files"
		chmod -R u+wr "$BASEDIR/files" || error "Failed to change the protection"
	}
	echo $NAME > "$BASEDIR/active_config"
	if [ -d $BASEDIR ]; then
		rm -Rf $PKG_BUILD_DIR
		echo "Preparing Custom Source Directory link: $BASEDIR"
		ln -snf $BASEDIR $PKG_BUILD_DIR
	else
		error "Custom Source Directory $BASEDIR is invalid"
	fi
}

set -e # fail if any commands fails

symlink_tree $BASEDIR
env_link_config
