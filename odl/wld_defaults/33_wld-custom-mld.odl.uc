%populate {
    object WiFi {
        object 'SSID' {
{% for ( let Itf in BD.Interfaces ) : if ( BDfn.isInterfaceWirelessAp(Itf.Name) && BDfn.isInterfaceLan(Itf.Name) ) : %}
            object '{{Itf.Alias}}' {
                parameter MLDUnit = -1;
            }
{% endif; endfor; %}
{% for ( let Itf in BD.Interfaces ) : if ( BDfn.isInterfaceWirelessAp(Itf.Name) && BDfn.isInterfaceGuest(Itf.Name) ) : %}
            object '{{Itf.Alias}}' {
                parameter MLDUnit = -1;
            }
{% endif; endfor; %}
        }
    }
}