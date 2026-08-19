/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	int nRet = -EXIT_FAILURE;
	int nChildRet = -1;
	nRet = scapi_spawn(argv[1], atoi(argv[2]), &nChildRet);

	if( nRet < 0){
		printf("Couldn't spawn the process");
	}
	printf("Done with blocking\n");
	while(1);
}
