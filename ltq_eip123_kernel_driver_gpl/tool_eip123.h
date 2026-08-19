#ifndef __TOOL_EIP123_H__
#define __TOOL_EIP123_H__

/*
** this set the endian mode of EIP123 in axi slave register
** 0 = little endian
** 1 = big endian (default after reset)
**
** return val :
** 0 -> NO ERROR
** everything else is ERROR
*/
int  eip123_tool_set_axi_slave_endian(unsigned char mode);
void eip123_tool_hw_reset(void);

#endif /* __TOOL_EIP123_H__ */
