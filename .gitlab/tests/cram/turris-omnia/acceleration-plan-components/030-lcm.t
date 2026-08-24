Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check Cthulhu.Sandbox datamodel:

  $ R "ubus -S call Cthulhu.Sandbox.Instances.1 _get | jsonfilter -e @[*].Status -e @[*].Enable -e @[*].SandboxId | sort"
  Up
  generic
  true

Check Cthulhu.Config datamodel:

  $ R "ubus -S call Cthulhu.Config _get | jsonfilter -e @[*].UseOverlayFS -e @[*].DefaultBackend -e @[*].ImageLocation | sort"
  /lcm_data/rlyeh/images
  /usr/lib/cthulhu-lxc/cthulhu-lxc.so
  true

Install testing prplOS container v1:

  $ cat > /tmp/run-container <<EOF
  > ubus-cli SoftwareModules.InstallDU\( \
  > URL="docker://registry.gitlab.com/prpl-foundation/prplos/prplos/prplos/lcm-test-mvebu-cortexa9:prplos-v1", \
  > UUID="0f032bd7-54bd-5b81-b14e-9441d730092f", \
  > ExecutionEnvRef="generic", \
  > NetworkConfig = { "AccessInterfaces" = [{"Reference" = "Lan"}] } \
  > \)
  > EOF
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run-container)'" > /dev/null

Check that prplOS container v1 is running:

  $ sleep 30

  $ R "ubus -S call Cthulhu.Container.Instances.1 _get | jsonfilter -e @[*].Status -e @[*].Bundle -e @[*].BundleVersion -e @[*].ContainerId -e @[*].Alias | sort"
  Running
  c879945e-d002-5775-88a8-e29bc0c641b4
  cpe-c879945e-d002-5775-88a8-e29bc0c641b4
  prpl-foundation/prplos/prplos/prplos/lcm-test-mvebu-cortexa9
  prplos-v1

  $ container_ip=$(R "ubus call DHCPv4Server.Pool.3.Client.1.IPv4Address.1 _get | jsonfilter -e @[*].IPAddress")
  $ R "ssh -y root@$container_ip 'cat /etc/container-version ; ip r' 2> /dev/null"
  1
  default via 192.168.5.1 dev lcm0 
  192.168.5.0/24 dev lcm0 scope link  src 192.168.5.* (re)

Update to prplOS container v2:

  $ cat > /tmp/run-container <<EOF
  > ubus-cli SoftwareModules.DeploymentUnit.cpe-c879945e-d002-5775-88a8-e29bc0c641b4.Update\( \
  > URL="docker://registry.gitlab.com/prpl-foundation/prplos/prplos/prplos/lcm-test-mvebu-cortexa9:prplos-v2", \
  > UUID="0f032bd7-54bd-5b81-b14e-9441d730092f", \
  > ExecutionEnvRef="generic", \
  > NetworkConfig = { "AccessInterfaces" = [{"Reference" = "Lan"}] } \
  > \)
  > EOF
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run-container)'" > /dev/null

Check that prplOS container v2 is running:

  $ sleep 30

  $ R "ubus -S call Cthulhu.Container.Instances.1 _get | jsonfilter -e @[*].Status -e @[*].Bundle -e @[*].BundleVersion -e @[*].ContainerId -e @[*].Alias | sort"
  Running
  c879945e-d002-5775-88a8-e29bc0c641b4
  cpe-c879945e-d002-5775-88a8-e29bc0c641b4
  prpl-foundation/prplos/prplos/prplos/lcm-test-mvebu-cortexa9
  prplos-v2

  $ container_ip=$(R "ubus call DHCPv4Server.Pool.3.Client.2.IPv4Address.1 _get | jsonfilter -e @[*].IPAddress")
  $ R "ssh -y root@$container_ip 'cat /etc/container-version ; ip r' 2> /dev/null"
  2
  default via 192.168.5.1 dev lcm0 
  192.168.5.0/24 dev lcm0 scope link  src 192.168.5.* (re)

Uninstall prplOS testing container:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli SoftwareModules.DeploymentUnit.cpe-c879945e-d002-5775-88a8-e29bc0c641b4.Uninstall\(\)'" > /dev/null;  sleep 5

Check that prplOS container is not running:

  $ R "ubus -S call Cthulhu.Container.Instances.1 _get"
  [4]

Check that Rlyeh has no container images:

  $ R "ubus -S call Rlyeh.Images _get"
  {"Rlyeh.Images.":{}}
  {}
  {"amxd-error-code":0}
