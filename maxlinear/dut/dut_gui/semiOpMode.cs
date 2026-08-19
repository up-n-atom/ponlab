/*
 *  <legal_notice>
 *   MaxLinear, Inc. retains all right, title and interest (including all intellectual
 *   property rights) in and to this computer program, which is protected by applicable
 *   intellectual property laws.  Unless you have obtained a separate written license from
 *   MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
 *   to utilize all or a part of this computer program for any purpose (including
 *   reproduction, distribution, modification, and compilation into object code), and you
 *   must immediately destroy or return all copies of this computer program.  If you are
 *   licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
 *   to utilize this computer program are limited by the terms of that license.
 *  
 *   This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
 *   authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
 *   of this computer program and related information and to not disclose this computer
 *   program and related information to any other person or entity.
 *  
 *   Misuse of this computer program or any information contained in it may results in
 *   violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
 *   trade secret, patent, contractual, and other legal rights.
 *  
 *   THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
 *   EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
 *   MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
 *  
 *  ***************************************************************************************
 *                                          Copyright (c) 2021/2022, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Globalization;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32;
using dut_cli;
using static DUT_GUI.GUI_BoxItems;

/// Handles the power control (semi-op) mode
/// 
namespace DUT_GUI
{
    /// In each limit set, there is a list of limits for different parameters (domain, freq, etc...)
    class limitSetItem_t
    {
        readonly string domain;  // regulatory domain
        readonly ushort freq;      // frequency
        readonly int limit;      // the power limit

        public limitSetItem_t(string domain, ushort freq, int limit)
        {
            this.domain = domain;
            this.freq = freq;
            this.limit = limit;
        }

        public bool isEqual(string rhsDomain, ushort rhsFreq)
        {
            return (domain == rhsDomain && freq == rhsFreq);
        }

        public int Limit { get { return limit; } }
    };

    public class hardware_t : IEquatable<hardware_t>
    {
        readonly byte type;
        readonly char revision;

        public byte Type { get { return type; } }
        public byte Revision { get { return (byte)revision; } }
        public string TypeStr { get { return "0x" + type.ToString("X2"); } }
        public string RevisionStr
        {
            get
            {
                if (Char.IsUpper(revision)) return "" + revision;
                else return "0x" + ((byte)revision).ToString("X2");
            }
        }

        public hardware_t(string strType, string strRevision)
        {
            type = (byte)DUT_GUI.ParseString(strType);

            if (strRevision.Length == 1 && Char.IsLetter(strRevision[0]))
                revision = Char.ToUpper(strRevision[0]);
            else revision = (char)DUT_GUI.ParseString(strRevision);
        }

        public hardware_t(byte type, char revision)
        {
            this.type = type;
            this.revision = revision;
        }

        public override int GetHashCode() => ((type << 8) + revision).GetHashCode();

        public override bool Equals(object obj) => this.Equals(obj as hardware_t);

        public bool Equals(hardware_t rhs)
        {
            return (type == rhs.type) && (revision == rhs.revision);
        }
    };

    /// Each limit set corresponds to specific hardwares (type & revision)
    /// Contains a list of limit set items
    class limitSet_t : List<limitSetItem_t>
    {
        readonly List<hardware_t> hardwares = new List<hardware_t>();

        public bool ContainsHW(hardware_t HW) { return hardwares.Contains(HW); }
        public void AddHardware(hardware_t HW) { hardwares.Add(HW); }
    };

    class regulatoryItem_t
    {
        readonly string domain;          // regulatory domain
        readonly int channel;            // channel
        readonly int mitigationFactor;   // Mitigation Factor
        readonly Band band;              // 6000 or 5000 or 2400
        readonly int limit;              // power limit

        public regulatoryItem_t(string domain, int channel, int mitigationFactor, Band band, int limit)
        {
            this.domain = domain;
            this.channel = channel;
            this.mitigationFactor = mitigationFactor;
            this.band = band;
            this.limit = limit;
        }

        public bool isPartOf(string rhsDomain, Band rhsBand)
        {
            return (domain == rhsDomain && band == rhsBand);
        }

        public bool isEqual(string rhsDomain, int rhsChannel, Band rhsBand)
        {
            return (channel == rhsChannel) && isPartOf(rhsDomain, rhsBand);
        }

        public int Channel { get { return channel; } }
        public int Limit { get { return limit; } }
        public int MitigationFactor { get { return mitigationFactor; } }
    }

    class semiOpModeManager
    {
        readonly Dictionary<string, limitSet_t> HWlimitSets = new Dictionary<string, limitSet_t>();
        readonly List<regulatoryItem_t> regulatoryTable = new List<regulatoryItem_t>();
        readonly List<string> regDomains = new List<string>();
        limitSet_t activeLimitSet = null;
        readonly List<string> log = new List<string>(); // Log of last operation

        public string[] RegDomainsArray { get { return regDomains.ToArray(); } }
        public List<string> Log { get { return log; } }

        class iniException_t : ApplicationException
        {
            public iniException_t(string line) : base("Bad line in ini file: " + line) { }
        }

        /// <summary>
        /// Finds the HW limit set that corresponds to the hardware from the EEPROM
        /// </summary>
        public void SetActiveLimitSet(hardware_t activeHW)
        {
            foreach (limitSet_t limitSet in HWlimitSets.Values)
            {
                if (limitSet.ContainsHW(activeHW))
                {
                    activeLimitSet = limitSet;
                    return;
                }
            }
            throw new ApplicationException("Cannot find HW limit set for the active hardware configuration.");
        }

        static public Band getBandbyFreq(ushort freq)
        {
            if (freq >= 5950)
            {
                return Band.BAND_6000MHZ;
            }
            else if (freq >= 4000)
            {
                return Band.BAND_5000MHZ;
            }
            else
            {
                return Band.BAND_2400MHZ;
            }
        }

        /// <summary>
        /// Finds the TX power limit according to the tables of hardware limits
        /// </summary>
        int GetHWPowerLimit(string domain, byte channel, PhyMode phyMode, Band band, Bandwidth bandwidth)
        {
            ushort centerFrequency = Channels.getCenterFrequency(channel, phyMode, band, bandwidth);
            limitSetItem_t limitSetItem = activeLimitSet.Find(delegate (limitSetItem_t item) { return item.isEqual(domain, centerFrequency); });

            if (limitSetItem == null) return int.MaxValue; // There is no hardware limit for this channel
            return limitSetItem.Limit;
        }

        /// <summary>
        /// Finds the TX power limit according to regulatory domain
        /// </summary>
        int GetRegPowerLimit(string domain, int channel, Band band, Bandwidth bandwidth, int numTXants)
        {
            regulatoryItem_t primaryChItem = regulatoryTable.Find(delegate (regulatoryItem_t item) { return (item.isEqual(domain, channel, band)); });
            if (primaryChItem == null) throw new ApplicationException("Cannot find regulatory limit value for channel " + channel);

            // Find the power limit and subtract the appropriate mitigation factor (primary / alternate)
            int tableLimit;
            log.Clear();
            //			log.Add("Primary channel limit = " + primaryChItem.Limit.ToString());
            //Arad: TODO- what should be the altChannel in the new channel mode??? TODO- talk to Yossi and Yoram
            int altChannel = channel; //Set to PrimeIndex=LOW_40_LOW_20 
            if (bandwidth != 0/*20MHz*/)
            {
                regulatoryItem_t altChItem = regulatoryTable.Find(delegate (regulatoryItem_t item) { return (item.isEqual(domain, altChannel, band)); });
                if (altChItem == null) throw new ApplicationException("Cannot find regulatory limit value for alt. channel " + altChannel);

                //				log.Add("Alt. channel limit = " + altChItem.Limit.ToString());
                if (primaryChItem.Limit <= altChItem.Limit) tableLimit = primaryChItem.Limit - primaryChItem.MitigationFactor;
                else tableLimit = altChItem.Limit - altChItem.MitigationFactor;
            }
            else tableLimit = primaryChItem.Limit - primaryChItem.MitigationFactor;

            //			log.Add("Limit from table (after mitigation factor) = " + tableLimit.ToString());
            return tableLimit - (int)(10.0 * Math.Log10(numTXants) + 0.5); // round to int
        }

        /// <summary>
        /// Returns the allowed power limit according to the regulations and hardware
        /// </summary>
        /// <param name="domain">Regulatory domain</param>
        /// <param name="channel">Channel</param>
        /// <param name="band">Band (5000 or 2400) </param>
        /// <param name="bandwidth">Spectrum bandwidth</param>
        /// <param name="TXants">TX Antennas that are transmitting</param>
        /// <returns>The power limit</returns>
        public int GetPowerLimit(string domain, byte channel, PhyMode phyMode, Band band, Bandwidth bandwidth, bool[] TXants)
        {
            int numTXants = 0;
            foreach (bool isOn in TXants) { if (isOn) ++numTXants; }
            if (numTXants <= 0) throw new ApplicationException("No TX antennas are ON.");
            //			log.Add("Checking power limit for: domain="+domain+", ch="+channel+" (freq="+GetFreq(channel, band, isChannelBonding, isUpperCB).ToString()
            //				+ "), " + numTXants.ToString() + " TX ant" + ((numTXants > 1) ? "s" : ""));
            int HWlimit = GetHWPowerLimit(domain, channel, phyMode, band, bandwidth);
            int regLimit = GetRegPowerLimit(domain, channel, band, bandwidth, numTXants);
            log.Add("HW Limit = " + (HWlimit == int.MaxValue ? "none" : HWlimit.ToString()) + ", Regulatory domain limit = " + regLimit);
            return Math.Min(HWlimit, regLimit); // Limit_CB or Limit_nCB
        }

        /// <summary>
        /// Returns an array of allowed channels for the specific domain
        /// </summary>
        /// <param name="domain"></param>
        /// <param name="band">5000 or 2400</param>
        /// <returns>Array of channel numbers strings</returns>
        public List<int> GetChannelList(string domain, Band band)
        {
            List<int> channels = new List<int>();
            foreach (regulatoryItem_t regItem in regulatoryTable)
            {
                if (regItem.isPartOf(domain, band))
                {
                    if (!channels.Contains(regItem.Channel)) channels.Add(regItem.Channel);
                }
            }
            channels.Sort();
            return channels;
        }

        /// Read the limits ini file, and fill the limits sets and regulatory domains in it
        /// 
        public semiOpModeManager()
        {
        }
    }
}
