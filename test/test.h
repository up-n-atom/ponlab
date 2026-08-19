/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#define MAX_VAL 20
#define RET_TEST_CODE 100

typedef struct 
{
	int a;
	char b[MAX_VAL];
}MyB_t;

typedef struct
{
	int a;
	char b[MAX_VAL];
	int c;
	float d;
	MyB_t f;
	int x;
}MyA_t;
