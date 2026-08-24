Skip test on nec-wx3000hp until LCM-579 is fixed:

  $ [ "$DUT_BOARD" = "nec-wx3000hp" ] && exit 80
  [1]

Create R alias:

  $ alias R="${CRAM_REMOTE_COMMAND:-}"

Check that Sandbox is not configured properly:

  $ R "ubus -S call Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1 _get"
  [4]

  $ R "ubus -S call Cthulhu.Config _get | jsonfilter -e @[*].DhcpCommand"
  

Configure Sandbox:

  $ cat > /tmp/run-sandbox <<EOF
  > ubus-cli Cthulhu.Config.DhcpCommand=\"udhcpc -r 192.168.1.200 -i\"
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Type="Veth"
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.+
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1.Bridge="br-lan"
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1.Interface="eth0"
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1.EnableDhcp=1
  > ubus-cli Cthulhu.Sandbox.Instances.1.NetworkNS.Enable=1
  > ubus-cli "Cthulhu.Sandbox.start(SandboxId=\"generic\")"
  > EOF
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run-sandbox)'" > /dev/null
  $ sleep 10

Check that Sandbox was configured properly:

  $ R "ubus -S call Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1 _get"
  {"Cthulhu.Sandbox.Instances.1.NetworkNS.Interfaces.1.":{"EnableDhcp":true,"Interface":"eth0","Bridge":"br-lan"}}

  $ R "ubus -S call Cthulhu.Config _get | jsonfilter -e @[*].DhcpCommand"
  udhcpc -r 192.168.1.200 -i

Install testing prplOS container v1:

  $ cat > /tmp/run-container <<EOF
  > ubus-cli "SoftwareModules.InstallDU(URL=\"docker://registry.gitlab.com/prpl-foundation/prplos/prplos/prplos-testing-container-intel_mips-xrx500:v1\", UUID=\"prplos-testing\", ExecutionEnvRef=\"generic\")"
  > EOF
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run-container)'" > /dev/null

Check that prplOS container v1 is running:

  $ sleep 50

  $ R "ubus -S call Cthulhu.Container.Instances.1 _get | jsonfilter -e @[*].Status -e @[*].Bundle -e @[*].BundleVersion -e @[*].ContainerId -e @[*].Alias | sort"
  Running
  cpe-prplos-testing
  prplos-testing
  prplos/prplos-testing-container-intel_mips-xrx500
  v1

  $ R "ssh -y root@192.168.1.200 'cat /etc/container-version' 2> /dev/null"
  1

Update to prplOS container v2:

  $ cat > /tmp/run-container <<EOF
  > ubus-cli "SoftwareModules.DeploymentUnit.cpe-prplos-testing.Update(URL=\"docker://registry.gitlab.com/prpl-foundation/prplos/prplos/prplos-testing-container-intel_mips-xrx500:v2\")"
  > EOF
  $ script --command "ssh -t root@$TARGET_LAN_IP '$(cat /tmp/run-container)'" > /dev/null

Check that prplOS container v2 is running:

  $ sleep 40

  $ R "ubus -S call Cthulhu.Container.Instances.2 _get | jsonfilter -e @[*].Status -e @[*].Bundle -e @[*].BundleVersion -e @[*].ContainerId -e @[*].Alias | sort"
  Running
  cpe-prplos-testing
  prplos-testing
  prplos/prplos-testing-container-intel_mips-xrx500
  v2

  $ R "ssh -y root@192.168.1.200 'cat /etc/container-version' 2> /dev/null"
  2

Uninstall prplOS testing container:

  $ script --command "ssh -t root@$TARGET_LAN_IP 'ubus-cli SoftwareModules.DeploymentUnit.cpe-prplos-testing.Uninstall\(\)'" > /dev/null;  sleep 5

Check that prplOS container is not running:

  $ R "ubus -S call Cthulhu.Container.Instances.2 _get"
  [4]

Check that Rlyeh has no container images:

  $ R "ubus -S call Rlyeh.Images _get"
  {"Rlyeh.Images.":{}}

Check that container image is gone from the filesystem as well:

  $ R "ls -al /lcm_data/rlyeh/images/prplos"
  ls: /lcm_data/rlyeh/images/prplos: No such file or directory
  [1]
