Skip test on nec-wx3000hp until LCM-579 is fixed:

  $ [ "$DUT_BOARD" = "nec-wx3000hp" ] && exit 80
  [1]

Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that random LXC binaries work:

  $ R lxc-info
  lxc-info: No container name specified
  [1]

  $ R lxc-device
  lxc-device: No container name specified
  [1]

Check Cthulhu.Config datamodel:

  $ R "ubus -S call Cthulhu.Config _get"
  {"Cthulhu.Config.":{"PluginLocation":"/usr/lib/amx/cthulhu/plugins","ImageLocation":"/lcm_data/rlyeh/images","UseOverlayFS":true,"UseBundles":false,"OnboardingLocation":"/etc/amx/cthulhu/onboard","StorageLocation":"/lcm_data/cthulhu","OnboardingFile":"/lcm_data/cthulhu/cthulhu_onboarded","DefaultBackend":"/usr/lib/cthulhu-lxc/cthulhu-lxc.so","BundleLocation":"/lcm_data/celephais/bundles","BlobLocation":"/lcm_data/rlyeh/blobs"}}
  {}
  {"amxd-error-code":0}

Check Rlyeh datamodel:

  $ R "ubus -S call Rlyeh _get"
  {"Rlyeh.":{"ImageLocation":"/lcm_data/rlyeh/images","ROImageLocation":"/usr/rlyeh/images","ROStorageLocation":"/usr/rlyeh/blobs","CertificateVerification":true,"SignatureVerification":true,"RemainingDiskSpaceBytes":1000001,"StorageLocation":"/lcm_data/rlyeh/blobs","OnboardingFile":"/lcm_data/rlyeh_onboarded"}}
  {}
  {"amxd-error-code":0}

Check SoftwareModules datamodel:

  $ R "ubus -S call SoftwareModules _get"
  {"SoftwareModules.":{"ExecutionUnitNumberOfEntries":0,"ExecEnvNumberOfEntries":1,"DeploymentUnitNumberOfEntries":0}}
  {}
  {"amxd-error-code":0}

Check Timingila datamodel:

  $ R "ubus -S call Timingila _get"
  {"Timingila.":{"RmAfterUninstall":true,"ContainerPluginPath":"/usr/lib/timingila-cthulhu/timingila-cthulhu.so","PackagerPluginPath":"/usr/lib/timingila-rlyeh/timingila-rlyeh.so","version":"alpha"}}
  {}
  {"amxd-error-code":0}

Check that Rlyeh has no container images:

  $ R "ubus -S call Rlyeh.Images _get"
  {"Rlyeh.Images.":{}}
  {}
  {"amxd-error-code":0}

Check that registry.gitlab.com is accessible:

  $ R "curl --silent --show-error --connect-timeout 60 https://registry.gitlab.com"

Check that Rlyeh can download testing container:

  $ R "ubus -S call Rlyeh pull '{\"URI\":\"docker://registry.gitlab.com/prpl-foundation/prplos/prplos/prplos-testing-container-intel_mips-xrx500:v1\",\"DUID\":\"0f032bd7-54bd-5b81-b14e-9441d730092f\"}'"
  {"retval":""}
  {}
  {"amxd-error-code":0}

  $ R "ubus -t 60 wait_for Rlyeh.Images.1"

Check that Rlyeh has downloaded the testing container:

  $ R "ubus -S call Rlyeh.Images _get | jsonfilter -e @[*].Name -e @[*].Status | sort"
  Downloaded
  prplos/prplos/prplos-testing-container-intel_mips-xrx500

Remove testing container:

  $ R "ubus -S call Rlyeh remove '{\"DUID\":\"0f032bd7-54bd-5b81-b14e-9441d730092f\",\"Version\":\"v1\"}'"; sleep 5
  {"retval":""}
  {}
  {"amxd-error-code":0}

  $ R "ubus -S call Rlyeh.Images.1 _get | jsonfilter -e @[*].MarkForRemoval"
  true

  $ R "ubus -S call Rlyeh gc"
  {"retval":""}
  {}
  {"amxd-error-code":0}

Check that Rlyeh has no container images:

  $ R "ubus -S call Rlyeh.Images _get"
  {"Rlyeh.Images.":{}}
  {}
  {"amxd-error-code":0}

Check that testing image is gone from the filesystem as well:

  $ R "ls -al /lcm_data/rlyeh/images/prplos/prplos-testing-container-intel_mips-xrx500"
  ls: /lcm_data/rlyeh/images/prplos/prplos-testing-container-intel_mips-xrx500: No such file or directory
  [1]
