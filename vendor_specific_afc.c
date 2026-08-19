
/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "vendor_specific.h"
#include "utils.h"

/**
 * AFC platform dependent API implementation 
 */

/* Be invoked when start controlApp */
void vendor_init() {
	/* Bring 6 GHz down to 1 Antenna and limit max power to match LitePoint threshold */
    system("iw wlan4 iwlwav sCoCPower 0 1 1");
	//system("iw wlan4 iwlwav sPowerSelection 3");
	system("iw wlan4 iwlwav sFixedPower 32 255 12 1");
	//Disable wlan0 and wlan2 to avoid interferrence
	//Add change to force verify_cert to 1
	//Add check and change to append 192.165.100.15 testserver.wfatestorg.org to /etc/hosts
}

/* Be invoked when terminate controlApp */
void vendor_deinit() {
    system("iw wlan4 iwlwav sCoCPower 1 0 0");
	system("iw wlan4 iwlwav sPowerSelection 0");
	//Restore proper sFixedPower to 6GHz
}
