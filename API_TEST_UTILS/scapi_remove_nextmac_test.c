/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ltq_api_include.h>



int main(int argc, char** argv){
	int nRet = -EXIT_FAILURE;

	nRet = scapi_removeNextMacaddr(argv[1]);
	if(nRet == 0)
		printf("%s Removed from config\n", argv[1]);
	else
		printf("Error removing %s\n", argv[1]);
	return 0;
}
