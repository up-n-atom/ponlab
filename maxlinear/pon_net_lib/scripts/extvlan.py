#!/usr/bin/env python
from __future__ import print_function

EXAMPLE_USAGE = """
Examples:

Translate series of bytes into human readable Ext Vlan rule in json format:
$ ./extvlan.py --omci-hex "0xF8 0x00 0x00 0x00 0x80 0x00 0x00 0x01 0x40 0x0F 0x00 0x00 0x00 0x03 0x0F 0xAE"
{
    "FilterOuterPriority": 15,
    "FilterOuterVID": 4096,
    "FilterOuterTPID": 0,
    "FilterInnerPriority": 8,
    "FilterInnerVID": 0,
    "FilterInnerTPID": 0,
    "FilterEtherType": 1,
    "TreatTagsToRemove": 1,
    "TreatOuterPriority": 15,
    "TreatOuterVID": 0,
    "TreatOuterTPID": 0,
    "TreatInnerPriority": 3,
    "TreatInnerVID": 501,
    "TreatInnerTPID": 6
}

Translate cookie into human readable Ext Vlan rule in json format
$ ./extvlan.py --cookie f800000080000001400f000000030fae
{
    "FilterOuterPriority": 15,
    "FilterOuterVID": 4096,
    "FilterOuterTPID": 0,
    "FilterInnerPriority": 8,
    "FilterInnerVID": 0,
    "FilterInnerTPID": 0,
    "FilterEtherType": 1,
    "TreatTagsToRemove": 1,
    "TreatOuterPriority": 15,
    "TreatOuterVID": 0,
    "TreatOuterTPID": 0,
    "TreatInnerPriority": 3,
    "TreatInnerVID": 501,
    "TreatInnerTPID": 6
}

Build a "omci meads" command using command line switches
$ ./extvlan.py --meid 257 --fop 15 --fov 4096 --fot 0 --fip 15 --fiv 4096 --fit 0 --fe 0 --ttr 0 --top 15 --tov 0 --tot 0 --tip 3 --tiv 333 --tit 6
omci meads 171 257 6 0xF8 0x00 0x00 0x00 0xF8 0x00 0x00 0x00 0x00 0x0F 0x00 0x00 0x00 0x03 0x0A 0x6E

Parse json into "omci meads"
$ ./exttvlan.py --meid 12 --json '{"FilterOuterPriority": 15,
                                   "FilterOuterVID": 4096,
                                   "FilterOuterTPID": 0,
                                   "FilterInnerPriority": 8,
                                   "FilterInnerVID": 4096,
                                   "FilterInnerTPID": 0,
                                   "FilterEtherType": 1,
                                   "TreatTagsToRemove": 0,
                                   "TreatOuterPriority": 15,
                                   "TreatOuterVID": 4096,
                                   "TreatOuterTPID": 0,
                                   "TreatInnerPriority": 3,
                                   "TreatInnerVID": 14,
                                   "TreatInnerTPID":6}'
omci meads 171 12 6 0xF8 0x00 0x00 0x00 0x88 0x00 0x00 0x01 0x00 0x0F 0x80 0x00 0x00 0x03 0x00 0x76

Generate rule 1.8
$ ./extvlan.py --rule 1.8
omci meads 171 11 6 0xF8 0x00 0x00 0x00 0x88 0x00 0x00 0x00 0x00 0x0F 0x00 0x00 0x00 0x08 0x1F 0x40

Generate rule 0.5 and modify Treatment Inner Vid and Treatment Inner TPID
$ ./extvlan.py --rule 0.5 --fit 4 --fiv 333
omci meads 171 11 6 0xF8 0x00 0x00 0x00 0xF0 0xA6 0xC0 0x00 0x00 0x0F 0x00 0x00 0x00 0x05 0x1F 0x46

Generate rule 1.18 and print it as json
$ ./extvlan.py --rule 1.18 --as-json
{
    "FilterOuterPriority": 15,
    "FilterOuterVID": 4096,
    "FilterOuterTPID": 0,
    "FilterInnerPriority": 8,
    "FilterInnerVID": 10,
    "FilterInnerTPID": 0,
    "FilterEtherType": 0,
    "TreatTagsToRemove": 1,
    "TreatOuterPriority": 15,
    "TreatOuterVID": 0,
    "TreatOuterTPID": 0,
    "TreatInnerPriority": 8,
    "TreatInnerVID": 1000,
    "TreatInnerTPID": 0
}

Generate an array of struct pon_adapter_ext_vlan_filter containing all rules
$ ./extvlan.py --all --as-filter-struct | sed 's/$/},/'
{15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 15, 0, 0},
{15, 4096, 0, 15, 4096, 0, 1, 0, 15, 0, 0, 15, 0, 0},
{15, 4096, 0, 15, 4096, 0, 0, 3, 15, 4096, 0, 15, 4096, 0},
{15, 4096, 0, 15, 4096, 0, 1, 3, 15, 4096, 0, 15, 4096, 0},
{15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 5, 1000, 6},
{15, 4096, 0, 15, 4096, 0, 0, 0, 15, 0, 0, 10, 1000, 6},
{15, 4096, 0, 15, 4096, 0, 1, 0, 15, 0, 0, 10, 1000, 6},
{15, 4096, 0, 15, 4096, 0, 1, 0, 15, 0, 0, 5, 1000, 6},
{15, 4096, 0, 15, 4096, 0, 0, 0, 5, 2000, 6, 5, 1000, 6},
{15, 4096, 0, 15, 4096, 0, 1, 0, 5, 2000, 6, 5, 1000, 6},

The above 'omci meads' commands require Ext. Vlan ME to be created first.
You should create it with the following command:
$ omci mec 171 ME_ID ASSOCIATED_TYPE 0 INPUT_TPID OUT_TPID 0 0 ASSOCIATED_ID
For example:
$ omci mec 171 11 2 0 0x8100 0x88a8 0 0 257

To delete it:
$ omci med 171 11

If you want to see the rules that are being created by PON Net Lib you need to
set debug level to 1 (PRN):
$ omci pdmls libponnet 1

More complete example:
$ omci pdmls libponnet 1
$ omci mec 171 11 2 0 0x8100 0x88a8 0 0 257
$ omci meads 171 11 6 0xF8 0x00 0x00 0x00 0xF8 0x00 0x00 0x00 0x00 0x0F 0x00 0x00 0x00 0x05 0x1F 0x46
[libponnet] PRN in pon_net_find_ext_vlan_rule():1242 - found rule: 2.1: Do nothing (default flow); best_match=48,  match_count=1
[libponnet] PRN in pon_net_find_ext_vlan_rule():1242 - found rule: 1.1: Do nothing (default flow); best_match=10,  match_count=1
[libponnet] PRN in pon_net_find_ext_vlan_rule():1242 - found rule: 0.5: Insert one full tag X with fixed prio (default flow); best_match=4,  match_count=1
[libponnet] tc filter del dev eth0_0 handle 0x77 protocol all prio 64001 ingress flower
[libponnet] tc filter add dev eth0_0 ingress handle 0x77 protocol all prio 64001 flower skip_sw action vlan push id 1000 priority 5 protocol 0x88a8 cookie f8000000f8000000000f000000051f46
[libponnet] tc filter add dev eth0_0 egress handle 0x9 protocol 802.1ad prio 64001 flower skip_sw vlan_id 1000 vlan_prio 5 action vlan pop cookie f8000000f80005c0000f000000151f46
[libponnet] tc filter add dev eth0_0_2 egress handle 0x2 protocol 802.1ad prio 64001 flower skip_sw vlan_id 1000 vlan_prio 5 action vlan pop cookie f8000000f80005c0000f000000151f46
[libponnet] tc filter add dev eth0_0_3 egress handle 0x2 protocol 802.1ad prio 64001 flower skip_sw vlan_id 1000 vlan_prio 5 action vlan pop cookie f8000000f80005c0000f000000151f46
"""

from collections import defaultdict
from collections import OrderedDict
import json
import argparse
import re
import sys
import os

DIR = os.path.realpath(os.path.dirname(__file__))
EXT_VLAN_TABLE_FILE = "../src/pon_net_ext_vlan_table.h"
EXT_VLAN_TABLE_FULL_PATH = os.path.join(DIR, EXT_VLAN_TABLE_FILE)

OMCI_RULES_START = "static const struct omci_rules omci_rules[] = {"

FILTER_FIELDS = OrderedDict([
    ("FilterOuterPriority", 15),
    ("FilterOuterVID", 4096),
    ("FilterOuterTPID", 0),
    ("FilterInnerPriority", 15),
    ("FilterInnerVID", 4096),
    ("FilterInnerTPID", 0),
    ("FilterEtherType", 0),
    ("TreatTagsToRemove", 0),
    ("TreatOuterPriority", 15),
    ("TreatOuterVID", 4096),
    ("TreatOuterTPID", 0),
    ("TreatInnerPriority", 15),
    ("TreatInnerVID", 4096),
    ("TreatInnerTPID", 0),
])

def advance_to(file_iter, what):
    for line in file_iter:
        if OMCI_RULES_START in line:
            break;
    return file_iter

def all_rule_numbers():
    with open(EXT_VLAN_TABLE_FULL_PATH) as file:
        # Advance to OMCI_RULES_START
        file_iter = advance_to(iter(file), OMCI_RULES_START)

        regex = "{{([0-9]+), *([0-9]+),"

        for line in file_iter:
            match = re.search(regex, line)
            if match:
                major = match.group(1)
                minor = match.group(2)
                yield major, minor

def parse_ext_vlan_table(file, major, minor):
    # Advance to OMCI_RULES_START
    file_iter = advance_to(iter(file), OMCI_RULES_START)

    regex = "{{{{{}, {},".format(major, minor)

    rule = ""
    #Look for {{major, minor
    for line in file_iter:
        #We found the correct rule
        if re.search(regex, line):
            try:
                #Skip next line
                next(file_iter)

                for line in file_iter:
                    rule += line
                    if "}," in rule:
                        break

            except StopIteration:
                pass
            break

    if not rule:
        return None

    #Now the rule contains something like this:
    #    {15, 4096, 0, 15, 4096, 0, m_1_4, 0, 15, NA, NA, 15, NA, NA},\n

    #Remove unnecessary whitespace
    rule = " ".join(rule.split())

    #Remove the trailing ,
    rule = rule.strip(",")

    #Remove "{" and "}"
    rule = rule.strip("{}")

    #Now we have something like this:
    #15, 4096, 0, 15, 4096, 0, m_1_4, 0, 15, NA, NA, 15, NA, NA

    #It's time to replace constants
    #Please note that this is just a dummy search and replace so the order
    #does matter
    replace_list = [
        ("default_inner_VID", "15"),
        ("default_outer_VID", "30"),
        ("PxOr8", "4"),
        ("PyOr8", "7"),
        ("VIDc", "10"),
        ("VIDs", "100"),
        ("VIDx", "1000"),
        ("VIDy", "2000"),
        ("DEF", "0"),
        ("Pc", "2"),
        ("Px", "3"),
        ("Py", "4"),
        ("Ps", "5"),
        ("NA", "0"),
        ("X", "0"),
        ("E", "0"),
        ("S", "0"),
    ]

    for search, replace in replace_list:
        rule = rule.replace(search, replace)

    parts = [field.strip() for field in rule.split(",")]

    replace_list = [
        ("m_0_2_4_6_7", "2"),
        ("m_0_4_5_6_7", "4"),
        ("m_4_6_7", "4"),
        ("m_0_7", "5"),
        ("m_0_4", "3"),
        ("m_1_4", "2"),
    ]

    priority_fields = [0, 3, 8, 11]

    for key in priority_fields:
        value = parts[key]
        for search, replace in replace_list:
            value = value.replace(search, replace)
        parts[key] = value

    replace_list = [
        ("m_0_2_4_6_7", "0"),
        ("m_0_4_5_6_7", "0"),
        ("m_4_6_7", "6"),
        ("m_0_7", "0"),
        ("m_0_4", "0"),
        ("m_1_4", "1"),
    ]

    for key, value in enumerate(parts):
        for search, replace in replace_list:
            value = value.replace(search, replace)
        parts[key] = value

    parts = [int(field) for field in parts]

    parameters = OrderedDict(FILTER_FIELDS)

    for key, part in zip(parameters, parts):
        parameters[key] = part;

    return parameters

def to_binary_array(value, width):
    format_string = "{{:0>{width}b}}".format(width=width)
    binary_string = format_string.format(value)
    return list(int(i) for i in binary_string)

def to_value(binary_array):
    binary_string = "".join(str(i) for i in binary_array)
    return int(binary_string, 2)

def write_omci_string(binary_array):
    byte_array = []
    while binary_array:
        #take one byte from bineary array
        byte = binary_array[:8]
        binary_array = binary_array[8:]

        byte_array.append(to_value(byte))

    return " ".join("0x{:0>2X}".format(byte) for byte in byte_array)

#Example:
#format_rule({"FilterOuterPriority": 15, "FilterOuterVID": 4096,
#             "FilterOuterTPID": 0, "FilterInnerPriority": 8,
#             "FilterInnerVID": 4096, "FilterInnerTPID": 0,
#             "FilterEtherType": 1, "TreatTagsToRemove": 0,
#             "TreatOuterPriority": 15, "TreatOuterVID": 4096,
#             "TreatOuterTPID": 0, "TreatInnerPriority": 3,
#             "TreatInnerVID": 14, "TreatInnerTPID":6})
#
# 0xF8 0x00 0x00 0x00 0x88 0x00 0x00 0x10 0x00 0xF0 0x80 0x00 0x00 0x30 0x70 0x60
def format_rule(me_id, parameters):
    parameters = defaultdict(int, parameters)
    binary_array = []
    binary_array.extend(to_binary_array(parameters["FilterOuterPriority"], 4))
    binary_array.extend(to_binary_array(parameters["FilterOuterVID"], 13))
    binary_array.extend(to_binary_array(parameters["FilterOuterTPID"], 3))

    binary_array.extend(to_binary_array(0, 12))

    binary_array.extend(to_binary_array(parameters["FilterInnerPriority"], 4))
    binary_array.extend(to_binary_array(parameters["FilterInnerVID"], 13))
    binary_array.extend(to_binary_array(parameters["FilterInnerTPID"], 3))

    binary_array.extend(to_binary_array(0, 8))

    binary_array.extend(to_binary_array(parameters["FilterEtherType"], 4))

    binary_array.extend(to_binary_array(parameters["TreatTagsToRemove"], 2))

    binary_array.extend(to_binary_array(0, 10))

    binary_array.extend(to_binary_array(parameters["TreatOuterPriority"], 4))
    binary_array.extend(to_binary_array(parameters["TreatOuterVID"], 13))
    binary_array.extend(to_binary_array(parameters["TreatOuterTPID"], 3))

    binary_array.extend(to_binary_array(0, 12))

    binary_array.extend(to_binary_array(parameters["TreatInnerPriority"], 4))
    binary_array.extend(to_binary_array(parameters["TreatInnerVID"], 13))
    binary_array.extend(to_binary_array(parameters["TreatInnerTPID"], 3))

    return "omci meads 171 {} 6 {}".format(me_id, write_omci_string(binary_array))

def interpret_meads(s):
    tokens = s.split()
    hex_string = tokens[5:]
    byte_array = [ int(x, 16) for x in hex_string ]
    binary_array = []

    for byte in byte_array:
        binary_array.extend(to_binary_array(byte, 8))

    parameters = OrderedDict()

    parameters["FilterOuterPriority"] = to_value(binary_array[:4])
    binary_array = binary_array[4:]
    parameters["FilterOuterVID"] = to_value(binary_array[:13])
    binary_array = binary_array[13:]
    parameters["FilterOuterTPID"] = to_value(binary_array[:3])
    binary_array = binary_array[3:]

    binary_array = binary_array[12:]

    parameters["FilterInnerPriority"] = to_value(binary_array[:4])
    binary_array = binary_array[4:]
    parameters["FilterInnerVID"] = to_value(binary_array[:13])
    binary_array = binary_array[13:]
    parameters["FilterInnerTPID"] = to_value(binary_array[:3])
    binary_array = binary_array[3:]

    binary_array = binary_array[8:]

    parameters["FilterEtherType"] = to_value(binary_array[:4])
    binary_array = binary_array[4:]

    parameters["TreatTagsToRemove"] = to_value(binary_array[:2])
    binary_array = binary_array[2:]

    binary_array = binary_array[10:]

    parameters["TreatOuterPriority"] = to_value(binary_array[:4])
    binary_array = binary_array[4:]
    parameters["TreatOuterVID"] = to_value(binary_array[:13])
    binary_array = binary_array[13:]
    parameters["TreatOuterTPID"] = to_value(binary_array[:3])
    binary_array = binary_array[3:]

    binary_array = binary_array[12:]

    parameters["TreatInnerPriority"] = to_value(binary_array[:4])
    binary_array = binary_array[4:]
    parameters["TreatInnerVID"] = to_value(binary_array[:13])
    binary_array = binary_array[13:]
    parameters["TreatInnerTPID"] = to_value(binary_array[:3])
    binary_array = binary_array[3:]

    return parameters

def to_json(meads):
    d = interpret_meads(meads)
    return json.dumps(d, indent=4)

def to_meads(json_string):
    return format_rule(12, json.loads(json_string))

def to_filter_struct(fields):
    values = [str(value) for field, value in fields.items()]
    return "{" + ", ".join(values) + "}"

def cookie_to_omci_hex(cookie):
    chunks = [cookie[i:i+2] for i in range(0, len(cookie), 2)]
    with_0x = ["0x{}".format(x) for x in chunks]
    return " ".join(with_0x)

def print_rule(args):
    me_id = args.meid

    parameters = OrderedDict(FILTER_FIELDS)

    if args.rule:
        with open(EXT_VLAN_TABLE_FULL_PATH) as file:
            major, minor = args.rule.split(".")
            major = int(major)
            minor = int(minor)

            new_parameters = parse_ext_vlan_table(file, major, minor)
            if new_parameters:
                parameters = new_parameters

    if args.cookie:
        args.omci_hex = cookie_to_omci_hex(args.cookie);

    if args.omci_hex:
        parsed = json.loads(parse_hex(args.omci_hex))

        for k in parameters:
            parameters[k] = parsed[k]

        if not (args.as_json or args.as_filter_struct or args.as_omci_hex):
            args.as_json = True

    if args.fop:
        parameters["FilterOuterPriority"] = args.fop
    if args.fov:
        parameters["FilterOuterVID"] = args.fov
    if args.fot:
        parameters["FilterOuterTPID"] = args.fot
    if args.fip:
        parameters["FilterInnerPriority"] = args.fip
    if args.fiv:
        parameters["FilterInnerVID"] = args.fiv
    if args.fit:
        parameters["FilterInnerTPID"] = args.fit
    if args.fe:
        parameters["FilterEtherType"] = args.fe
    if args.ttr:
        parameters["TreatTagsToRemove"] = args.ttr
    if args.top:
        parameters["TreatOuterPriority"] = args.top
    if args.tov:
        parameters["TreatOuterVID"] = args.tov
    if args.tot:
        parameters["TreatOuterTPID"] = args.tot
    if args.tip:
        parameters["TreatInnerPriority"] = args.tip
    if args.tiv:
        parameters["TreatInnerVID"] = args.tiv
    if args.tit:
        parameters["TreatInnerTPID"] = args.tit

    if args.as_json:
        rule = json.dumps(parameters, indent=4)
    elif args.as_filter_struct:
        rule = to_filter_struct(parameters)
    else:
        rule = format_rule(me_id, parameters)

    print(rule)

def main():
    parser = argparse.ArgumentParser(epilog=EXAMPLE_USAGE,
                                     description="A helper script to generate commands useful when debugging Ext. Vlan",
                                     formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('--file', help="A file containing omci extvlan strings")
    parser.add_argument('--json', help="Rule", type=str)
    parser.add_argument('--omci-hex', help="OMCI Hex", type=str)
    parser.add_argument('--cookie', help="Cookie", type=str)
    parser.add_argument('--rule', help="For example --rule 2.11", type=str)
    parser.add_argument('--all', help="Print all rule numbers", action='store_true')

    parser.add_argument("--fop", type=int, help="Filter outer prio")
    parser.add_argument("--fov", type=int, help="Filter outer VID")
    parser.add_argument("--fot", type=int, help="Filter out TPID")
    parser.add_argument("--fip", type=int, help="Filter inner prio")
    parser.add_argument("--fiv", type=int, help="Filter inner VID")
    parser.add_argument("--fit", type=int, help="Filter inner TPID")
    parser.add_argument("--fe", type=int, help="Filter Ethertyoe")
    parser.add_argument("--ttr", type=int, help="Treatment tag to remove")
    parser.add_argument("--top", type=int, help="Treatment outer prio")
    parser.add_argument("--tov", type=int, help="Treatment outer VID")
    parser.add_argument("--tot", type=int, help="Treatment outer TPID")
    parser.add_argument("--tip", type=int, help="Treatment inner prio")
    parser.add_argument("--tiv", type=int, help="Treatment inner VID")
    parser.add_argument("--tit", type=int, help="Treatment inner TPID")
    parser.add_argument("--as-json", help="print result as JSON", action='store_true')
    parser.add_argument("--as-omci-hex", help="print result as OMCI Hex", action='store_true')
    parser.add_argument("--as-filter-struct", help="print result as struct omci_filter", action='store_true')

    parser.add_argument("--meid", type=int, default=11, help="Managed Entity ID to use in omci meads command string")

    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        sys.exit(1)
        return

    args = parser.parse_args()
    if args.json:
        parse_rule(args.json)
        return
    if args.file:
        parse_file(args.file)
        return
    if args.all:
        for major, minor in all_rule_numbers():
            args.rule = "{}.{}".format(major, minor)
            print_rule(args)
        return

    print_rule(args)

def parse_hex(hx):
    return to_json("omci meads 171 12 6 " + hx)

def parse_rule(rule):
    print(to_meads(rule))

def parse_file(filename):
    with open(filename) as f:
        for line in f:
            if re.match("^omci meads 171 [0-9]+ 6( [0-9A-Zx])+", line):
                print("rule ", end="")
                print(to_json(line))
            else:
                print(line, end="")

if __name__ == "__main__":
    main()
