/********************************************************************************
 
        Copyright (C) 2018 Intel Corporation
       Copyright (C) 2017-2018 Intel Corporation
        Lantiq Beteiligungs-GmbH & Co. KG
        Lilienthalstrasse 15, 85579 Neubiberg, Germany 
        For licensing information, see the file 'LICENSE' in the root folder of
        this software module.
 
********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : testApplication.c					*
 *         Description  : test application to test secure service APIs		*
 *  *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "fapi_sec_service.h"
#include "testApplication.h"

#define APP_MAX_SIZE 256


int main(int argc, char *argv[]){
	int ret=0;
	if(argc != 3){
		printf("Usage: ss_test_app <options>  <filepath>\n");
		printf("example: \n");
		printf("ss_test_app decrypt <file path>\n");
		printf("ss_test_app encrypt <file path>\n");
		printf("ss_test_app img_auth <file path>\n");
       		return -1;
	}
	
	char *choice = (char *)malloc(APP_MAX_SIZE);
	if(!choice){
		return -1;
	}

	sprintf_s(choice, APP_MAX_SIZE, "%s", argv[1]);

	char *file_path = (char *)malloc(APP_MAX_SIZE);
	if(!file_path){
		goto end;
	}

	sprintf_s(file_path, APP_MAX_SIZE, "%s", argv[2]);

	switchs(choice){
		cases ("decrypt")
			sse_param_t xSSEParamList={0};
			sse_buf_alloc(&xSSEParamList);

			sprintf_s(xSSEParamList.sPath,MAX_SS_FILE_PATH_SIZE,"%s",file_path);
			ret  = fapi_ssFRead(&xSSEParamList);
			if (ret == -1) {
				printf("Secure read failed");
				goto end;
			}

			printf("(len=%zu)  %s\n", xSSEParamList.len, xSSEParamList.pcBuf);

			sse_buf_free(&xSSEParamList);
			break;
			
		cases ("encrypt")
			sse_param_t xSSEParamList={0};
			sse_buf_alloc(&xSSEParamList);

			sprintf_s(xSSEParamList.sPath,MAX_SS_FILE_PATH_SIZE,"%s",file_path);
			ret = fapi_ssFWrite(&xSSEParamList);
			if(ret == -1){
				printf("Secure write failed");
				goto end;
			}

			printf("%s\n", xSSEParamList.pcBuf);

			sse_buf_free(&xSSEParamList);
			break;

		cases ("img_auth")
			int fd = -1;
			img_param_t img;		
			struct stat filesize={0};

			memset_s(&img,sizeof(img_param_t),0x0, sizeof(img_param_t));	

			fd = open(file_path, O_RDWR);
			if (fd < 0) {  
				fprintf(stderr, "The file %s could not be opened\n", file_path);
				goto end;
			}               

			if(fstat(fd, &filesize)){
				printf("fstat error: [%s]\n",strerror(errno)); 
				close(fd); 
				goto end;
			}

		
			img.src_img_fd=fd;
			img.src_img_len=filesize.st_size;
			img.src_img_addr = mmap(0, img.src_img_len, (PROT_READ|PROT_WRITE), MAP_SHARED, img.src_img_fd, 0);

			if(img.src_img_addr == MAP_FAILED){
				printf("MMAP failed... %s\n",strerror(errno));
				close(fd); 
				goto end;
			}
		
	
			ret = fapi_ssImgAuth(img);
			if(ret != 0){
				printf("Image validation failed");
			}

			if( munmap(img.src_img_addr, img.src_img_len) == -1 )
				printf("munmap failed\n");

			if(fd>=0)close(fd);
			
			break;
		 defaults
			break;
			
	}switchs_end;

end:
    if(file_path) free(file_path);
    if(choice) free(choice);
    return -1;
}

