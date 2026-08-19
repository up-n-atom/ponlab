/*
 * Driver for Cadence QSPI Controller
 *
 * Copyright (C) 2012 Altera Corporation
 * Copyright (C) 2018 Intel Corporation
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include "spi-mxl-qspi.h"
#include "spi-mxl-qspi-apb.h"

/* 1-beat single, 4-byte width = 4-byte single = 2**2 */
#define CQSPI_NUMSGLREQBYTES (2)
/* 16-beat burst, 4-byte width = 64-byte bursts = 2**6 */
#define CQSPI_NUMBURSTREQBYTES (6)

static unsigned int start_print;

#define debug_print(fmt, ...)	\
do {	\
	if (start_print)	\
		printk(fmt, ##__VA_ARGS__);	\
} while (0)

/**
 * Convert address buffer into word format (MSB first).
 */
static unsigned int cadence_qspi_apb_cmd2addr(const unsigned char *addr_buf,
					      unsigned int addr_width)
{
	unsigned int addr = 0;
	int i;

	debug_print("[%s] addr_buf[0]=0x%x addr_buf[1]=0x%x addr_buf[2]=0x%x "
		    "addr_buf[3]=0x%x addr_width %d\n", __func__,
		    addr_buf[0], addr_buf[1], addr_buf[2], addr_buf[3],
		    addr_width);

	if (addr_width > 4) {
		pr_err("Invalid address width %u\n", addr_width);
		return 0;
	}

	for (i = 0; i < addr_width; i++) {
		addr = addr << 8;
		addr |= addr_buf[i];
	}

	return addr;
}

void disable_qspi_direct_access(void *reg_base)
{
	u32 reg;

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg &= ~(CQSPI_REG_CONFIG_DIRECT_MASK);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
}

void enable_qspi_direct_access(void *reg_base)
{
	u32 reg;

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg |= CQSPI_REG_CONFIG_DIRECT_MASK;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
}

static void cadence_qspi_apb_read_fifo_data(void *dest,
					    const void *src_ahb_addr,
					    unsigned int bytes,
					    unsigned int flash_type)
{
	unsigned int temp;
	int remaining = bytes;
	unsigned int *dest_ptr = (unsigned int *)dest;
	unsigned int *src_ptr = (unsigned int *)src_ahb_addr;

	debug_print("[%s]dest=%p src_ahb_addr=%p bytes=0x%x\n",
		    __func__, dest, src_ahb_addr, bytes);
	if (!bytes)
		return;

	while (remaining > 0) {
		if (flash_type == QSPI_FLASH_TYPE_NOR) {
			if (remaining >= CQSPI_FIFO_WIDTH) {
				temp = CQSPI_READL(src_ptr);
				(*(u32 *)(dest_ptr)) = temp;
				remaining -= CQSPI_FIFO_WIDTH;
			} else {
				/* dangling bytes */
				temp = CQSPI_READL(src_ptr);
				(*(u32 *)(dest_ptr)) = temp;
				debug_print("[%s]dest_ptr=%p *src_ptr=0x%x\n",
					    __func__, dest_ptr, temp);
				remaining = 0;
				break;
			}
		} else {
			if (remaining >= CQSPI_FIFO_WIDTH) {
				(*(u32 *)(dest_ptr)) = CQSPI_READL(src_ptr);
				remaining -= CQSPI_FIFO_WIDTH;
			} else {
				/* dangling bytes */
				(*(u32 *)(dest_ptr)) = CQSPI_READL(src_ptr);
				remaining = 0;
				break;
			}
		}
		src_ptr++;
		dest_ptr++;
	}
}

static void cadence_qspi_apb_write_fifo_data(void *dest_ahb_addr,
					     const void *src,
					     unsigned int bytes,
					     unsigned int flash_type)
{
	unsigned int temp;
	int remaining = bytes;
	unsigned int *dest_ptr = (unsigned int *)dest_ahb_addr;
	unsigned int *src_ptr = (unsigned int *)src;

	debug_print("[%s]dest_ahb_addr=%p src=%p bytes=0x%x\n",
		    __func__, dest_ahb_addr, src, bytes);
	debug_print("[%s]*dest_ahb_addr=0x%x *src=0x%x\n",
		    __func__, *(unsigned int *)dest_ahb_addr,
		    *(unsigned int *)src);
	while (remaining > 0) {
		if (flash_type == QSPI_FLASH_TYPE_NOR) {
			if (remaining >= CQSPI_FIFO_WIDTH) {
				temp = *src_ptr;
				CQSPI_WRITEL(temp, dest_ptr);
				debug_print("[%s]dest_ptr=%p *src_ptr=0x%x\n",
					    __func__, dest_ptr, temp);
				remaining -= CQSPI_FIFO_WIDTH;
			} else {
				/* dangling bytes */
				memcpy(&temp, src_ptr, remaining);
				CQSPI_WRITEL(temp, dest_ptr);
				debug_print("[%s]dest_ptr=%p *src_ptr=0x%x\n",
					    __func__, dest_ptr, temp);
				break;
			}
		} else {
			if (remaining >= CQSPI_FIFO_WIDTH) {
				CQSPI_WRITEL(*src_ptr, dest_ptr);
				remaining -= CQSPI_FIFO_WIDTH;
			} else {
				/* dangling bytes */
				memcpy(&temp, src_ptr, remaining);
				CQSPI_WRITEL(temp, dest_ptr);
				break;
			}
		}
		src_ptr++;
		dest_ptr++;
	}
}

/* Return 1 if idle, otherwise return 0 (busy). */
static unsigned int cadence_qspi_wait_idle(void *reg_base)
{
	unsigned int count = 0;
	unsigned long timeout;

	timeout = cadence_qspi_init_timeout(CQSPI_TIMEOUT_MS);
	while (cadence_qspi_check_timeout(timeout)) {
		if (CQSPI_REG_IS_IDLE(reg_base)) {
			/* Read few times in succession to ensure it does
			 * not transition low again
			 */
			count++;
			if (count >= CQSPI_POLL_IDLE_RETRY)
				return 1;
		} else {
			count = 0;
		}
	}
	/* Timeout, in busy mode. */
	pr_err("QSPI: QSPI is still busy after %dms timeout.\n",
	       CQSPI_TIMEOUT_MS);
	return 0;
}

static void cadence_qspi_apb_readdata_capture(void *reg_base,
					      unsigned int bypass,
					      unsigned int delay)
{
	unsigned int reg;

	debug_print("%s %d %d\n", __func__, bypass, delay);
	cadence_qspi_apb_controller_disable(reg_base);

	reg = CQSPI_READL(reg_base + CQSPI_REG_READCAPTURE);
	if (bypass)
		reg |= (1 << CQSPI_REG_READCAPTURE_BYPASS_LSB);
	else
		reg &= ~(1 << CQSPI_REG_READCAPTURE_BYPASS_LSB);

	reg &= ~(CQSPI_REG_READCAPTURE_DELAY_MASK
		<< CQSPI_REG_READCAPTURE_DELAY_LSB);

	reg |= ((delay & CQSPI_REG_READCAPTURE_DELAY_MASK)
		<< CQSPI_REG_READCAPTURE_DELAY_LSB);

		CQSPI_WRITEL(reg, reg_base + CQSPI_REG_READCAPTURE);
	cadence_qspi_apb_controller_enable(reg_base);
}

static void cadence_qspi_apb_config_baudrate_div(void *reg_base,
						 unsigned int ref_clk_hz,
						 unsigned int sclk_hz)
{
	unsigned int reg;
	unsigned int div;

	debug_print("%s %d %d\n", __func__, ref_clk_hz, sclk_hz);
	cadence_qspi_apb_controller_disable(reg_base);

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg &= ~(CQSPI_REG_CONFIG_BAUD_MASK << CQSPI_REG_CONFIG_BAUD_LSB);
	div = ref_clk_hz / sclk_hz;
	/* Recalculate the baudrate divisor based on QSPI specification. */
	if (div > 32)
		div = 32;
	/* Check if even number. */
	if (div & 1)
		div = (div / 2);
	else
		div = (div / 2) - 1;
	debug_print("QSPI: ref_clk %dHz sclk %dHz div 0x%x\n", ref_clk_hz,
		    sclk_hz, div);
	div = (div & CQSPI_REG_CONFIG_BAUD_MASK) << CQSPI_REG_CONFIG_BAUD_LSB;
	reg |= div;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
	cadence_qspi_apb_controller_enable(reg_base);
}

static void cadence_qspi_apb_chipselect(void *reg_base,
					unsigned int chip_select,
					unsigned int decoder_enable)
{
	unsigned int reg;

	debug_print("%s\n", __func__);
	debug_print("QSPI: chipselect %d decode %d\n", chip_select,
		    decoder_enable);
	cadence_qspi_apb_controller_disable(reg_base);

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	/* docoder */
	if (decoder_enable) {
		reg |= CQSPI_REG_CONFIG_DECODE_MASK;
	} else {
		reg &= ~CQSPI_REG_CONFIG_DECODE_MASK;
		/* Convert CS if without decoder.
		 * CS0 to 4b'1110
		 * CS1 to 4b'1101
		 * CS2 to 4b'1011
		 * CS3 to 4b'0111
		 */
		chip_select = 0xF & ~(1 << chip_select);
	}
	reg &= ~(CQSPI_REG_CONFIG_CHIPSELECT_MASK
			<< CQSPI_REG_CONFIG_CHIPSELECT_LSB);
	reg |= (chip_select & CQSPI_REG_CONFIG_CHIPSELECT_MASK)
			<< CQSPI_REG_CONFIG_CHIPSELECT_LSB;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
	cadence_qspi_apb_controller_enable(reg_base);
}

static int cadence_qspi_apb_exec_flash_cmd(void *reg_base, unsigned int reg)
{
	unsigned long timeout;

	debug_print("%s: reg_base %p reg 0x%x\n",
		    __func__, reg_base, reg);
	/* Write the CMDCTRL without start execution. */
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CMDCTRL);
	/* Start execute */
	reg |= CQSPI_REG_CMDCTRL_EXECUTE_MASK;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CMDCTRL);
	debug_print("[%s] CQSPI_REG_CMDCTRL=0x%x\n", __func__, reg);
	/* Polling for completion. */
	timeout = cadence_qspi_init_timeout(CQSPI_TIMEOUT_MS);
	while (cadence_qspi_check_timeout(timeout)) {
		reg = CQSPI_READL(reg_base + CQSPI_REG_CMDCTRL) &
			CQSPI_REG_CMDCTRL_INPROGRESS_MASK;
		if (!reg)
			break;
	}
	if (reg != 0) {
		pr_err("QSPI: flash cmd execute time out\n");
		return -EIO;
	}
	/* Polling QSPI idle status. */
	if (!cadence_qspi_wait_idle(reg_base))
		return -EIO;
	return 0;
}

/* For command RDID, RDSR. */
static int cadence_qspi_apb_command_read(void *reg_base,
					 unsigned int txlen,
					 const unsigned char *txbuf,
					 unsigned int addrlen,
					 const unsigned char *addrbuf,
					 unsigned int rxlen,
					 unsigned char *rxbuf,
					 unsigned int flash_type)
{
	unsigned int reg, i;
	unsigned int addr_value;
	int status;

	debug_print("%s txlen %d txbuf %p rxlen %d rxbuf %p\n",
		    __func__, txlen, txbuf, rxlen, rxbuf);
#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi tx: ", DUMP_PREFIX_NONE,
		       32, 1, txbuf, txlen, false);
#endif
	if (!rxlen || rxlen > CQSPI_STIG_DATA_LEN_MAX || !rxbuf) {
		pr_err("QSPI: Invalid input argument, len %d rxbuf %p\n",
		       rxlen, rxbuf);
		return -EINVAL;
	}
	reg = txbuf[0] << CQSPI_REG_CMDCTRL_OPCODE_LSB;
	reg |= (0x1 << CQSPI_REG_CMDCTRL_RD_EN_LSB);
	if (addrlen) {
		/* Command with address */
		reg |= (0x1 << CQSPI_REG_CMDCTRL_ADDR_EN_LSB);
		/* Number of bytes to write. */
		reg |= ((addrlen - 1) & CQSPI_REG_CMDCTRL_ADD_BYTES_MASK)
			<< CQSPI_REG_CMDCTRL_ADD_BYTES_LSB;
		/* Get address */
		addr_value = cadence_qspi_apb_cmd2addr(&addrbuf[0], addrlen);
		CQSPI_WRITEL(addr_value, reg_base + CQSPI_REG_CMDADDRESS);
		debug_print("[%s] CQSPI_REG_CMDADDRESS=0x%x\n", __func__,
			    addr_value);
	}
	/* 0 means 1 byte. */
	reg |= (((rxlen - 1) & CQSPI_REG_CMDCTRL_RD_BYTES_MASK)
		<< CQSPI_REG_CMDCTRL_RD_BYTES_LSB);
	status = cadence_qspi_apb_exec_flash_cmd(reg_base, reg);
	if (status != 0)
		return status;

	/* Put read value into rxbuf. STIG data is in LE format.
	 * We should not use direct pointer cast to ensure it works
	 * regardless of CPU endianness.
	 */
	for (i = 0; i < rxlen; i++) {
		if (i == 0) {
			reg = readl(reg_base + CQSPI_REG_CMDREADDATALOWER);
			debug_print("[%s] CQSPI_REG_CMDREADDATALOWER=0x%x\n",
				    __func__, reg);
		} else if (i == 4) {
			reg = readl(reg_base + CQSPI_REG_CMDREADDATAUPPER);
			debug_print("[%s] CQSPI_REG_CMDREADDATAUPPER=0x%x\n",
				    __func__, reg);
		}
		rxbuf[i] = reg & 0xFF;
		reg = reg >> 8;
	}

#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi rx: ", DUMP_PREFIX_NONE,
		       32, 1, rxbuf, rxlen, false);
#endif
	return 0;
}

/* For commands: WRSR, WREN, WRDI, CHIP_ERASE, BE, etc. */
static int cadence_qspi_apb_command_write(void *reg_base,
					  unsigned int txlen,
					  const unsigned char *txbuf,
					  unsigned int addrlen,
					  const unsigned char *addrbuf,
					  unsigned int datalen,
					  const unsigned char *databuf,
					  unsigned int flash_type)
{
	unsigned int reg;
	unsigned int addr_value;
	unsigned int data = 0;
	int i;

	debug_print("%s txlen %d txbuf[0]=0x%x\n", __func__, txlen, txbuf[0]);
#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi tx: ", DUMP_PREFIX_NONE,
		       32, 1, txbuf, txlen, false);
#endif
	if (!txlen || !txbuf) {
		pr_err("QSPI: Invalid input argument, cmdlen %d txbuf %p\n",
		       txlen, txbuf);
		return -EINVAL;
	}
	reg = txbuf[0] << CQSPI_REG_CMDCTRL_OPCODE_LSB;
	if (datalen != 0) {
		if (datalen <= 4) {
			/* Command with data only. */
			reg |= (0x1 << CQSPI_REG_CMDCTRL_WR_EN_LSB);
			reg |= ((datalen - 1) & CQSPI_REG_CMDCTRL_WR_BYTES_MASK)
				<< CQSPI_REG_CMDCTRL_WR_BYTES_LSB;
			for (i = 0; i < datalen; i++)
				data |= databuf[i] << (8 * i);

			/* Write the data */
			CQSPI_WRITEL(data,
				     reg_base + CQSPI_REG_CMDWRITEDATALOWER);
			debug_print("[%s] CQSPI_REG_CMDWRITEDATALOWER=0x%x\n",
				    __func__, data);
		} else {
			/* Command with data only. */
			reg |= (0x1 << CQSPI_REG_CMDCTRL_WR_EN_LSB);
			reg |= ((datalen - 1) & CQSPI_REG_CMDCTRL_WR_BYTES_MASK)
				<< CQSPI_REG_CMDCTRL_WR_BYTES_LSB;
			for (i = 0; i < 4; i++)
				data |= databuf[i] << (8 * i);

			/* Write the data */
			CQSPI_WRITEL(data,
				     reg_base + CQSPI_REG_CMDWRITEDATALOWER);
			debug_print("[%s] CQSPI_REG_CMDWRITEDATALOWER=0x%x\n",
				    __func__, data);

			data = 0;
			for (i = 0; i < datalen - 4; i++)
				data |= databuf[i + 4] << (8 * i);

			/* Write the data */
			CQSPI_WRITEL(data,
				     reg_base + CQSPI_REG_CMDWRITEDATAUPPER);
			debug_print("[%s] CQSPI_REG_CMDWRITEDATAUPPER=0x%x\n",
				    __func__, data);
		}
	}
	if (addrlen) {
		/* Command with address */
		reg |= (0x1 << CQSPI_REG_CMDCTRL_ADDR_EN_LSB);
		/* Number of bytes to write. */
		reg |= ((addrlen - 1) & CQSPI_REG_CMDCTRL_ADD_BYTES_MASK)
			<< CQSPI_REG_CMDCTRL_ADD_BYTES_LSB;
		/* Get address */
		addr_value = cadence_qspi_apb_cmd2addr(&addrbuf[0], addrlen);
		CQSPI_WRITEL(addr_value, reg_base + CQSPI_REG_CMDADDRESS);
		debug_print("[%s] CQSPI_REG_CMDADDRESS=0x%x\n", __func__,
			    addr_value);
	}

	return cadence_qspi_apb_exec_flash_cmd(reg_base, reg);
}

static int cadence_qspi_apb_indirect_read_setup(void *reg_base,
						unsigned int ahb_phy_addr,
						unsigned int txlen,
						const unsigned char *txbuf,
						unsigned int addrlen,
						const unsigned char *addrbuf,
						unsigned int dummy_bytes,
						unsigned int flash_type,
						unsigned int quad_mode)
{
	unsigned int reg;
	unsigned int addr_value;
	unsigned int dummy_clk;

	debug_print("[%s]ahb_phy_addr=0x%x txlen=%d txbuf=%p addrlen=%d\n",
		    __func__, ahb_phy_addr, txlen, txbuf, addrlen);

#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi tx: ", DUMP_PREFIX_NONE,
		       32, 1, txbuf, txlen, false);
#endif
	CQSPI_WRITEL(ahb_phy_addr, reg_base + CQSPI_REG_INDIRECTTRIGGER);
	if (addrlen) {
		/* Get address */
		addr_value = cadence_qspi_apb_cmd2addr(&addrbuf[0], addrlen);
		CQSPI_WRITEL(addr_value,
			     reg_base + CQSPI_REG_INDIRECTRDSTARTADDR);
		debug_print("[%s]CQSPI_REG_INDIRECTRDSTARTADDR=0x%x\n",
			    __func__, addr_value);
	}

	/* Configure the opcode */
	reg = txbuf[0] << CQSPI_REG_RD_INSTR_OPCODE_LSB;
	/* Configure the quad */
	reg |= (quad_mode & CQSPI_REG_RD_INSTR_TYPE_DATA_MASK) <<
	       CQSPI_REG_RD_INSTR_TYPE_DATA_LSB;

	/* Setup dummy clock cycles */
	if (dummy_bytes) {
		if (dummy_bytes > CQSPI_DUMMY_BYTES_MAX)
			dummy_bytes = CQSPI_DUMMY_BYTES_MAX;
		reg |= (1 << CQSPI_REG_RD_INSTR_MODE_EN_LSB);
		/* Set all high to ensure chip doesn't enter XIP */
		CQSPI_WRITEL(0xFF, reg_base + CQSPI_REG_MODE_BIT);
		/* Convert to clock cycles. */
		dummy_clk = dummy_bytes * CQSPI_DUMMY_CLKS_PER_BYTE;
		/* Need to minus the mode byte (8 clocks). */
		dummy_clk -= CQSPI_DUMMY_CLKS_PER_BYTE;
		if (dummy_clk)
			reg |= (dummy_clk & CQSPI_REG_RD_INSTR_DUMMY_MASK)
				<< CQSPI_REG_RD_INSTR_DUMMY_LSB;
	} else if (flash_type == QSPI_FLASH_TYPE_NAND) {
		/* for qspi nand, we need at least 8 dummy cycles for rd */
		reg |= ((1 * CQSPI_DUMMY_CLKS_PER_BYTE)
				<< CQSPI_REG_RD_INSTR_DUMMY_LSB);
	}
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_RD_INSTR);
	debug_print("[%s]quad_mode=%d CQSPI_REG_RD_INSTR=0x%x\n", __func__,
		    quad_mode, reg);
	/* Set device size */
	reg = CQSPI_READL(reg_base + CQSPI_REG_SIZE);
	reg &= ~CQSPI_REG_SIZE_ADDRESS_MASK;
	reg |= (addrlen - 1);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_SIZE);
	debug_print("[%s]CQSPI_REG_SIZE=0x%x\n", __func__, reg);
	/* disable auto-polling */
	reg = CQSPI_READL(reg_base + CQSPI_REG_WRITE_COMPLETION_CTRL);
	reg |= (1 << WRITE_COMPLETION_CTRL_REG_DISABLE_POLLING_FLD_POS);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_WRITE_COMPLETION_CTRL);
	debug_print("[%s]CQSPI_REG_WRITE_COMPLETION_CTRL=0x%x\n", __func__,
		    reg);

	return 0;
}

static int cadence_qspi_apb_indirect_read_execute(
	struct struct_cqspi *cadence_qspi, unsigned int rxlen,
	unsigned char *rxbuf, unsigned int flash_type)
{
	unsigned int reg = 0;
	unsigned long timeout;
	struct platform_device *pdev = cadence_qspi->pdev;
	struct cqspi_platform_data *pdata = pdev->dev.platform_data;
	void *reg_base = cadence_qspi->iobase;
	void *ahb_base = cadence_qspi->qspi_ahb_virt;
	int remaining = (int)rxlen;
	int ret = 0;
	unsigned char *buf = rxbuf;
	unsigned long flags;

	debug_print("%s rxlen %d rxbuf %p\n", __func__, rxlen, rxbuf);
	CQSPI_WRITEL(0, reg_base + CQSPI_REG_INDIRECTRDWATERMARK);

	CQSPI_WRITEL(0xa, reg_base + CQSPI_INDIRECT_TRIGGER_ADDR_RANGE_REG);

	CQSPI_WRITEL(remaining, reg_base + CQSPI_REG_INDIRECTRDBYTES);
	debug_print("[%s]CQSPI_REG_INDIRECTRDBYTES=0x%x\n", __func__,
		    remaining);

	mb();

	CQSPI_WRITEL(pdata->fifo_depth - CQSPI_REG_SRAM_RESV_WORDS,
		     reg_base + CQSPI_REG_SRAMPARTITION);
	/* Clear all interrupts. */
	CQSPI_WRITEL(CQSPI_IRQ_STATUS_MASK, reg_base + CQSPI_REG_IRQSTATUS);
	CQSPI_WRITEL(0, reg_base + CQSPI_REG_IRQMASK);

	/* Start the indirect read transfer */
	CQSPI_WRITEL(CQSPI_REG_INDIRECTRD_START_MASK,
		     reg_base + CQSPI_REG_INDIRECTRD);
	debug_print("[%s]CQSPI_REG_INDIRECTRD=0x%x\n", __func__,
		    CQSPI_READL(reg_base + CQSPI_REG_INDIRECTRD));

	while (remaining > 0) {
		reg = CQSPI_GET_RD_SRAM_LEVEL(reg_base);
		/* convert to bytes */
		reg *= CQSPI_FIFO_WIDTH;
		reg = reg > remaining ? remaining : reg;
		/* Read data from FIFO. */
		spin_lock_irqsave(&cadence_qspi->lock, flags);
		cadence_qspi_apb_read_fifo_data(buf, ahb_base, reg,
						flash_type);
		spin_unlock_irqrestore(&cadence_qspi->lock, flags);

		buf += reg;
		remaining -= reg;
	}

	/* Check indirect done status */
	timeout = cadence_qspi_init_timeout(CQSPI_TIMEOUT_MS);
	while (cadence_qspi_check_timeout(timeout)) {
		reg = CQSPI_READL(reg_base + CQSPI_REG_INDIRECTRD);
		if (reg & CQSPI_REG_INDIRECTRD_DONE_MASK)
			break;
	}
	if (!(reg & CQSPI_REG_INDIRECTRD_DONE_MASK)) {
		pr_err("QSPI : Indirect read completion status error with "
			"reg 0x%08x\n", reg);
		ret = -ETIMEDOUT;
		goto failrd;
	}
	/* Disable interrupt */
	CQSPI_WRITEL(0, reg_base + CQSPI_REG_IRQMASK);
	/* Clear indirect completion status */
	CQSPI_WRITEL(CQSPI_REG_INDIRECTRD_DONE_MASK,
		     reg_base + CQSPI_REG_INDIRECTRD);
#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi rx: ", DUMP_PREFIX_NONE,
		       32, 1, rxbuf, rxlen, false);
#endif
	return 0;
failrd:
	/* Disable interrupt */
	CQSPI_WRITEL(0, reg_base + CQSPI_REG_IRQMASK);
	/* Cancel the indirect read */
	CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_CANCEL_MASK,
		     reg_base + CQSPI_REG_INDIRECTRD);
	return ret;
}

static int cadence_qspi_apb_indirect_write_setup(void *reg_base,
						 unsigned int ahb_phy_addr,
						 unsigned int txlen,
						 const unsigned char *txbuf,
						 unsigned int addrlen,
						 const unsigned char *addrbuf,
						 unsigned int flash_type,
						 unsigned int quad_mode)
{
	unsigned int reg;
	unsigned int addr_bytes = txlen;

	debug_print("%s txlen %d txbuf %p addr_bytes %d\n",
		    __func__, txlen, txbuf, addr_bytes);
#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi tx: ", DUMP_PREFIX_NONE,
		       32, 1, txbuf, txlen, false);
#endif
	if (!txbuf) {
		pr_err("QSPI: Invalid input argument, txlen %d txbuf %p\n",
		       txlen, txbuf);
		return -EINVAL;
	}
	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg &= ~(CQSPI_REG_CONFIG_DIRECT_MASK);
	reg &= ~(CQSPI_REG_CONFIG_DMA_MASK);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);

	CQSPI_WRITEL(ahb_phy_addr, reg_base + CQSPI_REG_INDIRECTTRIGGER);
	/* Set opcode. */
	reg = txbuf[0] << CQSPI_REG_WR_INSTR_OPCODE_LSB;
	reg |= (1 << DEV_INSTR_WR_CONFIG_REG_WEL_DIS_FLD_POS);
	if (txbuf[0] == MACRONIX_4PP_CMD &&
	    flash_type == QSPI_FLASH_TYPE_NOR &&
	    quad_mode == QUAD_QIO) {
		/* Configure the quad */
		reg |= (quad_mode & CQSPI_REG_WR_INSTR_TYPE_ADDR_MASK) <<
		       CQSPI_REG_WR_INSTR_TYPE_ADDR_LSB;
	}
	/* Configure the quad */
	reg |= (quad_mode & CQSPI_REG_WR_INSTR_TYPE_DATA_MASK) <<
	       CQSPI_REG_WR_INSTR_TYPE_DATA_LSB;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_WR_INSTR);
	debug_print("[%s]quad_mode=%d CQSPI_REG_WR_INSTR=0x%x\n", __func__,
		    quad_mode, reg);
	/* Setup write address. */
	reg = cadence_qspi_apb_cmd2addr(&addrbuf[0], addrlen);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_INDIRECTWRSTARTADDR);
	reg = CQSPI_READL(reg_base + CQSPI_REG_SIZE);
	reg &= ~CQSPI_REG_SIZE_ADDRESS_MASK;
	reg |= ((addrlen - 1) & CQSPI_REG_SIZE_ADDRESS_MASK);
	CQSPI_WRITEL(reg, reg_base +  CQSPI_REG_SIZE);

	/* disable auto-polling */
	reg = CQSPI_READL(reg_base + CQSPI_REG_WRITE_COMPLETION_CTRL);
	reg |= (1 << 14);
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_WRITE_COMPLETION_CTRL);
	return 0;
}

static int cadence_qspi_apb_indirect_write_execute(
	struct struct_cqspi *cadence_qspi, unsigned int txlen,
	const unsigned char *txbuf, unsigned int flash_type)
{
	int ret;
	unsigned int reg = 0;
	void *reg_base = cadence_qspi->iobase;
	void *ahb_base = cadence_qspi->qspi_ahb_virt;
	struct platform_device *pdev = cadence_qspi->pdev;
	struct cqspi_platform_data *pdata = pdev->dev.platform_data;
	struct cqspi_flash_pdata *f_pdata =
			&pdata->f_pdata[cadence_qspi->current_cs];
	unsigned int page_size = f_pdata->page_size;
	int remaining = (int)txlen;
	u32 val;
	unsigned int write_bytes;
	u16 fill_level = 0;
	unsigned long flags;

	debug_print("%s txlen %d txbuf %p\n", __func__, txlen, txbuf);
#ifdef DEBUG
	/* Dump the QSPI command */
	print_hex_dump(KERN_DEBUG, "qspi tx: ", DUMP_PREFIX_NONE,
		       32, 1, txbuf, txlen, false);
#endif
	CQSPI_WRITEL(0xa, reg_base + CQSPI_INDIRECT_TRIGGER_ADDR_RANGE_REG);
	CQSPI_WRITEL(0x0, reg_base + CQSPI_REG_INDIRECTWRWATERMARK);
	reg = CQSPI_READL(reg_base + CQSPI_REG_SIZE);
	reg &= ~(CQSPI_REG_SIZE_PAGE_MASK << CQSPI_REG_SIZE_PAGE_LSB);
	reg &= ~(CQSPI_REG_SIZE_BLOCK_MASK << CQSPI_REG_SIZE_BLOCK_LSB);
	reg |= (f_pdata->page_size << CQSPI_REG_SIZE_PAGE_LSB);
	reg |= (f_pdata->block_size << CQSPI_REG_SIZE_BLOCK_LSB);
	CQSPI_WRITEL(reg, reg_base +  CQSPI_REG_SIZE);


	CQSPI_WRITEL(CQSPI_REG_SRAM_PARTITION_WR,
		     reg_base + CQSPI_REG_SRAMPARTITION);

	while (remaining > 0) {
		/* Calculate number of bytes to write. */
		write_bytes = remaining > page_size ? page_size : remaining;

		CQSPI_WRITEL(write_bytes, reg_base + CQSPI_REG_INDIRECTWRBYTES);

		/* Clear all interrupts. */
		CQSPI_WRITEL(CQSPI_IRQ_STATUS_MASK, reg_base + CQSPI_REG_IRQSTATUS);
		CQSPI_WRITEL(CQSPI_IRQ_MASK_WR, reg_base + CQSPI_REG_IRQMASK);

		/* Ensure everything is ready before starting */
		wmb();

		reinit_completion(&cadence_qspi->transfer_complete);

		/* In case of QSPI write, even if CPU(600/800MHz) is much
		 * faster than SPI baudrate (50MHz), without lock, QSPI write
		 * could be pre-empted by other interrupts and tasklets/timers,
		 * it may cause QSPI write underrun. When QSPI write underrun occurs,
		 * SRAM somehow stalls. Once QSPI reads SRAM content, it will hang up.
		*/
		spin_lock_irqsave(&cadence_qspi->lock, flags);
		/* Start Write */
		CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_START_MASK,
					 reg_base + CQSPI_REG_INDIRECTWR);
		cadence_qspi_apb_write_fifo_data(ahb_base, txbuf, write_bytes,
						 flash_type);
		spin_unlock_irqrestore(&cadence_qspi->lock, flags);

		ret = wait_for_completion_timeout(&cadence_qspi->transfer_complete,
						  msecs_to_jiffies(CQSPI_TIMEOUT_MS));
		if (!ret) {
			pr_err("QSPI: Indirect write timeout\n");
			ret = -ETIMEDOUT;
			goto failwr;
		} else {
			ret = 0;
		}
		txbuf  += write_bytes;
		remaining -= write_bytes;

		ret = readl_poll_timeout(reg_base + CQSPI_REG_INDIRECTWR, val,
				 val & CQSPI_REG_INDIRECTWR_DONE_MASK,
				 10, 1000 * USEC_PER_MSEC);
		if (ret) {
			pr_err("QSPI: Indirect write completion status error with  0x%08x\n", val);
			goto failwr;
		}
	}

	/* We observe issues in underrun condition (i.e. in high SPI clock
	 * or low CPU clock) in which write transfer fail in
	 * between, which eventually causes issues at higher layer (e.g. file
	 * system corruption). To workaround, we check the sram fill level
	 * after write. If it is not zero, we assume transfer failure, do hard
	 * recovery, and return -EAGAIN so that user layer can retry operation
	 * in a clean way.
	 */
	fill_level = (((CQSPI_READL(reg_base + CQSPI_REG_SDRAMLEVEL)) >>
			 CQSPI_REG_SDRAMLEVEL_WR_LSB) &
			 CQSPI_REG_SDRAMLEVEL_WR_MASK);
	if (fill_level) {
				pr_debug("%s fill level is %u\n", __func__, fill_level);

				CQSPI_WRITEL(0, reg_base + CQSPI_REG_IRQMASK);
				CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_DONE_MASK,
					     reg_base + CQSPI_REG_INDIRECTWR);
				CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_CANCEL_MASK,
					     reg_base + CQSPI_REG_INDIRECTWR);

				cadence_qspi_apb_controller_init(cadence_qspi);
				return -EAGAIN;
	}

failwr:
	/* Disable interrupt. */
	CQSPI_WRITEL(0, reg_base + CQSPI_REG_IRQMASK);
	/* Clear indirect completion status */
	CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_DONE_MASK,
		     reg_base + CQSPI_REG_INDIRECTWR);

	/* Cancel the indirect write */
	if (ret)
		CQSPI_WRITEL(CQSPI_REG_INDIRECTWR_CANCEL_MASK,
			     reg_base + CQSPI_REG_INDIRECTWR);

	return ret;
}

void cadence_qspi_apb_controller_enable(void *reg_base)
{
	unsigned int reg;

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg |= CQSPI_REG_CONFIG_ENABLE_MASK;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
}

void cadence_qspi_apb_controller_disable(void *reg_base)
{
	unsigned int reg;

	reg = CQSPI_READL(reg_base + CQSPI_REG_CONFIG);
	reg &= ~CQSPI_REG_CONFIG_ENABLE_MASK;
	CQSPI_WRITEL(reg, reg_base + CQSPI_REG_CONFIG);
}

unsigned int cadence_qspi_apb_is_controller_ready(void *reg_base)
{
	return cadence_qspi_wait_idle(reg_base);
}

void cadence_qspi_apb_controller_init(struct struct_cqspi *cadence_qspi)
{
	/* reset */
	reset_control_assert(cadence_qspi->reset);
	udelay(1);
	reset_control_deassert(cadence_qspi->reset);

	cadence_qspi_apb_controller_disable(cadence_qspi->iobase);

	/* Configure the remap address register, no remap */
	CQSPI_WRITEL(0, cadence_qspi->iobase + CQSPI_REG_REMAP);
	/* Disable all interrupts. */
	CQSPI_WRITEL(0, cadence_qspi->iobase + CQSPI_REG_IRQMASK);

	cadence_qspi_apb_controller_enable(cadence_qspi->iobase);
}

unsigned int calculate_ticks_for_ns(unsigned int ref_clk_hz,
				    unsigned int ns_val)
{
	unsigned int ticks;

	ticks = ref_clk_hz;
	ticks /= 1000;
	ticks *= ns_val;
	ticks +=  999999;
	ticks /= 1000000;
	return ticks;
}

void cadence_qspi_apb_delay(struct struct_cqspi *cadence_qspi,
			    unsigned int ref_clk, unsigned int sclk_hz)
{
	void __iomem *iobase = cadence_qspi->iobase;
	struct platform_device *pdev = cadence_qspi->pdev;
	struct cqspi_platform_data *pdata = pdev->dev.platform_data;
	struct cqspi_flash_pdata *f_pdata =
			&pdata->f_pdata[cadence_qspi->current_cs];
	unsigned int ref_clk_ns;
	unsigned int sclk_ns;
	unsigned int tshsl, tchsh, tslch, tsd2d;
	unsigned int reg;
	unsigned int tsclk;

	debug_print("%s %d %d\n", __func__, ref_clk, sclk_hz);

	cadence_qspi_apb_controller_disable(cadence_qspi->iobase);
	/* Convert to ns. */
	ref_clk_ns = (1000000000) / pdata->master_ref_clk_hz;
	/* Convert to ns. */
	sclk_ns = (1000000000) / sclk_hz;
	/* calculate the number of ref ticks for one sclk tick */
	tsclk = (pdata->master_ref_clk_hz + sclk_hz - 1) / sclk_hz;
	tshsl = calculate_ticks_for_ns(pdata->master_ref_clk_hz,
				       f_pdata->tshsl_ns);
	/* this particular value must be at least one sclk */
	if (tshsl < tsclk)
		tshsl = tsclk;
	tchsh = calculate_ticks_for_ns(pdata->master_ref_clk_hz,
				       f_pdata->tchsh_ns);
	tslch = calculate_ticks_for_ns(pdata->master_ref_clk_hz,
				       f_pdata->tslch_ns);
	tsd2d = calculate_ticks_for_ns(pdata->master_ref_clk_hz,
				       f_pdata->tsd2d_ns);
	debug_print("%s tshsl %d tsd2d %d tchsh %d tslch %d\n",
		    __func__, tshsl, tsd2d, tchsh, tslch);
	reg = ((tshsl & CQSPI_REG_DELAY_TSHSL_MASK)
			<< CQSPI_REG_DELAY_TSHSL_LSB);
	reg |= ((tchsh & CQSPI_REG_DELAY_TCHSH_MASK)
			<< CQSPI_REG_DELAY_TCHSH_LSB);
	reg |= ((tslch & CQSPI_REG_DELAY_TSLCH_MASK)
			<< CQSPI_REG_DELAY_TSLCH_LSB);
	reg |= ((tsd2d & CQSPI_REG_DELAY_TSD2D_MASK)
			<< CQSPI_REG_DELAY_TSD2D_LSB);
	CQSPI_WRITEL(reg, iobase + CQSPI_REG_DELAY);
	cadence_qspi_apb_controller_enable(cadence_qspi->iobase);
}

void cadence_qspi_switch_cs(struct struct_cqspi *cadence_qspi,
			    unsigned int cs)
{
	unsigned int reg;
	struct platform_device *pdev = cadence_qspi->pdev;
	struct cqspi_platform_data *pdata = pdev->dev.platform_data;
	struct cqspi_flash_pdata *f_pdata = &pdata->f_pdata[cs];
	void __iomem *iobase = cadence_qspi->iobase;

	debug_print("%s\n", __func__);
	cadence_qspi_apb_controller_disable(iobase);
	/* Configure page size and block size. */
	reg = CQSPI_READL(iobase + CQSPI_REG_SIZE);
	/* clear the previous value */
	reg &= ~(CQSPI_REG_SIZE_PAGE_MASK << CQSPI_REG_SIZE_PAGE_LSB);
	reg &= ~(CQSPI_REG_SIZE_BLOCK_MASK << CQSPI_REG_SIZE_BLOCK_LSB);
	reg |= (f_pdata->page_size << CQSPI_REG_SIZE_PAGE_LSB);
	reg |= (f_pdata->block_size << CQSPI_REG_SIZE_BLOCK_LSB);
	CQSPI_WRITEL(reg, iobase + CQSPI_REG_SIZE);
	debug_print("[%s]f_pdata->page_size=0x%x f_pdata->block_size=0x%x\n",
		    __func__, f_pdata->page_size, f_pdata->block_size);
	debug_print("[%s]CQSPI_REG_SIZE=0x%x\n", __func__,
		    CQSPI_READL(iobase + CQSPI_REG_SIZE));
	/* configure the chip select */
	cadence_qspi_apb_chipselect(iobase, cs, pdata->ext_decoder);
	cadence_qspi_apb_controller_enable(iobase);
}

int cadence_qspi_apb_process_queue(struct struct_cqspi *cadence_qspi,
				   struct spi_device *spi, unsigned int n_trans,
				   struct spi_transfer **spi_xfer)
{
	struct platform_device *pdev = cadence_qspi->pdev;
	struct cqspi_platform_data *pdata = pdev->dev.platform_data;
	struct cqspi_flash_pdata *f_pdata;
	void __iomem *iobase = cadence_qspi->iobase;
	struct spi_transfer *cmd_xfer = NULL;
	struct spi_transfer *data_xfer = NULL;
	struct spi_transfer *dummy_xfer = NULL;
	struct spi_transfer *addr_xfer = NULL;
	int i;
	int ret;
	int mode;
	unsigned int sclk;
	int rdid_length;

	for (i = 0; i < n_trans; i++)
		dev_dbg(&pdev->dev, "xfer[%d] %d\n", i, spi_xfer[i]->len);

	/* Switch chip select. */
	if (cadence_qspi->current_cs != spi->chip_select) {
		cadence_qspi->current_cs = spi->chip_select;
		cadence_qspi_switch_cs(cadence_qspi, spi->chip_select);
	}

	f_pdata = &pdata->f_pdata[cadence_qspi->current_cs];
	if (f_pdata->flash_type == QSPI_FLASH_TYPE_NOR)
		rdid_length = 6; /* SPI_NOR_MAX_ID_LEN */
	else
		rdid_length = 3;

	/* deduce transfer type from number of transactions */
	switch (n_trans) {
	case 1: /* cmd only */
		cmd_xfer = spi_xfer[0];
		break;
	case 2: /* cmd + data (rx) or cmd + addr (tx) */
		cmd_xfer = spi_xfer[0];
		if (f_pdata->flash_type == QSPI_FLASH_TYPE_NAND) {
			if (spi_xfer[1]->rx_buf)
				data_xfer = spi_xfer[1];
			else
				addr_xfer = spi_xfer[1];
		} else {
			data_xfer = spi_xfer[1];
		}
		break;
	case 3: /* cmd + addr + data */
		cmd_xfer = spi_xfer[0];
		addr_xfer = spi_xfer[1];
		data_xfer = spi_xfer[2];
		break;
	case 4: /* cmd + addr + dummy + data */
		cmd_xfer = spi_xfer[0];
		addr_xfer = spi_xfer[1];
		dummy_xfer = spi_xfer[2];
		data_xfer = spi_xfer[3];
		break;
	default:
		dev_err(&pdev->dev, "Unsupported n_trans %u\n", n_trans);
		return -EINVAL;
	}

	/* determine transfer mode */
	if (!data_xfer)
		mode = STIG_WRITE_MODE;
	else if (data_xfer->len > rdid_length)
		mode = data_xfer->tx_buf ? IDC_WRITE_MODE : IDC_READ_MODE;
	else
		mode = data_xfer->tx_buf ? STIG_WRITE_MODE : STIG_READ_MODE;

	/* Setup baudrate divisor and delays */
	sclk = cmd_xfer->speed_hz ?
		cmd_xfer->speed_hz : spi->max_speed_hz;
	cadence_qspi_apb_controller_disable(iobase);
	cadence_qspi_apb_config_baudrate_div(iobase,
					     pdata->master_ref_clk_hz, sclk);
	cadence_qspi_apb_delay(cadence_qspi,
			       pdata->master_ref_clk_hz, sclk);
	cadence_qspi_apb_readdata_capture(iobase, 1,
					  f_pdata->read_delay);
	cadence_qspi_apb_controller_enable(iobase);

	/* execute transfer */
	switch (mode) {
	case STIG_WRITE_MODE:
		ret = cadence_qspi_apb_command_write(iobase,
			cmd_xfer->len, cmd_xfer->tx_buf,
			addr_xfer ? addr_xfer->len : 0,
			addr_xfer ? addr_xfer->tx_buf : NULL,
			data_xfer ? data_xfer->len : 0,
			data_xfer ? data_xfer->tx_buf : NULL,
			f_pdata->flash_type);
		break;
	case STIG_READ_MODE:
		ret = cadence_qspi_apb_command_read(iobase,
			cmd_xfer->len, cmd_xfer->tx_buf,
			addr_xfer ? addr_xfer->len : 0,
			addr_xfer ? addr_xfer->tx_buf : NULL,
			data_xfer ? data_xfer->len : 0,
			data_xfer ? data_xfer->rx_buf : NULL,
			f_pdata->flash_type);
		break;
	case IDC_WRITE_MODE:
		ret = cadence_qspi_apb_indirect_write_setup(iobase,
			pdata->qspi_ahb_phy & pdata->qspi_ahb_mask,
			cmd_xfer->len, cmd_xfer->tx_buf,
			addr_xfer ? addr_xfer->len : 0,
			addr_xfer ? addr_xfer->tx_buf : NULL,
			f_pdata->flash_type,
			f_pdata->quad);
		ret = cadence_qspi_apb_indirect_write_execute(cadence_qspi,
			data_xfer->len, data_xfer->tx_buf,
			f_pdata->flash_type);
		break;
	case IDC_READ_MODE:
		ret = cadence_qspi_apb_indirect_read_setup(iobase,
			pdata->qspi_ahb_phy & pdata->qspi_ahb_mask,
			cmd_xfer->len, cmd_xfer->tx_buf,
			addr_xfer ? addr_xfer->len : 0,
			addr_xfer ? addr_xfer->tx_buf : NULL,
			dummy_xfer ? dummy_xfer->len : 0,
			f_pdata->flash_type,
			f_pdata->quad);
		ret = cadence_qspi_apb_indirect_read_execute(cadence_qspi,
			data_xfer->len, data_xfer->rx_buf,
			f_pdata->flash_type);
		break;
	default:
		dev_err(&pdev->dev, "Unsupported mode %u\n", mode);
		return -EINVAL;
	}

	return ret;
}

MODULE_LICENSE("Dual BSD/GPL");
