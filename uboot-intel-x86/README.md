
## How to create new configs

- go to the target feed (target/linux/intel_x86/), subfolder of the subtarget
  (lgm) and there the subfolder "uboot_configs"

- take one of the existing config files (for a board which has probably the most settings in common) and copy it to a new file, named according your board

- go back to your main build folder ("openwrt") and delete the "tmp" folder

- select the new config in "make menuconfig" (de-select others to speedup next steps)

- execute ```make V=s package/uboot-intel-x86/configure CONFIG_TARGET=<config-name>```
  (the ```<config-name>``` is the name of your new file without the "_defconfig" part)
  - the calling of "make menuconfig" via the package has problems if the build-log is enabled, so either switch is off in your config or add "CONFIG_BUILD_LOG=n" to the previous command

- When exiting from the previous menuconfig, the config file is copied back to the "uboot_configs" folder and ready for being built

- The build can be done directly by calling ```make V=s package/uboot-intel-x86/compile```
  If it failed or the generated u-boot binary is not working as expected, go back to the configure step. Otherwise you are done.

**Your new board configuration is ready!**
