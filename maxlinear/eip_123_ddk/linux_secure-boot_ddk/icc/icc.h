#ifndef __ICC_H__
#define __ICC_H__

#include <stdint.h>

uint32_t icc_loadimage(uint8_t **pp_buf, unsigned int *buf_size, uint8_t *p_chunk_mode);
uint32_t icc_reply(uint8_t val); // 0 = fail, 1 = success
uint32_t icc_freeimage(uint8_t *p_buf, unsigned int buf_size);

uint32_t icc_init_server();
uint32_t icc_init_cli();
uint32_t icc_send_chunk(uint8_t *p_buf, uint32_t buf_size, uint8_t lastChunk); //lastChunk 0:false, 1:true
uint32_t icc_recv_chunk(uint8_t **p_buf, uint32_t *buf_size, uint8_t *lastChunk); //lastChunk 0:false, 1:true
uint32_t icc_wait_reply(uint8_t *val); // 0 = fail, 1 = success
#endif /* __ICC_H__ */
