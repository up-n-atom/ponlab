/******************************************************************************

                               Copyright (c) 2013
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define LOG_LOCAL_GID   GID_CCR_PCIE
#define LOG_LOCAL_FID   0

#define __PCIE_IO_MASK__        ((1u << 25) - 1) /* 32M - total PCIe io space */
#define __CCR_IO_MASK__         ((1u << 24) - 1) /* 16M - accessible from the host */
#define __CCR_IO_MASK_G7__      ((1u << 25) - 1) /* 32M - accessible from the host */
/* Checking for ability to access from the host also for alignment to 4-bytes word */
#define __CCR_IO_CHECK__(addr)  ((0u == (~(__CCR_IO_MASK__) & (addr))) && (0u == ((addr) & 3u)))

#define WLAN_CLOCK_XO_MHZ                           40  /* "slow" XO  clock  40 MHz */
#define WLAN_CLOCK_PLL_MHZ                          640 /* "fast" PLL clock 640 MHz */

#define WLAN_TIME_US_TO_CLOCKS_MHZ(mhz, us)         ((mhz) * (us))
#define WLAN_TIME_NS_TO_CLOCKS_MHZ(mhz, ns)         MAX(1, ((mhz) * (ns) / 1000)) /* at least 1 */

/* Concatenate 2 names */
#define _N_N_(name1, name2)     name1 ## _ ## name2

/* Enum with addresses for access to the registers listed in the include file */
/* Item example:
 *  Input   _HW_REG_DEF_(SYS_IF_UM, REG_EMERALD_ENV, SYSTEM_INFO)
 *  Output  SYS_IF_UM_SYSTEM_INFO = REG_EMERALD_ENV_BAR0 + (REG_EMERALD_ENV_SYSTEM_INFO - EMERALD_ENV_BASE_ADDRESS)
*/
typedef enum {
#define _HW_REG_DEF_(ccr_block, reg_block, name) \
            _N_N_(ccr_block, name) = _N_N_(ccr_block, BAR0) + \
            (_N_N_(REG_ ## reg_block, name) - _N_N_(reg_block, BASE_ADDRESS)),

        _PCIE_COMMON_HW_REGS_LIST_
        _PCIE_HW_DEPEND_REGS_LIST_

#undef  _HW_REG_DEF_
} pcie_regs_t;

#define MAC_GEN_REGS_FPGA_VERSION   MAC_GEN_REGS_HYP_MAC_TOP_GPI_INT


/* Checking for ability to access from the host for all listed registers */
#define _HW_REG_DEBUG_  1

#if _HW_REG_DEBUG_

static pcie_regs_t    _pcie_regs_table[] = {
#define _HW_REG_DEF_(ccr_block, reg_block, name)    _N_N_(ccr_block, name),

        _PCIE_COMMON_HW_REGS_LIST_
        _PCIE_HW_DEPEND_REGS_LIST_

#undef  _HW_REG_DEF_
};

static BOOL
_mtlk_ccr_is_addr_valid (pcie_regs_t addr, uint32 ccr_io_mask)
{
    BOOL res = TRUE;
    char *msg = "Ok";

    if (0u != ((addr) & 3u)) {
        msg = "is not word aligned";
        res = FALSE;
    } else if (0u != (~(ccr_io_mask) & (addr))) {
        msg = "is not accessible from the host";
        res = FALSE;
    }

    if (res) {
      ILOG1_DS("PCIE addr 0x%08X %s", addr, msg);
    } else {
      ELOG_DS("PCIE addr 0x%08X %s", addr, msg);
    }

    return res;
}

static int
_mtlk_ccr_pcie_all_regs_validate (BOOL is_hw_gen7)
{
    BOOL    is_valid = TRUE;
    size_t  i;
    uint32 ccr_io_mask = __CCR_IO_MASK__;

    if (is_hw_gen7)
      ccr_io_mask = __CCR_IO_MASK_G7__;

    for (i = 0; i < MTLK_ARRAY_SIZE(_pcie_regs_table); i++) {
        if (!_mtlk_ccr_is_addr_valid(_pcie_regs_table[i], ccr_io_mask)) {
            is_valid = FALSE;
        }
    }

    return (is_valid ?  MTLK_ERR_OK : MTLK_ERR_UNKNOWN);
}

#else /* _HW_REG_DEBUG_ */
static int
_mtlk_ccr_pcie_all_regs_validate (void)
{
    return MTLK_ERR_OK;
}
#endif /* _HW_REG_DEBUG_ */

/*--------------------------------------------------------------*/
/* CCR mem base address */

typedef mtlk_pcie_ccr_t    _mtlk_pcieg6_ccr_t;

#define CCR_MEM_PAS(ccr_mem)        ((volatile void *)((mtlk_pcie_ccr_t *)ccr_mem)->pas)
#define CCR_MEM_ADDR(ccr_mem, addr) (CCR_MEM_PAS(ccr_mem) + (addr))

/* Read/write access using ccr_mem */
#define __ccr_mem_readl(ccr_mem, addr, val)     __ccr_readl(CCR_MEM_ADDR(ccr_mem, addr), (val))
#define __ccr_mem_writel(ccr_mem, val, addr)    __ccr_writel((val), CCR_MEM_ADDR(ccr_mem, addr))
#define __ccr_mem_resetl(ccr_mem, addr, flag)   __ccr_resetl(CCR_MEM_ADDR(ccr_mem, addr), flag)
#define __ccr_mem_setl(ccr_mem, addr, flag)     __ccr_setl(CCR_MEM_ADDR(ccr_mem, addr), flag)

/*--------------------------------------------------------------*/
/* API to CCR registers access */

/* Trace CCR functions names and params */
#define CCR_FUNC_TRACE  0   /* 0 - no debug, 1 - debug */

#if CCR_FUNC_TRACE
#define _CCR_DBG_FUNC_          mtlk_osal_emergency_print("(%04d): %s",     __LINE__, __FUNCTION__);
#define _CCR_DBG_FUNC_ARG1(a)   mtlk_osal_emergency_print("(%04d): %s(%u)", __LINE__, __FUNCTION__, (a));
#define _CCR_DBG_FUNC_ARG2(a,b) mtlk_osal_emergency_print("(%04d): %s(%u, %u)", \
                                                            __LINE__, __FUNCTION__, (a), (b));
#else /* CCR_FUNC_TRACE */
#define _CCR_DBG_FUNC_
#define _CCR_DBG_FUNC_ARG1(a)
#define _CCR_DBG_FUNC_ARG2(a,b)
#endif /* CCR_FUNC_TRACE */

/* Debug print instead of real read/write */
#define CCR_REG_DEBUG   0   /* 0 - no debug, 1 - debug */

#if CCR_REG_DEBUG
/* Write REG with value */
#define _CCR_WRITE_REG_VALUE_(ccr_mem, reg_name, reg_val) \
        { mtlk_osal_emergency_print("(%4d) WR  addr 0x%08X  data 0x%08X", __LINE__, reg_name, reg_val); }
        /* __ccr_mem_writel(ccr_mem, reg_val, reg_name) */

/* Read REG value */
#define _CCR_READ_REG_VALUE_(ccr_mem, reg_name, reg_val) \
        { mtlk_osal_emergency_print("(%4d) RD  addr 0x%08X  data 0x%08X", __LINE__, reg_name, reg_val); }
        /* __ccr_mem_readl(ccr_mem, reg_name, reg_val) */

/* Read/Modify/Write secured operation by BFIELD_INFO */
#define _CCR_RMW_REG_FIELD_(ccr_mem, secure, reg_name, info, value) \
  { uint32 reg_val = 0; \
    mtlk_osal_emergency_print("(%4d) RMW addr 0x%08X  field(%d,%d)  value 0x%08X  secure %d", \
                                __LINE__, reg_name, \
                                _MTLK_BFIELD_INFO_UNPACK(info, 0), \
                                _MTLK_BFIELD_INFO_UNPACK(info, 8), \
                                value, secure); \
    _CCR_READ_REG_VALUE_(ccr_mem, reg_name, reg_val); \
    MTLK_BFIELD_SET(reg_val, info, value); \
    if (secure) __pcie_open_secure_write_register_upper(ccr_mem); \
    _CCR_WRITE_REG_VALUE_(ccr_mem, reg_name, reg_val); \
  }

#else /* CCR_REG_DEBUG */

/* Write REG with value */
#define _CCR_WRITE_REG_VALUE_(ccr_mem, reg_name, reg_val) \
    __ccr_mem_writel(ccr_mem, reg_val, reg_name)

/* Read REG value */
#define _CCR_READ_REG_VALUE_(ccr_mem, reg_name, reg_val) \
    __ccr_mem_readl(ccr_mem, reg_name, reg_val)

/* Read/Modify/Write secured operation by BFIELD_INFO */
#define _CCR_RMW_REG_FIELD_(ccr_mem, secure, reg_name, info, value) \
  { uint32 reg_val; \
    MTLK_HW_RW_REORD_DELAY; \
    _CCR_READ_REG_VALUE_(ccr_mem, reg_name, reg_val); \
    rmb(); \
    MTLK_BFIELD_SET(reg_val, info, value); \
    if (secure) __pcie_open_secure_write_register_upper(ccr_mem); \
    _CCR_WRITE_REG_VALUE_(ccr_mem, reg_name, reg_val); \
    wmb(); \
  }

#endif /* CCR_REG_DEBUG */

/*--------------------------------------------------------------*/

#define MTLK_GEN6_DISABLE_INTERRUPT     (0x0)

#define WAVE_PCIE_MIPS_FREQ_DEFAULT  240

#define WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_OFFSET (0x00000000)
#define WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_MASK   (0x0000FFFF)


#define WAVE_PCIE_FPGA_VERSION_MINOR_OFFSET                  (0)
#define WAVE_PCIE_FPGA_VERSION_MINOR_MASK                    (0x0000000F)
#define WAVE_PCIE_FPGA_VERSION_MAJOR_OFFSET                  (4)
#define WAVE_PCIE_FPGA_VERSION_MAJOR_MASK                    (0x000000F0)
#define WAVE_PCIE_WLAN_HW_BASE_VERSION_MINOR_OFFSET          (8)
#define WAVE_PCIE_WLAN_HW_BASE_VERSION_MINOR_MASK            (0x00003F00)
#define WAVE_PCIE_WLAN_HW_BASE_VERSION_MAJOR_OFFSET          (14)
#define WAVE_PCIE_WLAN_HW_BASE_VERSION_MAJOR_MASK            (0x0000C000)

/* ============================================== */

/* FIXME: access to REG MIPS_CTL have to be defined using FW shared headers */
#define G6_MIPS_RESET                 0x02
#define G6_RELEASE_FROM_RESET         0x01
 
/*--------------------------------------------------------------*/

#define WAVE_PCIE_REG_SOC_REGS_RCU_BOOT_MSPS_STATUS_OFFSET  (0x00000008)
#define WAVE_PCIE_REG_SOC_REGS_RCU_BOOT_MSPS_STATUS_MASK    (0x00000300)
#define MSPS_STATUS_MODE_DUAL_PCI                           (0x00)

static __INLINE BOOL
_wave_pcie_bar1_is_mode_dual_pci (void *bar1)
{
  void *addr = bar1 + SOC_REGS_RCU_BOOT;
  uint32 rcu_boot;
  uint32 msps_status;

  __ccr_readl(addr, rcu_boot);
  msps_status = rcu_boot & WAVE_PCIE_REG_SOC_REGS_RCU_BOOT_MSPS_STATUS_MASK;
  msps_status >>= WAVE_PCIE_REG_SOC_REGS_RCU_BOOT_MSPS_STATUS_OFFSET;
  mtlk_osal_emergency_print("RCU_BOOT: 0x%08X, mspsStatus: 0x%02X, address: 0x%px",
                            rcu_boot, msps_status, addr);
  return (msps_status == MSPS_STATUS_MODE_DUAL_PCI);
}

/*--------------------------------------------------------------*/

static __INLINE uint32
_wave_pcie_bar1_chip_id_read (void *bar1)
{
  uint32 system_info;

  __ccr_readl(bar1 + SYS_IF_UM_SYSTEM_INFO, system_info);
  system_info &=  WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_MASK;
  system_info >>= WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_OFFSET;
  mtlk_osal_emergency_print("PCIE bus ChipID is:0x%04X address is:0x%px",
                            system_info, bar1 + SYS_IF_UM_SYSTEM_INFO);

  return system_info;
}

static uint32
_mtlk_pcie_ccr_read_chip_id(void *ccr_mem)
{
  uint32 system_info;

  __ccr_mem_readl(ccr_mem, SYS_IF_UM_SYSTEM_INFO, system_info);
  system_info &=  WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_MASK;
  system_info >>= WAVE_PCIE_SYS_IF_UM_SYSTEM_INFO_CHIPID_OFFSET;

  mtlk_osal_emergency_print("PCIE bus ChipVersion:%s ChipID is:0x%04X",
                            ((system_info == WAVE_WAVE600_B0_CHIP_ID) ? "A1" :
                            ((system_info == WAVE_WAVE600_D2_CHIP_ID) ? "D2" :
                            ((system_info == WAVE_WAVE600_D2_B0_CHIP_ID) ? "D2_B0" :
                            ((system_info == WAVE_WAVE700_CHIP_ID)    ? "W7" :
                            ((system_info == WAVE_WAVE700_B0_CHIP_ID) ? "W7_B0" : "UNKNOWN"))))),
                            system_info);

  return system_info;
}

static uint32
_mtlk_pcie_ccr_read_uram_version(void *ccr_mem)
{
  uint32 fpga_version, a, b, c, d;

  __ccr_mem_readl(ccr_mem, MAC_GEN_REGS_FPGA_VERSION, fpga_version);
  a = fpga_version & WAVE_PCIE_WLAN_HW_BASE_VERSION_MAJOR_MASK;
  a >>= WAVE_PCIE_WLAN_HW_BASE_VERSION_MAJOR_OFFSET;
  b = fpga_version & WAVE_PCIE_WLAN_HW_BASE_VERSION_MINOR_MASK;
  b >>= WAVE_PCIE_WLAN_HW_BASE_VERSION_MINOR_OFFSET;
  c = fpga_version & WAVE_PCIE_FPGA_VERSION_MAJOR_MASK;
  c >>= WAVE_PCIE_FPGA_VERSION_MAJOR_OFFSET;
  d = fpga_version & WAVE_PCIE_FPGA_VERSION_MINOR_MASK;
  d >>= WAVE_PCIE_FPGA_VERSION_MINOR_OFFSET;

  return fpga_version;
}

BOOL __MTLK_IFUNC   wave_hw_radio_band_cfg_is_single (mtlk_hw_t *hw);
static int
_mtlk_pcie_ccr_get_hw_dump_info (void *ccr_mem, wave_hw_dump_info_t **hw_dump_info)
{
  MTLK_UNREFERENCED_PARAM(ccr_mem);
  MTLK_UNREFERENCED_PARAM(hw_dump_info);
  return 0; /* zero size */
}

#ifdef CPTCFG_IWLWAV_TSF_TIMER_ACCESS_ENABLED
static void
_mtlk_pcie_read_hw_timestamp(void *ccr_mem, uint32 *low, uint32 *high)
{
  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, TSF_TIMER_TSF_TIMER_LOW_RPORT, *low);
  __ccr_mem_readl(ccr_mem, TSF_TIMER_TSF_TIMER_HIGH_RPORT, *high);
}
#endif /* CPTCFG_IWLWAV_TSF_TIMER_ACCESS_ENABLED */

#if (!MTLK_CCR_DEBUG_PRINT_INTR_REGS)
#define _mtlk_pcie_ccr_print_irq_regs mtlk_ccr_dummy_ret_void
#endif /* MTLK_CCR_DEBUG_PRINT_INTR_REGS */

#define _mtlk_pcie_get_afe_value mtlk_ccr_dummy_ret_void_array
#define _mtlk_pcie_bist_efuse mtlk_ccr_dummy_ret_void_param
#define _mtlk_pcie_exit_debug_mode mtlk_ccr_dummy_ret_void
#define _mtlk_pcie_release_ctl_from_reset mtlk_ccr_dummy_ret_void_param

/* Put interrupt-related functions into single section */
static void   _mtlk_pcie_enable_interrupts(void *ccr_mem, uint32 mask)                    __MTLK_INT_HANDLER_SECTION;
static void   _mtlk_pcie_disable_interrupts(void *ccr_mem, uint32 mask)                   __MTLK_INT_HANDLER_SECTION;
static uint32 _mtlk_pcie_clear_interrupts_if_pending(void *ccr_mem, uint32 clear_mask)    __MTLK_INT_HANDLER_SECTION;
static BOOL   _mtlk_pcie_disable_interrupts_if_pending(void *ccr_mem, uint32 mask)        __MTLK_INT_HANDLER_SECTION;
static void   _mtlk_pcie_initiate_doorbell_interrupt(void *ccr_mem)                       __MTLK_INT_HANDLER_SECTION;
static void   _mtlk_pcie_ccr_ctrl_ring_init(void *ccr_mem, void *ring_regs, unsigned char *mmb_base) __MTLK_INT_HANDLER_SECTION;
static uint32 _mtlk_pcie_ccr_ring_get_hds_to_process(void *ccr_mem, void *ring_regs)      __MTLK_INT_HANDLER_SECTION;
static uint32 _mtlk_pcie_is_interrupt_pending(void *ccr_mem)                              __MTLK_INT_HANDLER_SECTION;
static void   _mtlk_pcie_recover_interrupts(void *ccr_mem)                                __MTLK_INT_HANDLER_SECTION;

static void
_mtlk_pcie_enable_interrupts(void *ccr_mem, uint32 mask)
{
  MTLK_ASSERT(NULL != ccr_mem);

  /* Note: check the implementation for the future FPGA */
  __ccr_mem_writel(ccr_mem, mask, HOST_IF_IRQS_HOST_IRQ_MASK);
}

static void
_mtlk_pcie_disable_interrupts(void *ccr_mem, uint32 mask)
{
  MTLK_ASSERT(NULL != ccr_mem);

  /* Note: check the implementation for the future FPGA */
  __ccr_mem_writel(ccr_mem, MTLK_GEN6_DISABLE_INTERRUPT, HOST_IF_IRQS_HOST_IRQ_MASK);
}

static uint32
_mtlk_pcie_is_interrupt_pending (void *ccr_mem)
{
  uint32 status;

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_STATUS, status);

   return status;
}

static void
_mtlk_pcie_recover_interrupts (void *ccr_mem)
{
  uint32 irq_mask_dummy;
  uint32 irq_mask_cur;

  MTLK_ASSERT(NULL != ccr_mem);

  /* Read current status of interrupt mask */
  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_MASK, irq_mask_cur);

  /* Disable interrupts */
  __ccr_mem_writel(ccr_mem, MTLK_GEN6_DISABLE_INTERRUPT, HOST_IF_IRQS_HOST_IRQ_MASK);
#if LINUX_VERSION_IS_LESS(5,2,0)
  mmiowb();
#else
  wmb();
#endif
  /* Read back to ensure transaction is reached end point */
  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_MASK, irq_mask_dummy);
  /* Enable interrupts even they were disabled side effect: one dummy interrupt */
  __ccr_mem_writel(ccr_mem, MTLK_IRQ_ALL, HOST_IF_IRQS_HOST_IRQ_MASK);
}

/* WARNING                                                      */
/* Currently we do not utilize (and have no plans to utilize)   */
/* interrupt sharing on Gen 6 platform. However, Gen 6          */
/* hardware supports this ability.                              */
/* For now, in all *_if_pending functions we assume there is no */
/* interrupt sharing, so we may not check whether arrived       */
/* interrupt is our. This save us CPU cycles and code lines.    */
/* In case interrupt sharing will be used, additional checks    */
/* for interrupt appurtenance to be added into these functions. */

static uint32
_mtlk_pcie_clear_interrupts_if_pending(void *ccr_mem, uint32 clear_mask)
{
  uint32 status;

  MTLK_ASSERT(NULL != ccr_mem);

#define MTLK_GEN6_CLEAR_RAB_INTERRUPT       (0x1)

  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_STATUS, status);

  if (status & MTLK_IRQ_LEGACY & clear_mask) {
    __ccr_mem_writel(ccr_mem, MTLK_GEN6_CLEAR_RAB_INTERRUPT,
               HOST_IF_IRQS_PHI_INTERRUPT_CLEAR);
  }

  return status;
}

static BOOL
_mtlk_pcie_disable_interrupts_if_pending(void *ccr_mem, uint32 mask)
{
  _mtlk_pcie_disable_interrupts(ccr_mem, mask);
  return TRUE;
}

static void
_mtlk_pcie_initiate_doorbell_interrupt(void *ccr_mem)
{
#define MTLK_GEN6_GENERATE_DOOR_BELL    (0x1)

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_writel(ccr_mem, MTLK_GEN6_GENERATE_DOOR_BELL,
               HOST_IF_IRQS_NPU2UPI_INTERRUPT_SET);
}

static __INLINE void
__pcie_open_secure_write_register_upper(void *ccr_mem)
{
  ILOG2_V("UMAC");
  wmb();
  __ccr_mem_writel(ccr_mem, 0xAAAA, SYS_IF_UM_SECURE_WRITE);
  wmb();
  __ccr_mem_writel(ccr_mem, 0x5555, SYS_IF_UM_SECURE_WRITE);
  wmb();
}

static __INLINE uint32
_mtlk_pcie_get_progmodel_version(void *ccr_mem)
{
  uint32 val;

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, PHY_RX_TD_PHY_PROGMODEL_VER, val);

  return val;
}

static __INLINE uint32
_mtlk_pcie_get_progmodel_version_modified(void *ccr_mem)
{
  uint32 val;

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, PHY_RX_TD_PHY_PROGMODEL_VER_MODIFIED, val);

  return val;
}

static BOOL
_mtlk_pcie_check_bist(void *ccr_mem, uint32 *bist_result)
{
  MTLK_ASSERT(NULL != ccr_mem);
  /* Not needed on G6 */
  MTLK_UNREFERENCED_PARAM(ccr_mem);

  *bist_result = 0;
  return TRUE;
}

static int
_mtlk_pcie_eeprom_override_write (void *ccr_mem, uint32 data, uint32 mask)
{
  uint32 val;

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, SOC_REGS_EEPROM_SW_ACCESS, val);

  val = (val & (~mask))| data;

  __ccr_mem_writel(ccr_mem, val, SOC_REGS_EEPROM_SW_ACCESS);

  return MTLK_ERR_OK;
}

static uint32
_mtlk_pcie_eeprom_override_read (void *ccr_mem)
{
  uint32 val;

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, SOC_REGS_EEPROM_SW_ACCESS, val);

  return val;
}

static uint32
_mtlk_pcie_ccr_get_mips_freq(void *ccr_mem, uint32 chip_id)
{
  MTLK_UNREFERENCED_PARAM(ccr_mem);
  return WAVE_PCIE_MIPS_FREQ_DEFAULT;
}

static int
_mtlk_pcie_ccr_init(void *ccr_mem, mtlk_hw_t *hw, void *bar0, void *bar1, void *bar1_phy, void *dev_ctx, void **shram, BOOL is_hw_gen7)
{
  mtlk_pcie_ccr_t *pcie_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  MTLK_ASSERT(NULL != pcie_mem);

  pcie_mem->hw = hw;
  pcie_mem->pas = bar1;
  pcie_mem->pas_physical = bar1_phy;
  pcie_mem->dev_ctx = dev_ctx;

  *shram = bar1 + SHRAM_WRITABLE_OFFSET;

  return _mtlk_ccr_pcie_all_regs_validate(is_hw_gen7);
}

static void _mtlk_pcie_ccr_cleanup(void *ccr_mem)
{
  mtlk_pcie_ccr_t *pcie_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  MTLK_ASSERT(NULL != pcie_mem);

  /* cleanup all values at once */
  memset(pcie_mem, 0, sizeof(mtlk_pcie_ccr_t));
}

static void
_mtlk_pcie_ccr_ctrl_ring_init(void *ccr_mem, void *ring_regs, unsigned char *mmb_base)
{
  mtlk_ring_regs *regs = (mtlk_ring_regs *)ring_regs;

  MTLK_ASSERT(NULL != regs);
  MTLK_UNREFERENCED_PARAM(mmb_base);

  regs->p_IN_cntr    = (uint32*)(CCR_MEM_ADDR(ccr_mem, HOST_IF_ACC_HOST_MAC_MAILBOX_ACCUM_ADD));
  regs->p_OUT_status = (uint32*)(CCR_MEM_ADDR(ccr_mem, HOST_IF_ACC_MAC_HOST_MAILBOX_ACCUM_COUNT));
  regs->p_OUT_cntr   = (uint32*)(CCR_MEM_ADDR(ccr_mem, HOST_IF_ACC_MAC_HOST_MAILBOX_ACCUM_SUB));
  regs->int_OUT_cntr = mtlk_raw_readl(regs->p_OUT_status);
}

static uint32
_mtlk_pcie_ccr_ring_get_hds_to_process (void *ccr_mem, void *ring_regs)
{
  mtlk_ring_regs *regs = (mtlk_ring_regs *)ring_regs;
  uint32 value;

  MTLK_ASSERT(NULL != regs);

  /* p_tx_OUT_status already contains number of HDs */
  value = mtlk_raw_readl(regs->p_OUT_status);
  rmb();
  return LE32_TO_CPU(value);
}

static void
_mtlk_pcie_ccr_ring_clear_interrupt (void *ring_regs, uint32 hds_processed)
{
  mtlk_ring_regs *regs = (mtlk_ring_regs *)ring_regs;

  MTLK_ASSERT(NULL != regs);

  regs->int_OUT_cntr += hds_processed;

  /* clear interrupt */
  mtlk_raw_writel(CPU_TO_LE32(hds_processed), regs->p_OUT_cntr);
  wmb();
}

static void
_mtlk_pcie_ccr_ring_initiate_doorbell_interrupt (void *ccr_mem, void *ring_regs)
{
  mtlk_ring_regs *regs = (mtlk_ring_regs*)ring_regs;

  MTLK_ASSERT(NULL != regs);

  /* just one HD added, accumulator register */
  mtlk_raw_writel(CPU_TO_LE32(1), regs->p_IN_cntr);
  wmb();
}

static void
_mtlk_pcie_ccr_ring_initiate_bulk_doorbell_interrupt (void *ccr_mem, void *ring_regs, uint32 hds_ready)
{
  mtlk_ring_regs *regs = (mtlk_ring_regs*)ring_regs;

  MTLK_ASSERT(NULL != regs);

  /* bulk of HD added, accumulator register */
  mtlk_raw_writel(CPU_TO_LE32(hds_ready), regs->p_IN_cntr);
  wmb();
}

static void
_mtlk_pcie_eeprom_init (void *ccr_mem)
{
  uint32 reg_val;

#define EEPROM_PULLUP_GPIO_SCL (1<<4)
#define EEPROM_PULLUP_GPIO_SDA (1<<5)

  MTLK_ASSERT(NULL != ccr_mem);

  __ccr_mem_readl(ccr_mem, SOC_REGS_GPIO_PULLUP, reg_val);
  reg_val |= EEPROM_PULLUP_GPIO_SCL;
  reg_val |= EEPROM_PULLUP_GPIO_SDA;
  __ccr_mem_writel(ccr_mem, reg_val, SOC_REGS_GPIO_PULLUP);
}

/*------ WAVE PCIE EFUSE mapping ---------------------*/

#define WAVE_PCIE_MAX_EFUSE_SIZE                       (128)   /* 1024 bits = 128 bytes = 32 words */
#define WAVE_PCIE_EFUSE_STATUS_READ_MAX                (1000)  /* max number of attempts to read */

/* SOC_REGS_EFUSE_ADDRESS */
#define SOC_REGS_EFUSE_BITS_PER_WORD                (2 << 5) /* uint32 word access */
#define SOC_REGS_EFUSE_ADDRESS_BIT_ADDR             MTLK_BFIELD_INFO(0, 5) /* for write ops only */
#define SOC_REGS_EFUSE_ADDRESS_WORD_ADDR            MTLK_BFIELD_INFO(5, 5) /* for read and wrire */

/* SOC_REGS_EFUSE_CONTROL */
#define SOC_REGS_EFUSE_CONTROL_EFUSE_WR_RDN         MTLK_BFIELD_INFO(0, 1) /* 0 - read, 1 - write */
#define SOC_REGS_EFUSE_CONTROL_EFUSE_READ_REQ       MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_CONTROL_EFUSE_WR_RDN, 0, uint32)
#define SOC_REGS_EFUSE_CONTROL_EFUSE_WRITE_REQ      MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_CONTROL_EFUSE_WR_RDN, 1, uint32)

/* SOC_REGS_EFUSE_STATUS */
#define SOC_REGS_EFUSE_STATUS_EFUSE_BUSY            MTLK_BFIELD_INFO(0, 1) /* 0 - cleared */
#define SOC_REGS_EFUSE_STATUS_EFUSE_BUSY_CLEARED    (0)
#define SOC_REGS_EFUSE_STATUS_EFUSE_INIT_DONE       MTLK_BFIELD_INFO(1, 1) /* 1 - init done */
#define SOC_REGS_EFUSE_STATUS_EFUSE_INIT_DONE_SET   (1)
#define SOC_REGS_EFUSE_STATUS_EFUSE_FSM_STATE       MTLK_BFIELD_INFO(2, 3) /* state machine (for debug) */

/* SOC_REGS_EFUSE_PCIE_FSM_OVR */
#define SOC_REGS_EFUSE_PCIE_FSM_OVR_CTL_OVERRIDE    (1)
#define SOC_REGS_EFUSE_PCIE_FSM_OVR_CTL_NO_OVERRIDE (0)

/* SOC_REGS_EFUSE_PD */
#define SOC_REGS_EFUSE_PD_DISABLE                   (0)
#define SOC_REGS_EFUSE_PD_ENABLE                    (1)

/* SOC_REGS_EFUSE_TIMER_CONFIG1 */
#define SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TPGM             MTLK_BFIELD_INFO( 0, 16)
#define SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TSU_CS_STROBE    MTLK_BFIELD_INFO(16, 16)

/* SOC_REGS_EFUSE_TIMER_CONFIG2 */
#define SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TRD              MTLK_BFIELD_INFO( 0, 16)
#define SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TSU_PS_CSB       MTLK_BFIELD_INFO(16, 16)

#define SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TPGM_US          (12)  /* microseconds */
#define SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TSU_CS_STROBE_NS (8)   /* nanoseconds */
#define SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TRD_NS           (200) /* nanoseconds */
#define SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TSU_PS_CSB_NS    (71)  /* nanoseconds */

/* Configure EFUSE timers according to MAC clocking */
static int
_mtlk_pcie_efuse_timer_config (void *ccr_mem)
{
    uint32  clk_sel, clk_mhz;
    uint32  field1, field2; /* fields */
    uint32  timer1, timer2; /* words */

    _CCR_READ_REG_VALUE_(ccr_mem, SYS_IF_UM_CLOCK_SELECT, clk_sel);
    clk_sel = MTLK_BFIELD_GET(clk_sel, SYS_IF_UM_CLOCK_SELECT_CPU_CLOCK_SELECT);

    switch (clk_sel)
    {
      case SYS_IF_UM_CLOCK_SELECT_CPU_CLOCK_SELECT_XO:
        clk_mhz = WLAN_CLOCK_XO_MHZ;
        break;
      case SYS_IF_UM_CLOCK_SELECT_CPU_CLOCK_SELECT_PLL:
        clk_mhz = WLAN_CLOCK_PLL_MHZ;
        break;
      default:
        WLOG_D("Don't configure EFUSE timers due to wrong CLOCK_SELECT value %u", clk_sel);
        return MTLK_ERR_NOT_SUPPORTED;
    }

    field1 = WLAN_TIME_US_TO_CLOCKS_MHZ(clk_mhz, SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TPGM_US);
    field2 = WLAN_TIME_NS_TO_CLOCKS_MHZ(clk_mhz, SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TSU_CS_STROBE_NS);

    timer1 = MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TPGM,          field1, uint32) |
             MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_TIMER_CONFIG1_EFUSE_TSU_CS_STROBE, field2, uint32);

    /* TRD requires additional 4 clock cycles */
    field1 = WLAN_TIME_NS_TO_CLOCKS_MHZ(clk_mhz, SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TRD_NS) + 4;
    field2 = WLAN_TIME_NS_TO_CLOCKS_MHZ(clk_mhz, SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TSU_PS_CSB_NS);

    timer2 = MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TRD,           field1, uint32) |
             MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_TIMER_CONFIG2_EFUSE_TSU_PS_CSB,    field2, uint32);

    ILOG1_DDD("Set EFUSE timers to 0x%08X and 0x%08X for clock %u MHz", timer1, timer2, clk_mhz);

    _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_TIMER_CONFIG1, timer1);
    _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_TIMER_CONFIG2, timer2);

    return MTLK_ERR_OK;
}

static __INLINE BOOL
__mtlk_pcie_efuse_status_is_busy_cleared (uint32 reg_val)
{
  return (MTLK_BFIELD_GET(reg_val, SOC_REGS_EFUSE_STATUS_EFUSE_BUSY) ==
                                   SOC_REGS_EFUSE_STATUS_EFUSE_BUSY_CLEARED);
}

static __INLINE BOOL
__mtlk_pcie_efuse_status_is_init_done_set (uint32 reg_val)
{
  return (MTLK_BFIELD_GET(reg_val, SOC_REGS_EFUSE_STATUS_EFUSE_INIT_DONE) ==
                                   SOC_REGS_EFUSE_STATUS_EFUSE_INIT_DONE_SET);
}

static int
_mtlk_pcie_efuse_open (mtlk_pcie_ccr_t *ccr_mem)
{
  uint32 reg_val;
  BOOL   status = FALSE;
  int    res;
  int    n;

  /* Configure EFUSE timers according to the current clock */
  res = _mtlk_pcie_efuse_timer_config(ccr_mem);
  if (MTLK_ERR_OK != res) {
    return res; /* already logged */
  }

  /* 1. Disable PD bit in efuse_PD register and set control "override" */
  _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_PD, SOC_REGS_EFUSE_PD_DISABLE);
  _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_PCIE_FSM_OVR, SOC_REGS_EFUSE_PCIE_FSM_OVR_CTL_OVERRIDE);

  /* 2. Check eFuse control status for busy bit is cleared and init_done bit is set */
  for (n = 0; ((!status) && (n < WAVE_PCIE_EFUSE_STATUS_READ_MAX)); n++) {
    _CCR_READ_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_STATUS, reg_val);
    status = (__mtlk_pcie_efuse_status_is_busy_cleared(reg_val) &&
              __mtlk_pcie_efuse_status_is_init_done_set(reg_val));
  }

  if (status) {
      return MTLK_ERR_OK;
  } else {
    ELOG_D("eFuse open: status not ready: 0x%02X", reg_val);
    return MTLK_ERR_NOT_READY;
  }
}

static void
_mtlk_pcie_efuse_close(void *ccr_mem)
{
  /* 7. Set control "no override" and Enable PD bit in efuse_PD register */
  _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_PCIE_FSM_OVR, SOC_REGS_EFUSE_PCIE_FSM_OVR_CTL_NO_OVERRIDE);
  _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_PD, SOC_REGS_EFUSE_PD_ENABLE);
}

static __INLINE BOOL
__mtlk_pcie_efuse_status_busy_cleared (void *ccr_mem)
{
  uint32 reg_val;
  int    n;
  BOOL   status = FALSE;

  /* 5. Poll the eFuse status register until the control_busy bit is cleared */
  for (n = 0; ((!status) && (n < WAVE_PCIE_EFUSE_STATUS_READ_MAX)); n++) {
    _CCR_READ_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_STATUS, reg_val);
    status = __mtlk_pcie_efuse_status_is_busy_cleared(reg_val);
  }

  if (!status) {
    ELOG_D("eFuse busy timeout, eFuse status 0x%08X", reg_val);
  }

  return status;
}

/* Checking for eFuse access addr/size/buf */
/* Gen6 specific: word access */
static BOOL
_mtlk_pcie_efuse_access_check (const char *op, void *buf, uint32 addr, uint32 size)
{
  ILOG0_SDDP("eFuse %s: addr %u, size %u, buf 0x%p", op, addr, size, buf);
  if ((sizeof(uint32) - 1) & ((wave_addr)buf | addr | size)) {
    ELOG_S("eFuse %s: unaligned uint32 access: at least one of buf/addr/size", op);
    return FALSE;
  } else
  if (WAVE_PCIE_MAX_EFUSE_SIZE <= addr ||
      WAVE_PCIE_MAX_EFUSE_SIZE <  size ||
      WAVE_PCIE_MAX_EFUSE_SIZE <  (addr + size))
  {
    ELOG_SDD("eFuse %s: addr and/or size too big: %u, %u", op, addr, size);
    return FALSE;
  }

  return TRUE;
}

/* Read data from eFuse */
/* Gen6 specific: word access */
/* Return value - number of bytes was read */
static uint32
_mtlk_pcie_efuse_read_data (void *ccr_mem, void *buf, uint32 addr, uint32 size)
{
  mtlk_pcie_ccr_t *pcie_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  uint32    word_addr, word_num;
  uint32    bytes_read = 0;
  uint32   *words, word;

  MTLK_ASSERT(NULL != pcie_mem);
  MTLK_ASSERT(NULL != buf);

  if (!_mtlk_pcie_efuse_access_check("read", buf, addr, size)) {
    return 0; /* bytes_read */
  }

  if (MTLK_ERR_OK != _mtlk_pcie_efuse_open(pcie_mem))
    goto FINISH;

  words = (uint32 *)buf;
  word_addr = addr / sizeof(uint32);
  for (word_num = 0; word_num < (size / sizeof(uint32)); word_num++, word_addr++) {
    /* Write the Word Address */
    _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_ADDRESS,
                          MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_ADDRESS_WORD_ADDR, word_addr, uint32));

    /* Write "Read Request" to the eFuse control register. */
    _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_CONTROL, SOC_REGS_EFUSE_CONTROL_EFUSE_READ_REQ);

    /* Wait for request is finished */
    if (FALSE == __mtlk_pcie_efuse_status_busy_cleared(ccr_mem))
      goto FINISH;

    /* Read the value from the eFuse data register */

    _CCR_READ_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_DATA, word);
    words[word_num] = HOST_TO_MAC32(word); /* bytes reordering */
    ILOG1_DDD("eFuse[%2u] = 0x%08X (0x%08X)", word_num, words[word_num], word);
  }

  bytes_read = word_num * sizeof(uint32); /* all data complete */

FINISH:
  _mtlk_pcie_efuse_close(pcie_mem);

  return bytes_read;
}

static void
_mtlk_pcie_efuse_pre_write_check(void *ccr_mem, uint8 *check_buf, uint32 addr, uint32 size)
{
#define EFUSE_CLEAN_PATTERN 0x00
  uint32 i;

  if (!_mtlk_pcie_efuse_read_data(ccr_mem, check_buf, addr, size)) {
    ELOG_V("eFuse read error: Pre-write validation is not possible");
    return;
  }

  for (i = 0; i < size; i++) {
    if (EFUSE_CLEAN_PATTERN != check_buf[i]) {
      ELOG_V("eFuse not clean: correct write may not be possible!");
      mtlk_dump(0, check_buf, size, "eFuse target area:");
      break;
    }
  }
}

static void
_mtlk_pcie_efuse_post_write_check(void *ccr_mem, uint8 *check_buf, uint8 *write_buf, uint32 addr, uint32 size)
{
  uint32 i;
  BOOL write_err = FALSE;

  if (!_mtlk_pcie_efuse_read_data(ccr_mem, check_buf, addr, size)) {
    ELOG_V("eFuse read error: Post-write validation is not possible");
    return;
  }

  for (i = 0; i < size; i++) {
    if (write_buf[i] != check_buf[i]) {
      write_err = TRUE;
#ifdef CONFIG_WAVE_DEBUG
      WLOG_DDD("eFuse write validation error: offset: %u, written: 0x%x, read back: 0x%x",
        (i + addr), write_buf[i], check_buf[i]);
#else
      WLOG_V("eFuse write validation error");
#endif
    }
  }
  if (!write_err) {
    ILOG0_V("eFuse write validation passed");
  }
}

/* Write data to eFuse */
/* Gen6 specific: word access */
/* Return value - number of bytes was written */
/* */
static uint32
_mtlk_pcie_efuse_write_data(void *ccr_mem, void *buf, uint32 addr, uint32 size)
{
  uint32    word_addr, word_num, bit_num;
  uint32    bytes_written = 0, offset = addr;
  uint32    *words;
  uint8     *check_buf = NULL;

  MTLK_ASSERT(NULL != ccr_mem);
  MTLK_ASSERT(NULL != buf);

  ELOG_V("Gen6 EFUSE writing is unsupported");
  return 0;

  if (!_mtlk_pcie_efuse_access_check("write", buf, addr, size)) {
    goto FAIL;
  }

  /* check if eFuse target area is clean */
  check_buf = mtlk_osal_mem_alloc(WAVE_PCIE_MAX_EFUSE_SIZE, MTLK_MEM_TAG_EEPROM);
  if (NULL == check_buf) {
    ELOG_V("Failed to allocate validation buffer");
    goto FAIL;
  }
  _mtlk_pcie_efuse_pre_write_check(ccr_mem, check_buf, offset, size);

  if (MTLK_ERR_OK != _mtlk_pcie_efuse_open(ccr_mem))
    goto FINISH;

  mtlk_dump(0, buf, size, "eFuse data to write:");

  words = (uint32 *)buf;
  word_addr = addr / sizeof(uint32);
  for (word_num = 0; word_num < (size / sizeof(uint32)); word_num++, word_addr++) {
    /* Write 1 word bitwise, BIT0 is first */
    for (bit_num = 0; bit_num < SOC_REGS_EFUSE_BITS_PER_WORD; bit_num++) {
      /* only 1s are burned */
      if (words[word_num] & (1u << bit_num)) {
        /* compose word and bit adress */
        _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_ADDRESS,
                              (MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_ADDRESS_BIT_ADDR,  bit_num,   uint32) |
                               MTLK_BFIELD_VALUE(SOC_REGS_EFUSE_ADDRESS_WORD_ADDR, word_addr, uint32)));

        /* Write "Write Request" to the eFuse control register */
        _CCR_WRITE_REG_VALUE_(ccr_mem, SOC_REGS_EFUSE_CONTROL, SOC_REGS_EFUSE_CONTROL_EFUSE_WRITE_REQ);

        /* wait for writing is finished */
        if (FALSE == __mtlk_pcie_efuse_status_busy_cleared(ccr_mem)) {
#ifdef CONFIG_WAVE_DEBUG
          ELOG_DDD("eFuse write error! start_addr: 0x%04X, word:0x%02X, bit: %2u",
                    (int)addr, word_num, bit_num);
#else
          ELOG_V("eFuse write error!");
#endif
          goto FINISH;
        }
      }
    }
  }

  bytes_written = word_num * sizeof(uint32);

FINISH:
  _mtlk_pcie_efuse_close(ccr_mem);

  /* verify write */
  if (bytes_written) {
    _mtlk_pcie_efuse_post_write_check(ccr_mem, check_buf, buf, offset, size);
  }

  if (check_buf) {
    mtlk_osal_mem_free(check_buf);
  }

FAIL:
  return bytes_written;
}

/* Get eFuse size */
/* Return value - number of bytes in eFuse */
static uint32
_mtlk_pcie_efuse_get_size(void *ccr_mem)
{
  MTLK_UNREFERENCED_PARAM(ccr_mem);
  return WAVE_PCIE_MAX_EFUSE_SIZE;
}

/* Power data per antenna are in 0.5 dB units i.e. in S8.1 format */
#define PW_FIXP_NBI     (8) /* number of bits of integer part */
#define PW_FIXP_NBF     (1) /* number of bits of fractional part */
#define PW_NOF_ANTS     (4) /* number of antennas */

static int
_mtlk_pcie_read_phy_inband_power(void *ccr_mem, unsigned radio_id, uint32 ant_mask,
                                   int32 *noise_estim, int32 *system_gain)
{
  uint32 ant_offs;
  uint32 reg_addr, value;
  int ant_num;

  ILOG2_DD("RadioID %u ant_mask 0x%02X", radio_id, ant_mask);

  for (ant_num = 0; ant_num < PW_NOF_ANTS; ant_num++) {
    if (0 == (ant_mask & (1 << ant_num))) {
      /* default values for unavailable antenna */
      noise_estim[ant_num] = FIXP_MIN_INT(PW_FIXP_NBI, PW_FIXP_NBF);
      system_gain[ant_num] = 0;
    } else {
      switch (ant_num) {
        default:
        case 0: ant_offs = 0; break;
        case 1: ant_offs = B0_PHY_AGC_ANT1_OFFSET_FROM_BAR0 - B0_PHY_AGC_ANT0_OFFSET_FROM_BAR0; break;
        case 2: ant_offs = B0_PHY_AGC_ANT2_OFFSET_FROM_BAR0 - B0_PHY_AGC_ANT0_OFFSET_FROM_BAR0; break;
        case 3: ant_offs = B0_PHY_AGC_ANT3_OFFSET_FROM_BAR0 - B0_PHY_AGC_ANT0_OFFSET_FROM_BAR0; break;
      }

      /* Register addresses depend on the Radio ID */
      reg_addr = (0 == radio_id) ? B0_PHY_AGC_ANT0_IB_POWER_REG
                                 : B1_PHY_AGC_ANT0_IB_POWER_REG;

      _CCR_READ_REG_VALUE_(ccr_mem, ant_offs + reg_addr, value);
      value = MTLK_BFIELD_GET(value, PHY_AGC_ANT0_IB_POWER_IBPOWER);
      noise_estim[ant_num] = wave_fixp_sign_ext(value, PW_FIXP_NBI, PW_FIXP_NBF);

      reg_addr = (0 == radio_id) ? B0_PHY_AGC_ANT0_INBAND_RSSI_SYSTEM_GAIN_REG
                                 : B1_PHY_AGC_ANT0_INBAND_RSSI_SYSTEM_GAIN_REG;

      _CCR_READ_REG_VALUE_(ccr_mem, ant_offs + reg_addr, value);
      value = MTLK_BFIELD_GET(value, PHY_AGC_ANT0_INBAND_RSSI_SYSTEM_GAIN_REG_IBGAIN);
      system_gain[ant_num] = wave_fixp_sign_ext(value, PW_FIXP_NBI, PW_FIXP_NBF);
    }
  }

  return MTLK_ERR_OK;
}

/* The following PCIE general APIs should not be duplicated */
#ifdef _WAVE_PCIE_GENERAL_API_

#ifdef CPTCFG_IWLWAV_DEBUG
int mtlk_pcieg6_ccr_all_regs_validate (void)
{
#if _HW_REG_DEBUG_
  return _mtlk_ccr_pcie_all_regs_validate(false);
#else
  return _mtlk_ccr_pcie_all_regs_validate();
#endif
}
#endif  /* CPTCFG_IWLWAV_DEBUG */

#endif /* _WAVE_PCIE_GENERAL_API_ */

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID
