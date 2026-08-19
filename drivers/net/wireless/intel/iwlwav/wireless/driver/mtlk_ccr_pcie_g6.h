/******************************************************************************
         Copyright (c) 2020, MaxLinear, Inc.
         Copyright (c) 2013  Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define LOG_LOCAL_GID   GID_CCR_PCIE
#define LOG_LOCAL_FID   8

typedef enum {
  LMAC_CPU_NUM0,
  LMAC_CPU_NUM1,
  LMAC_CPU_NUM_TOTAL
} e_lmac_cpu_num;

/*--------- Table of FW dump files ---------------*/
/* All IO base addresses are offsets from BAR0
 * Required order in the table:
 *    Common files for both radios
 *    (B0) Radio 0 specific
 *    (B1) Radio 1 specific
*/

enum g6_proc_fw_dump_idx { /* index in array */
  /* Common files for both radios */
  FW_IDX_CMN_FIRST = 0,
  FW_IDX_IRAM      = FW_IDX_CMN_FIRST,
  FW_IDX_SHRAM,
  FW_IDX_IF_COUNTERS,
  FW_IDX_IF_RAM,
  FW_IDX_IF_REG,
  FW_IDX_IF_SCP,
  FW_IDX_DESC_RAM,
  FW_IDX_DLM,
  FW_IDX_DMA_LL,
  FW_IDX_RX_PP_RAM,
  FW_IDX_TRA_IND,
  FW_IDX_PHY_PROGMODEL,
  FW_IDX_CMN_LAST,

  /* (B0) Radio 0 specific */
  FW_IDX_B0_FIRST  = FW_IDX_CMN_LAST,
  FW_IDX_B0_TX_RAM = FW_IDX_B0_FIRST,
  FW_IDX_B0_TX_REG,
  FW_IDX_B0_TX_SCP,
  FW_IDX_B0_TX_SEL,
  FW_IDX_B0_TX_STD,
  FW_IDX_B0_RX_RAM,
  FW_IDX_B0_RX_REG,
  FW_IDX_B0_RX_SCP,
  FW_IDX_B0_AUTO_RESP,
  FW_IDX_B0_TEST_BUS,
  FW_IDX_B0_PHY_RX_TD_REG096,
  FW_IDX_B0_PHY_RX_TD_REG_IF4A,
  FW_IDX_B0_LAST,

  /* (B1) Radio 1 specific */
  FW_IDX_B1_FIRST  = FW_IDX_B0_LAST,
  FW_IDX_B1_TX_RAM = FW_IDX_B1_FIRST,
  FW_IDX_B1_TX_REG,
  FW_IDX_B1_TX_SCP,
  FW_IDX_B1_TX_SEL,
  FW_IDX_B1_TX_STD,
  FW_IDX_B1_RX_RAM,
  FW_IDX_B1_RX_REG,
  FW_IDX_B1_RX_SCP,
  FW_IDX_B1_AUTO_RESP,
  FW_IDX_B1_TEST_BUS,
  FW_IDX_B1_PHY_RX_TD_REG096,
  FW_IDX_B1_PHY_RX_TD_REG_IF4A,
  FW_IDX_B1_LAST,
  /* Total number */
  FW_IDX_TOTAL = FW_IDX_B1_LAST
};

/*--- MACs and GenRiscs: offsets from bar0 and sizes ---*/

#define WAVE_PCIE_IFCPU_IRAM_START    (MAC_GENRISC_HOST_IRAM_OFFSET_FROM_BAR0)
#define WAVE_PCIE_IFCPU_IRAM_SIZE     (MAC_GENRISC_HOST_IRAM_SIZE)

/* Special defines for "test_bus_b0" */
#define TEST_BUS_SINGLE_BAND_SIZE           (B0_PHY_TEST_BUS_SIZE)          /* Single band - full size */
#define TEST_BUS_DUAL_BAND_SIZE             ((B0_PHY_TEST_BUS_SIZE) / 2)    /* Dual band   - half size */

/* todo: review */
#define DLM_DUMP_SIZE                       (0x0000ABE4)

/* Partial dump of MAC_GENRISC_HOST_REG and B0/B1_MAC_GENRISC_TX/RX_REG */
#define MAC_GENRISC_REG_DUMP_SIZE           (0x100)
#define BX_MAC_GENRISC_REG_DUMP_SIZE        MAC_GENRISC_REG_DUMP_SIZE

static wave_fw_dump_info_t  g6_proc_fw_dump_files[FW_IDX_TOTAL] = {
  /* Common files for both radios */
  [ FW_IDX_IRAM         ] = 		 { "iram",                			PROC_FW_IO,  CPU_IRAM_OFFSET_FROM_BAR0,                CPU_IRAM_SIZE                },
  [ FW_IDX_SHRAM        ] = 		 { "shram",               			PROC_FW_IO,  SHARED_RAM_OFFSET_FROM_BAR0,              SHARED_RAM_SIZE              },
  [ FW_IDX_IF_COUNTERS  ] = 		 { "host_if_counters",    			PROC_FW_IO,  HOST_IF_COUNTERS_OFFSET_FROM_BAR0,        HOST_IF_COUNTERS_SIZE        },
  [ FW_IDX_IF_RAM       ] = 		 { "host_if_ram",         			PROC_FW_IO,  MAC_GENRISC_HOST_IRAM_OFFSET_FROM_BAR0,   MAC_GENRISC_HOST_IRAM_SIZE   },
  [ FW_IDX_IF_REG       ] = 		 { "host_if_reg",         			PROC_FW_IO,  MAC_GENRISC_HOST_REG_OFFSET_FROM_BAR0,    MAC_GENRISC_REG_DUMP_SIZE    }, /* MAC_GENRISC_HOST_REG_SIZE */
  [ FW_IDX_IF_SCP       ] = 		 { "host_if_scpad",       			PROC_FW_IO,  MAC_GENRISC_HOST_SPRAM_OFFSET_FROM_BAR0,  MAC_GENRISC_HOST_SPRAM_SIZE  },
  [ FW_IDX_DESC_RAM     ] = 		 { "descriptor_ram",      			PROC_FW_IO,  DESCRIPTOR_RAM_OFFSET_FROM_BAR0,          DESCRIPTOR_RAM_SIZE          },
  [ FW_IDX_DLM          ] = 		 { "dlm",                 			PROC_FW_IO,  DLM_OFFSET_FROM_BAR0,                     DLM_DUMP_SIZE                },
  [ FW_IDX_DMA_LL       ] = 		 { "dma_linked_list",     			PROC_FW_IO,  WLAN_ARM_LL_MEM_OFFSET_FROM_BAR0,         WLAN_ARM_LL_MEM_SIZE         },
  [ FW_IDX_RX_PP_RAM    ] = 		 { "rxpp_ram",            			PROC_FW_IO,  RX_PP_OFFSET_FROM_BAR0,                   RX_PP_SIZE                   },
  [ FW_IDX_TRA_IND      ] = 		 { "traffic_ind",         			PROC_FW_IO,  TRAFFIC_INDICATOR_OFFSET_FROM_BAR0,       TRAFFIC_INDICATOR_SIZE       },
  [ FW_IDX_PHY_PROGMODEL] = 		 { "phy_progmodel",       			PROC_FW_IO,  PHY_RX_TD_PHY_PROGMODEL_VER,             (sizeof(uint32) * 2)          }, /* progmodel version and modified bit */
  /* (B0) Radio 0 specific */
  [ FW_IDX_B0_TX_SEL    ] = 		 { "tx_selector_b0",      			PROC_FW_IO,  B0_TX_SELECTOR_OFFSET_FROM_BAR0,          B0_TX_SELECTOR_SIZE          },
  [ FW_IDX_B0_TX_RAM    ] = 		 { "tx_sender_ram_b0",    			PROC_FW_IO,  B0_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0,  B0_MAC_GENRISC_TX_IRAM_SIZE  },
  [ FW_IDX_B0_TX_REG    ] = 		 { "tx_sender_reg_b0",    			PROC_FW_IO,  B0_MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0,   BX_MAC_GENRISC_REG_DUMP_SIZE }, /* B0_MAC_GENRISC_TX_REG_SIZE */
  [ FW_IDX_B0_TX_SCP    ] = 		 { "tx_sender_scpad_b0",  			PROC_FW_IO,  B0_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0, B0_MAC_GENRISC_TX_SPRAM_SIZE },
  [ FW_IDX_B0_TX_STD    ] = 		 { "tx_sender_std_b0",    			PROC_FW_IO,  B0_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0,   B0_MAC_GENRISC_TX_STD_SIZE   },
  [ FW_IDX_B0_RX_RAM    ] = 		 { "rx_handler_ram_b0",   			PROC_FW_IO,  B0_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0,  B0_MAC_GENRISC_RX_IRAM_SIZE  },
  [ FW_IDX_B0_RX_REG    ] = 		 { "rx_handler_reg_b0",   			PROC_FW_IO,  B0_MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0,   BX_MAC_GENRISC_REG_DUMP_SIZE }, /* B0_MAC_GENRISC_RX_REG_SIZE */
  [ FW_IDX_B0_RX_SCP    ] = 		 { "rx_handler_scpad_b0", 			PROC_FW_IO,  B0_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0, B0_MAC_GENRISC_RX_SPRAM_SIZE },
  [ FW_IDX_B0_AUTO_RESP ] = 		 { "auto_resp_ram_b0",    			PROC_FW_IO,  B0_AUTO_RESP_MEMS_OFFSET_FROM_BAR0,       B0_AUTO_RESP_MEMS_SIZE       },
  [ FW_IDX_B0_TEST_BUS  ] = 		 { "test_bus_b0",         			PROC_FW_IO,  B0_PHY_TEST_BUS_OFFSET_FROM_BAR0,         TEST_BUS_SINGLE_BAND_SIZE    },
  [ FW_IDX_B0_PHY_RX_TD_REG096 ] = 	 { "phy_rx_td_reg096_b0", 			PROC_FW_IO,  B0_PHY_RX_TD_PHY_RXTD_REG096,             (sizeof(uint32) * 1)         },
  [ FW_IDX_B0_PHY_RX_TD_REG_IF4A ] = { "phy_rx_td_if_phy_rxtd_if4a_b0", 	PROC_FW_IO,  B0_PHY_RX_TD_IF_PHY_RXTD_IF4A,        (sizeof(uint32) * 1)         },
  /* (B1) Radio 1 specific */
  [ FW_IDX_B1_TX_SEL    ] = 		 { "tx_selector_b1",      			PROC_FW_IO,  B1_TX_SELECTOR_OFFSET_FROM_BAR0,          B1_TX_SELECTOR_SIZE          },
  [ FW_IDX_B1_TX_RAM    ] = 		 { "tx_sender_ram_b1",    			PROC_FW_IO,  B1_MAC_GENRISC_TX_IRAM_OFFSET_FROM_BAR0,  B1_MAC_GENRISC_TX_IRAM_SIZE  },
  [ FW_IDX_B1_TX_REG    ] = 		 { "tx_sender_reg_b1",    			PROC_FW_IO,  B1_MAC_GENRISC_TX_REG_OFFSET_FROM_BAR0,   BX_MAC_GENRISC_REG_DUMP_SIZE }, /* B1_MAC_GENRISC_TX_REG_SIZE */
  [ FW_IDX_B1_TX_SCP    ] = 		 { "tx_sender_scpad_b1",  			PROC_FW_IO,  B1_MAC_GENRISC_TX_SPRAM_OFFSET_FROM_BAR0, B1_MAC_GENRISC_TX_SPRAM_SIZE },
  [ FW_IDX_B1_TX_STD    ] = 		 { "tx_sender_std_b1",    			PROC_FW_IO,  B1_MAC_GENRISC_TX_STD_OFFSET_FROM_BAR0,   B1_MAC_GENRISC_TX_STD_SIZE   },
  [ FW_IDX_B1_RX_RAM    ] = 		 { "rx_handler_ram_b1",   			PROC_FW_IO,  B1_MAC_GENRISC_RX_IRAM_OFFSET_FROM_BAR0,  B1_MAC_GENRISC_RX_IRAM_SIZE  },
  [ FW_IDX_B1_RX_REG    ] = 		 { "rx_handler_reg_b1",   			PROC_FW_IO,  B1_MAC_GENRISC_RX_REG_OFFSET_FROM_BAR0,   BX_MAC_GENRISC_REG_DUMP_SIZE }, /* B1_MAC_GENRISC_RX_REG_SIZE */
  [ FW_IDX_B1_RX_SCP    ] = 		 { "rx_handler_scpad_b1", 			PROC_FW_IO,  B1_MAC_GENRISC_RX_SPRAM_OFFSET_FROM_BAR0, B1_MAC_GENRISC_RX_SPRAM_SIZE },
  [ FW_IDX_B1_AUTO_RESP ] = 		 { "auto_resp_ram_b1",    			PROC_FW_IO,  B1_AUTO_RESP_MEMS_OFFSET_FROM_BAR0,       B1_AUTO_RESP_MEMS_SIZE       },
  [ FW_IDX_B1_TEST_BUS  ] = 		 { "test_bus_b1",         			PROC_FW_IO,  B1_PHY_TEST_BUS_OFFSET_FROM_BAR0,         TEST_BUS_DUAL_BAND_SIZE      },
  [ FW_IDX_B1_PHY_RX_TD_REG096 ] = 	 { "phy_rx_td_reg096_b1", 			PROC_FW_IO,  B1_PHY_RX_TD_PHY_RXTD_REG096,             (sizeof(uint32) * 1)         },
  [ FW_IDX_B1_PHY_RX_TD_REG_IF4A ] = { "phy_rx_td_if_phy_rxtd_if4a_b1", 	PROC_FW_IO,  B1_PHY_RX_TD_IF_PHY_RXTD_IF4A, 	   (sizeof(uint32) * 1)         },

};

/*--------------------------------------------------------------*/

static int
_mtlk_pcie_ccr_get_size_fw_dump_info (void *ccr_mem, wave_fw_dump_info_t *fw_info)
{
  mtlk_pcie_ccr_t *pcie_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  int size = ARRAY_SIZE(g6_proc_fw_dump_files);
  uint32 reg_val = 0;

  MTLK_ASSERT(ccr_mem);

  /* Size of the B0_TEST_BUS file depends on Single/Dual band mode */
  if (wave_hw_radio_band_cfg_is_single(pcie_mem->hw)) {
    fw_info[FW_IDX_B0_TEST_BUS].io_size = TEST_BUS_SINGLE_BAND_SIZE;
    size = FW_IDX_B0_LAST;
  } else {
    fw_info[FW_IDX_B0_TEST_BUS].io_size = TEST_BUS_DUAL_BAND_SIZE;
    size = FW_IDX_B1_LAST;
  }

  /* WAV600-2: FW Dump should not include Black zone when Firewall is enabled*/
  MTLK_FIREWALL_ON_UPDATE_SHARED_RAM_SIZE(g6_proc_fw_dump_files[FW_IDX_SHRAM].io_size,reg_val);

  /* Set size to 0 for files which are unsupported by dummy phy */
  /* FIXME: what about PDXP (Emulation) with real PHY ? */
  if (wave_hw_mmb_card_is_phy_dummy(pcie_mem->hw)) {
    fw_info[FW_IDX_B0_TEST_BUS].io_size  = 0;
    fw_info[FW_IDX_B1_TEST_BUS].io_size  = 0;
    fw_info[FW_IDX_B0_PHY_RX_TD_REG096].io_size = 0;
    fw_info[FW_IDX_B1_PHY_RX_TD_REG096].io_size = 0;
  }

  /* Different size of DESC_RAM */
  if (wave_hw_mmb_card_is_asic(pcie_mem->hw)) {
    fw_info[FW_IDX_DESC_RAM].io_size = DESCRIPTOR_RAM_SIZE;
  } else { /* non ASIC */
    fw_info[FW_IDX_DESC_RAM].io_size = REDUCED_DESCRIPTOR_RAM_SIZE;
  }

  return size;
}

static int
_mtlk_pcie_ccr_get_fw_dump_info (void *ccr_mem, wave_fw_dump_info_t **fw_info)
{
  size_t alloc_size = 0;

  MTLK_ASSERT(NULL != fw_info);
  MTLK_ASSERT(NULL == *fw_info);

  alloc_size = sizeof(g6_proc_fw_dump_files);
  *fw_info = mtlk_osal_mem_alloc(alloc_size, LQLA_MEM_TAG_FW_RECOVERY);
  if (*fw_info == NULL) {
    ELOG_V("Failed to alloc memory for fw_info");
    return MTLK_ERR_NO_MEM;
  }

  wave_memcpy(*fw_info, alloc_size, &g6_proc_fw_dump_files[0], sizeof(g6_proc_fw_dump_files));

  return _mtlk_pcie_ccr_get_size_fw_dump_info(ccr_mem, *fw_info);
}

static __INLINE void
__pcie_open_secure_write_register_lower(void *ccr_mem, e_lmac_cpu_num  lmac_cpu_num)
{
  pcie_regs_t secure_write_register =
    (lmac_cpu_num == LMAC_CPU_NUM0) ?
    SYS_IF_LM0_SECURE_WRITE :
    SYS_IF_LM1_SECURE_WRITE;

  ILOG2_D("LMAC%d", lmac_cpu_num);
  wmb();
  __ccr_mem_writel(ccr_mem, 0xAAAA, secure_write_register);
  wmb();
  __ccr_mem_writel(ccr_mem, 0x5555, secure_write_register);
  wmb();
}

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID
