/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <ltq_api_include.h>

int main(int argc, char** argv)
{
	char mac[SCAPI_MAC_LEN] = {0};

	int nRet = scapi_getNextMacaddr(argv[1], mac);
	if( nRet == 0){
		printf("Success. Next Mac = %s\n", mac);
	}
	else
		printf("Failure. ERROR = %d\n", nRet);
	return 0;
}
