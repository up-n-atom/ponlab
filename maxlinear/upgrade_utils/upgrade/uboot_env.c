/******************************************************************************

Copyright (c) 2014
Lantiq Deutschland GmbH

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "command.h"

env_t env;

static int usage(char *str)
{
	printf("To get or set environment variable in u-boot configuraton\n");
	printf("        %s --get options\n", str);
	printf("    available options:\n");
	printf
	    ("        --name parameter              parameter name to be got\n");
	printf("        %s --set options\n", str);
	printf("    available options:\n");
	printf
	    ("        --name parameter              parameter name to be set\n");
	printf("        --value value                 value to be assigned\n");
        printf("        %s --add options\n", str);
        printf("    available options:\n");
        printf
            ("        --name parameter              parameter name to be add\n");
        printf("        --value value                 value to be assigned\n");
	return 0;
}

int main(int argc, char *argv[])
{
	int c;
	int option_index = 0;
	static int oper_add = 0, oper_get = 0, oper_set = 0;
	char *str_name = NULL, *str_value = NULL;
	char *output = NULL;
	static struct option long_options[] = {
		{"add", no_argument, &oper_add, 1},
		{"get", no_argument, &oper_get, 1},
		{"set", no_argument, &oper_set, 1},
		{"name", required_argument, 0, '1'},
		{"value", required_argument, 0, '2'},
		{NULL, 0, 0, 0}
	};

	if (argc < 4 || argc > 6) {
		usage(argv[0]);
		return -1;
	}

	while ((c =
		getopt_long(argc, argv, "1:2:", long_options,
			    &option_index)) != EOF) {
		switch (c) {
		case 0:
			break;
		case '1':
			str_name = optarg;
			break;
		case '2':
			str_value = optarg;
			break;
		default:
			goto ERR_RET;
		}
	}

	if (oper_get == 1 && oper_set == 1 && oper_add == 1)
		goto ERR_RET;
	if (oper_get == 0 && oper_set == 0 && oper_add == 0)
		goto ERR_RET;
	if (str_name == NULL)
		goto ERR_RET;
	if (oper_set == 1 && str_value == NULL)
		goto ERR_RET;
	if (oper_add == 1 && str_value == NULL)
		goto ERR_RET;


	if (read_env() != 0) {
		printf("Can not retrive u-boot configuration\n");
		return -1;
	}

	output = (char *)get_env(str_name);
	if (output == NULL && oper_add == 1)
	{
		goto addoper;
	}
	if(output == NULL)
	{
		printf("parameter %s is not existed\n", str_name);
		return -1;
	}
	if (oper_get == 1) {
		printf("%s\n", output);
	} 
	else if(oper_set == 1) 
	{		// oper_set == 1
		if (str_value == NULL) {
			return -1;
		}
		if (set_env(str_name, str_value) != 0) {
			printf
			    ("Can not set u-boot configuration (overflow?)\n");
			return -1;
		}
	}
	else
	{
addoper:
		// oper_add == 1
		if((str_name == NULL) || (str_value == NULL)){
			printf("Can not add u-boot configuration due to insufficient input\n");
			return -1;
		}

	 	if(add_env(str_name, str_value) != 0) {
			printf("Can not add u-boot configuration (overflow?)\n");
			return -1;
		}
	}
	if((oper_set == 1) || (oper_add == 1))
	{
		saveenv();
		if (!output && !str_value) {
			printf("parameter %s value changed from %s to %s\n", str_name, output, str_value);
		}
	}

	return 0;
      ERR_RET:
	usage(argv[0]);
	return -1;
}
