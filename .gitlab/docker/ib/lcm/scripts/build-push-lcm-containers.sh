#!/bin/bash

set -e

CI_DESIGNATED_BRANCH=${CI_DESIGNATED_BRANCH:-prplos}
CI_COMMIT_REF_SLUG=${CI_COMMIT_REF_SLUG:-ynezz-lcm-containers}
CI_REGISTRY_IMAGE=${CI_REGISTRY_IMAGE:-registry.gitlab.com/prpl-foundation/prplos/prplos}

CI_LCM_CONTAINER_NAME=${IMAGE_NAME:-lcm-test-mvebu-cortexa9}
CI_IB_ROOTFS_PATH=${CI_IB_ROOTFS_PATH:-bin/targets/mvebu/cortexa9}
CI_IB_CONTAINER=${CI_IB_CONTAINER:-ib-mvebu-cortexa9}
CI_IB_PROFILE=${CI_IB_PROFILE:-cznic_turris-omnia}
CI_IB_PACKAGES="${CI_IB_PACKAGES:-"-ath10k-firmware-qca988x-ct -ca-bundle \
  -dnsmasq -e2fsprogs -firewall4 -fstools -iwinfo -kernel -kmod-ath10k-ct \
  -kmod-ath9k -kmod-fs-vfat -kmod-gpio-button-hotplug \
  -kmod-i2c-mux-pca954x -kmod-leds-turris-omnia -kmod-nft-offload \
  -kmod-nls-cp437 -kmod-nls-iso8859-1 libc libgcc -libustream-wolfssl \
  -logd -mkf2fs -mtd -nftables -odhcp6c -odhcpd-ipv6only -opkg \
  -partx-utils -uboot-envtools uci -uclient-fetch \
  -urandom-seed -urngd -wpad-basic-mbedtls"}"
CI_IB_PACKAGES="${CI_IB_PACKAGES%"${CI_IB_PACKAGES##*[![:space:]]}"}"

build_rootfs()
{
  docker run \
    --volume "$(pwd)/bin":/home/builder/bin \
    --volume "$(pwd)/.gitlab":/home/builder/.gitlab \
    "$CI_REGISTRY_IMAGE/$CI_DESIGNATED_BRANCH/$CI_IB_CONTAINER:$CI_COMMIT_REF_SLUG" sh -c "
    cd /home/builder &&
    sudo chmod a+rwx bin &&
    for version in v1 v2; do
      echo \"Creating image $CI_IB_PROFILE \$version ...\" ; \
      make image \
        PROFILE=\"$CI_IB_PROFILE\" \
        PACKAGES=\"$CI_IB_PACKAGES\" \
        FILES=\".gitlab/docker/ib/lcm/files.container/\$version\" \
        EXTRA_IMAGE_NAME=\$version \
        CONFIG_TARGET_ROOTFS_TARGZ=y ; \
    done \
    "
}

build_push_container()
{
  local path;
  local rootfs;
  local filename;
  local rootfs_dir;
  local container_name;

  for version in v1 v2; do
    rootfs="$(echo "${CI_IB_ROOTFS_PATH}"/*"$version"*rootfs.tar.gz)"
    path="$(dirname "$rootfs")"
    filename="$(basename "$(basename "$rootfs" .tar.gz)")"
    rootfs_dir="$path/$filename"
    container_name="${CI_LCM_CONTAINER_NAME}-$version"

    [ -d "$rootfs_dir" ] && rm -fr "$rootfs_dir"
    mkdir -p "$rootfs_dir"
    tar -C "$rootfs_dir" -xf "$rootfs"

    docker buildx prune --force

    docker build \
      --tag "$container_name" \
      --file .gitlab/docker/ib/lcm/Dockerfile \
      "$rootfs_dir"
    docker tag "$container_name" "$CI_REGISTRY_IMAGE/$CI_DESIGNATED_BRANCH/$container_name:$CI_COMMIT_REF_SLUG"
    docker push "$CI_REGISTRY_IMAGE/$CI_DESIGNATED_BRANCH/$container_name:$CI_COMMIT_REF_SLUG"

    docker tag "$container_name" "$CI_REGISTRY_IMAGE/$CI_DESIGNATED_BRANCH/$CI_LCM_CONTAINER_NAME:${CI_COMMIT_REF_SLUG}-$version"
    docker push "$CI_REGISTRY_IMAGE/$CI_DESIGNATED_BRANCH/$CI_LCM_CONTAINER_NAME:${CI_COMMIT_REF_SLUG}-$version"
  done
}

build_rootfs
build_push_container
