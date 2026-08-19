%populate {
    object WiFi {
        object Vendor {
            object Daemon {
                instance add ("hostapd", Name = "hostapd") {
                    object ExecOptions {
                        parameter LogOutputPath = "Stdout";
                        parameter LogDebugLevel = "Info";
                    }
                }
                instance add ("wpa_supplicant", Name = "wpa_supplicant") {
                    object ExecOptions {
                        parameter LogOutputPath = "Stdout";
                        parameter LogDebugLevel = "Info";
                        parameter SupplicantMasterMode = false;
                    }
                }
            }
        }
    }
}
