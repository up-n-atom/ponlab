#!/usr/bin/env python3

import argparse
import os
import sys
import logging
import json
import time

from labgrid import Environment, StepReporter
from labgrid.driver import ExecutionError
from labgrid.consoleloggingreporter import ConsoleLoggingReporter


class OpenWrtConsoleShell:
    def __init__(self, args, shell):
        self.args = args
        self.shell = shell

    def run(self, cmd, timeout=30):
        result = None

        try:
            logging.info(f"executing: {cmd}")
            result = self.shell.run_check(cmd, timeout=timeout)
        except ExecutionError as e:
            stdout = " ".join(e.stdout)
            stderr = " ".join(e.stderr)
            logging.error(f"failed: '{e.msg}' stdout='{stdout}' stderr='{stderr}'")
            return None, (e.msg, e.stdout, e.stderr)

        if len(result) > 0:
            logging.info("result:\n" + "\n".join(result) + "\n" + "-" * 80)

        return result

    def ubus_call(self, cmd):
        result = None

        cmd = f"ubus call {cmd}"
        try:
            logging.info(f"executing: {cmd}")
            result = self.shell.run_check(cmd)
        except ExecutionError as e:
            stdout = " ".join(e.stdout).replace("\t", "")
            stderr = " ".join(e.stderr)
            logging.error(f"failed: {e.msg} stdout='{stdout}' stderr='{stderr}'")
            return None, e.stdout[-1], (e.msg, e.stdout, e.stderr)

        result = json.loads(" ".join(result))
        logging.info(f"result: {result}")
        return result


class UbusTR181:
    def __init__(self, args, shell):
        self.args = args
        self.shell = shell

    def api_call(self, method, path, parameters=None):
        cmd = f"{path} {method}"
        if parameters:
            p = json.dumps({"parameters": parameters})
            cmd += f" '{p}'"

        return self.shell.ubus_call(cmd)

    def get(self, path, parameters=None):
        return self.api_call("_get", path, parameters)

    def set(self, path, parameters):
        return self.api_call("_set", path, parameters)

    def add(self, path, parameters):
        return self.api_call("_add", path, parameters)

    def remove(self, path, parameters=None):
        return self.api_call("_del", path, parameters)

    def call(self, path, method, parameters):
        cmd = f"{path} {method}"
        if parameters:
            p = json.dumps(parameters)
            cmd += f" '{p}'"

        return self.shell.ubus_call(cmd)


class TestbedDevice:
    def __init__(self, args):
        self.args = args
        self.shell = None
        self.env = Environment(config_file=self.args.config)
        ConsoleLoggingReporter.start(args.console_logpath)
        self.target = self.env.get_target(args.target)

    def init_shell(self):
        if self.shell:
            return

        self.shell_driver = self.target.get_driver("ShellDriver")
        self.shell = OpenWrtConsoleShell(self.args, self.shell_driver)

    def init_swconfig_glinet(self):
        self.shell.run("swconfig dev switch0 vlan 1 set vid 201")
        self.shell.run("swconfig dev switch0 vlan 1 set ports '0t 3t 4t'")

        self.shell.run("swconfig dev switch0 vlan 2 set vid 101")
        self.shell.run("swconfig dev switch0 vlan 2 set ports '0t 5t'")

        self.shell.run("swconfig dev switch0 vlan 1 show")
        self.shell.run("swconfig dev switch0 vlan 2 show")

        self.shell.run(
            """
          uci add network switch_vlan &&
          uci set network.@switch_vlan[-1]=switch_vlan &&
          uci set network.@switch_vlan[-1].device='switch0' &&
          uci set network.@switch_vlan[-1].vlan='2' &&
          uci set network.@switch_vlan[-1].vid='101' &&
          uci set network.@switch_vlan[-1].ports='0t 5t' &&
          uci set network.@switch_vlan[0].vid='201' &&
          uci set network.@switch_vlan[0].ports='3t 4t 0t' &&
          uci commit network
            """
        )

    def _init_swconfig(self):
        if self.board_name.startswith("glinet"):
            self.init_swconfig_glinet()

    def _init_wan_vlan(self):
        self.shell.run("ubus -t 60 wait_for WANManager.WAN")
        self.shell.run("ubus-cli WANManager.WAN.2.Intf.1.VlanID=101")
        self.shell.run("ubus-cli WANManager.WAN.2.Intf.2.VlanID=100")
        self.shell.run(
            'ubus call WANManager setWANMode \'{ "WANMode": "Ethernet_DHCP" }\''
        )

    def _init_lan_vlan(self):
        bridge_lan_ports = {
            "mxl,lightning mountainmxl,octopusmxl,octopus-641": "Bridging.Bridge.1.Port.4",
            "mxl,lightning mountainmxl,lgp": "Bridging.Bridge.1.Port.3",
            "prpl,haze": "Bridging.Bridge.1.Port.4",
            "cznic,turris-omnia": "Bridging.Bridge.2.Port.5",
            "EASY350 ANYWAN (GRX350) Axepoint Asurada model": "Bridging.Bridge.1.Port.5",
        }
        bridge_vlan_port = bridge_lan_ports.get(
            self.board_name,
            bridge_lan_ports.get(self.model, "Bridging.Bridge.1.Port.2"),
        )

        self.shell.run('ubus-cli Bridging.Bridge.1.Standard="802.1Q-2005"')
        self.shell.run(
            "ubus-cli Bridging.Bridge.1.VLAN.+{Alias='vlan201', Name='vlan201', VLANID=201, Enable=1}"
        )
        self.shell.run(
            f'ubus-cli {bridge_vlan_port}.AcceptableFrameTypes="AdmitOnlyVLANTagged"'
        )
        self.shell.run(f'ubus-cli {bridge_vlan_port}.PVID="201"')
        self.shell.run(f'ubus-cli {bridge_vlan_port}.Type="CustomerVLANPort"')
        self.shell.run(f"ubus-cli {bridge_vlan_port}.Enable=1")

        self.shell.run(
            f"ubus-cli Bridging.Bridge.1.VLANPort+{{Alias='LAN', Name='vlan201', Port='Device.{bridge_vlan_port}.', VLAN='Device.Bridging.Bridge.1.VLAN.1.', Enable=1}}"
        )

    def init_vlans(self):
        self.init_shell()

        system_info = self.shell.ubus_call("system board")
        if not system_info:
            logging.error("Unable to determine running DUT board!")
            return

        self.model = system_info["model"]
        self.board_name = system_info["board_name"]
        logging.info(f"Running on `{self.board_name}` board, model is `{self.model}`.")

        self.ubus_tr181 = UbusTR181(self.args, self.shell)
        self._init_wan_vlan()
        self._init_lan_vlan()
        self._init_swconfig()

        logging.info(
            "Let the system apply the new configuration, waiting 15 seconds..."
        )
        time.sleep(15)

        self.shell.run("ubus-cli Ethernet.VLANTermination.?")
        self.shell.run("(cat /proc/vlan101 || cat /proc/net/vlan/vlan101) 2> /dev/null")
        self.shell.run("(cat /proc/vlan201 || cat /proc/net/vlan/vlan201) 2> /dev/null")
        self.shell.run("ip address show vlan101")
        self.shell.run("ip address show vlan201")
        self.shell.run("brctl show")
        self.shell.run("ip route show")

    def console_dump_system_state(self):
        self.init_shell()
        self.shell.ubus_call("system board")
        self.shell.run("getDebugInformation -a -o /dev/console", timeout=180)

    def console_recover_ssh_access(self):
        self.init_shell()
        self.shell.run("iptables -P INPUT ACCEPT")
        self.shell.run("iptables -L INPUT")
        self.shell.run("/etc/init.d/dropbear restart; sleep 5")
        self.shell.run("logread | grep dropbear | tail -10")
        self.shell.run("netstat -nlp | grep :22")
        self.shell.run("ping -c1 192.168.1.1; ping -c1 192.168.1.2")

    def boot_into(self):
        strategy = self.target.get_driver("UBootStrategy")
        dest = self.args.destination
        if dest == "shell":
            strategy.transition("shell")
        if dest == "bootloader":
            strategy.transition("uboot")

    def power(self):
        power = self.target.get_driver("PowerProtocol")
        action = self.args.action
        if action == "on":
            power.on()
        if action == "off":
            power.off()
        if action == "cycle":
            power.cycle()

    def check_network(self):
        host = self.args.remote_host
        shell = self.target.get_driver("ShellDriver")
        shell.wait_for("ubus-cli 'IP.Interface.[Alias==\"lan\"].Status?0'", "Up", 60.0)

        shell.wait_for("ping -c1 {} || true".format(host), ", 0% packet loss", 180.0)


def main():
    logging.basicConfig(
        level=logging.INFO, format="%(levelname)7s: %(message)s", stream=sys.stderr
    )

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--config",
        type=str,
        default=os.environ.get("TB_CONFIG", ".testbed/labgrid/default.yaml"),
        help="config file (default: %(default)s)",
    )
    parser.add_argument(
        "-t",
        "--target",
        type=str,
        default=os.environ.get("TB_TARGET", None),
        help="target device",
    )
    parser.add_argument(
        "-o",
        "--console-logpath",
        type=str,
        default=os.environ.get("TB_CONSOLE_LOGPATH", os.getcwd()),
        help="path for console logfile (default: %(default)s)",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="count",
        default=int(os.environ.get("TB_VERBOSE", 0)),
        help="enable verbose mode",
    )
    parser.add_argument(
        "-d",
        "--debug",
        action="store_true",
        default=os.environ.get("TB_DEBUG"),
        help="enable debug mode",
    )

    subparsers = parser.add_subparsers(dest="command", title="available subcommands")

    subparser = subparsers.add_parser("power", help="control target power")
    subparser.add_argument(
        "action", choices=["on", "off", "cycle"], help="power on/off/cycle target"
    )
    subparser.set_defaults(func=TestbedDevice.power)

    subparser = subparsers.add_parser("boot_into", help="boot target into console")
    subparser.add_argument(
        "destination",
        choices=["shell", "bootloader"],
        help="boot target either into system shell or bootloader console",
    )
    subparser.set_defaults(func=TestbedDevice.boot_into)

    subparser = subparsers.add_parser(
        "check_network", help="ensure that network is usable"
    )
    subparser.add_argument(
        "-r",
        "--remote-host",
        default="192.168.1.2",
        help="remote host used for ping check (default: %(default)s)",
    )
    subparser.add_argument(
        "-n", "--network", default="lan", help="target network (default: %(default)s)"
    )
    subparser.set_defaults(func=TestbedDevice.check_network)

    subparser = subparsers.add_parser(
        "init_vlans", help="initialize VLAN configuration"
    )
    subparser.set_defaults(func=TestbedDevice.init_vlans)

    subparser = subparsers.add_parser(
        "console_dump_system_state", help="dump system state using serial console"
    )
    subparser.set_defaults(func=TestbedDevice.console_dump_system_state)

    subparser = subparsers.add_parser(
        "console_recover_ssh_access",
        help="try to recover SSH access using serial console",
    )
    subparser.set_defaults(func=TestbedDevice.console_recover_ssh_access)

    args = parser.parse_args()
    if args.verbose >= 1:
        StepReporter.start()

    if args.debug:
        logging.getLogger().setLevel(logging.DEBUG)

    if not args.target:
        print("target device name is mandatory")
        exit(1)

    if not args.command:
        print("command is missing")
        exit(1)

    device = TestbedDevice(args)
    args.func(device)


if __name__ == "__main__":
    main()
