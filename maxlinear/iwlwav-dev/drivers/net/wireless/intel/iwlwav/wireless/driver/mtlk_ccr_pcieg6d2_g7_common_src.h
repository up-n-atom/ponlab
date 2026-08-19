/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#define LOG_LOCAL_GID   GID_CCR_PCIE
#define LOG_LOCAL_FID   6

#if (MTLK_CCR_DEBUG_PRINT_INTR_REGS)
static void
_mtlk_pcie_ccr_print_irq_regs(void *ccr_mem)
{
  mtlk_pcie_ccr_t *pci_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  uint32 host_irq_mask, host_irq_status;
  uint32 phi_interrupt_clear;

  MTLK_ASSERT(NULL != pci_mem);

  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_MASK, host_irq_mask);
  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_HOST_IRQ_STATUS, host_irq_status);
  __ccr_mem_readl(ccr_mem, HOST_IF_IRQS_PHI_INTERRUPT_CLEAR, phi_interrupt_clear);

  ILOG0_P  ("PCIE pas=%p\n", pci_mem->pas);
  ILOG0_DDD("PCIE IRQ regs: host_irq_mask=%08X, host_irq_status=%08X, phi_interrupt_clear=%08X",
            host_irq_mask, host_irq_status, phi_interrupt_clear);

  mtlk_hw_debug_print_all_ring_regs(pci_mem->hw);
}
#endif /* MTLK_CCR_DEBUG_PRINT_INTR_REGS */

static void
_mtlk_pcie_secure_release_cpus_reset (void *ccr_mem)
{
  MTLK_ASSERT(NULL != ccr_mem);

  ILOG0_V("Release UMAC");
  __ccr_mem_writel(ccr_mem, HOST_IF_UCPU_STAT_HOST_START_UCPU, HOST_IF_UCPU_STAT);

  mtlk_osal_msleep(10);
}

static uint32
_wave_pcie_read_hw_type (void *ccr_mem)
{
  uint32 system_info;

  MTLK_ASSERT(NULL != ccr_mem);
  __ccr_mem_readl(ccr_mem, HOST_IF_WLAN_EFUSE_INDICATION, system_info);
  return MTLK_BFIELD_GET(system_info, HOST_IF_WLAN_EFUSE_INDICATION_HW_TYPE);
}

static uint32
_wave_pcie_read_hw_rev (void *ccr_mem)
{
  uint32 system_info;

  MTLK_ASSERT(NULL != ccr_mem);
  __ccr_mem_readl(ccr_mem, HOST_IF_WLAN_EFUSE_INDICATION, system_info);
  return MTLK_BFIELD_GET(system_info, HOST_IF_WLAN_EFUSE_INDICATION_HW_REV);
}

static uint32
_wave_pcie_read_chip_type (void *ccr_mem)
{
  uint32 system_info;

  MTLK_ASSERT(NULL != ccr_mem);
  __ccr_mem_readl(ccr_mem, HOST_IF_WLAN_EFUSE_INDICATION, system_info);
  return MTLK_BFIELD_GET(system_info, HOST_IF_WLAN_EFUSE_INDICATION_CHIP_TYPE);
}




/* Unsupported APIs */
#define _mtlk_pcie_init_pll           mtlk_ccr_dummy_ret_ok_param
#define _mtlk_pcie_power_on_cpus      mtlk_ccr_dummy_ret_void
#define _mtlk_pcie_put_cpus_to_reset  mtlk_ccr_dummy_ret_void
#define _mtlk_pcie_put_ctl_to_reset   mtlk_ccr_dummy_ret_void
#define _mtlk_pcie_reset_mac          mtlk_ccr_dummy_ret_void
#define _mtlk_pcie_set_msi_intr_mode  mtlk_ccr_dummy_ret_void_param

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID
