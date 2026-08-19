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

//#define DUT_USE_3_ANT_WAV500
#define DUT_USE_ANT_MASK

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Linq;
using dut_cli;
using System.Reflection;

namespace DUT_GUI
{
    public class DUT_t
    {
        public const string defaultIpAddress = "192.168.1.1";
        public const ushort defaultTcpPort = 22222;
        public const uint defaultConnectionTimeout = 5000;

        readonly Connection connection = new Connection();
        readonly Logger logger;
        Dut dut;

        public bool IsInitialized { get; set; }
        public string IpAddress { get; private set; }

        readonly bool[] txAnts = new bool[Defines.maxNumTxAntennas];
        readonly bool[] rxAnts = new bool[Defines.maxNumRxAntennas];
        public int hwRev = 0;

        public HardwareType HardwareType { get; private set; } = HardwareType.HARDWARE_TYPE_INVALID;
        public Band RfBandCurrent { get; private set; } = Band.BAND_INVALID;
        public Band[] RfSupportedBands { get; private set; } = Array.Empty<Band>();
        public bool[] TXants { get { return txAnts; } }
        public bool[] RXants { get { return rxAnts; } }
        public bool IsTransmitting { get; private set; } = false;

        public bool GetTxAntennasMask(out AntennaMask antennaMask)
        {
            return dut.getEnabledTxAntennaMask(out antennaMask);
        }

        public bool GetRxAntennasMask(out AntennaMask antennaMask)
        {
            return dut.getEnabledRxAntennaMask(out antennaMask);
        }

        public bool GetMaxTxAntennasMask(out AntennaMask antennaMask)
        {
            return dut.getAvailableTxAntennaMask(out antennaMask);
        }

        public bool GetMaxRxAntennasMask(out AntennaMask antennaMask)
        {
            return dut.getAvailableRxAntennaMask(out antennaMask);
        }

        public bool GetCalibrationFileVersion(out CalibrationFileVersion version, out CalibrationFileSubversion subversion)
        {
            return dut.getCalibrationFileVersion(out version, out subversion);
        }

        public bool GetTssiCalData(out CalibrationFileVersion version, out TssiCalibrationData[] data)
        {
            return dut.getTssiCalibrationData(out version, out data);
        }

        public bool GetRssiCalData(out CalibrationFileVersion version, out RssiCalibrationData[] data)
        {
            return dut.getRssiCalibrationData(out version, out data);
        }

        public bool DriverRelease()
        {
            IsInitialized = false;
            HardwareType = HardwareType.HARDWARE_TYPE_INVALID;
            return dut.driverRelease();
        }

        public string LastError
        {
            get
            {
                return dut.getLastError();
            }
        }

        public byte EEPROMversion
        {
            get
            {
                if (dut.getNvmVersion(out byte version))
                {
                    return version;
                }
                else
                {
                    return 0;
                }
            }
        }

        private static uint CalcDistance(int curChannel, int channel)
        {
            return (uint)Math.Abs(curChannel - channel);
        }

        public static void GetTpcFreqs(IEnumerable<TssiCalibrationDataVer6> tssiCalibrationData, byte channel, Bandwidth spectrumBandwidth, Band band, out List<TpcFreqVer6> tpcFreqs)
        {
            tpcFreqs = new List<TpcFreqVer6>();

            TpcFreqVer6 tpcFreq_low, tpcFreq_high;

            try
            {
                // Set same or lower channel from cal data in tpcFreq_low
                tpcFreq_low = tssiCalibrationData.Where(x => ((x.bw == spectrumBandwidth) && (x.band == band) && (x.channel <= channel)))
                        .OrderByDescending(x => x.channel)
                        .Select(x => new TpcFreqVer6 { distance = CalcDistance(channel, x.channel), data = x })
                        .First();
            }
            catch (Exception)
            {
                // If same or lower channel not found - set distance to max int value
                tpcFreq_low = new TpcFreqVer6 { distance = uint.MaxValue };
            }

            try
            {
                // Set higher channel from cal data in tpcFreq_high
                tpcFreq_high = tssiCalibrationData.Where(x => ((x.bw == spectrumBandwidth) && (x.band == band) && (x.channel > channel)))
                        .OrderBy(x => x.channel)
                        .Select(x => new TpcFreqVer6 { distance = CalcDistance(channel, x.channel), data = x })
                        .First();
            }
            catch (Exception)
            {
                // If higher channel not found - set distance to max int value
                tpcFreq_high = new TpcFreqVer6 { distance = uint.MaxValue };
            }

            // Check if tpcFreq_low has valid data
            if (tpcFreq_low.distance != uint.MaxValue)
            {
                // tpcFreq_low will be in first tpc result if its distance is same or lower than tpcFreq_high
                if (tpcFreq_low.distance <= tpcFreq_high.distance)
                {
                    tpcFreqs.Add(tpcFreq_low);
                }
                else
                {
                    tpcFreqs.Add(tpcFreq_low);
                    tpcFreqs.Add(tpcFreq_high);
                }
            }

            // Check if tpcFreq_high has valid data
            if (tpcFreq_high.distance != uint.MaxValue)
            {
                // Don't set tpcFreq_high at all if we found the exact channel (distance is 0)         
                if (tpcFreq_low.distance != 0)
                {
                    tpcFreqs.Add(tpcFreq_high);
                }
            }
            if (tpcFreqs.Count == 3)
            {
                tpcFreqs.RemoveAt(2);
            }
        }

        public DUT_t(Logger logger)
        {
            this.logger = logger;
        }

        public bool GetMaxPacketLength(PhyMode phyMode, out uint maxPacketLength)
        {
            return dut.calculateMaxPacketLength(phyMode, out maxPacketLength);
        }

        public bool GetPacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, byte nss, uint numSymbols, out uint packetLength)
        {
            return dut.calculatePacketLength(phyMode, signalBandwidth, mcs, nss, numSymbols, out packetLength);
        }

        public bool GetRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, byte nss, Gi gi, out float rateMbps)
        {
            return dut.calculatePhyDataRate(phyMode, signalBandwidth, mcs, nss, gi, out rateMbps);
        }

        public bool SetTransmitPowerLevel(byte powerLevel)
        {
            return dut.setTransmitPowerLevel(powerLevel);
        }

        public bool SetRate(Bandwidth signalBandwidth, Mcs mcs, byte nss, Gi gi, Ltf ltf, out float rateMbps)
        {
            return dut.setRate(signalBandwidth, mcs, nss, gi, ltf, out rateMbps);
        }

        static public string Version
        {
            get
            {
                return Dut.getVersion();
            }
        }

        static public string BuildTag
        {
            get
            {
                return Dut.getBuildTag();
            }
        }

        public bool ReadChipId(out string chipIdStr)
        {
            if (dut.getChipId(out ushort chipId))
            {
                chipIdStr = "0x" + chipId.ToString("X");
                return true;
            }

            chipIdStr = "";
            return false;
        }

        public bool UpdateAnts(bool dllUpdate = true)
        {
            return UpdateTxAnts(dllUpdate) && UpdateRxAnts(dllUpdate);
        }

        public void SetTXantLocal(int ant, bool isON)
        {
            txAnts[ant] = isON;
        }

        public void SetRXantLocal(int ant, bool isON)
        {
            rxAnts[ant] = isON;
        }

        public bool SetTXant(int ant, bool isON)
        {
            txAnts[ant] = isON;
            return UpdateTxAnts();
        }

        public AntennaMask GetActiveTxAntennaMask()
        {
            AntennaMask antennaMask = new AntennaMask(0);
            byte bit = 1;

            for (int i = 0; i < txAnts.Length; ++i, bit <<= 1)
            {
                if (txAnts[i])
                {
                    antennaMask.Value |= bit;
                }
            }
            return antennaMask;
        }

        public AntennaMask GetActiveRxAntennaMask()
        {
            AntennaMask antennaMask = new AntennaMask(0);
            byte bit = 1;

            for (int i = 0; i < rxAnts.Length; ++i, bit <<= 1)
            {
                if (rxAnts[i])
                {
                    antennaMask.Value |= bit;
                }
            }
            return antennaMask;
        }

        public bool UpdateTxAnts(bool dllUpdate = true)
        {
            AntennaMask antennaMask = GetActiveTxAntennaMask();

            if (dllUpdate)
            {
                if (!dut.setEnabledTxAntennaMask(antennaMask))
                {
                    return false;
                }
            }

            if (antennaMask.Value == 0)
            {
                IsTransmitting = false;
            }

            return true;
        }

        public bool SetRXant(int ant, bool isON)
        {
            rxAnts[ant] = isON;
            return UpdateRxAnts();
        }

        public bool UpdateRxAnts(bool dllUpdate = true)
        {
            AntennaMask antennaMask = GetActiveRxAntennaMask();

            if (dllUpdate)
            {
                return dut.setEnabledRxAntennaMask(antennaMask);
            }

            return true;
        }

        public bool StartTxPackets(ushort repetitions, uint packetLength, bool longData, bool beamforming, CodingType codingType)
        {
            if (dut.startTx(repetitions, packetLength, longData, beamforming, codingType))
            {
                IsTransmitting = true;
                return true;
            }

            return false;
        }

        public bool StopTxPackets()
        {
            if (dut.stopTx())
            {
                IsTransmitting = false;
                return true;
            }

            return false;
        }

        public bool SetTransmitPowerControl(bool closedLoop, byte powerLimit)
        {
            return dut.setTransmitPowerControl(closedLoop, powerLimit);
        }

        public bool SetChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, byte lowestChannel, byte primaryChannelIndex, RegulationType regulationType)
        {
            if (dut.setChannel(phyMode, spectrumBandwidth, lowestChannel, primaryChannelIndex, regulationType))
            {
                IsTransmitting = false;
                return true;
            }

            return false;
        }

        public bool BurnInfo(byte countryCode, byte[] macAddress, byte[] serialNumber, byte week, byte year, NvMemoryType memoryType, NvMemorySize memorySize)
        {
            return dut.setCardInfo(countryCode, macAddress, serialNumber, week, year) && dut.writeCalibrationFile(memoryType, memorySize) && dut.flushNvm();
        }

        public bool BurnCountryCode(byte countryCode, NvMemoryType memoryType, NvMemorySize memorySize)
        {
            if (!dut.getCardInfo(out _, out byte[] macAddress, out byte[] serialNumber, out byte week, out byte year))
            {
                return false;
            }
            return dut.setCardInfo(countryCode, macAddress, serialNumber, week, year) && dut.writeCalibrationFile(memoryType, memorySize) && dut.flushNvm();
        }

        public bool BurnFile(NvMemoryType memoryType, NvMemorySize memorySize)
        {
            return dut.writeCalibrationFile(memoryType, memorySize) && dut.flushNvm();
        }

        public bool ParseFile(String fileName)
        {
            return dut.loadNvmFromFile(fileName);
        }

        public bool RSSIVector(out short[] rssi)
        {
            return dut.getInbandRssi(out rssi);
        }

        public bool MpduPacketsCounter(out uint receivedPackets, out uint errorPackets)
        {
            return dut.getMpduPacketCounters(out receivedPackets, out errorPackets);
        }

        public bool PhyPacketsCounter(out uint receivedPackets, out uint crcErrors, out uint forwardedPackets)
        {
            return dut.getPhyPacketCounters(out receivedPackets, out crcErrors, out forwardedPackets);
        }

        public bool ResetMpduPacketsCounter()
        {
            return dut.resetMpduPacketCounters();
        }

        public bool ResetPacketsCounter()
        {
            return dut.resetPhyPacketCounters();
        }

        public bool EnableRxAggregation(bool enabled)
        {
            return dut.setRxAggregationEnabled(enabled);
        }

        public bool GetBbicCddValues(byte numTxAntennas, out uint offset1, out uint offset2, out uint offset3)
        {
            return dut.getBbicCddValues(numTxAntennas, out offset1, out offset2, out offset3);
        }

        public bool SetBbicCddValues(byte numTxAntennas, uint offset1, uint offset2, uint offset3)
        {
            return dut.setBbicCddValues(numTxAntennas, offset1, offset2, offset3);
        }

        public bool ReadEEPROM(ulong address, out byte[] data)
        {
            if (dut.getNvmSize(out ulong size))
            {
                return dut.readNvm(address, out data, size, true);
            }

            data = null;
            return false;
        }

        public bool WriteCompleteEEPROM(NvMemoryType memoryType, NvMemorySize memorySize)
        {
            return dut.writeCalibrationFile(memoryType, memorySize);
        }

        public bool BurnEEPROM(uint address, byte[] data, uint length)
        {
            return dut.writeNvm(address, data, length) && dut.flushNvm();
        }

        public bool FlushNvMemory()
        {
            return dut.flushNvm();
        }

        public bool ReadVoltageValue(out uint[] values)
        {
            return dut.getTransmitVoltages(out values);
        }

        public bool ReadRegister(ChipModule chipModule, uint address, uint mask, out uint value)
        {
            return dut.readRegister(chipModule, address, mask, out value);
        }

        public bool WriteRegister(ChipModule chipModule, uint address, uint mask, uint value)
        {
            return dut.writeRegister(chipModule, address, mask, value);
        }

        public bool ReadMemory(ChipModule chipModule, uint address, out byte[] data, uint length)
        {
            return dut.readMemory(chipModule, address, out data, length);
        }

        public bool WriteMemory(ChipModule chipModule, uint address, byte[] data, uint length)
        {
            return dut.writeMemory(chipModule, address, data, length);
        }

        // Arad: TODO- implement cache for CIS ?
        // Advantage - reduce access to dll
        // Drobacks: can't change CIS (eeprom structure) on the fly
        public void HardwareTypeInit()
        {
            if (!IsInitialized)
            {
                HardwareType = HardwareType.HARDWARE_TYPE_INVALID;
                hwRev = 0;
                RfBandCurrent = Band.BAND_INVALID;
                RfSupportedBands = Array.Empty<Band>();
            }
            else
            {
                HardwareType = GetHardwareType();
                RfBandCurrent = GetBandCurrent();
                RfSupportedBands = GetSupportedBands();
            }
        }

        public bool StartCW(sbyte amp, Int16 tone)
        {
            bool ok = dut.startCw(amp, tone);
            if (ok)
            {
                IsTransmitting = true;
            }

            return ok;
        }

        public bool StopCW()
        {
            bool ok = dut.stopCw();
            if (ok)
            {
                IsTransmitting = false;
            }

            return ok;
        }

        public bool SetIFS(uint ifs)
        {
            return dut.setIfs(ifs);
        }

        public bool SetSpacelessTransmission(bool enabled)
        {
            if (dut.setSpacelessTxEnabled(enabled))
            {
                IsTransmitting = enabled;
                return true;
            }

            return false;
        }

        public bool GetEEPROMInfo(out byte countryCode, out byte[] macAddress, out byte[] serialNumber, out byte week, out byte year)
        {
            return dut.getCardInfo(out countryCode, out macAddress, out serialNumber, out week, out year);
        }

        public bool GetComponentVersion(VersionedComponent type, out string version)
        {
            return dut.getComponentVersion(type, out version);
        }

        public bool MacFilterGetCount(out uint receivedPackets)
        {
            return dut.getMacPacketCounters(out receivedPackets);
        }

        public bool MacFilterSetCount()
        {
            return dut.resetMacPacketCounters();
        }

        public bool Connect(string ipAddress, ushort tcpPort, uint connectionTimeout)
        {
            bool connected = connection.open(ipAddress, tcpPort, connectionTimeout);
            if (connected)
            {
                IpAddress = ipAddress;
            }
            return connected;
        }

        public bool Initialize(byte wlanIndex, bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize)
        {
            dut = new Dut(wlanIndex, connection, logger);
            IsInitialized = dut.driverInit(snifferMode, memoryType, memorySize, Band.BAND_INVALID);
            return IsInitialized;
        }

        public bool WriteXtal(ushort data)
        {
            return dut.setXtalRegValue(data);
        }

        public bool ReadXtal(out ushort data)
        {
            return dut.getXtalRegValue(out data);
        }

        public bool WriteXtalCalDataToEEPROM(ushort data, NvMemoryType memoryType, NvMemorySize memorySize)
        {
            return dut.setXtalCalValue(data) && dut.writeCalibrationFile(memoryType, memorySize) && dut.flushNvm();
        }

        public bool ReadXtalCalDataFromEEPROM(out ushort data)
        {
            return dut.getXtalCalValue(out data);
        }

        public bool ReadRxEvm(out byte[] rxEvm)
        {
            return dut.getRxEvm(out rxEvm);
        }

        public bool ReadProdFlag(out bool productionFlag)
        {
            return dut.getProductionFlag(out productionFlag);
        }

        public bool BurnProdFlag(bool productionFlag)
        {
            return dut.setProductionFlag(productionFlag, true) && dut.flushNvm();
        }

        public HardwareType GetHardwareType()
        {
            if (dut.getHardwareType(out HardwareType hardwareType))
            {
                return hardwareType;
            }

            return HardwareType.HARDWARE_TYPE_INVALID;
        }

        public Band GetBandCurrent()
        {
            if (dut.getBand(out Band band))
            {
                return band;
            }

            return Band.BAND_INVALID;
        }

        public Band[] GetSupportedBands()
        {
            if (dut.getSupportedBands(out Band[] supportedBands))
            {
                return supportedBands;
            }

            return Array.Empty<Band>();
        }


        public bool LoadBeamformingMatrixFromFileSet(
            string primaryHeaderFile, string primaryValuesFile, string primaryExtValuesEhtFile,
            string secondaryHeaderFile, string secondaryValuesFile, string secondaryExtValuesEhtFile)
        {
            return dut.loadBeamformingMatrixFromFileSet(
                primaryHeaderFile, primaryValuesFile, primaryExtValuesEhtFile,
                secondaryHeaderFile, secondaryValuesFile, secondaryExtValuesEhtFile);
        }

        public bool ValidateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth)
        {
            return dut.validateBeamformingHeaderRegister(expectedPhyMode, expectedBandwidth);
        }

        public float GetTemperature()
        {
            if (dut.getTemperature(out float temperature))
            {
                return temperature;
            }

            return -1;
        }

        public bool SetRUParams(uint userOne, uint userTwo)
        {
            return dut.setRuParams(userOne, userTwo);
        }
    }

    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// Check for a new version of DUT, in the installation directory,
        /// using mtVerCtrl.dll, and then start the GUI
        /// </summary>
        [STAThread]
        static void Main()
        {
            DUT_GUI dutGUI = null;
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                dutGUI = new DUT_GUI();

                if (dutGUI.ToShowExceptionMsg) // It will be false if an error already happened in the construction
                {
                    dutGUI.Init(true);
                    Application.Run(dutGUI);
                }
            }
            catch (Exception ex)
            {
                if ((dutGUI == null) || (dutGUI.ToShowExceptionMsg))
                    MessageDialog.ShowError("Error running DUT GUI: " + ex.Message + "\n\nPlease make sure you have installed Microsoft Visual C++ 2019 Redistributable from\nhttps://aka.ms/vs/17/release/vc_redist.x64.exe.");
            }
        }
    }
}
