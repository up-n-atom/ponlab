#include "tool_eip123.h"
#include <asm/io.h> 

#ifndef ioremap_nocache
#define ioremap_nocache ioremap
#endif

#define ltq_r32(reg)         __raw_readl(reg)
#define ltq_w32(val, reg)    __raw_writel(val, reg)

#define SSX6_REGBASE 0x13F00000
#define SSX6_REGSIZE 0x00100000

#define SSX0_REGBASE 0x1FF00000
#define SSX0_REGSIZE 0x00100000

#define NGI_ENDIAN_REGBASE 0x1608005C

#ifdef CONFIG_SOC_PRX300_BOOTCORE
#define SSX1_REGBASE 0x18000000
#define SSX1_REGSIZE 0x00100000

#undef NGI_ENDIAN_REGBASE
#define NGI_ENDIAN_REGBASE 0x1618005C
#endif /* CONFIG_SOC_PRX300_BOOTCORE */

static void eip123_tool_hw_init(void)
{
	/*
	 * !<WW: removed! ngi setup should be done by ngi driver in linux
	 * also some of below setting seem to interfere with WLAN association
	*/
	#if 0
	void __iomem* ssx6_base = 0;
	void __iomem* ssx0_base = 0;
	void __iomem* ngi_endian_base = 0;
	
	ssx6_base = ioremap_nocache( SSX6_REGBASE, SSX6_REGSIZE);
	ssx0_base = ioremap_nocache( SSX0_REGBASE, SSX0_REGSIZE);
	ngi_endian_base = ioremap_nocache( 0x1608005C, 4);

	/* SSX6 */
    ltq_w32(0x1f,ssx6_base+0x81458);   //ssb6:ln60
    ltq_w32(0x1f,ssx6_base+0x81450);
    ltq_w32(0x7f,ssx6_base+0x80858);   //ssb6:ddr,memmax ,vector 0
    ltq_w32(0x7f,ssx6_base+0x80850);
    ltq_w32(0x7f,ssx6_base+0x80878);   //ssb6:ddr,memmax ,vector 1
    ltq_w32(0x7f,ssx6_base+0x80870);
    ltq_w32(0x7f,ssx6_base+0x80898);   //ssb6:ddr,memmax ,vector 2
    ltq_w32(0x7f,ssx6_base+0x80890);
    ltq_w32(0x7f,ssx6_base+0x808b8);   //ssb6:ddr,memmax ,vector 3
    ltq_w32(0x7f,ssx6_base+0x808b0);
    ltq_w32(0x7f,ssx6_base+0x808d8);   //ssb6:ddr,memmax ,vector 4
    ltq_w32(0x7f,ssx6_base+0x808d0);
    ltq_w32(0x7f,ssx6_base+0x808f8);   //ssb6:ddr,memmax ,vector 5
    ltq_w32(0x7f,ssx6_base+0x808f0);
    ltq_w32(0x7f,ssx6_base+0x80918);   //ssb6:ddr,memmax ,vector 6
    ltq_w32(0x7f,ssx6_base+0x80910);
    ltq_w32(0x7f,ssx6_base+0x80938);   //ssb6:ddr,memmax ,vector 7
    ltq_w32(0x7f,ssx6_base+0x80930);
    ltq_w32(0x1f,ssx6_base+0x80c58);   //ssb6:ddr,upctl
    ltq_w32(0x1f,ssx6_base+0x80c50);
    ltq_w32(0x1f,ssx6_base+0x81058);   //ssb6:ddr,phy
    ltq_w32(0x1f,ssx6_base+0x81050);
    ltq_w32(0x1f,ssx6_base+0x80458);   //ssb6:iocu
    ltq_w32(0x1f,ssx6_base+0x80450);
    ltq_w32(0x1f,ssx6_base+0x80058);   //ssb6:reg6, region 0
    ltq_w32(0x1f,ssx6_base+0x80050);
    ltq_w32(0x1f,ssx6_base+0x80078);   //ssb6:reg6, region 1
    ltq_w32(0x1f,ssx6_base+0x80070);
	
	/* SSX0 */
    ltq_w32(0x1f,ssx0_base+0x82858);   //ss0:ln01
    ltq_w32(0x1f,ssx0_base+0x82850);
    ltq_w32(0x1f,ssx0_base+0x82C58);   //ss0:ln02
    ltq_w32(0x1f,ssx0_base+0x82C50);
    ltq_w32(0x1f,ssx0_base+0x83058);   //ss0:ln03
    ltq_w32(0x1f,ssx0_base+0x83058);
    ltq_w32(0x1f,ssx0_base+0x83458);   //ss0:ex04
    ltq_w32(0x1f,ssx0_base+0x83450);
    ltq_w32(0x1f,ssx0_base+0x83858);   //ss0:ex05
    ltq_w32(0x1f,ssx0_base+0x83850);
    ltq_w32(0x1f,ssx0_base+0x83C58);   //ss0:ln06
    ltq_w32(0x1f,ssx0_base+0x83C50);
    ltq_w32(0x1f,ssx0_base+0x80858);   //ss0:rom
    ltq_w32(0x1f,ssx0_base+0x80850);
    ltq_w32(0x1f,ssx0_base+0x80c58);   //ss0:otp
    ltq_w32(0x1f,ssx0_base+0x80c50);
    ltq_w32(0x1f,ssx0_base+0x80458);   //ss0:ssb, region 0
    ltq_w32(0x1f,ssx0_base+0x80450);
    ltq_w32(0x1f,ssx0_base+0x80478);   //ss0:ssb, region 1
    ltq_w32(0x1f,ssx0_base+0x80470);
    ltq_w32(0x1f,ssx0_base+0x80498);   //ss0:ssb, region 2
    ltq_w32(0x1f,ssx0_base+0x80490);
    ltq_w32(0x1f,ssx0_base+0x804b8);   //ss0:ssb, region 3
    ltq_w32(0x1f,ssx0_base+0x804b0);
    ltq_w32(0x1f,ssx0_base+0x81058);   //ssx0, cbm1
    ltq_w32(0x1f,ssx0_base+0x81050);
    ltq_w32(0x1f,ssx0_base+0x81458);   //ssx0, cbm2
    ltq_w32(0x1f,ssx0_base+0x81450);
    ltq_w32(0x1f,ssx0_base+0x82058);   //ssx0, dma3
    ltq_w32(0x1f,ssx0_base+0x82050);
    ltq_w32(0x1f,ssx0_base+0x82458);   //ssx0, dma4
    ltq_w32(0x1f,ssx0_base+0x82450);
    ltq_w32(0x1f,ssx0_base+0x81858);   //ssx0, eip97
    ltq_w32(0x1f,ssx0_base+0x81850);
    ltq_w32(0x1f,ssx0_base+0x81C58);   //ssx0, eip123
    ltq_w32(0x1f,ssx0_base+0x81C50);
    ltq_w32(0x1f,ssx0_base+0x80058);   //ssx0, reg0-r0
    ltq_w32(0x1f,ssx0_base+0x80050);
    ltq_w32(0x1f,ssx0_base+0x80078);   //ssx0, reg0-r1
    ltq_w32(0x1f,ssx0_base+0x80070);
	#endif

	// do APTV_CORE_END to BE here if not done so 
	eip123_tool_set_axi_slave_endian(0); // little endian
	//ltq_w32(0xdffff,ngi_endian_base); // all be
}
#ifdef CONFIG_SOC_PRX300_BOOTCORE
#define REG_AXI_ENDIAN  0x1618004C
#else /* CONFIG_SOC_PRX300_BOOTCORE */
#define REG_AXI_ENDIAN  0x1608004C
#endif /* CONFIG_SOC_PRX300_BOOTCORE */
#define BIT_MASK_EIP123 0x00000008
#define BIT_EIP123      3 

/* ** this set the endian mode of EIP123 in axi slave register 
** 0 = little endian 
** 1 = big endian (default after reset) 
**
** return val :
** 0 -> NO ERROR
** everything else is ERROR
*/
int eip123_tool_set_axi_slave_endian(unsigned char mode)
{
	void __iomem* reg = 0;
	unsigned int  val = 0;

	reg = ioremap_nocache(REG_AXI_ENDIAN, 4);

	val = ltq_r32(reg);	
	
	if( mode )
		ltq_w32( val | (1<<BIT_EIP123), reg);
	else
		ltq_w32( val&(~BIT_MASK_EIP123) , reg);

	return 0; 
}

/* lazy way , without ioremap for testing */
#define SSX0_BASE_ADDR            ssx0_base
#define IE123W_IA_AGENT_CONTROL  (SSX0_BASE_ADDR + 0x10820)
#define IE123W_IA_AGENT_STATUS   (SSX0_BASE_ADDR + 0x10828)
#define IE123R_IA_AGENT_CONTROL  (SSX0_BASE_ADDR + 0x10c20)
#define IE123R_IA_AGENT_STATUS   (SSX0_BASE_ADDR + 0x10c28)
#define TE123_TA_AGENT_CONTROL   (SSX0_BASE_ADDR + 0x2820)
#define TE123_TA_AGENT_STATUS    (SSX0_BASE_ADDR + 0x2828)
/*
** HW reset code the only way i know, the soft reset is screwed btw , hence this is for safety
*/
void eip123_tool_hw_reset(void)
{
	void __iomem* ssx0_base = 0;
	unsigned int rd_data;
	
	eip123_tool_hw_init();

	#ifdef CONFIG_SOC_PRX300_BOOTCORE
	ssx0_base = ioremap_nocache( SSX1_REGBASE, SSX1_REGSIZE);
	#else /* CONFIG_SOC_PRX300_BOOTCORE */
	ssx0_base = ioremap_nocache( SSX0_REGBASE, SSX0_REGSIZE);
	#endif /* CONFIG_SOC_PRX300_BOOTCORE */
	printk( KERN_INFO "IE123W_IA_AGENT_CONTROL.REJECT\n");
    // EIP123 Write IA
    // Set IA_AGENT_CONTROL.REJECT register bit
    rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);
    rd_data = rd_data |(0x1 << 4);
	ltq_w32(rd_data, IE123W_IA_AGENT_CONTROL);

    rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);

	printk( KERN_INFO "IE123W_IA_AGENT_STATUS\n");
    // Poll IA_AGENT_STATUS fields BURST, READEX, RESP_WAITING, and REQ_ACTIVE
    rd_data = ltq_r32(IE123W_IA_AGENT_STATUS);
    while ((rd_data & 0x000000f0) != 0x0){
		rd_data = ltq_r32(IE123W_IA_AGENT_STATUS);
	}

	printk( KERN_INFO "IE123R_IA_AGENT_CONTROL.REJECT\n");
    // EIP123 Read IA
    // Set IA_AGENT_CONTROL.REJECT register bit
    rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);
    rd_data = rd_data |(0x1 << 4);
	ltq_w32(rd_data, IE123R_IA_AGENT_CONTROL);
	
    rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);

	printk( KERN_INFO "IE123R_IA_AGENT_STATUS\n");
    // Poll IA_AGENT_STATUS fields BURST, READEX, RESP_WAITING, and REQ_ACTIVE
	rd_data = ltq_r32(IE123R_IA_AGENT_STATUS);
    while ((rd_data & 0x000000f0) != 0x0){
		rd_data = ltq_r32(IE123R_IA_AGENT_STATUS);
	}
	
	printk( KERN_INFO "TE123_TA_AGENT_CONTROL.REJECT\n");
    // Set TA_AGENT_CONTROL.REJECT register bit
    rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);
    rd_data = rd_data |(0x1 << 4);
	ltq_w32(rd_data, TE123_TA_AGENT_CONTROL);
	
    rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);

    // Poll TA_AGENT_STATUS fields BURST, READEX, REQ_WAITING, and RESP_ACTIVE
    rd_data = ltq_r32(TE123_TA_AGENT_STATUS);
    while ((rd_data & 0x000000f0) != 0x0) {
	rd_data = ltq_r32(TE123_TA_AGENT_STATUS);
    }

	printk( KERN_INFO "TE123_TA_AGENT_CONTROL.CORE_RESET\n");
    // Set TA_AGENT_CONTROL.CORE_RESET register bit
	rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);
    rd_data = rd_data|(0x1);
	ltq_w32(rd_data, TE123_TA_AGENT_CONTROL);
    rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);

	printk( KERN_INFO "IE123W_IA_AGENT_CONTROL.CORE_RESET\n");
    // EIP123 Write IA
    // Set IA_AGENT_CONTROL.CORE_RESET register bit
	rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);
	rd_data = rd_data|(0x1);
	ltq_w32(rd_data, IE123W_IA_AGENT_CONTROL);
    rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);

	printk( KERN_INFO "IE123R_IA_AGENT_CONTROL.CORE_RESET\n");
    // EIP123 Read IA
    // Set IA_AGENT_CONTROL.CORE_RESET register bit
	rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);
	rd_data = rd_data|(0x1);
	ltq_w32(rd_data, IE123R_IA_AGENT_CONTROL);
    rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);

	printk( KERN_INFO "CLR CORE_RESET & REJECT IE123W_IA_AGENT_CONTROL\n");
    // EIP123 Write IA
    // Clear IA_AGENT_CONTROL fields CORE_RESET and REJECT
	rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1 << 4);
	ltq_w32(rd_data, IE123W_IA_AGENT_CONTROL);
	rd_data = ltq_r32(IE123W_IA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1);
	ltq_w32(rd_data, IE123W_IA_AGENT_CONTROL);

    // EIP123 Read IA
	printk( KERN_INFO "CLR CORE_RESET & REJECT IE123R_IA_AGENT_CONTROL\n");
	// Clear IE123R_IA_AGENT_CONTROL fields CORE_RESET and REJECT
	rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1 << 4);
	ltq_w32(rd_data, IE123R_IA_AGENT_CONTROL);
	rd_data = ltq_r32(IE123R_IA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1);
	ltq_w32(rd_data, IE123R_IA_AGENT_CONTROL);

	printk( KERN_INFO "CLR CORE_RESET & REJECT TE123_TA_AGENT_CONTROL\n");
    // Clear TE123_TA_AGENT_CONTROL fields CORE_RESET and REJECT
	rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1 << 4);
	ltq_w32(rd_data, TE123_TA_AGENT_CONTROL);
	rd_data = ltq_r32(TE123_TA_AGENT_CONTROL);
	rd_data = rd_data & ~(0x1);
	ltq_w32(rd_data, TE123_TA_AGENT_CONTROL);

	printk( KERN_INFO "Reset :DONE!\n");
}

