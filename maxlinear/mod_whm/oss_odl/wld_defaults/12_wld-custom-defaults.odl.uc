%populate {
    object WiFi {
        object AutoNeighbourAddition {
            parameter Enable = 1;
        }
        object DaemonMgt.Daemon {
            object 'hostapd' {
                object ExecutionSettings {
                    parameter UseGlobalInstance = "On";
                }
            }
        }
    }
}
