/******************************************************************************
         Copyright (c) 2020, MaxLinear, Inc.
         Copyright (c) 2013  Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define LOG_LOCAL_GID   GID_CCR_PCIE
#define LOG_LOCAL_FID   10

/* Secure Boot: FW binary images loading address and sizes */
#define WAVE_FW_FC_IRAM_START       (0x00400000)
#define WAVE_FW_FC_IRAM_SIZE        (0x000001E8)
#define WAVE_FW_DC_IRAM_SIZE        (0x000001E8)
#define WAVE_FW_FW_HDR_IRAM_SIZE    (0x00000188)
#define WAVE_FW_PSD_HDR_IRAM_SIZE   (0x00000188)
#define WAVE_FW_CAL_HDR_IRAM_SIZE   (0x00000188)
#define WAVE_FW_TXCPU_IRAM_SIZE     (0x00010000)

/* Table with sizes of FW binary images.
   Only exactly defined sizes are filled in.
   Unfilled items suppose actual size of FW.
 */
static const uint32 _wave_fw_size[WAVE_FW_NUM_CNT] =
{
  [WAVE_FW_NUM_UM]            = WAVE_PCIE_URAM_IRAM_SIZE,     /* Unified RAM */
  [WAVE_FW_NUM_SECB_FC]       = WAVE_FW_FC_IRAM_SIZE,         /* FW certificate */
  [WAVE_FW_NUM_SECB_DC]       = WAVE_FW_DC_IRAM_SIZE,         /* DUT certificate */
  [WAVE_FW_NUM_SECB_FW_HDR]   = WAVE_FW_FW_HDR_IRAM_SIZE,     /* Signed FW header */
  [WAVE_FW_NUM_SECB_PSD_HDR]  = WAVE_FW_PSD_HDR_IRAM_SIZE,    /* Signed PSD header */
  [WAVE_FW_NUM_SECB_CAL_HDR]  = WAVE_FW_CAL_HDR_IRAM_SIZE,    /* Signed Cal file header */
  [WAVE_FW_NUM_TX]            = WAVE_PCIE_B0_TXCPU_IRAM_SIZE, /* Tx Sender CPUs */
  [WAVE_FW_NUM_RX]            = WAVE_PCIE_B0_RXCPU_IRAM_SIZE, /* Rx Handler CPUs */
  [WAVE_FW_NUM_IF]            = WAVE_PCIE_IFCPU_IRAM_SIZE,    /* Host Interface CPUs */
  [WAVE_FW_NUM_SECB_IF_1]     = WAVE_PCIE_IFCPU_IRAM_SIZE,
  [WAVE_FW_NUM_SECB_IF_2]     = WAVE_PCIE_IFCPU_IRAM_SIZE,
  [WAVE_FW_NUM_SECB_IF_3]     = WAVE_PCIE_IFCPU_IRAM_SIZE,
  [WAVE_FW_NUM_SECB_IF_4]     = WAVE_PCIE_IFCPU_IRAM_SIZE,
  [WAVE_FW_NUM_SECB_IF_5]     = WAVE_PCIE_IFCPU_IRAM_SIZE,
};

static int
_mtlk_pcie_ccr_get_sec_boot_fw_info (void *ccr_mem, wave_fw_num_e fw_num, mtlk_fw_info_t *fw_info)
{
  mtlk_pcie_ccr_t *pcie_mem = (mtlk_pcie_ccr_t *)ccr_mem;
  MTLK_ASSERT(ARRAY_SIZE(fw_info->mem) >= 3);

  /* Default memory chunks values for any FW bin */
  fw_info->mem[0].start   = 0;
  fw_info->mem[0].length  = 0;
  fw_info->mem[0].swap    = FALSE;
  fw_info->mem[1].start   = 0;
  fw_info->mem[1].length  = 0;
  fw_info->mem[1].swap    = FALSE;
  fw_info->mem[2].start   = 0;
  fw_info->mem[2].length  = 0;
  fw_info->mem[2].swap    = FALSE;

  /* FW specific */
  switch (fw_num)
  {
    /* Fixed loading address */
    case WAVE_FW_NUM_UM: /* URAM */
      fw_info->mem[0].start   = ((unsigned char*)pcie_mem->pas) + WAVE_PCIE_URAM_IRAM_START;
      break;
    case WAVE_FW_NUM_SECB_FC: /* FW certificate */
      fw_info->mem[0].start   = ((unsigned char*)pcie_mem->pas) + WAVE_FW_FC_IRAM_START;
      break;

    /* All below have variable loading address */
    case WAVE_FW_NUM_SECB_DC:       /* DUT certificate */
    case WAVE_FW_NUM_SECB_FW_HDR:   /* FW header */
    case WAVE_FW_NUM_SECB_PSD_HDR:  /* PSD header */
    case WAVE_FW_NUM_SECB_CAL_HDR:  /* Cal header */
    case WAVE_FW_NUM_TX:            /* Tx Sender CPUs */
    case WAVE_FW_NUM_RX:            /* Rx Handler CPUs */
    case WAVE_FW_NUM_IF:            /* Host Interface CPUs */
    case WAVE_FW_NUM_SECB_IF_1:
    case WAVE_FW_NUM_SECB_IF_2:
    case WAVE_FW_NUM_SECB_IF_3:
    case WAVE_FW_NUM_SECB_IF_4:
    case WAVE_FW_NUM_SECB_IF_5:
	case WAVE_FW_NUM_SECB_PM_CMN:    /* ProgModel Common */
    case WAVE_FW_NUM_SECB_PM_ANT_RX: /* ProgModel Rx Ant */
    case WAVE_FW_NUM_SECB_PM_ANT_TX: /* ProgModel Tx Ant */
    case WAVE_FW_NUM_SECB_PSD_FILE:  /* PSD file */
    case WAVE_FW_NUM_SECB_CAL_FILE:  /* Cal file */
      break;
    /* Not supported */
    default:
      return MTLK_ERR_NOT_SUPPORTED;
  }

  fw_info->mem[0].length = _wave_fw_size[fw_num];

  return MTLK_ERR_OK;
}

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID

