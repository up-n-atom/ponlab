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
            }
{% if (Radio.OperatingFrequency == "5GHz") : %}
            object '{{Radio.Alias}}' {
                parameter AutoChannelEnable = true;
                parameter OperatingChannelBandwidth = "80MHz";
                object IEEE80211ax {
                    parameter MBSSIDAdvertisementMode = "Off";
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
