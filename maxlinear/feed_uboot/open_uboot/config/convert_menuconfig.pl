#!/usr/bin/perl
# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

use strict;
my $PATH = $ARGV[0];
($PATH and -d $PATH) or die 'invalid path';
my $DEFCONFIG = $ARGV[1];
($DEFCONFIG and -f $DEFCONFIG) or die 'invalid config file';

my %config;
my $counter = 0;

open CONFIG, $DEFCONFIG or die 'cannot open config file';
while (<CONFIG>) {
	/^CONFIG_([\w_]+)=([ym])/ and $config{$1} = $2;
	/^CONFIG_([\w_]+)=(\d+)/ and $config{$1} = $2;
	/^CONFIG_([\w_]+)=(".+")/ and $config{$1} = $2;
}
close CONFIG;

open FIND, "find \"$PATH\" -name \"Kconfig.*\" -o -name \"Kconfig\" |";
while (<FIND>) {
	chomp;
	my $input = $_;
	s/^$PATH\///g;
	s/sysdeps\/linux\///g;
	my $output = $_;
	print STDERR "$input => $output\n";
	$output =~ /^(.+)\/[^\/]+$/ and system("mkdir -p $1");
	
	open INPUT, $input;
	open OUTPUT, ">$output";
	my ($cur, $default_set, $line, $condition);
	while ($line = <INPUT>) {
		next if $line =~ /^\s*mainmenu/;

		# FIXME: make this dynamic
		$line =~ s/default FEATURE_BUFFERS_USE_MALLOC/default FEATURE_BUFFERS_GO_ON_STACK/;
		$line =~ s/default FEATURE_SH_IS_NONE/default FEATURE_SH_IS_ASH/;

		if ($line =~ /^\s*config\s*([\w_]+)/) {
			$cur = $1;
			undef $default_set;
		}

		# this is to remove the UBOOTVERSION from the toplevel UGW Kconfig
		if (($line =~ m/UBOOTVERSION/) || $counter > 0) {
			$counter += 1;
			if ($counter > 3) {
				$counter = 0;
			} else {
				next
			}
		}
		
		if ($line =~ /^\s*(menu|choice|end|source)/) {
			undef $cur;
			undef $default_set;
		}
		
		# deal with the Kconfig sourcing of other Kconfig files to make relative paths
		if ($line =~ /^\s*(source)/) {				# case source with colon
			if ($line =~ /"([^"]*)"/) {				# for source with double quotes
				my $tmp = $1;						# save string
				my @pwd = split /\//, $_; 			# split Kconfig curdir
				my @cmp = split /[\/\s]/, $line; 	# split source Kconfig dir
				shift @cmp;							# gets rid of 'source'
				my $depth = @pwd - 1;				# get array size (or dir depth)
				$cmp[0] =~ s/^"//;					# remove leading open double qoute
				$tmp =~ s/\r//g;					# remove cr
				my $retval = need_rel_path(\@pwd, \@cmp);
				if ($retval != 1) {					# check if need relative path
					my $newstring;
					my @newarray = @cmp;			# store array to be edited, remove leading dir names, keep trailing ones
					foreach (my $i = 0; $i < @pwd; $i++) {
						if ($cmp[$i] eq $pwd[$i]) {
							shift @newarray;		# remove trailing dir names, e.g arch/mips/mach-lantiq/Kconfig -> mach-lantiq/Kconfig
						}
					}
					foreach my $i (@newarray) {		# since we edit in array, add back the '/'
						$newstring .= $i."/";
					}
					$newstring =~ s/\/$//g;			# remove unwanted slash at the end
					$newstring =~ s/\"$//g;			# remove unwanted double quote at the end
					($line) = "source \"" . $newstring. "\"\n";

				} 
				else {								# else we need to include relative path
					my $leading_char = "..\/" x $depth;
					($line) = "source \"" . $leading_char.$tmp. "\"\n";
				}
			} else {									# case source without colon
				my @pwd = split /\//, $_; 				# split Kconfig curdir
				my $tmpline = $line;
				$tmpline =~ s/^\s+//;					# remove leading spaces
				$tmpline =~ s/^\t//;					# remove leading tabs
				$tmpline =~ s/\r//g;					# remove cr
				chomp $tmpline;
				my @cmp = split /[\/\s+]/, $tmpline;	# split source Kconfig dir
				shift @cmp;								# gets rid of 'source'
				my $depth = @pwd - 1;					# get array size (or dir depth)
				my $retval = need_rel_path(\@pwd, \@cmp);
				if ($retval != 1) {						# check if need relative path
					my $newstring;
					my @newarray = @cmp;
					foreach (my $i = 0; $i < @pwd; $i++) {
						if ($cmp[$i] eq $pwd[$i]) {
							shift @newarray;
						}
					}
					foreach my $i (@newarray) {
						$newstring .= $i."/";
					}
					$newstring =~ s/\/$//g;
					$newstring =~ s/\"$//g;
					($line) = "source \"" . $newstring. "\"\n";
				} 
				else {									# else we need to include relative path
					my $leading_char = "..\/" x $depth;
					$tmpline =~ s/^\s*(source)//;		# remove 'source' from the string
					$tmpline =~ s/^\s+//;				# remove leading spaces
					($line) = "source \"" . $leading_char.$tmpline. "\"\n";
				}
			}
		}

		# fixes the stragling if (*.) in the Kconfig
		$line =~ s/^(\s*(default[^\r\n]+if)\s+\!?)([0-9A-Z_])/$1UBOOT_CONFIG_$3/g;
		$line =~ s/^(\s*(default[^\r\n]+if)\s+\!?)([\(])/$1(UBOOT_CONFIG_/g;
		$line =~ s/^(\s*(prompt "[^"]+" if|config|depends|depends on|select|menuconfig|default|default[\s\S]+if)\s+\!?)([A-Z_])/$1UBOOT_CONFIG_$3/g;
		$line =~ s/(( \|\| | \&\& | \( )!?)([A-Z_])/$1UBOOT_CONFIG_$3/g;
		# fixes all the && in Kconifg
		$line =~ s/((&& ) !?)([A-Z_])/$1UBOOT_CONFIG_$3/g;
		$line =~ s/^if\s*(?!LANTIQ_UBOOT)([\w_]+)/if UBOOT_CONFIG_$1/g;
		if ($line =~ /(LANTIQ_UBOOT_)([\w]+)/) {
			# for LANTIQ_UBOOT_* with if statement at the start of the line
			$line =~ s/^if\s*(LANTIQ_UBOOT_)([\w]+)/if UBOOT_CONFIG_$1$2/g;
			# for LANTIQ_UBOOT_* as a default selection
			$line =~ s/[^#]if\s*(LANTIQ_UBOOT_)([\w]+)/if UBOOT_CONFIG_$1$2/g;
		}

		# removes the extra UBOOT_CONFIG_ added 
		$line =~ s/(\s*(UBOOT_CONFIG_UBOOT_CONFIG)+)/ UBOOT_CONFIG/g;
		# fixes spaces after brackets
		$line =~ s/(\s*(UBOOT_CONFIG_ )+)/UBOOT_CONFIG_/g;
		$line =~ s/\s*((UBOOT_CONFIG_[\s!])+)/!UBOOT_CONFIG_/g;
		
		if ($cur) {
			($cur eq 'LFS') and do {
				$line =~ s/^(\s*(bool|tristate|string))\s*".+"$/$1/;
			};
			if ($line =~ /^\s*default/) {
				my $c;
				$default_set = 1;
				$c = $config{$cur} or $c = 'n';

				#$line =~ s/^(\s*default\s*)(\w+|"[^"]*")(.*)/$1$c$3/;
			}
		}
		$line =~ s/\r//g;
		print OUTPUT $line;
	}
	close OUTPUT;
	close INPUT;
	
}
close FIND;

sub need_rel_path {
    my ($curdir_ref, $source_ref) = @_;	# from scalar
    my @curdir = @{ $curdir_ref };		# get first array ref
    my @source = @{ $source_ref };		# get second array ref
    my $retval = 0;

    #print "curdir: @curdir\n";
    #print "source: @source\n";

    foreach (my $i = 0; $i < @curdir; $i++)  {
        if ($curdir[$i] ne $source[$i]) {		# compare each directory string
            if ($curdir[$i] ne "Kconfig") {		# if the string does not match & curdir string is not Kconfig
                $retval = 1;					# means the directory has to be a relative path
            }									# e.g arch/mips/mach-lantiq/Kconfig & board/lantiq/Kconfig
        }
    }

    return $retval;	
}

