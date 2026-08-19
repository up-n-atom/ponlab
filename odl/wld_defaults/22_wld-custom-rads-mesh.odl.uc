%populate {
    object WiFi {
        object Radio {
{% for ( let Radio in BD.Radios ) : %}
            object '{{Radio.Alias}}' {
                parameter OperatingStandardsFormat = "Legacy";
                parameter OperatingStandards = "ax";
                parameter RegulatoryDomain = "US";
                parameter DTIMPeriod = 2;
                parameter MultiUserMIMOEnabled = 1;
                object ChannelMgt {
                    parameter AcsBootChannel = -1;
                }
                parameter ExternalAcsMgmt = false;
            }
{% if (Radio.OperatingFrequency == "5GHz") : %}
            object '{{Radio.Alias}}' {
                parameter AutoChannelEnable = true;
                parameter OperatingChannelBandwidth = "80MHz";
                object IEEE80211ax {
                    parameter MBSSIDAdvertisementMode = "Off";
                }
                object Vendor {
                    object ACS {
                        parameter AcsFallbackChan = "36 0 20";
                    }
                }
                object ChannelMgt {
                    object BgDfs {
                        parameter PreclearEnable = false;
                    }
                }
            }
{% elif (Radio.OperatingFrequency == "6GHz") : %}
            object '{{Radio.Alias}}' {
                parameter Channel = 37;
                parameter OperatingChannelBandwidth = "160MHz";
                object IEEE80211ax {
                    parameter MBSSIDAdvertisementMode = "On";
                }
                object Vendor {
                    parameter OverrideMBSSID = true;
                    object ACS {
                        parameter AcsFils = true;
                    }
                }
            }
{% elif (Radio.OperatingFrequency == "2.4GHz") : %}
            object '{{Radio.Alias}}' {
                parameter AutoChannelEnable = true;
                parameter OperatingChannelBandwidth = "20MHz";
                parameter ObssCoexistenceEnable = true;
                parameter STA_Mode = 1;
                parameter STASupported_Mode = 1;
                object IEEE80211ax {
                    parameter MBSSIDAdvertisementMode = "Off";
                }
            }
{% endif; endfor; %}
        }
    }
}
