/******************************************************************************

         Copyright (c) 2022, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/
/*
 * $Id$
 *
 * HW card statistics
 *
 */

#include "mtlkinc.h"
#include "hw_mmb.h"
#include "hw_mmb_priv.h"

#define LOG_LOCAL_GID   GID_HWMMB
#define LOG_LOCAL_FID   3


#define MTLK_GET_STATS_ADDRESS(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? (void*)MTLK_GET_STATS_ADDRESS_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? (void*)MTLK_GET_STATS_ADDRESS_D2(stats, parameter) : \
                            (void*)MTLK_GET_STATS_ADDRESS_G7(stats, parameter)))

#define MTLK_GET_STATS_ADDRESS_B0(stats, parameter) \
(&(((StatisticsDb_t_wave600b*)stats)->parameter))

#define MTLK_GET_STATS_ADDRESS_D2(stats, parameter) \
(&(((StatisticsDb_t_wave600d2*)stats)->parameter))

#define MTLK_GET_STATS_ADDRESS_G7(stats, parameter) \
(&(((StatisticsDb_t_wave700*)stats)->parameter))

#define MTLK_GET_STATS(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_GET_STATS_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_GET_STATS_D2(stats, parameter) : \
MTLK_GET_STATS_G7(stats, parameter)))

#define MTLK_GET_STATS_B0(stats, parameter) \
(((StatisticsDb_t_wave600b*)stats)->parameter)

#define MTLK_GET_STATS_D2(stats, parameter) \
(((StatisticsDb_t_wave600d2*)stats)->parameter)

#define MTLK_GET_STATS_G7(stats, parameter) \
(((StatisticsDb_t_wave700*)stats)->parameter)

#define MTLK_DIFF_STATS(stats_dst, stats_src, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_DIFF_STATS_B0(stats_dst, stats_src, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_DIFF_STATS_D2(stats_dst, stats_src, parameter) : \
MTLK_DIFF_STATS_G7(stats_dst, stats_src, parameter)))

#define MTLK_DIFF_STATS_B0(stats_dst, stats_src, parameter) \
(((StatisticsDb_t_wave600b*)stats_dst)->parameter - ((StatisticsDb_t_wave600b*)stats_src)->parameter )

#define MTLK_DIFF_STATS_D2(stats_dst, stats_src, parameter) \
(((StatisticsDb_t_wave600d2*)stats_dst)->parameter - ((StatisticsDb_t_wave600d2*)stats_src)->parameter )

#define MTLK_DIFF_STATS_G7(stats_dst, stats_src, parameter) \
(((StatisticsDb_t_wave700*)stats_dst)->parameter - ((StatisticsDb_t_wave700*)stats_src)->parameter )

#define MTLK_CLEAN_STATS(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_CLEAN_STATS_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_CLEAN_STATS_D2(stats, parameter) : \
MTLK_CLEAN_STATS_G7(stats, parameter)))

#define MTLK_CLEAN_STATS_B0(stats, parameter) \
memset(MTLK_GET_STATS_ADDRESS_B0(stats, parameter), \
       0, sizeof(MTLK_GET_STATS_B0(stats, parameter)))

#define MTLK_CLEAN_STATS_D2(stats, parameter) \
memset(MTLK_GET_STATS_ADDRESS_D2(stats, parameter), \
       0, sizeof(MTLK_GET_STATS_D2(stats, parameter)))

#define MTLK_CLEAN_STATS_G7(stats, parameter) \
memset(MTLK_GET_STATS_ADDRESS_G7(stats, parameter), \
       0, sizeof(MTLK_GET_STATS_G7(stats, parameter)))

#define MTLK_CONVERT_ENDIANESS_UINT16(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_CONVERT_ENDIANESS_UINT16_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_CONVERT_ENDIANESS_UINT16_D2(stats, parameter) : \
MTLK_CONVERT_ENDIANESS_UINT16_G7(stats, parameter)))

#define MTLK_CONVERT_ENDIANESS_UINT16_B0(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint16) == sizeof(MTLK_GET_STATS_B0(stats, parameter))); \
MTLK_GET_STATS_B0(stats, parameter) = MAC_TO_HOST16(MTLK_GET_STATS_B0(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_UINT16_D2(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint16) == sizeof(MTLK_GET_STATS_D2(stats, parameter))); \
MTLK_GET_STATS_D2(stats, parameter) = MAC_TO_HOST16(MTLK_GET_STATS_D2(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_UINT16_G7(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint16) == sizeof(MTLK_GET_STATS_G7(stats, parameter))); \
MTLK_GET_STATS_G7(stats, parameter) = MAC_TO_HOST16(MTLK_GET_STATS_G7(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_CONVERT_ENDIANESS_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_CONVERT_ENDIANESS_D2(stats, parameter) : \
MTLK_CONVERT_ENDIANESS_G7(stats, parameter)))

#define MTLK_CONVERT_ENDIANESS_UINT32 MTLK_CONVERT_ENDIANESS  /* alias */

#define MTLK_CONVERT_ENDIANESS_B0(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint32) == sizeof(MTLK_GET_STATS_B0(stats, parameter))); \
MTLK_GET_STATS_B0(stats, parameter) = MAC_TO_HOST32(MTLK_GET_STATS_B0(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_D2(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint32) == sizeof(MTLK_GET_STATS_D2(stats, parameter))); \
MTLK_GET_STATS_D2(stats, parameter) = MAC_TO_HOST32(MTLK_GET_STATS_D2(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_G7(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint32) == sizeof(MTLK_GET_STATS_G7(stats, parameter))); \
MTLK_GET_STATS_G7(stats, parameter) = MAC_TO_HOST32(MTLK_GET_STATS_G7(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_UINT64(stats, parameter, id) \
((_chipid_is_gen6_b0(id)) ? MTLK_CONVERT_ENDIANESS_UINT64_B0(stats, parameter) : \
((_chipid_is_gen6_d2(id)) ? MTLK_CONVERT_ENDIANESS_UINT64_D2(stats, parameter) : \
MTLK_CONVERT_ENDIANESS_UINT64_G7(stats, parameter)))

#define MTLK_CONVERT_ENDIANESS_UINT64_B0(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint64) == sizeof(MTLK_GET_STATS_B0(stats, parameter))); \
MTLK_GET_STATS_B0(stats, parameter) = MAC_TO_HOST64(MTLK_GET_STATS_B0(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_UINT64_D2(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint64) == sizeof(MTLK_GET_STATS_D2(stats, parameter))); \
MTLK_GET_STATS_D2(stats, parameter) = MAC_TO_HOST64(MTLK_GET_STATS_D2(stats, parameter)); \
}) \

#define MTLK_CONVERT_ENDIANESS_UINT64_G7(stats, parameter) \
({ \
MTLK_STATIC_ASSERT(sizeof(uint64) == sizeof(MTLK_GET_STATS_G7(stats, parameter))); \
MTLK_GET_STATS_G7(stats, parameter) = MAC_TO_HOST64(MTLK_GET_STATS_G7(stats, parameter)); \
}) \

#define MTLK_GET_STATS_SIZE(_stats, _array, id) \
((_chipid_is_gen6_b0(id)) ? (sizeof(MTLK_GET_STATS_B0(_stats, _array))) : \
((_chipid_is_gen6_d2(id)) ? (sizeof(MTLK_GET_STATS_D2(_stats, _array))) : \
(sizeof(MTLK_GET_STATS_G7(_stats, _array)))))

#define MTLK_GET_STATS_ARRAY_SIZE_16(_stats, _array, id) \
({ size_t size; \
 if (_chipid_is_gen6_b0(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_B0(_stats, _array))% sizeof(uint16))); \
    size = sizeof(MTLK_GET_STATS_B0(_stats, _array)); \
 } else if (_chipid_is_gen6_d2(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_D2(_stats, _array))% sizeof(uint16))); \
    size = sizeof(MTLK_GET_STATS_D2(_stats, _array)); \
 } else { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_G7(_stats, _array))% sizeof(uint16))); \
    size = sizeof(MTLK_GET_STATS_G7(_stats, _array)); \
 } \
 size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_16_B0(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_B0(_stats, _array))% sizeof(uint16))); \
   size = sizeof(MTLK_GET_STATS_B0(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_16_D2(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_D2(_stats, _array))% sizeof(uint16))); \
   size = sizeof(MTLK_GET_STATS_D2(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_16_G7(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_G7(_stats, _array))% sizeof(uint16))); \
   size = sizeof(MTLK_GET_STATS_G7(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_32(_stats, _array, id) \
({ size_t size; \
 if (_chipid_is_gen6_b0(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_B0(_stats, _array))% sizeof(uint32))); \
    size = sizeof(MTLK_GET_STATS_B0(_stats, _array)); \
 } else if ( _chipid_is_gen6_d2(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_D2(_stats, _array))% sizeof(uint32))); \
    size = sizeof(MTLK_GET_STATS_D2(_stats, _array)); \
 } else { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_G7(_stats, _array))% sizeof(uint32))); \
    size = sizeof(MTLK_GET_STATS_G7(_stats, _array)); \
 } \
 size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_32_B0(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_B0(_stats, _array))% sizeof(uint32))); \
   size = sizeof(MTLK_GET_STATS_B0(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_32_D2(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_D2(_stats, _array))% sizeof(uint32))); \
   size = sizeof(MTLK_GET_STATS_D2(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_32_G7(_stats, _array) \
({ size_t size; \
   MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_G7(_stats, _array))% sizeof(uint32))); \
   size = sizeof(MTLK_GET_STATS_G7(_stats, _array));\
   size; \
})

#define MTLK_GET_STATS_ARRAY_SIZE_64(_stats, _array, id) \
({ size_t size; \
 if (_chipid_is_gen6_b0(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_B0(_stats, _array))% sizeof(uint64))); \
    size = sizeof(MTLK_GET_STATS_B0(_stats, _array)); \
 } else if (_chipid_is_gen6_d2(id)) { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_D2(_stats, _array))% sizeof(uint64))); \
    size = sizeof(MTLK_GET_STATS_D2(_stats, _array)); \
 } else { \
    MTLK_STATIC_ASSERT(0 == (sizeof(MTLK_GET_STATS_G7(_stats, _array))% sizeof(uint64))); \
    size = sizeof(MTLK_GET_STATS_G7(_stats, _array)); \
 } \
 size; \
})


#define MTLK_CONVERT_ENDIANESS_ARRAY_16(_stats, _array, _chip_id) \
__mtlk_convert_endianess_array_uint16((uint16*)MTLK_GET_STATS_ADDRESS(_stats, _array, chip_id), MTLK_GET_STATS_ARRAY_SIZE_16(_stats, _array, chip_id))

#define MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(_stats, _array) \
__mtlk_convert_endianess_array_uint16((uint16*)MTLK_GET_STATS_ADDRESS_B0(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_16_B0(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(_stats, _array) \
__mtlk_convert_endianess_array_uint16((uint16*)MTLK_GET_STATS_ADDRESS_D2(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_16_D2(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(_stats, _array) \
__mtlk_convert_endianess_array_uint16((uint16*)MTLK_GET_STATS_ADDRESS_G7(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_16_G7(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_32(_stats, _array, _chip_id) \
__mtlk_convert_endianess_array_uint32((uint32*)MTLK_GET_STATS_ADDRESS(_stats, _array, chip_id), MTLK_GET_STATS_ARRAY_SIZE_32(_stats, _array, chip_id))

#define MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(_stats, _array) \
__mtlk_convert_endianess_array_uint32((uint32*)MTLK_GET_STATS_ADDRESS_B0(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_32_B0(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(_stats, _array) \
__mtlk_convert_endianess_array_uint32((uint32*)MTLK_GET_STATS_ADDRESS_D2(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_32_D2(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(_stats, _array) \
__mtlk_convert_endianess_array_uint32((uint32*)MTLK_GET_STATS_ADDRESS_G7(_stats, _array), MTLK_GET_STATS_ARRAY_SIZE_32_G7(_stats, _array))

#define MTLK_CONVERT_ENDIANESS_ARRAY_64(_stats, _array, _chip_id) \
__mtlk_convert_endianess_array_uint64((uint64*)MTLK_GET_STATS_ADDRESS(_stats, _array, chip_id), MTLK_GET_STATS_ARRAY_SIZE_64(_stats, _array, chip_id))

/* Linear value in fixed point representation in hex, obtained by converting 0 to -99dBm into linear value(mW) */
/* Fixed point value representaion format used:1Q31 */
/* Note: No value above 0 dBm is handled. */
const uint32 dbm_linear_lup_table[100] = {
0x80000000, 0x65AC8C2F, 0x50C335D3, 0x4026E73C, 0x32F52CFE, 0x287A26C4, 0x2026F30F, 0x198A1357, 0x144960C5, 0x101D3F2D,
0xCCCCCCC, 0xA2ADAD1, 0x8138561, 0x66A4A52, 0x518847F, 0x40C3713, 0x337184E, 0x28DCEBB, 0x207567A, 0x19C8651,
0x147AE14, 0x1044914, 0xCEC089, 0xA43AA1, 0x8273A6, 0x679F1B, 0x524F3B, 0x416179, 0x33EF0C, 0x2940A1,
0x20C49B, 0x1A074E, 0x14ACDA, 0x106C43, 0xD0B90, 0xA5CB5, 0x83B1F, 0x689BF, 0x53181, 0x42010,
0x346DC, 0x29A54, 0x21149, 0x1A46D, 0x14DF4, 0x10945, 0xD2B6, 0xA75F, 0x84F3, 0x699B,
0x53E2, 0x42A2, 0x34ED, 0x2A0A, 0x2165, 0x1A86, 0x1512, 0x10BC, 0xD4B, 0xA8F,
0x863, 0x6A9, 0x54A, 0x434, 0x356, 0x2A7, 0x21B, 0x1AC, 0x154, 0x10E,
0xD6, 0xAA, 0x87, 0x6B, 0x55, 0x43, 0x35, 0x2A, 0x22, 0x1B,
0x15, 0x11, 0xD, 0xA, 0x8, 0x6, 0x5, 0x4, 0x3, 0x2,
0x2, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

void __MTLK_IFUNC
wave_hw_reset_he_eht_la_mu_stats (mtlk_hw_t *hw, int group_id, BOOL full_reset)
{
  int i;
  int chip_id = hw_mmb_get_chip_id(hw);
  hw_statistics_t *hw_stats = &hw->hw_stats;

  if ((group_id >= 0) && (group_id < MTLK_ARRAY_SIZE(hw_stats->mu_he_eht_la_stats32))) {
    if (full_reset) {
      memset(&hw_stats->mu_he_eht_la_stats32[group_id], 0, sizeof(hw_stats->mu_he_eht_la_stats32[group_id]));
      memset(&hw_stats->mu_he_eht_la_stats[group_id], 0, sizeof(hw_stats->mu_he_eht_la_stats[group_id]));
    }
    else {
      memset(&hw_stats->mu_he_eht_la_stats32[group_id].dl, 0, sizeof(hw_stats->mu_he_eht_la_stats32[group_id].dl));
      memset(&hw_stats->mu_he_eht_la_stats32[group_id].ul, 0, sizeof(hw_stats->mu_he_eht_la_stats32[group_id].ul));
    }
  }
  else {
    if (full_reset) {
      memset(hw_stats->mu_he_eht_la_stats32, 0, sizeof(hw_stats->mu_he_eht_la_stats32));
      memset(hw_stats->mu_he_eht_la_stats, 0, sizeof(hw_stats->mu_he_eht_la_stats));
    }
    else {
      for (i = 0; i < MTLK_ARRAY_SIZE(hw_stats->mu_he_eht_la_stats32); i++) {
        memset(&hw_stats->mu_he_eht_la_stats32[i].dl, 0, sizeof(hw_stats->mu_he_eht_la_stats32[i].dl));
        memset(&hw_stats->mu_he_eht_la_stats32[i].ul, 0, sizeof(hw_stats->mu_he_eht_la_stats32[i].ul));
      }
    }
  }

  hw_stats->mu_he_eht_la_dl_info = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.dlToggleBitmap, chip_id);
  hw_stats->mu_he_eht_la_ul_info = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.ulToggleBitmap, chip_id);
}

static __INLINE void __stats_accumulate_32bit_from_16bit (uint32* sum, uint16* prev_val, uint16 new_val)
{
  uint16 delta;
  delta = new_val - *prev_val;
  *sum += delta;
  *prev_val = new_val;
}

static __INLINE void __stats_accumulate_32bit_from_8bit (uint32* sum, uint8* prev_val, uint8 new_val)
{
  uint8 delta;
  delta = new_val - *prev_val;
  *sum += delta;
  *prev_val = new_val;
}

static __INLINE void __wave_reset_stats_if_new_mu_group (mtlk_hw_t *hw, int chip_id, int group_id, int new_fw_idx, int new_type)
{
  BOOL is_new_group = FALSE;
  hw_statistics_t *hw_stats = &hw->hw_stats;
  uint32 dlToggleBitmap = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.dlToggleBitmap, chip_id);
  uint32 ulToggleBitmap = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.ulToggleBitmap, chip_id);

  if ((hw_stats->mu_he_eht_la_stats32[group_id].fw_idx != new_fw_idx) ||
      (hw_stats->mu_he_eht_la_stats32[group_id].type != new_type)) {
    is_new_group = TRUE;
  }
  else if (new_type == MU_GROUP_DL) {
    if (MTLK_BIT_GET(dlToggleBitmap, group_id) ^ MTLK_BIT_GET(hw_stats->mu_he_eht_la_dl_info, group_id))
      is_new_group = TRUE;
  }
  else { /* MU_GROUP_UL */
    if (MTLK_BIT_GET(ulToggleBitmap, group_id) ^ MTLK_BIT_GET(hw_stats->mu_he_eht_la_ul_info, group_id))
      is_new_group = TRUE;
  }

  if (!is_new_group)
    return;

  wave_hw_reset_he_eht_la_mu_stats(hw, group_id, TRUE);

  hw_stats->mu_he_eht_la_stats32[group_id].fw_idx = new_fw_idx;
  hw_stats->mu_he_eht_la_stats32[group_id].type = new_type;
  hw_stats->mu_he_eht_la_stats32[group_id].active = TRUE;
}

static void _wave_accumulate_mu_he_eht_la_group_stats (mtlk_hw_t *hw, int chip_id, int group_id, BOOL is_dl)
{
  int fw_idx, i, j;
  uint8 fw_stats_grp;
  hw_statistics_t *hw_stats = &hw->hw_stats;
  int max_mcs = _chipid_is_gen7(chip_id) ? MAX_MCS_GEN7 : MAX_MCS;

  if (is_dl) {
    for (fw_idx = 0; fw_idx < MAX_NUM_OF_HE_MU_DL_GROUPS; fw_idx++) {
      fw_stats_grp = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].groupId, chip_id);
      if (fw_stats_grp == group_id)
        break;
    }

    if (MAX_NUM_OF_HE_MU_DL_GROUPS == fw_idx) {
      WLOG_D("MU LA Stats: group %d set in DL bitmap but no data available", group_id);
      return;
    }

    __wave_reset_stats_if_new_mu_group(hw, chip_id, group_id, fw_idx, MU_GROUP_DL);

    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.protectionSentCnt,
                                        &hw_stats->mu_he_eht_la_stats[group_id].dl.protectionSentCnt,
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].protectionSentCnt, chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.protectionSucceededCnt,
                                        &hw_stats->mu_he_eht_la_stats[group_id].dl.protectionSucceededCnt,
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].protectionSucceededCnt, chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.dlPhaseAllocFailCnt,
                                        &hw_stats->mu_he_eht_la_stats[group_id].dl.dlPhaseAllocFailCnt,
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].dlPhaseAllocFailCnt, chip_id));

    for (i = 0; i < MAX_STA_IN_GROUP; i++) {
      __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.heMuFastProbeTriggeredCnt[i],
                                          &hw_stats->mu_he_eht_la_stats[group_id].dl.heMuFastProbeTriggeredCnt[i],
                                          MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].heMuFastProbeTriggeredCnt[i], chip_id));
      __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.heMuFastProbeNotFinishedCnt[i],
                                          &hw_stats->mu_he_eht_la_stats[group_id].dl.heMuFastProbeNotFinishedCnt[i],
                                          MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].heMuFastProbeNotFinishedCnt[i], chip_id));
      __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.heMuCollisionDetectedCnt[i],
                                          &hw_stats->mu_he_eht_la_stats[group_id].dl.heMuCollisionDetectedCnt[i],
                                          MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].heMuCollisionDetectedCnt[i], chip_id));

      for (j = 0; j < max_mcs; j++) {
        __stats_accumulate_32bit_from_8bit(&hw_stats->mu_he_eht_la_stats32[group_id].dl.mcsHistogram[i][j],
                                           &hw_stats->mu_he_eht_la_stats[group_id].dl.mcsHistogram[i][j],
                                           MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[fw_idx].mcsHistogram[i][j], chip_id));
      }
    }

    return;
  }

  /* Uplink */
  for (fw_idx = 0; fw_idx < MAX_NUM_OF_HE_MU_UL_GROUPS; fw_idx++) {
    fw_stats_grp = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].groupId, chip_id);
    if (fw_stats_grp == group_id)
      break;
  }

  if (MAX_NUM_OF_HE_MU_UL_GROUPS == fw_idx) {
    WLOG_D("MU LA Stats: group %d set in UL bitmap but no data available", group_id);
    return;
  }

  __wave_reset_stats_if_new_mu_group(hw, chip_id, group_id, fw_idx, MU_GROUP_UL);

  __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.ulPCStabilityCnt,
                                      &hw_stats->mu_he_eht_la_stats[group_id].ul.ulPCStabilityCnt,
                                      MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].ulPCStabilityCnt, chip_id));

  for (i = 0; i < MAX_STA_IN_GROUP; i++) {
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.fastDropCnt[i],
                                        &hw_stats->mu_he_eht_la_stats[group_id].ul.fastDropCnt[i],
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].fastDropCnt[i], chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.invalidReportsCnt[i],
                                        &hw_stats->mu_he_eht_la_stats[group_id].ul.invalidReportsCnt[i],
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].invalidReportsCnt[i], chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.rxPPDUsCnt[i],
                                        &hw_stats->mu_he_eht_la_stats[group_id].ul.rxPPDUsCnt[i],
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].rxPPDUsCnt[i], chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.changedWpCnt[i],
                                        &hw_stats->mu_he_eht_la_stats[group_id].ul.changedWpCnt[i],
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].changedWpCnt[i], chip_id));
    __stats_accumulate_32bit_from_16bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.collisionDetectedCnt[i],
                                        &hw_stats->mu_he_eht_la_stats[group_id].ul.collisionDetectedCnt[i],
                                        MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].collisionDetectedCnt[i], chip_id));

    for (j = 0; j < max_mcs; j++) {
      __stats_accumulate_32bit_from_8bit(&hw_stats->mu_he_eht_la_stats32[group_id].ul.mcsHistogram[i][j],
                                         &hw_stats->mu_he_eht_la_stats[group_id].ul.mcsHistogram[i][j],
                                         MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[fw_idx].mcsHistogram[i][j], chip_id));
    }
  }
}

static void _wave_update_mu_he_eht_la_stats (mtlk_hw_t *hw)
{
  BOOL dl_group_exists, ul_group_exists;
  int i, chip_id = hw_mmb_get_chip_id(hw);
  hw_statistics_t *hw_stats = &hw->hw_stats;
  uint32 dl_bitmap = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.dlBitmap, chip_id);
  uint32 ul_bitmap = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.ulBitmap, chip_id);

  MTLK_STATIC_ASSERT(MAX_NUM_OF_HE_GROUPS <= NUM_OF_BITS_IN_UINT32);
  for (i = 0; i < MAX_NUM_OF_HE_GROUPS; i++) {
    dl_group_exists = MTLK_BIT_GET(dl_bitmap, i);
    ul_group_exists = MTLK_BIT_GET(ul_bitmap, i);

    if (dl_group_exists && ul_group_exists) {
      WLOG_D("MU LA Stats: group %d provided as both DL and UL", i);
      wave_hw_reset_he_eht_la_mu_stats(hw, i, TRUE);
      continue;
    }

    if (dl_group_exists || ul_group_exists) {
      _wave_accumulate_mu_he_eht_la_group_stats(hw, chip_id, i, dl_group_exists);
      continue;
    }

    hw_stats->mu_he_eht_la_stats32[i].active = FALSE;
  }
}

mtlk_error_t __MTLK_IFUNC
wave_hw_get_la_mu_he_eht_stats (mtlk_hw_t *hw,
                                mtlk_wssa_drv_la_mu_he_eht_stats_t *out_stats,
                                mtlk_wssa_drv_la_mu_he_eht_input_t *params)
{
  mtlk_error_t res = MTLK_ERR_OK;
  int grp_id;
  wave_mu_he_eht_la_stats32_t *stats;
  int chip_id = hw_mmb_get_chip_id(hw);
  hw_statistics_t *hw_stats = &hw->hw_stats;

  if (!params) {
    ELOG_V("No input params provided");
    return MTLK_ERR_PARAMS;
  }

  grp_id = params->group_id;
  mtlk_osal_lock_acquire(&hw_stats->lock);

  if (params->reset_stats == LA_MU_HE_EHT_RESET_ALL) {
    wave_hw_reset_he_eht_la_mu_stats(hw, HE_GROUP_MANAGER_INVALID_GROUP_INDEX, FALSE);
    goto end;
  }
  else if (params->reset_stats == LA_MU_HE_EHT_RESET_GROUP) {
    wave_hw_reset_he_eht_la_mu_stats(hw, grp_id, FALSE);
    goto end;
  }

  if (!out_stats) {
    ELOG_V("No buffer for LA MU HE and EHT statistics provided");
    res = MTLK_ERR_PARAMS;
    goto end;
  }

  /* Read stats */
  stats = &hw_stats->mu_he_eht_la_stats32[grp_id];
  out_stats->type = stats->type;
  if (out_stats->type == MU_GROUP_INVALID)
    goto end;

  out_stats->active = stats->active;

  if (out_stats->type == MU_GROUP_DL) {
    out_stats->creation_toggle_bitmap = hw_stats->mu_he_eht_la_dl_info;
    out_stats->dl.dlPhaseAllocFailCnt = stats->dl.dlPhaseAllocFailCnt;
    out_stats->dl.protectionSentCnt = stats->dl.protectionSentCnt;
    out_stats->dl.protectionSucceededCnt = stats->dl.protectionSucceededCnt;

    wave_memcpy(out_stats->dl.heMuCollisionDetectedCnt, sizeof(out_stats->dl.heMuCollisionDetectedCnt),
                stats->dl.heMuCollisionDetectedCnt, sizeof(stats->dl.heMuCollisionDetectedCnt));
    wave_memcpy(out_stats->dl.heMuFastProbeNotFinishedCnt, sizeof(out_stats->dl.heMuFastProbeNotFinishedCnt),
                stats->dl.heMuFastProbeNotFinishedCnt, sizeof(stats->dl.heMuFastProbeNotFinishedCnt));
    wave_memcpy(out_stats->dl.heMuFastProbeTriggeredCnt, sizeof(out_stats->dl.heMuFastProbeTriggeredCnt),
                stats->dl.heMuFastProbeTriggeredCnt, sizeof(stats->dl.heMuFastProbeTriggeredCnt));
    wave_memcpy(out_stats->dl.mcsHistogram, sizeof(out_stats->dl.mcsHistogram),
                stats->dl.mcsHistogram, sizeof(stats->dl.mcsHistogram));

    wave_memcpy(out_stats->dl.staId, sizeof(out_stats->dl.staId),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].staId, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].staId, chip_id));
    wave_memcpy(out_stats->dl.targetRssi, sizeof(out_stats->dl.targetRssi),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].targetRssi, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].targetRssi, chip_id));
    wave_memcpy(out_stats->dl.txPower, sizeof(out_stats->dl.txPower),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].txPower, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[stats->fw_idx].txPower, chip_id));
  }
  else {
    out_stats->creation_toggle_bitmap = hw_stats->mu_he_eht_la_ul_info;
    out_stats->ul.ulPCStabilityCnt = stats->ul.ulPCStabilityCnt;

    wave_memcpy(out_stats->ul.fastDropCnt, sizeof(out_stats->ul.fastDropCnt),
                stats->ul.fastDropCnt, sizeof(stats->ul.fastDropCnt));
    wave_memcpy(out_stats->ul.invalidReportsCnt, sizeof(out_stats->ul.invalidReportsCnt),
                stats->ul.invalidReportsCnt, sizeof(stats->ul.invalidReportsCnt));
    wave_memcpy(out_stats->ul.rxPPDUsCnt, sizeof(out_stats->ul.rxPPDUsCnt),
                stats->ul.rxPPDUsCnt, sizeof(stats->ul.rxPPDUsCnt));
    wave_memcpy(out_stats->ul.changedWpCnt, sizeof(out_stats->ul.changedWpCnt),
                stats->ul.changedWpCnt, sizeof(stats->ul.changedWpCnt));
    wave_memcpy(out_stats->ul.collisionDetectedCnt, sizeof(out_stats->ul.collisionDetectedCnt),
                stats->ul.collisionDetectedCnt, sizeof(stats->ul.collisionDetectedCnt));
    wave_memcpy(out_stats->ul.mcsHistogram, sizeof(out_stats->ul.mcsHistogram),
                stats->ul.mcsHistogram, sizeof(stats->ul.mcsHistogram));

    wave_memcpy(out_stats->ul.staId, sizeof(out_stats->ul.staId),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].staId, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].staId, chip_id));
    wave_memcpy(out_stats->ul.targetRSSI, sizeof(out_stats->ul.targetRSSI),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].targetRSSI, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].targetRSSI, chip_id));
    wave_memcpy(out_stats->ul.longTermErrorRate, sizeof(out_stats->ul.longTermErrorRate),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].longTermErrorRate, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].longTermErrorRate, chip_id));
    wave_memcpy(out_stats->ul.workingPointErrorRate, sizeof(out_stats->ul.workingPointErrorRate),
                MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].workingPointErrorRate, chip_id),
                MTLK_GET_STATS_SIZE(hw_stats->stats_copy, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[stats->fw_idx].workingPointErrorRate, chip_id));
  }

end:
  mtlk_osal_lock_release(&hw_stats->lock);
  return res;
}

void __MTLK_IFUNC
wave_hw_get_la_mu_groups_stats (mtlk_hw_t *hw, mtlk_wssa_drv_la_mu_groups_stats_t *stats)
{
  int chip_id = hw->card_info.chip_id;
  hw_statistics_t *hw_stats = &hw->hw_stats;
  int i, max_removal_reason;

  memset(stats, 0, sizeof(mtlk_wssa_drv_la_mu_groups_stats_t));

  if (_chipid_is_gen7(chip_id)) {
    stats->is_gen6 = FALSE;
    max_removal_reason = MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS_GEN7;
  } else {
    stats->is_gen6 = TRUE;
    max_removal_reason = MAX_NUM_OF_MU_GROUPS_REMOVAL_REASONS;
 }

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->dlOfdmaGroupsCounters.groupsCreated = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsCreatedCounter, chip_id);
  stats->dlOfdmaGroupsCounters.groupsRemoved = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsRemovedCounter, chip_id);

  for (i = 0; i < max_removal_reason; i++) {
    stats->dlOfdmaGroupsCounters.removalReason[i] = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsRemovalReasonCounters[i], chip_id);
  }

  stats->ulOfdmaGroupsCounters.groupsCreated = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsCreatedCounter, chip_id);
  stats->ulOfdmaGroupsCounters.groupsRemoved = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsRemovedCounter, chip_id);

  for (i = 0; i < max_removal_reason; i++) {
    stats->ulOfdmaGroupsCounters.removalReason[i] = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsRemovalReasonCounters[i], chip_id);
  }

  stats->dlMimoGroupsCounters.groupsCreated = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsCreatedCounter, chip_id);
  stats->dlMimoGroupsCounters.groupsRemoved = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsRemovedCounter, chip_id);

  for (i = 0; i < max_removal_reason; i++) {
    stats->dlMimoGroupsCounters.removalReason[i] = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsRemovalReasonCounters[i], chip_id);
  }

  if (_chipid_is_gen7(chip_id)) {
    stats->ulMimoGroupsCounters.groupsCreated = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsCreatedCounter);
    stats->ulMimoGroupsCounters.groupsRemoved = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovedCounter);

    for (i = 0; i < max_removal_reason; i++) {
      stats->ulMimoGroupsCounters.removalReason[i] = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovalReasonCounters[i]);
    }
  }

  mtlk_osal_lock_release(&hw_stats->lock);
}

static void
_mtlk_hw_mhi_update_hw_stats (mtlk_hw_t *hw, hw_statistics_t *hw_stats)
{
    unsigned             cbw;
    unsigned             radio_idx;
    int                  chip_id = hw_mmb_get_chip_id(hw);

    mtlk_osal_lock_acquire(&hw_stats->lock);
    for (radio_idx = 0; radio_idx < hw->radio_descr->num_radios; radio_idx++) {
      for (cbw = 0; cbw < PHY_STAT_RATE_CBW_NUM; cbw++) {
        hw_stats->tx_power[radio_idx].pw_min_ant[cbw] =
              STAT_PW_TO_POWER(MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.minPower[radio_idx][cbw], chip_id));
        hw_stats->tx_power[radio_idx].pw_max_ant[cbw] =
              STAT_PW_TO_POWER(MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.maxPower[radio_idx][cbw], chip_id));
      }
    }

    mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
mtlk_hw_mhi_get_tx_power (mtlk_hw_t *hw, mtlk_hw_tx_power_t *tx_power, unsigned radio_idx)
{
    hw_statistics_t *hw_stats;

    MTLK_ASSERT(hw);
    MTLK_ASSERT(tx_power);

    hw_stats = &hw->hw_stats;
    mtlk_osal_lock_acquire(&hw_stats->lock);
    *tx_power = hw_stats->tx_power[radio_idx];  /* struct copy */
    mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
mtlk_hw_mhi_get_vap_stats (mtlk_hw_t *hw, mtlk_mhi_stats_vap_t *vap_stats, unsigned vap_id)
{
  hw_statistics_t        *hw_stats;
  int                     chip_id = hw_mmb_get_chip_id(hw);
  mtlk_vap_stats         *stats;

  MTLK_ASSERT(hw);
  MTLK_ASSERT(vap_id < hw->max_vaps_fw); /* max_vaps for card */

  hw_stats = &hw->hw_stats;
  stats = &vap_stats->stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->txInUnicastHd                        = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInUnicastHd, chip_id);
  stats->txInMulticastHd                      = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInMulticastHd, chip_id);
  stats->txInBroadcastHd                      = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInBroadcastHd, chip_id);
  stats->txInUnicastNumOfBytes                = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInUnicastNumOfBytes, chip_id);
  stats->txInMulticastNumOfBytes              = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInMulticastNumOfBytes, chip_id);
  stats->txInBroadcastNumOfBytes              = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].txInBroadcastNumOfBytes, chip_id);
  stats->rxOutUnicastHd                       = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutUnicatHd, chip_id);
  stats->rxOutMulticastHd                     = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutMulticastHd, chip_id);
  stats->rxOutBroadcastHd                     = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutBroadcastHd, chip_id);
  stats->rxOutUnicastNumOfBytes               = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutUnicastNumOfBytes, chip_id);
  stats->rxOutMulticastNumOfBytes             = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutMulticastNumOfBytes, chip_id);
  stats->rxOutBroadcastNumOfBytes             = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].rxOutBroadcastNumOfBytes, chip_id);
  stats->agerCount                            = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, hostIfCounters.vapCounts[vap_id].agerCount, chip_id);

  stats->dropMpdu                             = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].dropMpdu, chip_id);
  stats->mpduUnicastOrMngmnt                  = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].mpduUnicastOrMngmnt, chip_id);
  stats->mpduRetryCount                       = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].mpduRetryCount, chip_id);
  stats->mpduTypeNotSupported                 = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].mpduTypeNotSupported, chip_id);
  stats->replayData                           = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].replayData, chip_id);
  stats->replayMngmnt                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].replayMngmnt, chip_id);
  stats->ampdu                                = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].ampdu, chip_id);
  stats->bcMcCountVap                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.rxppVapCounts[vap_id].bcMcCountVap, chip_id);

  stats->rtsSuccessCount                      = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].rtsSuccessCount, chip_id);
  stats->qosTransmittedFrames                 = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].qosTransmittedFrames, chip_id);
  stats->transmittedAmsdu                     = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmittedAmsdu, chip_id);
  stats->transmittedOctetsInAmsdu             = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmittedOctetsInAmsdu, chip_id);
  stats->transmittedAmpdu                     = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmittedAmpdu, chip_id);
  stats->transmittedMpduInAmpdu               = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmittedMpduInAmpdu, chip_id);
  stats->transmittedOctetsInAmpdu             = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmittedOctetsInAmpdu, chip_id);
  stats->beamformingFrames                    = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].beamformingFrames, chip_id);
  stats->transmitStreamRprtMSDUFailed         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitStreamRprtMSDUFailed, chip_id);
  stats->rtsFailure                           = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].rtsFailure, chip_id);
  stats->ackFailure                           = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].ackFailure, chip_id);
  stats->failedAmsdu                          = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].failedAmsdu, chip_id);
  stats->retryAmsdu                           = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].retryAmsdu, chip_id);
  stats->multipleRetryAmsdu                   = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].multipleRetryAmsdu, chip_id);
  stats->amsduAckFailure                      = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].amsduAckFailure, chip_id);
  stats->implicitBarFailure                   = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].implicitBarFailure, chip_id);
  stats->explicitBarFailure                   = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].explicitBarFailure, chip_id);
  stats->transmitStreamRprtMultipleRetryCount = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitStreamRprtMultipleRetryCount, chip_id);
  stats->transmitBw20                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitBw20, chip_id);
  stats->transmitBw40                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitBw40, chip_id);
  stats->transmitBw80                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitBw80, chip_id);
  stats->transmitBw160                        = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitBw160, chip_id);
  if (_chipid_is_gen7(chip_id)) {
    stats->transmitBw320                      = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.vapCounts[vap_id].transmitBw320);
  } else {
    stats->transmitBw320                      = 0;
  }
  stats->rxGroupFrame                         = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.rxGroupFrame[vap_id], chip_id);
  stats->txSenderError                        = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, baaCounters.txSenderError[vap_id], chip_id);

  if (_chipid_is_gen6_b0(chip_id)) {
    stats->amsdu                              = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].amsdu);
    stats->amsduBytes                         = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].amsduBytes);
    stats->dropCount                          = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].dropCount);
    stats->mpduInAmpdu                        = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].mpduInAmpdu);
    stats->octetsInAmpdu                      = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].octetsInAmpdu);
    stats->rxClassifierSecurityMismatch       = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].rxCoordinatorSecurityMismatch);
    stats->tkipCount                          = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].tkipCount);
    stats->securityFailure                    = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot, rxCounters.vapCounts[vap_id].securityFailure);
  } else if (_chipid_is_gen6_d2(chip_id)){
    stats->amsdu                              = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].amsdu);
    stats->amsduBytes                         = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].amsduBytes);
    stats->dropCount                          = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].dropCount);
    stats->mpduInAmpdu                        = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].mpduInAmpdu);
    stats->octetsInAmpdu                      = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].octetsInAmpdu);
    stats->rxClassifierSecurityMismatch       = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].rxCoorSecurityMismatch);
    stats->tkipCount                          = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].tkipCount);
    stats->securityFailure                    = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].securityFailure);
  } else {
    stats->amsdu                              = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].amsdu);
    stats->amsduBytes                         = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].amsduBytes);
    stats->dropCount                          = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].dropCount);
    stats->mpduInAmpdu                        = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].mpduInAmpdu);
    stats->octetsInAmpdu                      = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].octetsInAmpdu);
    stats->rxClassifierSecurityMismatch       = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].rxCoorSecurityMismatch);
    stats->tkipCount                          = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].tkipCount);
    stats->securityFailure                    = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot, RxCoordinatorCounters.vapCounts[vap_id].securityFailure);
  }
  mtlk_osal_lock_release(&hw_stats->lock);
}

static __INLINE void
__wave_hw_mhi_sta_fill_tx_rate_statistic_wave600b (mtlk_mhi_stats_sta_tx_rate_t *dst_stats, StatisticsDb_t_wave600b *src_stats, uint32 sid)
{
    uint8 cbw = NUM_OF_BW;
    uint8 rate = 0;

    if ((sid < HW_NUM_OF_STATIONS) && (((cbw = src_stats->linkAdaptationStats.dataBwLimit[sid]) < NUM_OF_BW))) {
        dst_stats->dataBwLimit       = cbw;
        dst_stats->DataPhyMode       = src_stats->linkAdaptationStats.DataPhyMode[sid][cbw];
        dst_stats->scpData           = src_stats->linkAdaptationStats.scpData[sid][cbw];
        dst_stats->mcsData           = src_stats->linkAdaptationStats.mcsData[sid][cbw];
        dst_stats->nssData           = src_stats->linkAdaptationStats.nssData[sid][cbw];
        dst_stats->bfModeData        = src_stats->linkAdaptationStats.bfModeData[sid];
        dst_stats->stbcModeData      = src_stats->linkAdaptationStats.stbcModeData[sid];
        dst_stats->powerData         = src_stats->linkAdaptationStats.powerData[sid][cbw];
        dst_stats->powerManagement   = src_stats->linkAdaptationStats.powerManagement[sid][cbw];
        dst_stats->ManagementPhyMode = src_stats->linkAdaptationStats.ManagementPhyMode[sid][cbw];
        dst_stats->antennaSelectionData = src_stats->linkAdaptationStats.antennaSelectionData[sid][cbw];
        dst_stats->antennaSelectionManagement = src_stats->linkAdaptationStats.antennaSelectionManagement[sid][cbw];
        dst_stats->stationCapabilities = src_stats->linkAdaptationStats.stationCapabilities[sid];
        dst_stats->raState             = src_stats->linkAdaptationStats.raState[sid];
        dst_stats->raStability         = src_stats->linkAdaptationStats.raStability[sid];
        dst_stats->nssManagement       = src_stats->linkAdaptationStats.nssManagement[sid][cbw];
        dst_stats->mcsManagement       = src_stats->linkAdaptationStats.mcsManagement[sid][cbw];
        dst_stats->bw                  = src_stats->linkAdaptationStats.bw[sid];
        dst_stats->transmittedAmpdu    = src_stats->linkAdaptationStats.transmittedAmpdu[sid];
        dst_stats->totalTxTime         = src_stats->linkAdaptationStats.totalTxTime[sid];
        dst_stats->mpduInAmpdu         = src_stats->linkAdaptationStats.mpduInAmpdu[sid];
        dst_stats->ewmaTimeNsec        = src_stats->linkAdaptationStats.ewmaTimeNsec[sid];
        dst_stats->sumTimeNsec         = src_stats->linkAdaptationStats.sumTimeNsec[sid];
        dst_stats->numMsdus            = src_stats->linkAdaptationStats.numMsdus[sid];
        dst_stats->per                 = src_stats->linkAdaptationStats.per[sid];
        dst_stats->totalPsduLength     = src_stats->linkAdaptationStats.totalPsduLength[sid];
        dst_stats->totalZld            = src_stats->linkAdaptationStats.totalZld[sid];
        for (rate = 0; rate < RATES_BIT_MASK_SIZE; rate++) {
            dst_stats->ratesMask[rate] = src_stats->linkAdaptationStats.ratesMask[sid][rate];
        }

    }
    else {
        ILOG1_DD("Incorrect SID (%u) or channel BW (%u): cannot read TX rate statistics", sid, cbw);
    }
}

static __INLINE void
__wave_hw_mhi_sta_fill_tx_rate_statistic_wave600d2 (mtlk_mhi_stats_sta_tx_rate_t *dst_stats, StatisticsDb_t_wave600d2 *src_stats, uint32 sid)
{
    uint8 cbw = NUM_OF_BW;
    uint8 rate = 0;

    if ((sid < HW_NUM_OF_STATIONS) && (((cbw = src_stats->linkAdaptationStats.dataBwLimit[sid]) < NUM_OF_BW))) {
        dst_stats->dataBwLimit       = cbw;
        dst_stats->DataPhyMode       = src_stats->linkAdaptationStats.DataPhyMode[sid][cbw];
        dst_stats->scpData           = src_stats->linkAdaptationStats.scpData[sid][cbw];
        dst_stats->mcsData           = src_stats->linkAdaptationStats.mcsData[sid][cbw];
        dst_stats->nssData           = src_stats->linkAdaptationStats.nssData[sid][cbw];
        dst_stats->bfModeData        = src_stats->linkAdaptationStats.bfModeData[sid];
        dst_stats->stbcModeData      = src_stats->linkAdaptationStats.stbcModeData[sid];
        dst_stats->powerData         = src_stats->linkAdaptationStats.powerData[sid][cbw];
        dst_stats->powerManagement   = src_stats->linkAdaptationStats.powerManagement[sid][cbw];
        dst_stats->ManagementPhyMode = src_stats->linkAdaptationStats.ManagementPhyMode[sid][cbw];
        dst_stats->antennaSelectionData = src_stats->linkAdaptationStats.antennaSelectionData[sid][cbw];
        dst_stats->antennaSelectionManagement = src_stats->linkAdaptationStats.antennaSelectionManagement[sid][cbw];
        dst_stats->stationCapabilities = src_stats->linkAdaptationStats.stationCapabilities[sid];
        dst_stats->raState             = src_stats->linkAdaptationStats.raState[sid];
        dst_stats->raStability         = src_stats->linkAdaptationStats.raStability[sid];
        dst_stats->nssManagement       = src_stats->linkAdaptationStats.nssManagement[sid][cbw];
        dst_stats->mcsManagement       = src_stats->linkAdaptationStats.mcsManagement[sid][cbw];
        dst_stats->bw                  = src_stats->linkAdaptationStats.bw[sid];
        dst_stats->transmittedAmpdu    = src_stats->linkAdaptationStats.transmittedAmpdu[sid];
        dst_stats->totalTxTime         = src_stats->linkAdaptationStats.totalTxTime[sid];
        dst_stats->mpduInAmpdu         = src_stats->linkAdaptationStats.mpduInAmpdu[sid];
        dst_stats->ewmaTimeNsec        = src_stats->linkAdaptationStats.ewmaTimeNsec[sid];
        dst_stats->sumTimeNsec         = src_stats->linkAdaptationStats.sumTimeNsec[sid];
        dst_stats->numMsdus            = src_stats->linkAdaptationStats.numMsdus[sid];
        dst_stats->per                 = src_stats->linkAdaptationStats.per[sid];
        dst_stats->totalPsduLength     = src_stats->linkAdaptationStats.totalPsduLength[sid];
        dst_stats->totalZld            = src_stats->linkAdaptationStats.totalZld[sid];
        for (rate = 0; rate < RATES_BIT_MASK_SIZE; rate++) {
            dst_stats->ratesMask[rate] = src_stats->linkAdaptationStats.ratesMask[sid][rate];
        }
    }
    else {
        ILOG1_DD("Incorrect SID (%u) or channel BW (%u): cannot read TX rate statistics", sid, cbw);
    }
}

static __INLINE void
__wave_hw_mhi_sta_fill_tx_rate_statistic_wave700 (mtlk_mhi_stats_sta_tx_rate_t *dst_stats, StatisticsDb_t_wave700 *src_stats, uint32 sid)
{
  uint8 cbw = BW_MAX_G7;
  uint8 rate = 0;
  if ((sid < GEN7_MAX_SID) && (((cbw = src_stats->linkAdaptationStats.dataBwLimit[sid]) < BW_MAX_G7))) {
    dst_stats->dataBwLimit       = cbw;
    dst_stats->DataPhyMode       = src_stats->linkAdaptationStats.DataPhyMode[sid][cbw];
    dst_stats->scpData           = src_stats->linkAdaptationStats.scpData[sid][cbw];
    dst_stats->mcsData           = src_stats->linkAdaptationStats.mcsData[sid][cbw];
    dst_stats->nssData           = src_stats->linkAdaptationStats.nssData[sid][cbw];
    dst_stats->bfModeData        = src_stats->linkAdaptationStats.bfModeData[sid];
    dst_stats->stbcModeData      = src_stats->linkAdaptationStats.stbcModeData[sid];
    dst_stats->powerData         = src_stats->linkAdaptationStats.powerData[sid][cbw];
    dst_stats->powerManagement   = src_stats->linkAdaptationStats.powerManagement[sid][cbw];
    dst_stats->ManagementPhyMode = src_stats->linkAdaptationStats.ManagementPhyMode[sid][cbw];
    dst_stats->antennaSelectionData = src_stats->linkAdaptationStats.antennaSelectionData[sid][cbw];
    dst_stats->antennaSelectionManagement = src_stats->linkAdaptationStats.antennaSelectionManagement[sid][cbw];
    dst_stats->stationCapabilities = src_stats->linkAdaptationStats.stationCapabilities[sid];
    dst_stats->raState             = src_stats->linkAdaptationStats.raState[sid];
    dst_stats->raStability         = src_stats->linkAdaptationStats.raStability[sid];
    dst_stats->nssManagement       = src_stats->linkAdaptationStats.nssManagement[sid][cbw];
    dst_stats->mcsManagement       = src_stats->linkAdaptationStats.mcsManagement[sid][cbw];
    dst_stats->bw                  = src_stats->linkAdaptationStats.bw[sid];
    dst_stats->transmittedAmpdu    = src_stats->linkAdaptationStats.transmittedAmpdu[sid];
    dst_stats->totalTxTime         = src_stats->linkAdaptationStats.totalTxTime[sid];
    dst_stats->mpduInAmpdu         = src_stats->linkAdaptationStats.mpduInAmpdu[sid];
    dst_stats->ewmaTimeNsec        = src_stats->linkAdaptationStats.ewmaTimeNsec[sid];
    dst_stats->sumTimeNsec         = src_stats->linkAdaptationStats.sumTimeNsec[sid];
    dst_stats->numMsdus            = src_stats->linkAdaptationStats.numMsdus[sid];
    dst_stats->per                 = src_stats->linkAdaptationStats.per[sid];
    dst_stats->totalPsduLength     = src_stats->linkAdaptationStats.totalPsduLength[sid];
    dst_stats->totalZld            = src_stats->linkAdaptationStats.totalZld[sid];
    for (rate = 0; rate < RATES_BIT_MASK_SIZE; rate++) {
      dst_stats->ratesMask[rate] = src_stats->linkAdaptationStats.ratesMask[sid][rate];
    }
  } else {
      ILOG1_DD("Incorrect SID (%u) or channel BW (%u): cannot read TX rate statistics", sid, cbw);
  }
}

static void
_mtlk_hw_mhi_sta_fill_tx_rate_statistic (mtlk_hw_t *hw, mtlk_mhi_stats_sta_tx_rate_t *tx_stats, uint32 sta_sid)
{
    hw_statistics_t    *hw_stats = &hw->hw_stats;
    uint16              chip_id = hw_mmb_get_chip_id(hw);

    mtlk_osal_lock_acquire(&hw_stats->lock);

    if (_chipid_is_gen6_b0(chip_id))
      __wave_hw_mhi_sta_fill_tx_rate_statistic_wave600b(tx_stats, (StatisticsDb_t_wave600b *)hw_stats->stats_copy, sta_sid);
    else if (_chipid_is_gen6_d2(chip_id))
      __wave_hw_mhi_sta_fill_tx_rate_statistic_wave600d2(tx_stats, (StatisticsDb_t_wave600d2 *)hw_stats->stats_copy, sta_sid);
    else {
      __wave_hw_mhi_sta_fill_tx_rate_statistic_wave700(tx_stats, (StatisticsDb_t_wave700 *)hw_stats->stats_copy, sta_sid);
    }
    mtlk_osal_lock_release(&hw_stats->lock);
}

static void
_mtlk_sta_link_adapt_ofdma_count_stats (mtlk_hw_t *hw, const sta_entry *sta)
{

    mtlk_mhi_stats_lamuofdma_cntr_t *ofdma_count;
    mtlk_mhi_stats_sta_cntr_t       *stats;
    mtlk_mhi_stats_sta_cntr_t       *stats_snapshot;
    uint32                          chip_id;

    if ((sta == NULL) || (hw == NULL))
      return;

    chip_id = hw_mmb_get_chip_id(hw);
    ofdma_count = mtlk_sta_get_ofdma_count_stats(sta);
    stats = mtlk_sta_get_mhi_stat_array(sta);
    stats_snapshot = mtlk_sta_get_mhi_stat_snapshot_array(sta);

    if ((_chipid_is_gen6_b0(chip_id)) || (_chipid_is_gen6_d2_or_gen7(chip_id))) {
      ofdma_count->staRemovedNbiqBelowThresholdCounter = stats->staRemovedNbiqBelowThresholdCounter -
                                                         stats_snapshot->staRemovedNbiqBelowThresholdCounter;
      ofdma_count->staRemovedFavoringSuCounter = stats->staRemovedFavoringSuCounter -
                                                 stats_snapshot->staRemovedFavoringSuCounter;
      ofdma_count->staNotAddedNbiqBelowThresholdCounter = stats->staNotAddedNbiqBelowThresholdCounter -
                                                          stats_snapshot->staNotAddedNbiqBelowThresholdCounter;
    }
}

void __MTLK_IFUNC
wave_update_multicast_group_stats(mtlk_hw_t *hw, mtlk_core_t *cur_nic, unsigned vap_id)
{
  int idx;
  sta_entry *sta;
  uint32 packet_count, mcast_packet_count;
  mtlk_mhi_stats_sta_cntr_t *sta_stats;
  mtlk_mhi_stats_sta_cntr_t *sta_stats_snapshot;
  wave_mhi_PDstats_vap_t *mhi_vap_PDstats;
  uint64 byte_count, mcast_byte_count;
  uint32 chip_id = hw_mmb_get_chip_id(hw);
  hw_statistics_t *hw_stats = &hw->hw_stats;
  mcast_ip4_grp_t *ipv4_grp;
  mcast_ip4_src_t *ipv4_src;
  mcast_ip6_grp_t *ipv6_grp;
  mcast_ip6_src_t *ipv6_src;
  mcast_sta_t     *mcsta;

  if (cur_nic == NULL) {
    return;
  }

  if (!mtlk_core_mcast_module_is_available(cur_nic)) {
    return;
  }

  mhi_vap_PDstats = &cur_nic->mhi_vap_PDstat;

  mtlk_osal_lock_acquire(&cur_nic->mcast.lock);
  /* for IPv4 hash */
  for (idx = 0; idx < MCAST_HASH_SIZE; idx++) {
     for (ipv4_grp = cur_nic->mcast.mcast_ip4_hash[idx]; ipv4_grp != NULL; ipv4_grp = ipv4_grp->next) {
        for (ipv4_src = ipv4_grp->ip4_src_list; ipv4_src != NULL; ipv4_src = ipv4_src->next) {
           if (ipv4_src->info.grp_id >= MAX_NUM_OF_GROUPS) {
             continue;
           }
           if (_chipid_is_gen6_b0(chip_id)) {
             mtlk_osal_lock_acquire(&hw_stats->lock);
             mcast_packet_count = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxPackets[vap_id][ipv4_src->info.grp_id]);
             mcast_byte_count   = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxBytes[vap_id][ipv4_src->info.grp_id]);
             mtlk_osal_lock_release(&hw_stats->lock);
           } else if (_chipid_is_gen7(chip_id)) {
             mtlk_osal_lock_acquire(&hw_stats->lock);
             mcast_packet_count = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxPackets[vap_id][ipv4_src->info.grp_id]);
             mcast_byte_count   = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxBytes[vap_id][ipv4_src->info.grp_id]);
             mtlk_osal_lock_release(&hw_stats->lock);
           } else { /* Wave600D2 */
             mtlk_osal_lock_acquire(&hw_stats->lock);
             mcast_packet_count = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxPackets[vap_id][ipv4_src->info.grp_id]);
             mcast_byte_count   = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxBytes[vap_id][ipv4_src->info.grp_id]);
             mtlk_osal_lock_release(&hw_stats->lock);
           }

           packet_count = mcast_packet_count - mhi_vap_PDstats->PDstats.txInmulticastPD_ipv4[ipv4_src->info.grp_id];
           byte_count = mcast_byte_count - mhi_vap_PDstats->PDstats64.txInMulticastBytePD_ipv4[ipv4_src->info.grp_id];

           mhi_vap_PDstats->PDstats.txInmulticastPD_ipv4[ipv4_src->info.grp_id] = mcast_packet_count;
           mhi_vap_PDstats->PDstats64.txInMulticastBytePD_ipv4[ipv4_src->info.grp_id] = mcast_byte_count;

           for (mcsta = ipv4_src->info.mcsta; mcsta != NULL; mcsta = mcsta->next) {
              sta = mtlk_stadb_find_sta(&cur_nic->slow_ctx->stadb, mcsta->sta_addr.au8Addr);
                if (sta) {
                  sta_stats = mtlk_sta_get_mhi_stat_array(sta);
                  sta_stats_snapshot = mtlk_sta_get_mhi_stat_snapshot_array(sta);
                  sta_stats->multicastPacketSent += mcast_packet_count -
                                                    (sta_stats->prevMulticastPacketSent - sta_stats_snapshot->prevMulticastPacketSent);
                  sta_stats->multicastByteSent   += mcast_byte_count -
                                                    (sta_stats->prevMulticastByteSent - sta_stats_snapshot->prevMulticastByteSent);
                  sta_stats->prevMulticastPacketSent = mcast_packet_count;
                  sta_stats->prevMulticastByteSent = mcast_byte_count;
                  mtlk_sta_decref(sta);
                  mtlk_core_add_cnt(cur_nic, MTLK_CORE_CNT_PD_MULTICAST_PACKETS_SENT, packet_count);
                  mtlk_core_add64_cnt(cur_nic, MTLK_CORE_CNT_PD_MULTICAST_BYTES_SENT_64, byte_count);
              }
           }
        }
     }
  }

/* for IPv6 hash */
  for (idx = 0; idx < MCAST_HASH_SIZE; idx++) {
     for (ipv6_grp = cur_nic->mcast.mcast_ip6_hash[idx]; ipv6_grp != NULL; ipv6_grp = ipv6_grp->next) {
        for (ipv6_src = ipv6_grp->ip6_src_list; ipv6_src != NULL; ipv6_src = ipv6_src->next) {
           if (ipv6_src->info.grp_id >= MAX_NUM_OF_GROUPS) {
             continue;
           }
           if (_chipid_is_gen6_b0(chip_id)) {
             mtlk_osal_lock_acquire(&hw_stats->lock);
             mcast_packet_count = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxPackets[vap_id][ipv6_src->info.grp_id]);
             mcast_byte_count   = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxBytes[vap_id][ipv6_src->info.grp_id]);
             mtlk_osal_lock_release(&hw_stats->lock);
           } else { /* Wave600D2 and Wave700 */
             mtlk_osal_lock_acquire(&hw_stats->lock);
             mcast_packet_count = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxPackets[vap_id][ipv6_src->info.grp_id]);
             mcast_byte_count   = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                     MultiCastGroupStatistics.multiCastTxBytes[vap_id][ipv6_src->info.grp_id]);
             mtlk_osal_lock_release(&hw_stats->lock);
           }

           packet_count = mcast_packet_count - mhi_vap_PDstats->PDstats.txInmulticastPD_ipv6[ipv6_src->info.grp_id];
           byte_count = mcast_byte_count - mhi_vap_PDstats->PDstats64.txInMulticastBytePD_ipv6[ipv6_src->info.grp_id];

           mhi_vap_PDstats->PDstats.txInmulticastPD_ipv6[ipv6_src->info.grp_id] = mcast_packet_count;
           mhi_vap_PDstats->PDstats64.txInMulticastBytePD_ipv6[ipv6_src->info.grp_id] = mcast_byte_count;

           for (mcsta = ipv6_src->info.mcsta; mcsta != NULL; mcsta = mcsta->next) {
              sta = mtlk_stadb_find_sta(&cur_nic->slow_ctx->stadb, mcsta->sta_addr.au8Addr);
              if (sta) {
                sta_stats = mtlk_sta_get_mhi_stat_array(sta);
                sta_stats_snapshot = mtlk_sta_get_mhi_stat_snapshot_array(sta);
                sta_stats->multicastPacketSent += mcast_packet_count -
                                                  (sta_stats->prevMulticastPacketSent - sta_stats_snapshot->prevMulticastPacketSent);
                sta_stats->multicastByteSent   += mcast_byte_count -
                                                  (sta_stats->prevMulticastByteSent - sta_stats_snapshot->prevMulticastByteSent);
                sta_stats->prevMulticastPacketSent = mcast_packet_count;
                sta_stats->prevMulticastByteSent = mcast_byte_count;
                mtlk_sta_decref(sta);
                mtlk_core_add_cnt(cur_nic, MTLK_CORE_CNT_PD_MULTICAST_PACKETS_SENT, packet_count);
                mtlk_core_add64_cnt(cur_nic, MTLK_CORE_CNT_PD_MULTICAST_BYTES_SENT_64, byte_count);
              }
           }
        }
     }
  }
  mtlk_osal_lock_release(&cur_nic->mcast.lock);
}

uint32 __MTLK_IFUNC
wave_hw_calculate_airtime_efficiency (uint64 bytes, uint32 time)
{
  uint64 efficiency = bytes;

  /* modifies the 64-bit dividend _in_place_ */
  do_div(efficiency, time); /* ignore the reminder */

  if (efficiency > MAX_UINT32) {
    ILOG1_HHD("Efficiency of %llu bytes per second? deltaBytes %llu, time %u", efficiency, bytes, time);
    efficiency = 0;
  }
  return (uint32)efficiency;
}

void __MTLK_IFUNC
mtlk_hw_clean_sta_statistics_by_sid(mtlk_hw_t *hw, uint32 sta_sid)
{
  hw_statistics_t *hw_stats = &hw->hw_stats;
  int chip_id = hw_mmb_get_chip_id(hw);

  mtlk_osal_lock_acquire(&hw_stats->lock);

  MTLK_CLEAN_STATS(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid], chip_id);

  MTLK_CLEAN_STATS(hw_stats->stats_copy, rxCounters.staCounts[sta_sid], chip_id);
  if (_chipid_is_gen6_b0(chip_id)) {
    MTLK_CLEAN_STATS_B0(hw_stats->stats_copy, rxCounters.rdCount[sta_sid]);
    MTLK_CLEAN_STATS_B0(hw_stats->stats_copy, perClientStats.successByteCount[sta_sid]);
  } else {
    MTLK_CLEAN_STATS_D2(hw_stats->stats_copy, RxCoordinatorCounters.rdCount[sta_sid]);
    MTLK_CLEAN_STATS_D2(hw_stats->stats_copy, perClientStats.successByteCount[sta_sid]);
  }

  if (_chipid_is_gen6_b0(chip_id)) {
    MTLK_CLEAN_STATS_B0(hw_stats->stats_copy, linkAdaptationStats.per[sta_sid]);
  }

  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.totalPsduLength[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.totalZld[sta_sid], chip_id);

  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.numMsdus[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.transmittedAmpdu[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.ewmaTimeNsec[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.sumTimeNsec[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.totalTxTime[sta_sid], chip_id);

  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.DataPhyMode[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.scpData[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.mcsData[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.bfModeData[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.stbcModeData[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.powerData[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.powerManagement[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, linkAdaptationStats.ManagementPhyMode[sta_sid], chip_id);

  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.exhaustedCount[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.successCount[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.ampdu[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.mpduRetryCount[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.clonedCount[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.mpduInAmpdu[sta_sid], chip_id);
  MTLK_CLEAN_STATS(hw_stats->stats_copy, perClientStats.retryCount[sta_sid], chip_id);

  mtlk_osal_lock_release(&hw_stats->lock);
}

void wave_monitor_tx_stall(sta_entry *sta)
{
  if (sta->tx_rx_stall_warning.tx_warn_count == 0)
  {
    sta->tx_rx_stall_warning.prev_tx_successcount = sta->sta_stats64_cntrs.successCount;
    sta->tx_rx_stall_warning.prev_tx_retryCount   = sta->sta_stats64_cntrs.retryCount;
    sta->tx_rx_stall_warning.tx_warn_count++;
    return;
  }
  else if ((sta->tx_rx_stall_warning.prev_tx_successcount == sta->sta_stats64_cntrs.successCount) &&
            sta->tx_rx_stall_warning.prev_tx_retryCount < sta->sta_stats64_cntrs.retryCount)
  {
    sta->tx_rx_stall_warning.prev_tx_retryCount   = sta->sta_stats64_cntrs.retryCount;
    sta->tx_rx_stall_warning.tx_warn_count++;
  }
  else {
    sta->tx_rx_stall_warning.tx_warn_count = 0;
  }
}

void wave_monitor_rx_stall(sta_entry *sta)
{
#ifdef MTLK_WAVE_700
  mtlk_hw_t *hw = mtlk_vap_get_hw(sta->vap_handle);
  unsigned vap_id = mtlk_vap_get_id_fw(sta->vap_handle);

  if (mtlk_hw_type_is_gen7(hw)) {
    if ((wave_is_mld_sta(sta)) && (sta->info.MainVapId != vap_id))
      /* Skip Rx stall validation for secondary station, since "rxOutStaNumOfBytes" is updated only for main station */
      return;
  }
#endif
  if (sta->tx_rx_stall_warning.rx_warn_count == 0)
  {
    sta->tx_rx_stall_warning.prev_rx_count = sta->sta_stats64_cntrs.rxOutStaNumOfBytes;
    sta->tx_rx_stall_warning.prev_rx_retryCount = sta->sta_stats64_cntrs.rxRetryCount;
    sta->tx_rx_stall_warning.rx_warn_count++;
  }
  else if ((sta->tx_rx_stall_warning.prev_rx_count == sta->sta_stats64_cntrs.rxOutStaNumOfBytes) &&
          (sta->tx_rx_stall_warning.prev_rx_retryCount < sta->sta_stats64_cntrs.rxRetryCount))
  {
    sta->tx_rx_stall_warning.prev_rx_retryCount = sta->sta_stats64_cntrs.rxRetryCount;
    sta->tx_rx_stall_warning.rx_warn_count++;
  }
  else {
    sta->tx_rx_stall_warning.rx_warn_count = 0;
  }
}

/* This function accumulates 32-bit statistics into a 64-bit value.  */
/* One limitation of this function is that it cannot detect multiple */
/* over-runs in the 32-bit counter.                                  */
/* Upon return, prev_val is updated to new_val.                      */
static inline void __stats_accumulate_64bit (uint64* sum, uint32* prev_val, uint32 new_val)
{
  uint32 delta;
  delta = new_val - *prev_val;
  *sum += delta;
  *prev_val = new_val;
}

/* This function returns the difference between current and previous */
/* values of a 32-bit statistic. Difference is modulo or absolute.   */
/* One limitation of this function is that it cannot detect multiple */
/* over-runs in the 32-bit counter.                                  */
/* Upon return, prev_val is updated to new_val.                      */
static inline void __stats_difference (uint64* diff, uint32* prev_val, uint32 new_val)
{
  *diff     = new_val - *prev_val;
  *prev_val = new_val;
}

/* This function accumulates reset 32-bit statistics (restarts from 0)
 * into 64 bit. over-runs in the 32-bit counter.
 * Upon return, prev_val is updated to new_val.
 */
static inline void __reset_stats_accumulate_64bit (uint64* sum, uint32* prev_val, uint32 new_val)
{
  if (new_val < *prev_val) {
    *sum += new_val;
    *prev_val = new_val;
  }
  else
    __stats_accumulate_64bit(sum, prev_val, new_val);
}

/* This function accumulates new or potential overflown statistics into 64 bit value 
 * is_reset argument is used by the method to calculate a potential overflow or just to add it to the value
 * Upon return, prev_val is updated to new_val.
 */
static inline void __stats_accumulate_64bit_from_16bit_with_reset(uint64* out_sum, uint16* prev_val, uint16 new_val, uint8 is_reset)
{
  if (is_reset) {
    *out_sum += new_val;
  } else {
    *out_sum += ((new_val - *prev_val) & 0xFFFF);
  }

  *prev_val = new_val;
}

static uint8
__wave_get_ruid_from_bw (uint8 curr_bw)
{
  uint8 ru_id;
  switch(curr_bw) {
  case 3: /* for bw 160MHz */
    ru_id = RU_SIZE_996X2_TONE;
    break;
  case 2: /* for bw 80MHz */
    ru_id = RU_SIZE_996_TONE;
    break;
  case 1: /* for bw 40MHz */
    ru_id = RU_SIZE_484_TONE;
    break;
  case 0: /* for bw 20MHz */
  default:
    ru_id = RU_SIZE_242_TONE;
    break;
  }
  return ru_id;
}

static void
_mtlk_sta_update_dl_mimo_statistics (mtlk_hw_t *hw, sta_entry *sta)
{
  uint32 chip_id;
  uint16 sta_sid;
  hw_statistics_t *hw_stats;
  mtlk_mhi_stats_sta_cntr_t   *stats;
  mtlk_mhi_stats64_sta_cntr_t *stats64;

  uint8  mu_sta_entry;
  uint8  nss, mcs;
  uint8  no_of_mcs, no_of_nss;
  uint8  hw_stats_mu_group_state;
  uint16 hw_stats_sta_sid;
  uint32 hw_stats_su_stats, hw_stats_mu_stats;

  chip_id  = hw_mmb_get_chip_id(hw);
  sta_sid  = mtlk_sta_get_sid(sta);
  hw_stats = &hw->hw_stats;

  stats   = mtlk_sta_get_mhi_stat_array(sta);
  stats64 = mtlk_sta_get_mhi_stat64_array(sta);

  if (_chipid_is_gen7(chip_id)) {
    no_of_nss = REDUCED_SPATIAL_STREAMS_SIZE_GEN7;
    no_of_mcs = REDUCED_MCS_SIZE_GEN7;
  } else {
    no_of_nss = REDUCED_SPATIAL_STREAMS_SIZE;
    no_of_mcs = REDUCED_MCS_SIZE;
  }

  for (nss = 0; nss < no_of_nss; nss++) {
    hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                       linkAdaptationSuMuStatistics.suRateDl.staStatistics[sta_sid].nssPpdus[nss],
                                       chip_id);
    __reset_stats_accumulate_64bit(&stats64->dl_mimo.su_mimo_stats64.su_rate_stats64.nss_ppdus[nss],
                                   &stats->dl_mimo.su_mimo_stats.su_rate_stats.nss_ppdus[nss],
                                   hw_stats_su_stats);
  }

  for (mcs = 0; mcs < no_of_mcs; mcs++) {
    hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                       linkAdaptationSuMuStatistics.suRateDl.staStatistics[sta_sid].mcsPpdus[mcs],
                                       chip_id);
    __reset_stats_accumulate_64bit(&stats64->dl_mimo.su_mimo_stats64.su_rate_stats64.mcs_ppdus[mcs],
                                   &stats->dl_mimo.su_mimo_stats.su_rate_stats.mcs_ppdus[mcs],
                                   hw_stats_su_stats);
  }

  /* DL MU MIMO stats */
  for (mu_sta_entry = 0; mu_sta_entry < MAX_NUM_OF_MIMO_STA_STATS_ENTRIES; mu_sta_entry ++) {
    hw_stats_sta_sid = MTLK_GET_STATS(hw_stats->stats_copy,
                                      linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].staId,
                                      chip_id);
    hw_stats_mu_group_state = MTLK_GET_STATS(hw_stats->stats_copy,
                                             linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muGroupState,
                                             chip_id);

    if ((hw_stats_sta_sid == sta_sid) && (hw_stats_mu_group_state != 0)) {

      for (nss = 0; nss < no_of_nss; nss++) {
        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.nssPpdus[nss],
                                           chip_id);
        __reset_stats_accumulate_64bit(&stats64->dl_mimo.mu_mimo_stats64.mu_rate_stats64.nss_ppdus[nss],
                                       &stats->dl_mimo.mu_mimo_stats.mu_rate_stats.nss_ppdus[nss],
                                       hw_stats_mu_stats);
      }

      for (mcs = 0; mcs < no_of_mcs; mcs++) {
        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.mcsPpdus[mcs],
                                           chip_id);
        __reset_stats_accumulate_64bit(&stats64->dl_mimo.mu_mimo_stats64.mu_rate_stats64.mcs_ppdus[mcs],
                                       &stats->dl_mimo.mu_mimo_stats.mu_rate_stats.mcs_ppdus[mcs],
                                       hw_stats_mu_stats);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.attemptedSoundingPerMCS[mcs],
                                           chip_id);
        __reset_stats_accumulate_64bit(&stats64->dl_mimo.mu_mimo_stats64.attemptedsounding_per_mcs[mcs],
                                       &stats->dl_mimo.mu_mimo_stats.attemptedsounding_per_mcs[mcs],
                                       hw_stats_mu_stats);
      }

      hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                         linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.succeededSounding,
                                         chip_id);
      __reset_stats_accumulate_64bit(&stats64->dl_mimo.mu_mimo_stats64.succeeded_sounding,
                                     &stats->dl_mimo.mu_mimo_stats.succeeded_sounding,
                                     hw_stats_mu_stats);
    }
  }
}

static void
_mtlk_sta_update_ofdma_su_statistics (mtlk_hw_t *hw, sta_entry *sta)
{
  uint32 chip_id;
  uint16 sta_sid;
  hw_statistics_t *hw_stats;
  mtlk_mhi_stats_sta_cntr_t   *stats;
  mtlk_mhi_stats64_sta_cntr_t *stats64;

  uint8  ru_id, curr_bw;
  uint8  no_of_bw;
  uint8  new_entry_word_index;
  uint8  new_entry_bit_index;
  uint8  new_entry_value;
  uint8  new_entry_su_db_size_in_words;
  uint32 new_entry_bitmap;

  uint32 hw_stats_su_stats;
  uint32 tmp_su_mpdus;
  uint16 tmp_su_ppdus;

  chip_id  = hw_mmb_get_chip_id(hw);
  sta_sid  = mtlk_sta_get_sid(sta);
  hw_stats = &hw->hw_stats;

  stats   = mtlk_sta_get_mhi_stat_array(sta);
  stats64 = mtlk_sta_get_mhi_stat64_array(sta);

  no_of_bw = (_chipid_is_gen7(chip_id) ? BW_MAX_G7 : NUM_OF_BW);

  /* SU PPDUS */
  new_entry_su_db_size_in_words = MTLK_GET_STATS_ARRAY_SIZE_32(hw_stats->stats_copy,
                                                                linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suPpdusNewEntryBitArray,
                                                                chip_id);
  new_entry_su_db_size_in_words /= sizeof(uint32);

  hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.totalPpdusCount[sta_sid], chip_id);
  __reset_stats_accumulate_64bit(&stats64->ul_ppdus.total_ppdus, &stats->ul_ppdus.total_ppdus, hw_stats_su_stats);

  hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.totalPpdusCount[sta_sid], chip_id);
  __reset_stats_accumulate_64bit(&stats64->dl_ppdus.total_ppdus, &stats->dl_ppdus.total_ppdus, hw_stats_su_stats);

  new_entry_word_index = (sta_sid / NUM_OF_BITS_IN_UINT32);
  new_entry_bit_index  = (sta_sid % NUM_OF_BITS_IN_UINT32);

  for (curr_bw = 0; curr_bw < no_of_bw; curr_bw++) {
    ru_id = __wave_get_ruid_from_bw(curr_bw);

    if (new_entry_word_index < new_entry_su_db_size_in_words) {
      /* UL SU PPDUS Stats */
      /* Check if UL data is new */
      new_entry_bitmap = MTLK_GET_STATS(hw_stats->stats_copy,
                                        linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suPpdusNewEntryBitArray[new_entry_word_index],
                                        chip_id);
      new_entry_value = MTLK_BIT_GET(new_entry_bitmap, new_entry_bit_index);

      hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                         linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suStationsPpdus[sta_sid].suPpdusCount[curr_bw],
                                         chip_id);

      tmp_su_ppdus = stats->ul_ppdus.su_ppdus[curr_bw];
      __stats_accumulate_64bit_from_16bit_with_reset(&stats64->ul_ppdus.su_ppdus[curr_bw],
                                                     &stats->ul_ppdus.su_ppdus[curr_bw],
                                                     hw_stats_su_stats, new_entry_value);

      __stats_accumulate_64bit_from_16bit_with_reset(&stats64->ul_ppdus.mu_ru_ppdus[ru_id],
                                                     &tmp_su_ppdus,
                                                     hw_stats_su_stats, new_entry_value);

      /* DL SU PPDUS Stats */
      /* Check if DL data is new */
      new_entry_bitmap = MTLK_GET_STATS(hw_stats->stats_copy,
                                        linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suPpdusNewEntryBitArray[new_entry_word_index],
                                        chip_id);
      new_entry_value = MTLK_BIT_GET(new_entry_bitmap, new_entry_bit_index);

      hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                         linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suStationsPpdus[sta_sid].suPpdusCount[curr_bw],
                                         chip_id);

      tmp_su_ppdus = stats->dl_ppdus.su_ppdus[curr_bw];
      __stats_accumulate_64bit_from_16bit_with_reset(&stats64->dl_ppdus.su_ppdus[curr_bw],
                                                     &stats->dl_ppdus.su_ppdus[curr_bw],
                                                     hw_stats_su_stats, new_entry_value);

      __stats_accumulate_64bit_from_16bit_with_reset(&stats64->dl_ppdus.mu_ru_ppdus[ru_id],
                                                     &tmp_su_ppdus,
                                                     hw_stats_su_stats, new_entry_value);
    } else {
      ELOG_DD("SU PPDUs index error - STA: %d - Entry index: %d", sta_sid, new_entry_word_index);
    }

    /* DL SU MPDUS stats */
    hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                       linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[sta_sid].staMpdu[curr_bw].successMpdus,
                                       chip_id);
    tmp_su_mpdus = stats->dl_mpdus.su_mpdus[curr_bw].success_mpdus;
    __reset_stats_accumulate_64bit(&stats64->dl_mpdus.su_mpdus[curr_bw].success_mpdus,
                                   &stats->dl_mpdus.su_mpdus[curr_bw].success_mpdus,
                                   hw_stats_su_stats);
    __reset_stats_accumulate_64bit(&stats64->dl_mpdus.mu_mpdus[ru_id].success_mpdus,
                                   &tmp_su_mpdus, hw_stats_su_stats);

    hw_stats_su_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                       linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[sta_sid].staMpdu[curr_bw].failedMpdus,
                                       chip_id);
    tmp_su_mpdus = stats->dl_mpdus.su_mpdus[curr_bw].failed_mpdus;
    __reset_stats_accumulate_64bit(&stats64->dl_mpdus.su_mpdus[curr_bw].failed_mpdus,
                                   &stats->dl_mpdus.su_mpdus[curr_bw].failed_mpdus,
                                   hw_stats_su_stats);
    __reset_stats_accumulate_64bit(&stats64->dl_mpdus.mu_mpdus[ru_id].failed_mpdus,
                                   &tmp_su_mpdus, hw_stats_su_stats);
  }
}

static void
_mtlk_sta_update_ofdma_mu_statistics (mtlk_hw_t *hw, sta_entry *sta)
{
  uint32 chip_id;
  uint16 sta_sid;
  hw_statistics_t *hw_stats;
  mtlk_mhi_stats_sta_cntr_t   *stats;
  mtlk_mhi_stats64_sta_cntr_t *stats64;

  uint8  ru_id, mu_sta_entry;
  uint8  no_of_ru_sizes;
  uint8  new_entry_word_index;
  uint8  new_entry_bit_index;
  uint8  new_entry_value;
  uint8  new_entry_mu_db_size_in_words;
  uint32 new_entry_bitmap;

  uint32 hw_stats_mu_stats;
  uint16 hw_stats_sta_sid;
  uint8  hw_stats_mu_group_state;

  chip_id  = hw_mmb_get_chip_id(hw);
  sta_sid  = mtlk_sta_get_sid(sta);
  hw_stats = &hw->hw_stats;

  stats   = mtlk_sta_get_mhi_stat_array(sta);
  stats64 = mtlk_sta_get_mhi_stat64_array(sta);

  no_of_ru_sizes = (_chipid_is_gen7(chip_id) ? WAVE700_NUM_OF_RU_SIZES : WAVE600_NUM_OF_RU_SIZES);

  /* MU PPDUS */
  new_entry_mu_db_size_in_words = MTLK_GET_STATS_ARRAY_SIZE_32(hw_stats->stats_copy,
                                                               linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muPpdusNewEntryBitArray,
                                                               chip_id);
  new_entry_mu_db_size_in_words /= sizeof(uint32);
  for (mu_sta_entry = 0; mu_sta_entry < MAX_NUM_OF_MU_STA_STATS_ENTRIES; mu_sta_entry ++) {
    /* STA UL MU PPDUS */
    hw_stats_sta_sid = MTLK_GET_STATS(hw_stats->stats_copy,
                                      linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId,
                                      chip_id);
    hw_stats_mu_group_state = MTLK_GET_STATS(hw_stats->stats_copy,
                                             linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muGroupState,
                                             chip_id);

    if ((hw_stats_sta_sid == sta_sid) && (hw_stats_mu_group_state != 0)) {
      /* Check if this entry is new or not, this is for ul mu ppdus */
      new_entry_word_index = (mu_sta_entry / NUM_OF_BITS_IN_UINT32);
      new_entry_bit_index  = (mu_sta_entry % NUM_OF_BITS_IN_UINT32);

      if (new_entry_word_index < new_entry_mu_db_size_in_words) {
        new_entry_bitmap = MTLK_GET_STATS(hw_stats->stats_copy,
                                          linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muPpdusNewEntryBitArray[new_entry_word_index],
                                          chip_id);
        new_entry_value = MTLK_BIT_GET(new_entry_bitmap, new_entry_bit_index);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount,
                                           chip_id);
        __stats_accumulate_64bit_from_16bit_with_reset(&stats64->ul_ppdus.mu_ofdma_ppdus,
                                                       &stats->ul_ppdus.mu_ofdma_ppdus,
                                                       hw_stats_mu_stats, new_entry_value);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount,
                                           chip_id);
        __stats_accumulate_64bit_from_16bit_with_reset(&stats64->ul_ppdus.mu_mimo_ppdus,
                                                       &stats->ul_ppdus.mu_mimo_ppdus,
                                                       hw_stats_mu_stats, new_entry_value);

        for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
          hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                             linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount[ru_id],
                                             chip_id);
          __stats_accumulate_64bit_from_16bit_with_reset(&stats64->ul_ppdus.mu_ru_ppdus[ru_id],
                                                         &stats->ul_ppdus.mu_ru_ppdus[ru_id],
                                                         hw_stats_mu_stats, new_entry_value);
        }
      } else {
        ELOG_DD("UL MU PPDUs index error - mu sta entry: %d - Entry index: %d", mu_sta_entry, new_entry_word_index);
      }
    }

    /* STA DL MU PPDUS */
    hw_stats_sta_sid = MTLK_GET_STATS(hw_stats->stats_copy,
                                      linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId,
                                      chip_id);
    hw_stats_mu_group_state = MTLK_GET_STATS(hw_stats->stats_copy,
                                             linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muGroupState,
                                             chip_id);

    if ((hw_stats_sta_sid == sta_sid) && (hw_stats_mu_group_state != 0)) {
      /* Check if this entry is new or not, this is for dl mu ppdus */
      new_entry_word_index = (mu_sta_entry / NUM_OF_BITS_IN_UINT32);
      new_entry_bit_index  = (mu_sta_entry % NUM_OF_BITS_IN_UINT32);

      if (new_entry_word_index < new_entry_mu_db_size_in_words) {
        new_entry_bitmap = MTLK_GET_STATS(hw_stats->stats_copy,
                                          linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muPpdusNewEntryBitArray[new_entry_word_index],
                                          chip_id);
        new_entry_value = MTLK_BIT_GET(new_entry_bitmap, new_entry_bit_index);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount,
                                           chip_id);
        __stats_accumulate_64bit_from_16bit_with_reset(&stats64->dl_ppdus.mu_ofdma_ppdus,
                                                       &stats->dl_ppdus.mu_ofdma_ppdus,
                                                       hw_stats_mu_stats, new_entry_value);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount,
                                           chip_id);
        __stats_accumulate_64bit_from_16bit_with_reset(&stats64->dl_ppdus.mu_mimo_ppdus,
                                                       &stats->dl_ppdus.mu_mimo_ppdus,
                                                       hw_stats_mu_stats, new_entry_value);
      } else {
        ELOG_DD("DL MU PPDUs index error - mu sta entry: %d - Entry index: %d", mu_sta_entry, new_entry_word_index);
      }

      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
        if (new_entry_word_index < new_entry_mu_db_size_in_words) {
          hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                             linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount[ru_id],
                                             chip_id);
          __stats_accumulate_64bit_from_16bit_with_reset(&stats64->dl_ppdus.mu_ru_ppdus[ru_id],
                                                         &stats->dl_ppdus.mu_ru_ppdus[ru_id],
                                                         hw_stats_mu_stats, new_entry_value);
        } else {
          ELOG_DD("DL MU PPDUs index error - mu sta entry: %d - Entry index: %d", mu_sta_entry, new_entry_word_index);
        }

        /* DL MPDUS OFDMA stats */
        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].muRuMpdusCount[ru_id].successMpdus,
                                           chip_id);
        __reset_stats_accumulate_64bit(&stats64->dl_mpdus.mu_mpdus[ru_id].success_mpdus,
                                       &stats->dl_mpdus.mu_mpdus[ru_id].success_mpdus,
                                       hw_stats_mu_stats);

        hw_stats_mu_stats = MTLK_GET_STATS(hw_stats->stats_copy,
                                           linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].muRuMpdusCount[ru_id].failedMpdus,
                                           chip_id);
        __reset_stats_accumulate_64bit(&stats64->dl_mpdus.mu_mpdus[ru_id].failed_mpdus,
                                       &stats->dl_mpdus.mu_mpdus[ru_id].failed_mpdus,
                                       hw_stats_mu_stats);
      }
    }
  }
}

static void
_mtlk_sta_update_ofdma_statistics (mtlk_hw_t *hw, sta_entry *sta)
{
  _mtlk_sta_update_ofdma_su_statistics(hw, sta);
  _mtlk_sta_update_ofdma_mu_statistics(hw, sta);
}

static void
_mtlk_sta_update_statistics (mtlk_hw_t *hw, sta_entry *sta, stationPhyRxStatusDb_t *sta_status)
{
  uint32             new_rx_time, new_tx_time, airtime, efficiency = 0;
  uint32             sta_sid, rx_bytes, tx_bytes, delta_bytes;
  airtime_stats_t   *as = &sta->info.stats.airtime_stats;
#ifdef MTLK_PER_RATE_STAT
  mtlk_rate_stat_rx  old_stat_rx;
  mtlk_rate_stat_tx  old_stat_tx;
#endif
  mtlk_mhi_stats_sta_cntr_t   *values;
  mtlk_mhi_stats64_sta_cntr_t *stats64;
  hw_statistics_t *hw_stats;
  uint32 tx_errors;
  unsigned vap_id = mtlk_vap_get_id(sta->vap_handle);
  int chip_id = hw_mmb_get_chip_id(hw);
  int agreement, buffer_status;

  hw_stats = &hw->hw_stats;
  sta_sid = mtlk_sta_get_sid(sta);

  /* Get RX Time from PHY Statistics */
  new_rx_time = sta_status->perClientRxtimeUsage;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  new_tx_time = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.totalTxTime[sta_sid], chip_id);

  /* RX time and TX time are coming from different entities and requests,
   * so it is possible, although unlikely, that their sum is bigger than polling period */
  airtime = (new_rx_time - as->rx_time) + (new_tx_time - as->tx_time);
  airtime = MIN((MTLK_OSAL_USEC_IN_MSEC * hw->stats_poll_period), airtime);
  as->airtime_usage = MTLK_VALUE_TO_PERCENT(airtime, (MTLK_OSAL_USEC_IN_MSEC * hw->stats_poll_period));

  values = mtlk_sta_get_mhi_stat_array(sta);
  stats64 = mtlk_sta_get_mhi_stat64_array(sta);

#ifdef MTLK_PER_RATE_STAT
  mtlk_core_get_associated_dev_rate_info_tx_stats(sta, &old_stat_tx.tx);
  mtlk_core_get_associated_dev_rate_info_rx_stats(sta, &old_stat_rx.rx);
#endif
  rx_bytes = values->rxOutStaNumOfBytes;
  tx_bytes = values->mpduByteTransmitted;
  tx_errors = MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.vapCounts[vap_id].rtsFailure, chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.vapCounts[vap_id].transmitStreamRprtMSDUFailed, chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.vapCounts[vap_id].ackFailure, chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.vapCounts[vap_id].failedAmsdu, chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.txSenderError[vap_id], chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.fwPairWiseMicFailurePackets[vap_id], chip_id) +
              MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.fwMulticastReplayedPackets[vap_id], chip_id);

  if (_chipid_is_gen6_b0(chip_id)) {
    values->per  = MTLK_GET_STATS_B0(hw_stats->stats_copy, linkAdaptationStats.per[sta_sid]);
  }

  values->ewmaTimeNsec  = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.ewmaTimeNsec[sta_sid], chip_id);
  values->minRssi       = MTLK_GET_STATS(hw_stats->stats_copy, alphaFilterStats.minRssi[sta_sid], chip_id);
  values->maxRssi       = MTLK_GET_STATS(hw_stats->stats_copy, alphaFilterStats.maxRssi[sta_sid], chip_id);
  __stats_difference(&stats64->sumTimeNsec, &values->prevSumTimeNsec,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.sumTimeNsec[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->numMsdus, &values->numMsdus,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.numMsdus[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->mpduFirstRetransmission, &values->mpduFirstRetransmission,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid].mpduFirstRetransmission, chip_id)));
  __stats_accumulate_64bit(&stats64->mpduTransmitted, &values->mpduTransmitted,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid].mpduTransmitted, chip_id)));
  __stats_accumulate_64bit(&stats64->mpduByteTransmitted, &values->mpduByteTransmitted,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid].mpduByteTransmitted, chip_id)));
  __stats_accumulate_64bit(&stats64->rxOutStaNumOfBytes, &values->rxOutStaNumOfBytes,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid].rxOutStaNumOfBytes, chip_id)));
  __stats_accumulate_64bit(&stats64->swUpdateDrop, &values->swUpdateDrop,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, rxCounters.staCounts[sta_sid].swUpdateDrop, chip_id)));
  __stats_accumulate_64bit(&stats64->exhaustedCount, &values->exhaustedCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.exhaustedCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->successCount, &values->successCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.successCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->retryCount, &values->retryCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.retryCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->clonedCount, &values->clonedCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.clonedCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->perclientmpduInAmpdu, &values->perclientmpduInAmpdu,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.mpduInAmpdu[sta_sid], chip_id)));

  __stats_accumulate_64bit(&stats64->packetRetransCount, &values->packetRetransCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.packetRetransCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->oneOrMoreRetryCount, &values->oneOrMoreRetryCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.oneOrMoreRetryCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonClassifier, &values->dropCntReasonClassifier,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonClassifier[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonDisconnect, &values->dropCntReasonDisconnect,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonDisconnect[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonATF, &values->dropCntReasonATF,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonATF[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonTSFlush, &values->dropCntReasonTSFlush,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonTSFlush[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonReKey, &values->dropCntReasonReKey,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonReKey[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonSetKey, &values->dropCntReasonSetKey,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonSetKey[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonDiscard, &values->dropCntReasonDiscard,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonDiscard[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonDsabled, &values->dropCntReasonDsabled,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonDsabled[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->dropCntReasonAggError, &values->dropCntReasonAggError,
                                    (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.dropCntReasonAggError[sta_sid], chip_id)));

  __stats_accumulate_64bit(&stats64->rddelayed, &values->rddelayed,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, rxCounters.staCounts[sta_sid].rddelayed, chip_id)));
  __stats_accumulate_64bit(&stats64->rdDuplicateDrop, &values->rdDuplicateDrop,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, rxCounters.staCounts[sta_sid].rdDuplicateDrop, chip_id)));
  __stats_accumulate_64bit(&stats64->missingSn, &values->missingSn,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, rxCounters.staCounts[sta_sid].missingSn, chip_id)));
  __stats_accumulate_64bit(&stats64->ampdu, &values->ampdu,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.ampdu[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->rxRetryCount, &values->rxRetryCount,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, perClientStats.mpduRetryCount[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->transmittedAmpdu, &values->transmittedAmpdu,
                                   (MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.transmittedAmpdu[sta_sid], chip_id)));
  __stats_accumulate_64bit(&stats64->tx_errors, &values->tx_errors, tx_errors);

  if (_chipid_is_gen6_b0(chip_id)) {
    __stats_accumulate_64bit(&stats64->rdCount, &values->rdCount,
                                   (MTLK_GET_STATS_B0(hw_stats->stats_copy, rxCounters.rdCount[sta_sid])));
    __stats_accumulate_64bit(&stats64->mpduInAmpdu, &values->mpduInAmpdu,
                                   (MTLK_GET_STATS_B0(hw_stats->stats_copy, rxCounters.vapCounts[vap_id].mpduInAmpdu)));
  __stats_accumulate_64bit(&stats64->agerPdNoTransmitCountSta, &values->agerPdNoTransmitCountSta,
                                   (MTLK_GET_STATS_B0(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid].agerPdNoTransmitCountSta)));
  __stats_accumulate_64bit(&stats64->successByteCount, &values->successByteCount,
                                   (MTLK_GET_STATS_B0(hw_stats->stats_copy, perClientStats.successByteCount[sta_sid])));
  } else if (_chipid_is_gen6_d2(chip_id)) {
    __stats_accumulate_64bit(&stats64->rdCount, &values->rdCount,
                                   (MTLK_GET_STATS_D2(hw_stats->stats_copy, RxCoordinatorCounters.rdCount[sta_sid])));
    __stats_accumulate_64bit(&stats64->mpduInAmpdu, &values->mpduInAmpdu,
                                   (MTLK_GET_STATS_D2(hw_stats->stats_copy, RxCoordinatorCounters.vapCounts[vap_id].mpduInAmpdu)));
  __stats_accumulate_64bit(&stats64->agerPdNoTransmitCountSta, &values->agerPdNoTransmitCountSta,
                                   (MTLK_GET_STATS_D2(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid].agerPdNoTransmitCountSta)));
  __stats_accumulate_64bit(&stats64->successByteCount, &values->successByteCount,
                                   (MTLK_GET_STATS_D2(hw_stats->stats_copy, perClientStats.successByteCount[sta_sid])));
  } else {
    __stats_accumulate_64bit(&stats64->rdCount, &values->rdCount,
                                   (MTLK_GET_STATS_G7(hw_stats->stats_copy, RxCoordinatorCounters.rdCount[sta_sid])));
    __stats_accumulate_64bit(&stats64->mpduInAmpdu, &values->mpduInAmpdu,
                                   (MTLK_GET_STATS_G7(hw_stats->stats_copy, RxCoordinatorCounters.vapCounts[vap_id].mpduInAmpdu)));
    __stats_accumulate_64bit(&stats64->agerPdNoTransmitCountSta, &values->agerPdNoTransmitCountSta,
                                   (MTLK_GET_STATS_G7(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid].agerPdNoTransmitCountSta)));
    __stats_accumulate_64bit(&stats64->successByteCount, &values->successByteCount,
                                   (MTLK_GET_STATS_G7(hw_stats->stats_copy, perClientStats.successByteCount[sta_sid])));
  }

  _mtlk_sta_update_ofdma_statistics(hw, sta);
  _mtlk_sta_update_dl_mimo_statistics(hw, sta);

  __stats_accumulate_64bit(&stats64->txRetryCount, &values->txRetryCount,
                          (MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid].mpduRetransmission, chip_id)));
  __stats_accumulate_64bit(&stats64->channelTransmitTime, &values->channelTransmitTime,
                          (MTLK_GET_STATS(hw_stats->stats_copy, baaCounters.staCounts[sta_sid].channelTransmitTime, chip_id)));


  for(agreement = 0 ; agreement < TWT_MAX_INDIVIDUAL_AGREEMENTS_ALLOWED_PER_STA; agreement++ ) {
    values->twtStaParams.numOfAgreementsForSta = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].numOfAgreementsForSta, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].state = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].state, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].agreementType = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].agreementType, chip_id);

    values->twtStaParams.twtIndividualAgreement[agreement].operation.implicit = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].operation.implicit, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].operation.announced = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].operation.announced, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].operation.triggerEnabled = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].operation.triggerEnabled, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].params.individual.wakeTime = (MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].params.individual.wakeTimeHigh, chip_id));
    values->twtStaParams.twtIndividualAgreement[agreement].params.individual.wakeTime = (values->twtStaParams.twtIndividualAgreement[agreement].params.individual.wakeTime << 32) | (MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].params.individual.wakeTimeLow, chip_id));
    values->twtStaParams.twtIndividualAgreement[agreement].params.individual.wakeInterval = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].params.individual.wakeInterval, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].params.individual.minWakeDuration = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].params.individual.minWakeDuration, chip_id);
    values->twtStaParams.twtIndividualAgreement[agreement].params.individual.channel = MTLK_GET_STATS(hw_stats->stats_copy,twtStats.twtStaParams[sta_sid].twtIndividualAgreement[agreement].params.individual.channel, chip_id);
  }

  values->uplinkMuStats.uplinkMuType = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.uplinkMuStats[sta_sid].uplinkMuType, chip_id);
  if(values->uplinkMuStats.allocatedUplinkRuNum == HAL_MAX_RU_ALLOCATIONS_DRV) {
    values->uplinkMuStats.allocatedUplinkRuNum = 0;
    values->uplinkMuStats.uplinkRuNumFlag = 1;
  }
  values->uplinkMuStats.uplinkRuAllocations[values->uplinkMuStats.allocatedUplinkRuNum].subChannels = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.uplinkMuStats[sta_sid].ulRuSubChannels, chip_id);
  values->uplinkMuStats.uplinkRuAllocations[values->uplinkMuStats.allocatedUplinkRuNum].type = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.uplinkMuStats[sta_sid].ulRuType, chip_id);
  values->uplinkMuStats.allocatedUplinkRuNum++;

  buffer_status=0;
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt0, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt1, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt2, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt3, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt4, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt5, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt6, chip_id);
  values->uplinkMuStats.ulBufferStatus[buffer_status++].queueSize = MTLK_GET_STATS(hw_stats->stats_copy, uplinkBsrcTidCnt.BsrcPerTidCnt[sta_sid].bufStsCnt7, chip_id);

  values->downlinkMuStats.downlinkMuType = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.downlinkMuStats[sta_sid].downlinkMuType, chip_id);
  if(values->downlinkMuStats.allocatedDownlinkRuNum == HAL_MAX_RU_ALLOCATIONS_DRV) {
    values->downlinkMuStats.allocatedDownlinkRuNum = 0;
    values->downlinkMuStats.downlinkRuNumFlag = 1;
  }
  values->downlinkMuStats.downlinkRuAllocations[values->downlinkMuStats.allocatedDownlinkRuNum].subChannels = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.downlinkMuStats[sta_sid].dlRuSubChannels, chip_id);
  values->downlinkMuStats.downlinkRuAllocations[values->downlinkMuStats.allocatedDownlinkRuNum].type = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.downlinkMuStats[sta_sid].dlRuType, chip_id);
  values->downlinkMuStats.allocatedDownlinkRuNum++;
  for(buffer_status = 0; buffer_status < HAL_MAX_BSR_DRV; buffer_status++) {
    if(_chipid_is_gen6_b0(chip_id)) {
      values->downlinkMuStats.dlBufferStatus[buffer_status].queueSize = (MTLK_GET_STATS_B0(hw_stats->stats_copy, hostIfCounters.qosByteCountSta[sta_sid][buffer_status]) - HOST_INTERFACE_BYTE_COUNTER_RESET_VALUE_WAVE600B) * BYTE_COUNTER_RESOLUTION;
    } else if (_chipid_is_gen6_d2(chip_id)) {
      values->downlinkMuStats.dlBufferStatus[buffer_status].queueSize = (MTLK_GET_STATS_D2(hw_stats->stats_copy, hostIfQosCounters.qosByteCountSta[sta_sid][buffer_status]) - HOST_INTERFACE_BYTE_COUNTER_RESET_VALUE_WAVE600D2) * BYTE_COUNTER_RESOLUTION;
    } else {
      values->downlinkMuStats.dlBufferStatus[buffer_status].queueSize = (MTLK_GET_STATS_G7(hw_stats->stats_copy,
      hostIfQosCounters.qosByteCountSta[sta_sid][buffer_status]) - HOST_INTERFACE_BYTE_COUNTER_RESET_VALUE_WAVE700) * BYTE_COUNTER_RESOLUTION;
    }
  }
  for(buffer_status = 0; buffer_status < NUM_OF_TIDS; buffer_status++) {
     if(_chipid_is_gen6_b0(chip_id)) {
      values->qosTxSta[buffer_status] = MTLK_GET_STATS_B0(hw_stats->stats_copy, hostIfCounters.qosTxSta[sta_sid][buffer_status]);
    } else if(_chipid_is_gen6_d2(chip_id)) {
      values->qosTxSta[buffer_status] = MTLK_GET_STATS_D2(hw_stats->stats_copy, hostIfQosCounters.qosTxSta[sta_sid][buffer_status]);
    } else {
      values->qosTxSta[buffer_status] = MTLK_GET_STATS_G7(hw_stats->stats_copy, hostIfQosCounters.qosTxSta[sta_sid][buffer_status]);
    }
  }
  values->ulRuSubChannels = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.uplinkMuStats[sta_sid].ulRuSubChannels, chip_id);
  values->ulRuType = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.uplinkMuStats[sta_sid].ulRuType, chip_id);
  values->dlRuSubChannels = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.downlinkMuStats[sta_sid].dlRuSubChannels, chip_id);
  values->dlRuType = MTLK_GET_STATS(hw_stats->stats_copy, planManagerStats.downlinkMuStats[sta_sid].dlRuType, chip_id);

  mtlk_osal_lock_release(&hw_stats->lock);

  wave_monitor_tx_stall(sta);
  wave_monitor_rx_stall(sta);

  delta_bytes = (values->rxOutStaNumOfBytes - rx_bytes) + (values->mpduByteTransmitted - tx_bytes);

  /* If there is at least one percent air time usage */
  if (airtime && as->airtime_usage) {
    efficiency = wave_hw_calculate_airtime_efficiency((uint64)delta_bytes * MTLK_OSAL_USEC_IN_SEC, airtime);
  }

  as->rx_time = new_rx_time;
  as->tx_time = new_tx_time;
  as->airtime_efficiency = efficiency;
  ILOG3_DDD("Airtime %u (usec), Airtime %u (%%), Airtime Efficiency %u (Bytes/sec)",
            airtime, as->airtime_usage, as->airtime_efficiency);

#ifdef MTLK_PER_RATE_STAT
  if (sta->rate_stat_possible && (WAVE_RADIO_PDB_GET_INT(wave_vap_radio_get(sta->vap_handle), PARAM_DB_RADIO_STATIONS_STATS_ENABLED))){
    mtlk_rate_key key_rx;
    mtlk_rate_key key_tx;
    mtlk_per_key  key_per;
    mtlk_rate_stat_tx *stat_tx;
    mtlk_rate_stat_rx *stat_rx;
    mtlk_rate_stat_tx new_stat_tx;
    mtlk_rate_stat_rx new_stat_rx;
    mtlk_rate_stat_per stat_per;
    mtlk_rate_stat_per *tmp_stat_per;
    MTLK_HASH_ENTRY_T(rate) *h;
    MTLK_HASH_ENTRY_T(per)  *e;

    mtlk_osal_lock_acquire(&sta->lock);
    mtlk_sta_get_packet_error_rate_stats(sta, &stat_per.per);

    key_per.key.mcs = stat_per.per.mcs;
    key_per.key.nss = stat_per.per.nss;
    key_per.key.gi = stat_per.per.gi;
    key_per.key.bw = stat_per.per.bw;

    e = mtlk_hash_find_per(&sta->per_rate_stat_hash_packet_error_rate, &key_per);
    if (!e) {
      tmp_stat_per = mtlk_osal_mem_alloc(sizeof(mtlk_rate_stat_per) ,MTLK_MEM_TAG_MTDUMP_STATS);
      if (!tmp_stat_per) goto end;
      memset(tmp_stat_per, 0, sizeof(mtlk_rate_stat_per));
      tmp_stat_per->per.nss = stat_per.per.nss;
      tmp_stat_per->per.mcs = stat_per.per.mcs;
      tmp_stat_per->per.gi = stat_per.per.gi;
      tmp_stat_per->per.bw = stat_per.per.bw;
      mtlk_hash_insert_per(&sta->per_rate_stat_hash_packet_error_rate, &key_per, &tmp_stat_per->hentry);
    }
    else {
      tmp_stat_per = MTLK_CONTAINER_OF(e, mtlk_rate_stat_per, hentry);
    }
    tmp_stat_per->per.packet_error_rate = stat_per.per.packet_error_rate;

    mtlk_core_get_associated_dev_rate_info_tx_stats(sta, &new_stat_tx.tx);
    mtlk_core_get_associated_dev_rate_info_rx_stats(sta, &new_stat_rx.rx);

    key_tx.key.nss = new_stat_tx.tx.nss;
    key_tx.key.mcs = new_stat_tx.tx.mcs;
    key_tx.key.bw = new_stat_tx.tx.bw;
    h = mtlk_hash_find_rate(&sta->per_rate_stat_hash_tx, &key_tx);
    if (!h) {
      stat_tx = mtlk_osal_mem_alloc(sizeof(mtlk_rate_stat_tx) ,MTLK_MEM_TAG_MTDUMP_STATS);
      if (!stat_tx) goto end;
      memset(stat_tx, 0, sizeof(mtlk_rate_stat_tx));
      stat_tx->tx.nss = key_tx.key.nss;
      stat_tx->tx.mcs = key_tx.key.mcs;
      stat_tx->tx.bw = key_tx.key.bw;
      mtlk_hash_insert_rate(&sta->per_rate_stat_hash_tx, &key_tx, &stat_tx->hentry);
    }
    else {
      stat_tx = MTLK_CONTAINER_OF(h, mtlk_rate_stat_tx, hentry);
    }
    mtlk_core_calc_associated_dev_rate_info_tx_stats(&stat_tx->tx, &new_stat_tx.tx, &old_stat_tx.tx);
    key_rx.key.nss = new_stat_rx.rx.nss;
    key_rx.key.mcs = new_stat_rx.rx.mcs;
    key_rx.key.bw = new_stat_rx.rx.bw;
    h = mtlk_hash_find_rate(&sta->per_rate_stat_hash_rx, &key_rx);
    if (!h) {
      stat_rx = mtlk_osal_mem_alloc(sizeof(mtlk_rate_stat_rx) ,MTLK_MEM_TAG_MTDUMP_STATS);
      if (!stat_rx) goto end;
      memset(stat_rx, 0, sizeof(mtlk_rate_stat_rx));
      stat_rx->rx.nss = key_rx.key.nss;
      stat_rx->rx.mcs = key_rx.key.mcs;
      stat_rx->rx.bw = key_rx.key.bw;
      mtlk_hash_insert_rate(&sta->per_rate_stat_hash_rx, &key_rx, &stat_rx->hentry);
    }
    else {
      stat_rx = MTLK_CONTAINER_OF(h, mtlk_rate_stat_rx, hentry);
    }
    mtlk_core_calc_associated_dev_rate_info_rx_stats(&stat_rx->rx, &new_stat_rx.rx, &old_stat_rx.rx);
end:
    mtlk_osal_lock_release(&sta->lock);
  }
#endif
}

static void
_wave_update_mhi_vap_PDStats (mtlk_hw_t *hw, sta_entry *sta)
{
  mtlk_core_t               *cur_nic;
  mtlk_core_t               *master_core;
  mtlk_vap_handle_t         vap_handle;
  wave_mhi_PDstats_vap_t    *mhi_vap_PDstats;
  mtlk_error_t              res;
  uint32                    packet_count, prevTxInUnicastPD, tmp_packet_count;
  uint64                    byte_count, prevTxInUnicastBytePD, tmp_byte_count;
  mtlk_mhi_stats_sta_cntr_t *sta_stat_cntrs;
  mtlk_mhi_stats_sta_cntr_t *sta_stat_cntrs_snapshot;
  unsigned vap_id           = mtlk_vap_get_id(sta->vap_handle);

  sta_stat_cntrs = mtlk_sta_get_mhi_stat_array(sta);
  sta_stat_cntrs_snapshot = mtlk_sta_get_mhi_stat_snapshot_array(sta);
  master_core = mtlk_core_get_master(sta->vap_handle);
  res = mtlk_vap_manager_get_vap_handle(mtlk_vap_get_manager(master_core->vap_handle), vap_id, &vap_handle);
  if (MTLK_ERR_OK == res) {
    cur_nic = mtlk_vap_get_core(vap_handle);
    mhi_vap_PDstats = &cur_nic->mhi_vap_PDstat;

    /* For unicast PD stats */
    prevTxInUnicastPD = mhi_vap_PDstats->PDstats.txInUnicastPD;
    prevTxInUnicastBytePD = mhi_vap_PDstats->PDstats64.txInUnicastBytePD;

    mhi_vap_PDstats->PDstats.txInUnicastPD += ((sta_stat_cntrs->successCount - sta_stat_cntrs_snapshot->successCount) -
                                           (sta_stat_cntrs->prevUnicastPacketSent - sta_stat_cntrs_snapshot->prevUnicastPacketSent));
    sta_stat_cntrs->prevUnicastPacketSent = sta_stat_cntrs->successCount;

    mhi_vap_PDstats->PDstats64.txInUnicastBytePD += ((sta_stat_cntrs->successByteCount - sta_stat_cntrs_snapshot->successByteCount) -
                                                  (sta_stat_cntrs->prevUnicastByteSent - sta_stat_cntrs_snapshot->prevUnicastByteSent));
    sta_stat_cntrs->prevUnicastByteSent = sta_stat_cntrs->successByteCount;

    packet_count = mhi_vap_PDstats->PDstats.txInUnicastPD - prevTxInUnicastPD;
    byte_count = mhi_vap_PDstats->PDstats64.txInUnicastBytePD - prevTxInUnicastBytePD;
    mtlk_core_add_cnt(cur_nic, MTLK_CORE_CNT_PD_UNICAST_PACKETS_SENT, packet_count);
    mtlk_core_add64_cnt(cur_nic, MTLK_CORE_CNT_PD_UNICAST_BYTES_SENT_64, byte_count);

    /* for broadcastPD tx stats */
    tmp_packet_count = mtlk_osal_atomic_get(&mhi_vap_PDstats->PDstats.txInBroadcastPD);
    tmp_byte_count   = mtlk_osal_atomic64_get(&mhi_vap_PDstats->PDstats64.txInBroadcastBytePD);

    packet_count = tmp_packet_count - mhi_vap_PDstats->PDstats.prevTxInBroadcastPD;
    byte_count   = tmp_byte_count - mhi_vap_PDstats->PDstats64.prevTxInBroadcastBytePD;

    mhi_vap_PDstats->PDstats.prevTxInBroadcastPD = tmp_packet_count;
    mhi_vap_PDstats->PDstats64.prevTxInBroadcastBytePD = tmp_byte_count;

    mtlk_core_add_cnt(cur_nic, MTLK_CORE_CNT_PD_BROADCAST_PACKETS_SENT, packet_count);
    mtlk_core_add64_cnt(cur_nic, MTLK_CORE_CNT_PD_BROADCAST_BYTES_SENT_64, byte_count);
  }
}

static void
_hw_cleanup_phy_rx_status (mtlk_hw_t *hw)
{
    hw_phy_rx_status_t  *phy_rx_status = &hw->phy_rx_status;

    mtlk_osal_mem_free(phy_rx_status->db_data->staPhyRxStatus);
    mtlk_osal_mem_free(phy_rx_status->db_data->devicePhyRxStatus);
    mtlk_osal_mem_free(phy_rx_status->db_data);
    memset(phy_rx_status, 0, sizeof(*phy_rx_status));
}


void __MTLK_IFUNC
mtlk_hw_statistics_cleanup (mtlk_hw_t *hw)
{
    hw_statistics_t    *hw_stats;

    hw_stats = &hw->hw_stats;

    _hw_cleanup_phy_rx_status(hw);

    if (hw_stats->stats_data) mtlk_osal_mem_free(hw_stats->stats_data);
    if (hw_stats->stats_copy) mtlk_osal_mem_free(hw_stats->stats_copy);
    if (hw_stats->stats_snapshot) mtlk_osal_mem_free(hw_stats->stats_snapshot);

    mtlk_osal_lock_cleanup(&hw_stats->lock);
    mtlk_osal_lock_cleanup(&hw_stats->stats_scan_lock);

    memset(hw_stats, 0, sizeof(*hw_stats));
}

void mtlk_hw_set_stats_available (mtlk_hw_t *hw, BOOL value)
{
  hw_statistics_t *hw_stats = &hw->hw_stats;
  hw_stats->available = value;
  return;
}

BOOL mtlk_hw_get_stats_available (mtlk_hw_t *hw)
{
  hw_statistics_t *hw_stats = &hw->hw_stats;
  return hw_stats->available;
}

static inline void __mtlk_convert_endianess_array_uint16(uint16 *array, size_t size) {
    size_t i;
    size /= sizeof(uint16);
    for (i = 0; i < size; i++) {
        array[i] = MAC_TO_HOST16(array[i]);
        ILOG3_DD("Parameter - %d index - %d\n", array[i], i);
    }
}

static inline void __mtlk_convert_endianess_array_uint32(uint32 *array, size_t size) {
    size_t i;
    size /= sizeof(uint32);
    for (i = 0; i < size; i++) {
        array[i] = MAC_TO_HOST32(array[i]);
        ILOG3_DD("Parameter - %d index - %d\n", array[i], i);
    }
}

static inline void __mtlk_convert_endianess_array_uint64(uint64 *array, size_t size) {
    size_t i;
    size /= sizeof(uint64);
    for (i = 0; i < size; i++) {
        array[i] = MAC_TO_HOST64(array[i]);
        ILOG3_DD("Parameter - %d index - %d\n", array[i], i);
    }
}

static void
_mtlk_hw_check_and_convert_endianess (mtlk_hw_t *hw)
{
  hw_statistics_t         *hw_stats;
  int                      no_of_vaps, no_of_sta, no_of_bands, max_sta_mu_entry, max_sta_mimo_entry;
  int                      i,sta,agreement, ru_id, mu_sta_entry, no_of_bw, no_of_ru_sizes;
  int                      chip_id = hw_mmb_get_chip_id(hw);
  unsigned                 bw = 0;

  if (_chipid_is_gen7(chip_id)) {
    no_of_vaps = GEN7_MAX_VAP;
    no_of_sta = GEN7_MAX_SID;
    no_of_bands = GEN7_NUM_OF_BANDS;
    max_sta_mu_entry = MAX_NUM_OF_MU_STA_STATS_ENTRIES_GEN7;
    max_sta_mimo_entry = MAX_NUM_OF_MIMO_STA_STATS_ENTRIES_GEN7;
    no_of_bw = BW_MAX_G7;
    no_of_ru_sizes = WAVE700_NUM_OF_RU_SIZES;
  } else {
    no_of_vaps = GEN6_MAX_VAP;
    no_of_sta = GEN6_MAX_SID;
    no_of_bands = GEN6_NUM_OF_BANDS;
    max_sta_mu_entry = MAX_NUM_OF_MU_STA_STATS_ENTRIES;
    max_sta_mimo_entry = MAX_NUM_OF_MIMO_STA_STATS_ENTRIES;
    no_of_bw = NUM_OF_BW;
    no_of_ru_sizes = WAVE600_NUM_OF_RU_SIZES;
  }
  hw_stats = &hw->hw_stats;
  for (i = 0; i < no_of_vaps; i++) {
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInUnicastHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInMulticastHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInBroadcastHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInUnicastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInMulticastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].txInBroadcastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutUnicatHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutMulticastHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutBroadcastHd , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutUnicastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutMulticastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].rxOutBroadcastNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.vapCounts[i].agerCount , chip_id);

    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].mpduUnicastOrMngmnt , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].mpduRetryCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].dropMpdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].ampdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].mpduTypeNotSupported , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].replayData , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].replayMngmnt , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.rxppVapCounts[i].bcMcCountVap , chip_id);

    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].rtsSuccessCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].rtsFailure , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitStreamRprtMSDUFailed , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].qosTransmittedFrames , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmittedAmsdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmittedOctetsInAmsdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmittedAmpdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmittedMpduInAmpdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmittedOctetsInAmpdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].beamformingFrames , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].ackFailure , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].failedAmsdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].retryAmsdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].multipleRetryAmsdu , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].amsduAckFailure , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].implicitBarFailure , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].explicitBarFailure , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitStreamRprtMultipleRetryCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitBw20 , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitBw40 , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitBw80 , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.vapCounts[i].transmitBw160 , chip_id);
    if (_chipid_is_gen7(chip_id)) {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, baaCounters.vapCounts[i].transmitBw320);
    }

    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].amsdu);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].amsduBytes);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].mpduInAmpdu);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].octetsInAmpdu);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].rxCoordinatorSecurityMismatch);
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].amsdu);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].amsduBytes);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].mpduInAmpdu);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].octetsInAmpdu);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].rxCoorSecurityMismatch);
    } else {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].amsdu);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].amsduBytes);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].mpduInAmpdu);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].octetsInAmpdu);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].rxCoorSecurityMismatch);
    }

    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].tkipCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, rxCounters.vapCounts[i].securityFailure);
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].tkipCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].securityFailure);
    } else {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].tkipCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, RxCoordinatorCounters.vapCounts[i].securityFailure);
    }
  }

  if (_chipid_is_gen6_b0(chip_id)) {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muPpdusNewEntryBitArray);
  } else if (_chipid_is_gen6_d2(chip_id)) {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muPpdusNewEntryBitArray);
  } else if (_chipid_is_gen7(chip_id)) {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suPpdusNewEntryBitArray);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muPpdusNewEntryBitArray);
  }

  for (mu_sta_entry = 0; mu_sta_entry < max_sta_mu_entry; mu_sta_entry++) {
    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].muRuMpdusCount);
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].muRuMpdusCount);
    } else if (_chipid_is_gen7(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.muStationsPpdus[mu_sta_entry].muRuPpdusCount);
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.muStationsMpdus[mu_sta_entry].muRuMpdusCount);
    }
  }
  /* for HE MU MIMO */
  for (mu_sta_entry = 0; mu_sta_entry < max_sta_mimo_entry; mu_sta_entry++) {
    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.mcsPpdus);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.attemptedSoundingPerMCS);
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.mcsPpdus);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.attemptedSoundingPerMCS);
    } else if (_chipid_is_gen7(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].staId);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.mcsPpdus);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.staStatistics.muTableEntry[mu_sta_entry].muStatistics.attemptedSoundingPerMCS);
    }
  }
  for (i = 0; i < no_of_sta; i++) {
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, hostIfCounters.staCounts[i].rxOutStaNumOfBytes , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.staCounts[i].rddelayed , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.staCounts[i].swUpdateDrop , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.staCounts[i].rdDuplicateDrop , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.staCounts[i].missingSn , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.staCounts[i].mpduFirstRetransmission , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.staCounts[i].mpduTransmitted , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.staCounts[i].mpduByteTransmitted , chip_id);

    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, hostIfCounters.staCounts[i].agerPdNoTransmitCountSta);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, baaCounters.staCounts[i].mpduRetransmission);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, baaCounters.staCounts[i].channelTransmitTime);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].mcsPpdus);
      for (bw = 0; bw < no_of_bw; bw ++) {
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].failedMpdus);
      }
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, hostIfCounters.staCounts[i].agerPdNoTransmitCountSta);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, baaCounters.staCounts[i].mpduRetransmission);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, baaCounters.staCounts[i].channelTransmitTime);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].mcsPpdus);
      for (bw = 0; bw < no_of_bw; bw ++) {
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].failedMpdus);
      }
    } else {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, hostIfCounters.staCounts[i].agerPdNoTransmitCountSta);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, baaCounters.staCounts[i].mpduRetransmission);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, baaCounters.staCounts[i].channelTransmitTime);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.totalPpdusCount[i]);
      MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.stationsPpdus.suStationsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.staStatistics[i].mcsPpdus);
      for (bw = 0; bw < no_of_bw; bw ++) {
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mpdusDl.suStationsMpdus[i].staMpdu[bw].failedMpdus);
      }
    }
  }

  for (i = 0; i < no_of_bands; i++) {
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.bandCounts[i].barMpduCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.bandCounts[i].crcErrorCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, rxCounters.bandCounts[i].delCrcError , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.retryCounts[i].retryCount , chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, baaCounters.retryCounts[i].multipleRetryCount , chip_id);
  }

  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, rxCounters.qosRxSta, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwMngmntframesSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwMngmntFramesConfirmed, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwMngmntframesRecieved, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwctrlFramesSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwctrlFramesRecieved, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwMulticastReplayedPackets, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.fwPairWiseMicFailurePackets, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.groupMicFailurePackets, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.beaconProbeResponsePhyType, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.rxBasicReport, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.txFrameErrorCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.rxGroupFrame, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, baaCounters.txSenderError, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaRequestWasSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaRequestConfiremd, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndValid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndInvalid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusFailure, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseTimeout, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBaAgreementOpened, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasDiscarded, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasAnsweredWithAck, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasAnsweredWithBa, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventInactivityTimeout, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventDelbaWasSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventDelbaReceived, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventCloseTid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventOpenTid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventRemove, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventAdd, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventAddbaResponseReceivedWithIllegalTid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerInitiatorStationGlobalStats.Reserve, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaRequestAccepted, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaRequestWasReceivedWithInvalidParameters, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaResponseWasSentWithStatusSuccess, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaResponseWasSentWithStatusFailure, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBaAgreementOpened, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBaAgreementClosed, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBarWasReceivedWithBaAgreementOpened, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventInactivityTimeout, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventDelbaReceived, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventDelbaWasSent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventIllegalAggregationWasReceived, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventCloseTid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventRemove, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventAdd, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventAddbaRequestReceivedWithIllegalTid, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, tsManagerRecipientStationGlobalStats.Reserve, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.ratesMask, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.bfReportReceivedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.protectionSentCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.protectionSucceededCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.totalTxTime, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.ewmaTimeNsec, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.sumTimeNsec, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.numMsdus, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.transmittedAmpdu, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.rxDc, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.txLo, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.rxIq, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.txIq, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.per, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.totalPsduLength, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationStats.totalZld, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, alphaFilterStats.maxRssi, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, alphaFilterStats.minRssi, chip_id);

  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, multicastStats.notEnoughClonePds , chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  multicastStats.allClonesFinishedTransmission, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  multicastStats.noStationsInGroup, chip_id);

  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  trainingManagerStats.trainingTimerExpiredCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  trainingManagerStats.trainingStartedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  trainingManagerStats.trainingFinishedSuccessfullyCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  trainingManagerStats.trainingFinishedUnsuccessfullyCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  trainingManagerStats.trainingNotStartedCounter, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.ratesMask, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.protectionSentCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.protectionSucceededCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.DataPhyMode, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.powerData, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.scpData, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.dataBwLimit, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.groupGoodput, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.raState, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.raStability, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.nssData, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, linkAdaptationMuVhtStats.mcsData, chip_id);

  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuHeEhtStats.dlBitmap, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuHeEhtStats.ulBitmap, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuHeEhtStats.dlToggleBitmap, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuHeEhtStats.ulToggleBitmap, chip_id);

  for (i = 0; i < MAX_NUM_OF_HE_MU_DL_GROUPS; i++) {
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].protectionSentCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].protectionSucceededCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].dlPhaseAllocFailCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].heMuFastProbeTriggeredCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].heMuFastProbeNotFinishedCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].heMuCollisionDetectedCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtDLStatistics[i].staId, chip_id);
  }

  for (i = 0; i < MAX_NUM_OF_HE_MU_UL_GROUPS; i++) {
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].ulPCStabilityCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].fastDropCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].invalidReportsCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].rxPPDUsCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].changedWpCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].collisionDetectedCnt, chip_id);
    MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuHeEhtStats.laMuHeEhtULStatistics[i].staId, chip_id);
  }

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, ptaStats.illegalEvent, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, ptaStats.ignoreEvent, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.zigbeeStarvationTimerExpired, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.btStarvationTimerExpired, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.zigbeeDenyOverTime, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.btDenyOverTime, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.gpio0TxDemandLow, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.zigbeeGrnatNotUsed, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.btGrnatNotUsed, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.zigbeeGrnatUsed, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.btGrnatUsed, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.zigbeeTxOngoing, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.btTxOngoing, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.gpio, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.priorityIsHigh, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  ptaStats.priorityIsLow, chip_id);

  for (sta = 0; sta < no_of_sta; sta++) {
    for(agreement = 0 ; agreement < TWT_MAX_INDIVIDUAL_AGREEMENTS_ALLOWED_PER_STA; agreement++ ) {
      if (_chipid_is_gen6_b0(chip_id)) {
        MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, twtStats.numOfStaInSp);
      }
      else if(_chipid_is_gen6_d2(chip_id)) {
        MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, twtStats.numOfStaInSp);
      }
      else {
        MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, twtStats.numOfStaInSp);
      }
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].numOfAgreementsForSta, chip_id);
      MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].state, chip_id);
      MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].agreementType, chip_id);
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].params.individual.wakeTimeHigh, chip_id);
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].params.individual.wakeTimeLow, chip_id);
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].params.individual.wakeInterval, chip_id);
      MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].params.individual.minWakeDuration, chip_id);
      MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, twtStats.twtStaParams[sta].twtIndividualAgreement[agreement].params.individual.channel, chip_id);
#if 0 /* Broadcast not supported in FW */
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].twtAgreement[agreement].params.broadcast.tragetBeacon, chip_id);
      MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, twtStats.twtStaParams[sta].twtAgreement[agreement].params.broadcast.listenInterval, chip_id);
#endif
    }
  }

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.retryCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.successCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.successByteCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.exhaustedCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.clonedCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.mpduRetryCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.mpduInAmpdu, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.ampdu, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.packetRetransCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.oneOrMoreRetryCount, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonClassifier, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonDisconnect, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonATF, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonTSFlush, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonReKey, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonSetKey, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonDiscard, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonDsabled, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, perClientStats.dropCntReasonAggError, chip_id);

  for (i = 0; i < no_of_sta; i++) {
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt0, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt1, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt2, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt3, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt4, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt5, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt6, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data, uplinkBsrcTidCnt.BsrcPerTidCnt[i].bufStsCnt7, chip_id);
  }

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.calibrationMask, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.onlineCalibrationMask, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonClassifier, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonDisconnect, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonATF, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonTSFlush, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonReKey, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonSetKey, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonDiscard, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonDsabled, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.dropReasonAggError, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.defragStart, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.defragEnd, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.defragTimeout, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.classViolationErrors, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.rxhErrors, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.rxSmps, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.rxOmn, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.txMngFromHost, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.txDataFromHost, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.cyclicBufferOverflow, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.beaconsTransmitted, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.debugStatistics, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.exceedsETSIMaxSizeUnicast, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, generalStats.exceedsETSIMaxSizeMulticast, chip_id);

  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, dynamicBwStats.dynamicBW20MHz, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, dynamicBwStats.dynamicBW40MHz, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, dynamicBwStats.dynamicBW80MHz, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_32(hw_stats->stats_data, dynamicBwStats.dynamicBW160MHz, chip_id);

  for (i = 0; i < no_of_bands; i++) {
    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].irad);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].tsf);
    }
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].extStaRx, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].txPower, chip_id);
    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_ARRAY_16_B0(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].backgroundNoise);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].attemptedSoundingPerMCS);
      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].successMpdus);
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].failedMpdus);
        for (bw = 0; bw < no_of_bw; bw++) {
          MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
          MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
        }
      }
      for (bw = 0; bw < no_of_bw; bw++) {
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].failedMpdus);
      }
    } else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].backgroundNoise);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].attemptedSoundingPerMCS);
      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].successMpdus);
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].failedMpdus);
        for (bw = 0; bw < no_of_bw; bw++) {
          MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
          MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
        }
      }
      for (bw = 0; bw < no_of_bw; bw++) {
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].failedMpdus);
      }
    } else {
      MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data,  phyStatistics.devicePhyRxStatus[i].backgroundNoise);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].totalPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].suPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muOfdmaPpdusCount);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muMimoPpdusCount);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.suRateDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].nssPpdus);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].mcsPpdus);
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].succeededSounding);
      MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, linkAdaptationSuMuStatistics.mimoDl.bandStatistics[i].attemptedSoundingPerMCS);
      for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id++) {
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].muRuPpdusCount[ru_id]);
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].successMpdus);
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].muRuMpdus[ru_id].failedMpdus);
        for (bw = 0; bw < no_of_bw; bw++) {
          MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
          MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[i].bwRusInPpdus[bw][ru_id]);
        }
      }
      for (bw = 0; bw < no_of_bw; bw++) {
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].successMpdus);
        MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[i].suBandMpdus[bw].failedMpdus);
      }
    }
  }

  for (i = 0; i < no_of_sta; i++) {
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].phyRate, chip_id);
    if (_chipid_is_gen7(chip_id)) {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].psduRate);
    } else if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_UINT16_B0(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].psduRate);
    } else {
      MTLK_CONVERT_ENDIANESS_UINT16_D2(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].psduRate);
    }

    if (_chipid_is_gen6_b0(chip_id)) {
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].irad);
      MTLK_CONVERT_ENDIANESS_B0(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].lastTsf);
    }
    else if (_chipid_is_gen6_d2(chip_id)) {
      MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].tsf);
    } else {
      MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].tsf);
    }
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].perClientRxtimeUsage, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT16(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].rssiAckAvarage.count, chip_id);
    MTLK_CONVERT_ENDIANESS(hw_stats->stats_data,  phyStatistics.staPhyRxStatus[i].phyRateSynchedToPsduRate, chip_id);
  }

  for (i = 0; i < no_of_bands; i++) {
    MTLK_CONVERT_ENDIANESS_UINT64(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationCurrTime, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationBusy, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationBusyTx, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationBusyRx, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationBusyRxSelf, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilizationBusyExt, chip_id);
    MTLK_CONVERT_ENDIANESS_UINT32(hw_stats->stats_data,  currentChannelStats.channelUtilizationStats[i].chUtilization, chip_id);
  }

  if (_chipid_is_gen6_b0(chip_id)) {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, hostIfCounters.qosByteCountSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, hostIfCounters.qosTxSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, hostIfCounters.qosTxVap);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxPackets);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxBytes);

  } else if (_chipid_is_gen6_d2(chip_id)) {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, hostIfQosCounters.qosByteCountSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, hostIfQosCounters.qosTxSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, hostIfQosCounters.qosTxVap);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxPackets);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxBytes);

  } else {
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, hostIfQosCounters.qosByteCountSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, hostIfQosCounters.qosTxSta);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, hostIfQosCounters.qosTxVap);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxPackets);
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, MultiCastGroupStatistics.multiCastTxBytes);

  }
  if (_chipid_is_gen6_b0(chip_id))
    MTLK_CONVERT_ENDIANESS_ARRAY_32_B0(hw_stats->stats_data, rxCounters.rdCount);
  else if (_chipid_is_gen6_d2(chip_id))
    MTLK_CONVERT_ENDIANESS_ARRAY_32_D2(hw_stats->stats_data, RxCoordinatorCounters.rdCount);
  else
    MTLK_CONVERT_ENDIANESS_ARRAY_32_G7(hw_stats->stats_data, RxCoordinatorCounters.rdCount);


  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsCreatedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsRemovedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulOfdmaGroupsCounters.groupsRemovalReasonCounters, chip_id);

  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsCreatedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsRemovedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlOfdmaGroupsCounters.groupsRemovalReasonCounters, chip_id);

  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsCreatedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsRemovedCounter, chip_id);
  MTLK_CONVERT_ENDIANESS_ARRAY_16(hw_stats->stats_data, linkAdaptationMuGroupsStats.dlMimoGroupsCounters.groupsRemovalReasonCounters, chip_id);

  if (_chipid_is_gen6_d2(chip_id)) {
    MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsCreatedCounter);
    MTLK_CONVERT_ENDIANESS_D2(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovedCounter);
    MTLK_CONVERT_ENDIANESS_ARRAY_16_D2(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovalReasonCounters);
  } else if (_chipid_is_gen7(chip_id)) {
    MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsCreatedCounter);
    MTLK_CONVERT_ENDIANESS_G7(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovedCounter);
    MTLK_CONVERT_ENDIANESS_ARRAY_16_G7(hw_stats->stats_data, linkAdaptationMuGroupsStats.ulMimoGroupsCounters.groupsRemovalReasonCounters);
  }
}

void
wave_hw_reset_stats_snapshot(mtlk_hw_t *hw)
{
  size_t               size = 0;
  int                  chip_id;
  hw_statistics_t     *hw_stats = &hw->hw_stats;
  chip_id = hw_mmb_get_chip_id(hw);
  if (_chipid_is_gen6_b0(chip_id)) {
      size = sizeof(StatisticsDb_t_wave600b);
  } else if (_chipid_is_gen7(chip_id)) {
      size = sizeof(StatisticsDb_t_wave700);
  } else {
      size = sizeof(StatisticsDb_t_wave600d2);
  }
  memset(hw_stats->stats_snapshot, 0, size);
}

void __MTLK_IFUNC
wave_hw_reset_stats (mtlk_hw_t *hw)
{
  size_t  stats_size;
  int     chip_id;
  hw_statistics_t *hw_stats = &hw->hw_stats;

  chip_id = hw_mmb_get_chip_id(hw);
  if (_chipid_is_gen6_b0(chip_id)) {
    stats_size = sizeof(StatisticsDb_t_wave600b);
  } else if (_chipid_is_gen7(chip_id)) {
    stats_size = sizeof(StatisticsDb_t_wave700);
  } else {
    stats_size = sizeof(StatisticsDb_t_wave600d2);
  }

  memset(hw_stats->stats_data, 0, stats_size);
  memset(hw_stats->stats_copy, 0, stats_size);
}

void
wave_hw_stats_lock(mtlk_hw_t *hw, BOOL acquire)
{
  hw_statistics_t     *hw_stats = NULL;
  MTLK_ASSERT(hw);
  hw_stats = &hw->hw_stats;
  if (acquire)
    mtlk_osal_lock_acquire(&hw_stats->lock);
  else
    mtlk_osal_lock_release(&hw_stats->lock);
}

void
wave_hw_capture_stats_snapshot_for_reset(mtlk_hw_t *hw)
{
  size_t               size = 0;
  int                  chip_id;
  hw_statistics_t     *hw_stats = &hw->hw_stats;
  chip_id = hw_mmb_get_chip_id(hw);
  if (_chipid_is_gen6_b0(chip_id)) {
      size = sizeof(StatisticsDb_t_wave600b);
  } else if (_chipid_is_gen7(chip_id)) {
      size = sizeof(StatisticsDb_t_wave700);
  } else {
      size = sizeof(StatisticsDb_t_wave600d2);
  }
  wave_memcpy(hw_stats->stats_snapshot, size, hw_stats->stats_data, size);
}
static __INLINE void
__mtlk_core_reset_PD_stats (mtlk_core_t *core, mtlk_core_wss_cnt_id_e cnt_id)
{
  MTLK_ASSERT(cnt_id >= 0 && cnt_id < MTLK_CORE_CNT_LAST);

  mtlk_wss_reset_stat(core->wss, cnt_id);
}

static __INLINE void
__mtlk_core_reset_PD_stats64 (mtlk_core_t *core, mtlk_core_wss_cnt_id_e cnt_id)
{
  MTLK_ASSERT(cnt_id >= 0 && cnt_id < MTLK_CORE_CNT_LAST);

  mtlk_wss_reset_stat_64(core->wss, cnt_id);
}

void
wave_core_reset_vap_stats(mtlk_core_t *core)
{
  mtlk_vap_manager_t *vap_mgr;
  mtlk_vap_handle_t vap_handle;
  wave_mhi_PDstats_vap_t *mhi_vap_PDstats;

  unsigned vaps_count, vap_id;
  int i;

  vap_mgr = mtlk_vap_get_manager(core->vap_handle);
  vaps_count = mtlk_vap_manager_get_max_vaps_count(vap_mgr);

  for (vap_id = 0; vap_id < vaps_count; vap_id++) {
     if (mtlk_vap_manager_get_vap_handle(vap_mgr, vap_id, &vap_handle) == MTLK_ERR_OK) {
       mtlk_core_t *cur_nic = mtlk_vap_get_core(vap_handle);
       mhi_vap_PDstats = &cur_nic->mhi_vap_PDstat;
       for (i = MTLK_CORE_CNT_PD_BROADCAST_PACKETS_SENT; i <= MTLK_CORE_CNT_PD_MULTICAST_BYTES_SENT_64; i++) {
          if ((i == MTLK_CORE_CNT_PD_BROADCAST_BYTES_SENT_64) || (i == MTLK_CORE_CNT_PD_UNICAST_BYTES_SENT_64) ||
              (i == MTLK_CORE_CNT_PD_MULTICAST_BYTES_SENT_64))
            __mtlk_core_reset_PD_stats64(core, i);
          else
            __mtlk_core_reset_PD_stats(core, i);
       }
       memset(mhi_vap_PDstats, 0, sizeof(*mhi_vap_PDstats));
       mtlk_osal_atomic_set(&mhi_vap_PDstats->PDstats.txInBroadcastPD, 0);
       mtlk_osal_atomic64_set(&mhi_vap_PDstats->PDstats64.txInBroadcastBytePD, 0);
     }
  }
}

static mtlk_error_t
_mtlk_hw_parse_statistics (mtlk_hw_t *hw)
{
  mtlk_error_t         res = MTLK_ERR_OK;
  hw_statistics_t     *hw_stats;
  hw_phy_rx_status_t  *phy_rx_status;
  void                *dev_stats, *sta_stats;
  size_t               dev_ssize;
  size_t               size = 0;
  int                  chip_id = hw_mmb_get_chip_id(hw);
  unsigned             i, a;
  int                  no_of_vaps;
  int                  no_of_sta;
  int                  no_of_bands;

  hw_stats = &hw->hw_stats;
  phy_rx_status = &hw->phy_rx_status;

  if (_chipid_is_gen6_b0(chip_id)) {
      size = sizeof(StatisticsDb_t_wave600b);
  } else if (_chipid_is_gen7(chip_id)) {
      size = sizeof(StatisticsDb_t_wave700);
  } else {
      size = sizeof(StatisticsDb_t_wave600d2);
  }

  if (_chipid_is_gen7(chip_id)) {
    no_of_vaps = GEN7_MAX_VAP;
    no_of_sta = GEN7_MAX_SID;
    no_of_bands = GEN7_NUM_OF_BANDS;
  } else {
    no_of_vaps = HW_NUM_OF_VAPS;
    no_of_sta = HW_NUM_OF_STATIONS;
    no_of_bands = GEN6_NUM_OF_BANDS;
  }

  ILOG3_V("Dumping the statistics");
  mtlk_dump(3, hw_stats->stats_data, MIN(32,  size), "dump of Statistics");

  /* Convert endianess */
  _mtlk_hw_check_and_convert_endianess(hw);

  /* copy stats_data to stats_copy */
  wave_memcpy(hw_stats->stats_copy, size, hw_stats->stats_data, size);
  /* Copy phyStatistics to phy_rx_status */
  /* It is not possible to copy the entire PhyStatistics
   * due to its different size depending on the HW type */

  /* 1. Device status of each of the Radios */
  dev_ssize = MTLK_GET_STATS_SIZE(hw_stats->stats_copy, phyStatistics.devicePhyRxStatus[0], chip_id);
  for (i = 0; i < no_of_bands; i++) {
    dev_stats = MTLK_GET_STATS_ADDRESS(hw_stats->stats_copy, phyStatistics.devicePhyRxStatus[i], chip_id);

    if (!_chipid_is_gen6_d2_or_gen7 (chip_id)) {
      wave_memcpy(&phy_rx_status->db_data->devicePhyRxStatus[i].noise, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].noise),
                  &((devicePhyRxStatusDb_t_wave600b*)dev_stats)->noise, sizeof(((devicePhyRxStatusDb_t_wave600b*)dev_stats)->noise));
      wave_memcpy(&phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain),
                  &((devicePhyRxStatusDb_t_wave600b*)dev_stats)->rf_gain, sizeof(((devicePhyRxStatusDb_t_wave600b*)dev_stats)->rf_gain));
      phy_rx_status->db_data->devicePhyRxStatus[i].irad = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->irad;
      phy_rx_status->db_data->devicePhyRxStatus[i].tsf = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->tsf;
      phy_rx_status->db_data->devicePhyRxStatus[i].channel_load = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->channel_load;
      phy_rx_status->db_data->devicePhyRxStatus[i].totalChannelUtilization = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->totalChannelUtilization;
      phy_rx_status->db_data->devicePhyRxStatus[i].chNon80211Noise = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->chNon80211Noise;
      phy_rx_status->db_data->devicePhyRxStatus[i].CWIvalue = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->CWIvalue;
      phy_rx_status->db_data->devicePhyRxStatus[i].extStaRx = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->extStaRx;
      phy_rx_status->db_data->devicePhyRxStatus[i].txPower = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->txPower;
      phy_rx_status->db_data->devicePhyRxStatus[i].channelNum = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->channelNum;
      phy_rx_status->db_data->devicePhyRxStatus[i].activeAntMask = ((devicePhyRxStatusDb_t_wave600b*)dev_stats)->activeAntMask;
      phy_rx_status->db_data->devicePhyRxStatus[i].zwdfsAntRssi = RSSI_INVALID; /*data avaiable only for wave 700*/
      wave_memcpy(&phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise,
                  sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise),
                  &((devicePhyRxStatusDb_t_wave600b*)dev_stats)->backgroundNoise,
                  sizeof(((devicePhyRxStatusDb_t_wave600b*)dev_stats)->backgroundNoise));
      for(a = ARRAY_SIZE(((devicePhyRxStatusDb_t_wave600b*)dev_stats)->backgroundNoise); a < WAVE_STAT_MAX_ANTENNAS; a++) {
        phy_rx_status->db_data->devicePhyRxStatus[i].noise[a] = NOISE_INVALID;
        phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain[a] = 0;
        phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise[a] = NOISE_INVALID;
      }
    }
    else if(_chipid_is_gen6_d2(chip_id)) {
      memset(phy_rx_status->db_data->devicePhyRxStatus[i].noise, 0, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].noise)); /*data not available on 600-2/wave 700*/
      memset(phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain, 0, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain)); /*data not available on 600-2/wave 700*/
      phy_rx_status->db_data->devicePhyRxStatus[i].irad = 0; /*data not available on 600-2/wave 700*/
      phy_rx_status->db_data->devicePhyRxStatus[i].tsf = 0; /*data not available on 600-2/wave 700*/
      phy_rx_status->db_data->devicePhyRxStatus[i].channel_load = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->channel_load;
      phy_rx_status->db_data->devicePhyRxStatus[i].totalChannelUtilization = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->totalChannelUtilization;
      phy_rx_status->db_data->devicePhyRxStatus[i].chNon80211Noise = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->chNon80211Noise;
      phy_rx_status->db_data->devicePhyRxStatus[i].CWIvalue = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->CWIvalue;
      phy_rx_status->db_data->devicePhyRxStatus[i].extStaRx = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->extStaRx;
      phy_rx_status->db_data->devicePhyRxStatus[i].txPower = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->txPower;
      phy_rx_status->db_data->devicePhyRxStatus[i].channelNum = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->channelNum;
      phy_rx_status->db_data->devicePhyRxStatus[i].activeAntMask = ((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->activeAntMask;
      phy_rx_status->db_data->devicePhyRxStatus[i].zwdfsAntRssi = RSSI_INVALID; /* data available only for wave 700*/
      wave_memcpy(&phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise,
                  sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise),
                  &((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->backgroundNoise,
                  sizeof(((devicePhyRxStatusDb_t_wave600d2 *)dev_stats)->backgroundNoise));

      /* TODO: should 600-2 noiseFloor parameter be used as well? */
      MTLK_STATIC_ASSERT(ARRAY_SIZE(((devicePhyRxStatusDb_t_wave600d2*)dev_stats)->backgroundNoise) == WAVE_STAT_MAX_ANTENNAS);
      // no need to fill remaining elements
    } else {
      memset(phy_rx_status->db_data->devicePhyRxStatus[i].noise, 0, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].noise));
      memset(phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain, 0, sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].rf_gain));
      phy_rx_status->db_data->devicePhyRxStatus[i].irad = 0;
      phy_rx_status->db_data->devicePhyRxStatus[i].tsf = 0;
      phy_rx_status->db_data->devicePhyRxStatus[i].channel_load = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->channel_load;
      phy_rx_status->db_data->devicePhyRxStatus[i].totalChannelUtilization = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->totalChannelUtilization;
      phy_rx_status->db_data->devicePhyRxStatus[i].chNon80211Noise = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->chNon80211Noise;
      phy_rx_status->db_data->devicePhyRxStatus[i].CWIvalue = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->CWIvalue;
      phy_rx_status->db_data->devicePhyRxStatus[i].extStaRx = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->extStaRx;
      phy_rx_status->db_data->devicePhyRxStatus[i].txPower = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->txPower;
      phy_rx_status->db_data->devicePhyRxStatus[i].channelNum = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->channelNum;
      phy_rx_status->db_data->devicePhyRxStatus[i].activeAntMask = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->activeAntMask;
      phy_rx_status->db_data->devicePhyRxStatus[i].zwdfsAntRssi = ((devicePhyRxStatusDb_t_wave700 *)dev_stats)->zwdfsAntRssi;
      wave_memcpy(&phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise,
                  sizeof(phy_rx_status->db_data->devicePhyRxStatus[i].backgroundNoise),
                  &((devicePhyRxStatusDb_t_wave700 *)dev_stats)->backgroundNoise,
                  sizeof(((devicePhyRxStatusDb_t_wave700 *)dev_stats)->backgroundNoise));
      MTLK_STATIC_ASSERT(ARRAY_SIZE(((devicePhyRxStatusDb_t_wave700 *)dev_stats)->backgroundNoise) == WAVE_STAT_MAX_ANTENNAS);
      // no need to fill remaining elements
    }
  }

  /* 1. Station status for all STAs together */
  sta_stats = MTLK_GET_STATS_ADDRESS(hw_stats->stats_copy, phyStatistics.staPhyRxStatus, chip_id);
  for (i = 0; i < no_of_sta; i++)
  {
    if (_chipid_is_gen6_b0(chip_id)) {

      stationPhyRxStatusDb_t_wave600b *sta_phyrx = (stationPhyRxStatusDb_t_wave600b*)sta_stats + i;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRate = sta_phyrx->phyRate;
      phy_rx_status->db_data->staPhyRxStatus[i].perClientRxtimeUsage = sta_phyrx->perClientRxtimeUsage;
      phy_rx_status->db_data->staPhyRxStatus[i].psduRate = sta_phyrx->psduRate;
      phy_rx_status->db_data->staPhyRxStatus[i].irad = sta_phyrx->irad;
      phy_rx_status->db_data->staPhyRxStatus[i].lastTsf = sta_phyrx->lastTsf;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRateSynchedToPsduRate = sta_phyrx->phyRateSynchedToPsduRate;
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].noise,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].noise),
                  &sta_phyrx->noise,
                  sizeof(sta_phyrx->noise));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].gain,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].gain),
                  &sta_phyrx->gain,
                  sizeof(sta_phyrx->gain));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssi,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssi),
                  &sta_phyrx->rssi,
                  sizeof(sta_phyrx->rssi));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage),
                  &sta_phyrx->rssiAckAvarage,
                  sizeof(sta_phyrx->rssiAckAvarage));
      for(a = ARRAY_SIZE(phy_rx_status->db_data->staPhyRxStatus[i].rssi); a < WAVE_STAT_MAX_ANTENNAS; a++) {
        phy_rx_status->db_data->staPhyRxStatus[i].rssi[a] = RSSI_INVALID;
        phy_rx_status->db_data->staPhyRxStatus[i].noise[a] = NOISE_INVALID;
        phy_rx_status->db_data->staPhyRxStatus[i].gain[a] = 0;
        memset(phy_rx_status->db_data->staPhyRxStatus[i].rssiArray[a], RSSI_INVALID, sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiArray[a]));
      }

    } else if(_chipid_is_gen6_d2(chip_id)) {

      stationPhyRxStatusDb_t_wave600d2 *sta_phyrx = (stationPhyRxStatusDb_t_wave600d2*)sta_stats + i;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRate = sta_phyrx->phyRate;
      phy_rx_status->db_data->staPhyRxStatus[i].perClientRxtimeUsage = sta_phyrx->perClientRxtimeUsage;
      phy_rx_status->db_data->staPhyRxStatus[i].psduRate = sta_phyrx->psduRate;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRateSynchedToPsduRate = sta_phyrx->phyRateSynchedToPsduRate;
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].noise,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].noise),
                  &sta_phyrx->noise,
                  sizeof(sta_phyrx->noise));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].gain,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].gain),
                  &sta_phyrx->gain,
                  sizeof(sta_phyrx->gain));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssi,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssi),
                  &sta_phyrx->rssi,
                  sizeof(sta_phyrx->rssi));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssiArray,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiArray),
                  &sta_phyrx->rssiArray,
                  sizeof(sta_phyrx->rssiArray));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage), &sta_phyrx->rssiAckAvarage,
                  sizeof(sta_phyrx->rssiAckAvarage));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].evm,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].evm), &sta_phyrx->evm,
                  sizeof(sta_phyrx->evm));

      MTLK_STATIC_ASSERT(ARRAY_SIZE(sta_phyrx->rssi) == WAVE_STAT_MAX_ANTENNAS); // no need to fill remaining elements
    } else {

      stationPhyRxStatusDb_t_wave700 *sta_phyrx = (stationPhyRxStatusDb_t_wave700*)sta_stats + i;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRate = sta_phyrx->phyRate;
      phy_rx_status->db_data->staPhyRxStatus[i].perClientRxtimeUsage = sta_phyrx->perClientRxtimeUsage;
      phy_rx_status->db_data->staPhyRxStatus[i].psduRate = sta_phyrx->psduRate;
      phy_rx_status->db_data->staPhyRxStatus[i].phyRateSynchedToPsduRate = sta_phyrx->phyRateSynchedToPsduRate;
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].noise,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].noise),
                  &sta_phyrx->noise,
                  sizeof(sta_phyrx->noise));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].gain,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].gain),
                  &sta_phyrx->gain,
                  sizeof(sta_phyrx->gain));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssi,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssi),
                  &sta_phyrx->rssi,
                  sizeof(sta_phyrx->rssi));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssiArray,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiArray),
                  &sta_phyrx->rssiArray,
                  sizeof(sta_phyrx->rssiArray));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].rssiAckAvarage),
                  &sta_phyrx->rssiAckAvarage,
                  sizeof(sta_phyrx->rssiAckAvarage));
      wave_memcpy(&phy_rx_status->db_data->staPhyRxStatus[i].evm,
                  sizeof(phy_rx_status->db_data->staPhyRxStatus[i].evm),
                  &sta_phyrx->evm,
                  sizeof(sta_phyrx->evm));
      MTLK_STATIC_ASSERT(ARRAY_SIZE(sta_phyrx->rssi) == WAVE_STAT_MAX_ANTENNAS); // no need to fill remaining elements
    }
  }

  return res;
}

int __MTLK_IFUNC
mtlk_hw_get_statistics (mtlk_hw_t *hw)
{
  mtlk_txmm_msg_t          man_msg;
  mtlk_txmm_data_t        *man_entry;
  UMI_GET_STATISTICS_DATA *req;
  hw_statistics_t         *hw_stats;
  uint32                   dma_addr;
  int                      res = MTLK_ERR_UNKNOWN, size = 0;
  int                      chip_id = hw_mmb_get_chip_id(hw);
  mtlk_osal_hr_timestamp_t ts;

  if (_chipid_is_gen6_b0 (chip_id)) {
      size = sizeof(StatisticsDb_t_wave600b);
  } else if (_chipid_is_gen7 (chip_id)) {
      size = sizeof(StatisticsDb_t_wave700);
  } else {
      size = sizeof(StatisticsDb_t_wave600d2);
  }
  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, &hw->txmm, NULL))) {
    ELOG_D("card_idx %d: GET_STATISTICS_REQ init failed", hw->card_idx);
    return MTLK_ERR_NO_RESOURCES;
  }

  hw_stats = &hw->hw_stats;

  dma_addr = mtlk_osal_map_to_phys_addr(mtlk_ccr_get_dev_ctx(hw->ccr),
                                        hw_stats->stats_data, size,
                                        MTLK_DATA_FROM_DEVICE);

  man_entry->id = UM_MAN_GET_STATISTICS_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_GET_STATISTICS_DATA *) man_entry->payload;
  req->ddrBufferAddress = HOST_TO_MAC32(dma_addr);
  req->length        = HOST_TO_MAC32(size);
  req->status         = UMI_OK;

  ILOG3_DDD("CID-%02x: Statistics data: size %d, dma_addr 0x%08x, ",
                hw->card_idx, size, dma_addr);
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);

  mtlk_osal_unmap_phys_addr(mtlk_ccr_get_dev_ctx(hw->ccr),
                            dma_addr, size,
                            MTLK_DATA_FROM_DEVICE);

  mtlk_txmm_msg_cleanup(&man_msg);

  if (res != MTLK_ERR_OK) {
    ELOG_DD("card_idx %d: Sending failed, err=%i", hw->card_idx, res);
  } else if (UMI_OK != req->status) {
    ELOG_D("Failed to retrieve Statistics, error %d", req->status);
    res = MTLK_ERR_MAC;
  }

  if (res == MTLK_ERR_OK) {
    ts = mtlk_osal_hr_timestamp_us();
    ILOG3_DDPD("CID-%02x: Statistics Data: size %d, buffer 0x%p, phys_addr 0x%08X",
               hw->card_idx, size,
               hw_stats->stats_data, virt_to_phys(hw_stats->stats_data));

    mtlk_osal_lock_acquire(&hw_stats->lock);
    hw_stats->last_call_timestamp = ts;
    res = _mtlk_hw_parse_statistics(hw);
    _wave_update_mu_he_eht_la_stats(hw);
    mtlk_osal_lock_release(&hw_stats->lock);

    /* Update HW related statistics */
    _mtlk_hw_mhi_update_hw_stats(hw, hw_stats);
  }
  return res;
}

static int _hw_init_phy_rx_status (mtlk_hw_t *hw);

int __MTLK_IFUNC
mtlk_hw_statistics_init (mtlk_hw_t *hw)
{
    hw_statistics_t      *hw_stats;
    uint32                dma_addr;
    int                   res, size = 0;
    int                   chip_id = hw_mmb_get_chip_id(hw);
    int                   i;
    int                   no_of_bands;
    mtlk_osal_hr_timestamp_t ts;

    if (_chipid_is_gen6_b0 (chip_id)) {
        size = sizeof(StatisticsDb_t_wave600b);
    } else if (_chipid_is_gen7(chip_id)) {
        size = sizeof(StatisticsDb_t_wave700);
    } else {
        size = sizeof(StatisticsDb_t_wave600d2);
    }

    if (_chipid_is_gen7(chip_id)) {
      no_of_bands = GEN7_NUM_OF_BANDS;
    } else {
      no_of_bands = GEN6_NUM_OF_BANDS;
    }

    hw_stats = &hw->hw_stats;
    memset(hw_stats, 0, sizeof(*hw_stats));
    memset(hw->chan_statistics, 0, sizeof(hw->chan_statistics));

    ts = mtlk_osal_hr_timestamp_us();
    hw_stats->last_call_timestamp = ts;
    hw->chan_statistics_ts = wave_hw_chan_stats_ts(hw);

    for (i = 0; i < NUM_TOTAL_CHAN_STATS_SIZE; i++) {
      hw->chan_statistics[i].chan_stat_ts = hw->chan_statistics_ts;
    }

    for (i = 0; i < no_of_bands; i++) {
      hw->chan_util_value[i] = 0;
    }

    res = mtlk_osal_lock_init(&hw_stats->lock);
    if (MTLK_ERR_OK != res) {
      return res;
    }

    res = mtlk_osal_lock_init(&hw_stats->stats_scan_lock);
    if (MTLK_ERR_OK != res) {
      mtlk_osal_lock_cleanup(&hw_stats->lock);
      return res;
    }

    hw_stats->stats_data = mtlk_osal_mem_dma_alloc(size, MTLK_MEM_TAG_EXTENSION);
    hw_stats->stats_copy = mtlk_osal_mem_alloc(size, MTLK_MEM_TAG_EXTENSION);
    hw_stats->stats_snapshot = mtlk_osal_mem_alloc(size, MTLK_MEM_TAG_EXTENSION);
    if (!hw_stats->stats_data || !hw_stats->stats_copy || !hw_stats->stats_snapshot) {
        ELOG_V("Can't allocate statistics buffers");
        res = MTLK_ERR_NO_MEM;
        goto finish;
    }

    memset(hw_stats->stats_data, 0, size);
    memset(hw_stats->stats_copy, 0, size);
    memset(hw_stats->stats_snapshot, 0, size);

    mtlk_hw_set_stats_available(hw, FALSE);

    res = _hw_init_phy_rx_status(hw);
    if (MTLK_ERR_OK != res) {
        goto finish;
    }

    /* Initialization test for map and unmap of statistics */
    dma_addr = mtlk_osal_map_to_phys_addr(mtlk_ccr_get_dev_ctx(hw->ccr), hw_stats->stats_data,
                                          size, MTLK_DATA_TO_DEVICE);

    ILOG0_DDPDP("CID-%02x: Statistics data: size %d, buffer 0x%p, dma_addr 0x%08x, buff_copy 0x%p",
                hw->card_idx, size, hw_stats->stats_data, dma_addr, hw_stats->stats_copy);
    if (dma_addr) {
        mtlk_osal_unmap_phys_addr(mtlk_ccr_get_dev_ctx(hw->ccr), dma_addr, size, MTLK_DATA_TO_DEVICE);
        res = MTLK_ERR_OK;
    } else {
        ELOG_V("Statistics DMA mapped addr is 0");
        res = MTLK_ERR_UNKNOWN;
        goto finish;
    }

finish:
    if (MTLK_ERR_OK != res) {
        mtlk_hw_statistics_cleanup(hw); /* free allocated buffers */
    }

    return res;
}

uint32 __MTLK_IFUNC
mtlk_hw_sta_stat_to_power (mtlk_hw_t *hw, unsigned radio_idx, uint32 stat_value, uint32 cbw)
{
  MTLK_ASSERT(hw);

  /* Gen6: stat_value in StatPowerUnits */
  stat_value = STAT_PW_TO_POWER(stat_value);

  return stat_value;
}
/*--- end of MHI_STATISTICS --*/

/*------ PHY_RX_STATUS ------*/

static int
_hw_init_phy_rx_status (mtlk_hw_t *hw)
{
    hw_phy_rx_status_t  *phy_rx_status;
    uint32               size;
    uint32               max_nof_sta;   /* maximal possiblle nof_STAs according to FW shared files */
    uint32               cur_nof_sta;   /* nof_STAs from CHI area, i.e. actual supported by FW */
    uint32               all_sta_sid;
    uint8                no_of_bands;
    uint32               rssi_offs, noise_offs;

    phy_rx_status = &hw->phy_rx_status;
    memset(phy_rx_status, 0, sizeof(*phy_rx_status));

    all_sta_sid = GEN6_ALL_STA_SID;
    max_nof_sta = GEN6_MAX_SID;
    rssi_offs   = GEN6_PHY_RSSI_TO_DBM;
    noise_offs  = GEN6_NOISE_OFFS;
    no_of_bands = GEN6_NUM_OF_BANDS;
#ifdef MTLK_WAVE_700
    if (_hw_type_is_gen7(hw)) {
      all_sta_sid = GEN7_ALL_STA_SID;
      max_nof_sta = GEN7_MAX_SID;
      no_of_bands = GEN7_NUM_OF_BANDS;
    }
#endif

    cur_nof_sta = hw->max_stas;
    if (cur_nof_sta > max_nof_sta) {
        ELOG_DD("Number of supported STAs over: (%d > %d)", cur_nof_sta, max_nof_sta);
        return MTLK_ERR_NOT_SUPPORTED;
    }

    /* allocate buffer for the max nof_devices also maximal nof_STAs */
    size = (no_of_bands * sizeof(devicePhyRxStatusDb_t)) +
           (max_nof_sta * sizeof(stationPhyRxStatusDb_t));

    phy_rx_status->db_size     = size; /* proc files were created */

    phy_rx_status->db_data = mtlk_osal_mem_alloc(sizeof(wholePhyRxStatusDb_t), MTLK_MEM_TAG_EXTENSION);
    if (NULL == phy_rx_status->db_data) {
        ELOG_V("Can't allocate PhyRxStatus buffer");
        return MTLK_ERR_NO_MEM;
    }
    phy_rx_status->db_data->staPhyRxStatus = mtlk_osal_mem_alloc((max_nof_sta * sizeof(stationPhyRxStatusDb_t)), MTLK_MEM_TAG_EXTENSION);
    if (NULL == phy_rx_status->db_data->staPhyRxStatus) {
        ELOG_V("Can't allocate Station PhyRxStatus buffer");
        return MTLK_ERR_NO_MEM;
    }
    phy_rx_status->db_data->devicePhyRxStatus = mtlk_osal_mem_alloc((no_of_bands * sizeof(devicePhyRxStatusDb_t)), MTLK_MEM_TAG_EXTENSION);
    if (NULL == phy_rx_status->db_data->devicePhyRxStatus) {
        ELOG_V("Can't allocate Device PhyRxStatus buffer");
        return MTLK_ERR_NO_MEM;
    }

    phy_rx_status->all_sta_sid = all_sta_sid;
    phy_rx_status->max_sid     = cur_nof_sta; /* limit max SID with actual nof_STAs */
    phy_rx_status->rssi_offs   = rssi_offs;
    phy_rx_status->noise_offs  = noise_offs;

    return MTLK_ERR_OK;
}

int __MTLK_IFUNC
mtlk_hw_copy_phy_rx_status (mtlk_hw_t *hw, uint8 *buff, uint32 *size)
{
    hw_phy_rx_status_t  *phy_rx_status;
    uint32               max_nof_sta;
    uint8                no_of_bands;
    uint32               staSize, devSize;
    MTLK_ASSERT(hw);
    MTLK_ASSERT(buff);
    MTLK_ASSERT(size);

    max_nof_sta = GEN6_MAX_SID;
    no_of_bands = GEN6_NUM_OF_BANDS;
#ifdef MTLK_WAVE_700
    if (_hw_type_is_gen7(hw)) {
      max_nof_sta = GEN7_MAX_SID;
      no_of_bands = GEN7_NUM_OF_BANDS;
    }
#endif

    devSize = (no_of_bands * sizeof(devicePhyRxStatusDb_t));
    staSize = (max_nof_sta * sizeof(stationPhyRxStatusDb_t));

    phy_rx_status = &hw->phy_rx_status;
    if (*size < phy_rx_status->db_size) {
        return MTLK_ERR_NO_MEM;
    }

    *size = phy_rx_status->db_size;
    wave_memcpy(buff, staSize, phy_rx_status->db_data->staPhyRxStatus, staSize);
    wave_memcpy(buff + staSize, devSize, phy_rx_status->db_data->devicePhyRxStatus, devSize);

    return MTLK_ERR_OK;
}

static devicePhyRxStatusDb_t *
_wave_hw_radio_get_dev_status (mtlk_hw_t *hw, uint32 radio_id)
{
    MTLK_ASSERT(radio_id < GEN7_NUM_OF_BANDS);
    return &hw->phy_rx_status.db_data->devicePhyRxStatus[radio_id];
}

static void
_wave_sta_link_adapt_ul_dl_ppdus_ofdma_stats (mtlk_hw_t *hw, sta_entry *sta)
{
   wave_mhi_la_ofdma_sta_pdu_stats_t *sta_ul_stats = NULL;
   wave_mhi_la_ofdma_sta_pdu_stats_t *sta_dl_stats = NULL;
   mtlk_mhi_stats64_sta_cntr_t *stats64;
   mtlk_mhi_stats64_sta_cntr_t *stats64_snapshot;
   uint8 ru_id, curr_bw, no_of_bw = 0, no_of_ru_sizes = 0;
   uint64 total_su_ul_counts = 0, total_su_dl_counts = 0;
   uint32 chip_id = hw_mmb_get_chip_id(hw);

   if (sta == NULL) {
     return;
   }

   if (_chipid_is_gen7(chip_id)) {
     no_of_bw = BW_MAX_G7;
     no_of_ru_sizes = WAVE700_NUM_OF_RU_SIZES;
   } else {
     no_of_bw = NUM_OF_BW;
     no_of_ru_sizes = WAVE600_NUM_OF_RU_SIZES;
   }

   sta_ul_stats = wave_sta_get_ul_ofdma_stats(sta);
   sta_dl_stats = wave_sta_get_dl_ofdma_stats(sta);
   stats64 = mtlk_sta_get_mhi_stat64_array(sta);
   stats64_snapshot = mtlk_sta_get_mhi_stat64_snapshot_array(sta);

   /* UL and DL OFDMA Calculations */
   sta_ul_stats->sta_ppdu.sta_ppdus_stats.total_ppdus    = stats64->ul_ppdus.total_ppdus - stats64_snapshot->ul_ppdus.total_ppdus;
   sta_dl_stats->sta_ppdu.sta_ppdus_stats.total_ppdus    = stats64->dl_ppdus.total_ppdus - stats64_snapshot->dl_ppdus.total_ppdus;
   for (curr_bw = 0; curr_bw < no_of_bw; curr_bw++) {
     sta_ul_stats->sta_ppdu.su_ppdus[curr_bw] = stats64->ul_ppdus.su_ppdus[curr_bw] - stats64_snapshot->ul_ppdus.su_ppdus[curr_bw];
     total_su_ul_counts += sta_ul_stats->sta_ppdu.su_ppdus[curr_bw];
     sta_dl_stats->sta_ppdu.su_ppdus[curr_bw] = stats64->dl_ppdus.su_ppdus[curr_bw] - stats64_snapshot->dl_ppdus.su_ppdus[curr_bw];
     total_su_dl_counts += sta_dl_stats->sta_ppdu.su_ppdus[curr_bw];
   }
   sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus = stats64->ul_ppdus.mu_ofdma_ppdus - stats64_snapshot->ul_ppdus.mu_ofdma_ppdus;
   sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus  = stats64->ul_ppdus.mu_mimo_ppdus - stats64_snapshot->ul_ppdus.mu_mimo_ppdus;
   sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus       = sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus + sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus;
   sta_ul_stats->sta_ppdu.sta_ppdus_stats.he_ppdus       = total_su_ul_counts + sta_ul_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus;
   sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus = stats64->dl_ppdus.mu_ofdma_ppdus - stats64_snapshot->dl_ppdus.mu_ofdma_ppdus;
   sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus  = stats64->dl_ppdus.mu_mimo_ppdus - stats64_snapshot->dl_ppdus.mu_mimo_ppdus;
   sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus       = sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ofdma_ppdus + sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_mimo_ppdus;
   sta_dl_stats->sta_ppdu.sta_ppdus_stats.he_ppdus       = total_su_dl_counts + sta_dl_stats->sta_ppdu.sta_ppdus_stats.mu_ppdus;
   for (ru_id = 0; ru_id < no_of_ru_sizes; ru_id ++) {
     sta_ul_stats->sta_ppdu.sta_ppdus_stats.ru_ppdu_stats[ru_id].mu_ru_ppdus = stats64->ul_ppdus.mu_ru_ppdus[ru_id] -
                                                                      stats64_snapshot->ul_ppdus.mu_ru_ppdus[ru_id];
     sta_dl_stats->sta_ppdu.sta_ppdus_stats.ru_ppdu_stats[ru_id].mu_ru_ppdus = stats64->dl_ppdus.mu_ru_ppdus[ru_id] -
                                                                      stats64_snapshot->dl_ppdus.mu_ru_ppdus[ru_id];
   /* DL MPDUS OFDMA stats */
   sta_dl_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_count = stats64->dl_mpdus.mu_mpdus[ru_id].success_mpdus -
                                                                       stats64_snapshot->dl_mpdus.mu_mpdus[ru_id].success_mpdus;
   sta_dl_stats->sta_mpdu.ru_mpdu_stats[ru_id].ru_mpdus_err_count = stats64->dl_mpdus.mu_mpdus[ru_id].failed_mpdus -
                                                                           stats64_snapshot->dl_mpdus.mu_mpdus[ru_id].failed_mpdus;
   }
}

void
hw_mac_update_ofdma_band_stats(mtlk_hw_t *hw, mtlk_core_t *core)
{
   wave_radio_pdus_ofdma_stats64_t *ul_radio_stats_64 = NULL;
   wave_radio_pdus_ofdma_stats64_t *dl_radio_stats_64 = NULL;
   wave_radio_pdus_ofdma_stats_t   *ul_radio_stats = NULL;
   wave_radio_pdus_ofdma_stats_t   *dl_radio_stats = NULL;
   /*HE-MIMO stats */
   wave_radio_mimo_stats64_t *dl_radio_mimo_stats_64 = NULL;
   wave_radio_mimo_stats_t   *dl_radio_mimo_stats = NULL;
   hw_statistics_t      *hw_stats;
   wave_radio_t *radio;
   uint32 ru_id, tmp_radio_stats = 0, tmp_su_stats = 0;
   uint8 cbw, radio_idx, nss, mcs;
   uint32 chip_id = hw_mmb_get_chip_id(hw);
   uint64 prev_mu_ru_ppdus = 0;
   hw_stats = &hw->hw_stats;
   radio = wave_vap_radio_get(core->vap_handle);
   radio_idx = wave_radio_id_get(radio);
   wave_hw_get_radio_ul_ofdma_stats_64(radio, &ul_radio_stats_64);
   wave_hw_get_radio_dl_ofdma_stats_64(radio, &dl_radio_stats_64);
   wave_hw_get_radio_ul_ofdma_stats(radio, &ul_radio_stats);
   wave_hw_get_radio_dl_ofdma_stats(radio, &dl_radio_stats);
   wave_get_radio_dl_mimo_stats_64(radio, &dl_radio_mimo_stats_64);
   wave_get_radio_dl_mimo_stats(radio, &dl_radio_mimo_stats);

   /* UL OFDMA radio ppdus stats */
   mtlk_osal_lock_acquire(&hw_stats->lock);
   if (_chipid_is_gen6_b0(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);

     for (ru_id = 0; ru_id < WAVE600_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &ul_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       ul_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
         ul_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
   } else if (_chipid_is_gen6_d2(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);

     for (ru_id = 0; ru_id < WAVE600_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &ul_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       ul_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
         ul_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
   } else if (_chipid_is_gen7(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &ul_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);

     for (ru_id = 0; ru_id < WAVE700_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &ul_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       ul_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += ul_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       for (cbw = 0; cbw < BW_MAX_G7; cbw++) {
         ul_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusUl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
   }

   /* DL OFDMA radio ppdus stats */
   if (_chipid_is_gen6_b0(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);

     /* DL MPDUS SU OFDMA radio stats */
     for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
       ru_id = __wave_get_ruid_from_bw(cbw);
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].successMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count;
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].failedMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count;
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);
     }
     /*DL MPDUS SU OFDMA radio stats end */
     for (ru_id = 0; ru_id < WAVE600_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &dl_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       dl_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       /* DL MPDUS MU OFDMA stats */
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].successMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].success_mpdus_count,
                                      tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].failedMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].failed_mpdus_count,
                                      tmp_radio_stats);
       /*DL MPDUS MU OFDMA stats end */
       for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
         dl_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
     /* DL HE-MIMO stats */
     for (nss = 0; nss < REDUCED_SPATIAL_STREAMS_SIZE; nss++) {
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].nssPpdus[nss]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.nss_ppdus[nss],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.nss_ppdus[nss],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].nssPpdus[nss]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.nss_ppdus[nss],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.nss_ppdus[nss],
                                     tmp_radio_stats);
     }
     for (mcs = 0; mcs < REDUCED_MCS_SIZE; mcs++) {
       tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.mcs_ppdus[mcs],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.mcs_ppdus[mcs],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].attemptedSoundingPerMCS[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     tmp_radio_stats);
     }
     tmp_radio_stats = MTLK_DIFF_STATS_B0(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].succeededSounding);
     __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.succeeded_sounding,
                                    &dl_radio_mimo_stats->mu_mimo.succeeded_sounding,
                                    tmp_radio_stats);
   } else if (_chipid_is_gen6_d2(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);
     /* DL MPDUS SU OFDMA radio stats */
     for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
       ru_id = __wave_get_ruid_from_bw(cbw);
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].successMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count;
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].failedMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count;

       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);
     }
     /*DL MPDUS SU OFDMA radio stats end */
     for (ru_id = 0; ru_id < WAVE600_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &dl_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       dl_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       /* DL MPDUS OFDMA stats */
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].successMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].success_mpdus_count,
                                      tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].failedMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].failed_mpdus_count,
                                      tmp_radio_stats);
       /*DL MPDUS OFDMA stats end */
       for (cbw = 0; cbw < NUM_OF_BW; cbw++) {
         dl_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
     /* DL HE-MIMO stats */
     for (nss = 0; nss < REDUCED_SPATIAL_STREAMS_SIZE; nss++) {
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].nssPpdus[nss]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.nss_ppdus[nss],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.nss_ppdus[nss],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].nssPpdus[nss]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.nss_ppdus[nss],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.nss_ppdus[nss],
                                     tmp_radio_stats);
     }
     for (mcs = 0; mcs < REDUCED_MCS_SIZE; mcs++) {
       tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.mcs_ppdus[mcs],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.mcs_ppdus[mcs],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].attemptedSoundingPerMCS[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     tmp_radio_stats);
     }
     tmp_radio_stats = MTLK_DIFF_STATS_D2(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].succeededSounding);
     __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.succeeded_sounding,
                                    &dl_radio_mimo_stats->mu_mimo.succeeded_sounding,
                                    tmp_radio_stats);
   } else if (_chipid_is_gen7(chip_id)) {
     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].totalPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.total_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.total_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].suPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.su_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.su_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muOfdmaPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ofdma_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_ofdma_ppdus_count,
                                    tmp_radio_stats);

     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muMimoPpdusCount);
     __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_mimo_ppdus_count,
                                    &dl_radio_stats->ppdu_stats.mu_mimo_ppdus_count,
                                    tmp_radio_stats);
     /* DL MPDUS SU OFDMA radio stats */
     for (cbw = 0; cbw < BW_MAX_G7; cbw++) {
       ru_id = __wave_get_ruid_from_bw(cbw);
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].successMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count;
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].success_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].suBandMpdus[cbw].failedMpdus);
       tmp_su_stats = dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count;
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.su_mpdu_stats64[cbw].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.su_mpdu_stats[cbw].failed_mpdus_count,
                                      tmp_radio_stats);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &tmp_su_stats, tmp_radio_stats);
     }
     /*DL MPDUS SU OFDMA radio stats end */
     for (ru_id = 0; ru_id < WAVE700_NUM_OF_RU_SIZES; ru_id++) {
       prev_mu_ru_ppdus = dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id];
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].muRuPpdusCount[ru_id]);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id],
                                      &dl_radio_stats->ppdu_stats.mu_ru_ppdus[ru_id],
                                      tmp_radio_stats);
       dl_radio_stats_64->ppdu_stats64.total_mu_ru_ppdus += dl_radio_stats_64->ppdu_stats64.mu_ru_ppdus[ru_id] -  prev_mu_ru_ppdus;
       /* DL MPDUS OFDMA stats */
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].successMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].success_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].success_mpdus_count,
                                      tmp_radio_stats);

       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.mpdusDl.bandMpdus[radio_idx].muRuMpdus[ru_id].failedMpdus);
       __reset_stats_accumulate_64bit(&dl_radio_stats_64->mpdu_stats64.mu_mpdu_stats64[ru_id].failed_mpdus_count,
                                      &dl_radio_stats->mpdu_stats.mu_mpdu_stats[ru_id].failed_mpdus_count,
                                      tmp_radio_stats);
       /*DL MPDUS OFDMA stats end */
       for (cbw = 0; cbw < BW_MAX_G7; cbw++) {
         dl_radio_stats_64->ppdu_stats64.bw_ru_in_ppdus[cbw][ru_id] = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                                         linkAdaptationSuMuStatistics.ppdusDl.bandsPpdus[radio_idx].bwRusInPpdus[cbw][ru_id]);
       }
     }
     /* DL HE-MIMO stats */
     for (nss = 0; nss < REDUCED_SPATIAL_STREAMS_SIZE_GEN7; nss++) {
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].nssPpdus[nss]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.nss_ppdus[nss],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.nss_ppdus[nss],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].nssPpdus[nss]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.nss_ppdus[nss],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.nss_ppdus[nss],
                                     tmp_radio_stats);
     }
     for (mcs = 0; mcs < REDUCED_MCS_SIZE_GEN7; mcs++) {
       tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                            linkAdaptationSuMuStatistics.suRateDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
       __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->su_mimo.su_rate.mcs_ppdus[mcs],
                                      &dl_radio_mimo_stats->su_mimo.su_rate.mcs_ppdus[mcs],
                                      tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].mcsPpdus[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.mu_rate.mcs_ppdus[mcs],
                                     tmp_radio_stats);

      tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                           linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].attemptedSoundingPerMCS[mcs]);
      __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     &dl_radio_mimo_stats->mu_mimo.attemptedsounding_per_mcs[mcs],
                                     tmp_radio_stats);
     }
     tmp_radio_stats = MTLK_DIFF_STATS_G7(hw_stats->stats_copy, hw_stats->stats_snapshot,
                                          linkAdaptationSuMuStatistics.mimoDl.bandStatistics[radio_idx].succeededSounding);
     __reset_stats_accumulate_64bit(&dl_radio_mimo_stats_64->mu_mimo.succeeded_sounding,
                                    &dl_radio_mimo_stats->mu_mimo.succeeded_sounding,
                                    tmp_radio_stats);

   }
   mtlk_osal_lock_release(&hw_stats->lock);
}

static void
_wave_sta_link_adapt_ofdma_stats(mtlk_hw_t *hw, sta_entry *sta)
{
    _mtlk_sta_link_adapt_ofdma_count_stats(hw, sta);
    _wave_sta_link_adapt_ul_dl_ppdus_ofdma_stats(hw, sta);
}

/* Update TX/RX statistics for all STAs */
void
hw_mac_update_peers_stats (mtlk_hw_t *hw, mtlk_core_t *core)
{
    wholePhyRxStatusDb_t *phy_db = NULL;
    sta_db               *sta_db = NULL;
    const sta_entry      *sta = NULL;
    uint32                sta_sid;

    mtlk_stadb_iterator_t iter;

    ILOG3_D("CID-%04x", mtlk_vap_get_oid(core->vap_handle));

    phy_db = hw->phy_rx_status.db_data;
    MTLK_ASSERT(phy_db != NULL);

    sta_db = mtlk_core_get_stadb(core);
    sta = mtlk_stadb_iterate_first(sta_db, &iter);
    if (sta) {
      do {
        sta_sid = mtlk_sta_get_sid(sta);
        if (mtlk_hw_is_sid_valid(hw, sta_sid)) {
            stationPhyRxStatusDb_t *sta_status = &phy_db->staPhyRxStatus[sta_sid];

            mtlk_sta_update_phy_info((sta_entry *)sta, hw, sta_status);

            _mtlk_hw_mhi_sta_fill_tx_rate_statistic(hw, mtlk_sta_get_mhi_tx_stats((sta_entry *)sta), sta_sid);

            _mtlk_sta_update_statistics(hw, (sta_entry *)sta, sta_status);

            _wave_sta_link_adapt_ofdma_stats(hw, (sta_entry *)sta);

            mtlk_sta_update_mhi_peers_stats((sta_entry *)sta);

            _wave_update_mhi_vap_PDStats(hw, (sta_entry *)sta);
        }

        sta = mtlk_stadb_iterate_next(&iter);
      } while (sta);
      mtlk_stadb_iterate_done(&iter);
    }
}

void __MTLK_IFUNC
wave_hw_copy_chan_stats (mtlk_hw_t *hw, unsigned idx, devicePhyRxStatusDb_t *stats_buff)
{
  devicePhyRxStatusDb_t *dev_status = _wave_hw_radio_get_dev_status(hw, idx);
  wave_memcpy(stats_buff, sizeof(*stats_buff), dev_status, sizeof(*dev_status));
}

void get_channel_data_hw_save_chan_stats_info(mtlk_core_t *core, struct intel_vendor_channel_data *ch_data)
{
  wave_radio_t *radio = wave_vap_radio_get(core->vap_handle);
  mtlk_hw_t    *hw = wave_radio_get_hw(radio);
  unsigned      radio_idx = wave_radio_id_get(radio);

  memset(ch_data, 0, sizeof(*ch_data));
  ch_data->channel = WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_CHANNEL_CUR);
  ch_data->cwi_noise = hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].CWIvalue;
}

void scan_get_aocs_info (mtlk_core_t *core, struct intel_vendor_channel_data *ch_data)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  unsigned radio_idx;
  hw_statistics_t    *hw_stats;
  int chip_id = hw_mmb_get_chip_id(hw);
  unsigned idx;

  hw_stats = &hw->hw_stats;
  radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  mtlk_osal_lock_acquire(&hw_stats->lock);
  ch_data->dynBW20             = MTLK_GET_STATS(hw_stats->stats_copy, dynamicBwStats.dynamicBW20MHz[radio_idx], chip_id);
  ch_data->dynBW40             = MTLK_GET_STATS(hw_stats->stats_copy, dynamicBwStats.dynamicBW40MHz[radio_idx], chip_id);
  ch_data->dynBW80             = MTLK_GET_STATS(hw_stats->stats_copy, dynamicBwStats.dynamicBW80MHz[radio_idx], chip_id);
  ch_data->dynBW160            = MTLK_GET_STATS(hw_stats->stats_copy, dynamicBwStats.dynamicBW160MHz[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);

  ch_data->ext_sta_rx          = hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].extStaRx;
  ch_data->tx_power            = hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].txPower;
  ch_data->cwi_noise           = hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].CWIvalue;
  ch_data->busy_time           = ((uint32)(hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].channel_load)) * 255 / 100;
  ch_data->load                = hw->phy_rx_status.db_data->devicePhyRxStatus[radio_idx].channel_load;

  ch_data->filled_mask = CHDATA_DYNBW | CHDATA_CWI_NOISE | CHDATA_TX_POWER |
                         CHDATA_LOAD | CHDATA_BUSY_TIME | CHDATA_LOW_RSSI;

  idx = compute_stat_index(wave_vap_radio_get(core->vap_handle), ch_data->channel);
  if (idx != CHAN_IDX_ILLEGAL) {
    ch_data->rssi = mtlk_hw_get_channel_rssi(mtlk_vap_get_hw(core->vap_handle), idx);
    ch_data->filled_mask |= CHDATA_RSSI;
    if (ch_data->rssi != MIN_RSSI){
      ch_data->snr = (uint8) (ch_data->rssi - ch_data->cwi_noise);
      ch_data->filled_mask |= CHDATA_SNR;
    }
  }

  ILOG2_DDDDDDDDDD("debug AOCS_info: %d %d %d %d %d %d %d %d %d %d",
    ch_data->dynBW20, ch_data->dynBW40, ch_data->dynBW80, ch_data->dynBW160, ch_data->ext_sta_rx, ch_data->tx_power,
    ch_data->rssi, ch_data->snr, ch_data->cwi_noise, ch_data->busy_time);
}

void __MTLK_IFUNC
wave_reset_chan_stats (mtlk_hw_t *hw, unsigned idx)
{
  wave_drv_channel_stats_t  *drv_cs  = &hw->chan_statistics[idx];
  wifi_channelStats_t       *wifi_cs = &hw->chan_statistics[idx].wifi_chan_stats;

  if (CHAN_IDX_ILLEGAL == idx) {
    ELOG_D("Illegal channel index %u", idx);
    return;
  }

  wifi_cs->ch_utilization           = 0;
  wifi_cs->ch_utilization_total     = 0;
  wifi_cs->ch_utilization_busy      = 0;
  wifi_cs->ch_utilization_busy_tx   = 0;
  wifi_cs->ch_utilization_busy_rx   = 0;
  wifi_cs->ch_utilization_busy_self = 0;
  wifi_cs->ch_utilization_busy_ext  = 0;
  wifi_cs->ch_noise                 = MIN_NOISE;
  wifi_cs->ch_non_80211_noise       = MIN_NOISE;
  wifi_cs->ch_max_80211_rssi        = MIN_RSSI;

  drv_cs->chan_stat_ts = hw->chan_statistics_ts;
  drv_cs->chan_active_time = 0;
  drv_cs->chan_util_interf = 0;
  drv_cs->chan_idle_time = 0;
}

void __MTLK_IFUNC wave_hw_reset_chan_statistics (mtlk_hw_t *hw)
{
  wave_radio_descr_t *radio_descr;
  unsigned radio_idx;

  MTLK_ASSERT(hw);

  radio_descr = wave_card_radio_descr_get(hw);
  for (radio_idx = 0; radio_idx < radio_descr->num_radios; radio_idx++)
    wave_radio_reset_chan_statistics(hw, 0, radio_idx, TRUE);
}

void __MTLK_IFUNC wave_radio_reset_chan_statistics (mtlk_hw_t *hw, int ch_num, uint8 radio_id, BOOL full_reset)
{
  hw_statistics_t  *hw_stats;
  wave_radio_t *radio;
  unsigned idx;

  MTLK_ASSERT(NULL != hw);

  radio = __mtlk_hw_wave_radio_get(hw, radio_id);
  hw_stats = &hw->hw_stats;
  idx = compute_stat_index(radio, ch_num);

  mtlk_osal_lock_acquire(&hw_stats->lock);
  if (full_reset) {
    hw->chan_statistics_ts = wave_hw_chan_stats_ts(hw);
    wave_mac80211_reset_all_chan_stats(hw, wave_radio_band_get(radio));
    /* Reset also the Radio CCA stats */
    wave_radio_cca_stat_reset(radio);
  } else if (idx != CHAN_IDX_ILLEGAL) {
    hw->chan_statistics[idx].chan_stat_ts = hw->chan_statistics_ts;
  }
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
mtlk_hw_save_chan_statistics_info (mtlk_hw_t *hw, struct mtlk_chan_def *ccd, struct intel_vendor_channel_data *ch_data,
                                   BOOL ch_radar_noise, uint8 radio_id)
{
  hw_statistics_t    *hw_stats;
  int chip_id = hw_mmb_get_chip_id(hw);
  mtlk_hw_band_e band = ccd->chan.band;
  unsigned idx;

  MTLK_ASSERT((MTLK_HW_BAND_2_4_GHZ == band) || (MTLK_HW_BAND_5_2_GHZ == band) || (MTLK_HW_BAND_6_GHZ == band));

  if (!(MTLK_HW_BAND_2_4_GHZ == band) &&
      !(MTLK_HW_BAND_5_2_GHZ == band) &&
      !(MTLK_HW_BAND_6_GHZ == band)) {
    ELOG_D("Can't save statistics for unsupported band %d", band);
    return;
  }

  idx = compute_stat_index(__mtlk_hw_wave_radio_get(hw, radio_id), ch_data->channel);
  if (idx != CHAN_IDX_ILLEGAL) {
    wave_drv_channel_stats_t  *drv_cs  = &hw->chan_statistics[idx];
    wifi_channelStats_t       *wifi_cs = &drv_cs->wifi_chan_stats;

    hw_stats = &hw->hw_stats;
    mtlk_osal_lock_acquire(&hw_stats->lock);
    wifi_cs->ch_number                 = ch_data->channel;
    wifi_cs->ch_noise                  = ch_data->cwi_noise;
    wifi_cs->ch_radar_noise            = ch_radar_noise;

    wifi_cs->ch_utilization_busy      += MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationBusy, chip_id);
    wifi_cs->ch_utilization_busy_tx   += MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationBusyTx, chip_id);
    wifi_cs->ch_utilization_busy_rx   += MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationBusyRx, chip_id);
    wifi_cs->ch_utilization_busy_self += MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationBusyRxSelf, chip_id);
    wifi_cs->ch_utilization_busy_ext  += MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationBusyExt, chip_id);
    wifi_cs->ch_non_80211_noise        = MTLK_GET_STATS(hw_stats->stats_copy, phyStatistics.devicePhyRxStatus[radio_id].CWIvalue, chip_id);
    /* FW statistics chUtilization is only for the Channel Utilization parameter of the BSS Load element */
    hw->chan_util_value[radio_id]              = MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilization, chip_id);

    drv_cs->chan_util_interf           = wifi_cs->ch_utilization_busy - wifi_cs->ch_utilization_busy_tx - wifi_cs->ch_utilization_busy_self;

#ifdef WAVE_HW_CHAN_STATS_TSF_ENABLE
    hw->chan_statistics_ts             = MTLK_GET_STATS(hw_stats->stats_copy, currentChannelStats.channelUtilizationStats[radio_id].chUtilizationCurrTime, chip_id);
#else
    hw->chan_statistics_ts             = mtlk_osal_hr_timestamp_us();
#endif

    drv_cs->chan_active_time          += (hw->chan_statistics_ts - drv_cs->chan_stat_ts);
    drv_cs->chan_stat_ts               = hw->chan_statistics_ts;
    drv_cs->chan_idle_time             = drv_cs->chan_active_time - wifi_cs->ch_utilization_busy;

    /* Although RDK-B header names this field as 'total', they expect 'active' time to be provided in this field, instead of:
     * wifi_cs->ch_utilization_total   = wifi_cs->ch_utilization_busy_tx + wifi_cs->ch_utilization_busy_rx; */
    wifi_cs->ch_utilization_total      = drv_cs->chan_active_time;

    /* Channel utilization in percents */
    wifi_cs->ch_utilization            = WAVE_GET_PERCENTAGE(wifi_cs->ch_utilization_busy, drv_cs->chan_active_time);

    mtlk_osal_lock_release(&hw_stats->lock);
  }
}

void __MTLK_IFUNC
mtlk_hw_copy_stats_per_channel (wave_radio_t *radio, wifi_channelStats_t *chan_stats, int ch_num)
{
  unsigned idx;
  mtlk_hw_t *hw = wave_radio_get_hw(radio);
  idx = compute_stat_index(radio, ch_num);
  if (idx != CHAN_IDX_ILLEGAL) {
    *chan_stats = hw->chan_statistics[idx].wifi_chan_stats; /* struct copy */
    chan_stats->ch_number = ch_num; /* could be unfilled in hw stats */
  }
}

void __MTLK_IFUNC
mtlk_hw_copy_drv_stats_per_channel (wave_radio_t *radio, wave_drv_channel_stats_t *chan_stats, int ch_num)
{
  unsigned idx;
  mtlk_hw_t *hw = wave_radio_get_hw(radio);
  idx = compute_stat_index(radio, ch_num);
  if (idx != CHAN_IDX_ILLEGAL) {
    *chan_stats = hw->chan_statistics[idx]; /* struct copy */
    chan_stats->wifi_chan_stats.ch_number = ch_num; /* could be unfilled in hw stats */
  }
}

void __MTLK_IFUNC
mtlk_hw_get_rx_packets_and_bytes(mtlk_hw_t *hw, uint16 sta_sid, struct intel_vendor_unconnected_sta *sta_res_data)
{
  hw_statistics_t *hw_stats;
  uint16           chip_id = hw_mmb_get_chip_id(hw);

  hw_stats = &hw->hw_stats;

  if (mtlk_hw_is_sid_valid(hw, sta_sid)) {
    mtlk_osal_lock_acquire(&hw_stats->lock);
    if (_chipid_is_gen6_b0(chip_id)) {
      sta_res_data->rx_packets = MTLK_GET_STATS_B0(hw_stats->stats_copy, rxCounters.rdCount[sta_sid]);
    } else if (_chipid_is_gen6_d2(chip_id)) {
      sta_res_data->rx_packets = MTLK_GET_STATS_D2(hw_stats->stats_copy, RxCoordinatorCounters.rdCount[sta_sid]);
    } else {
      sta_res_data->rx_packets = MTLK_GET_STATS_G7(hw_stats->stats_copy, RxCoordinatorCounters.rdCount[sta_sid]);
    }

    sta_res_data->rx_bytes   = MTLK_GET_STATS(hw_stats->stats_copy, hostIfCounters.staCounts[sta_sid].rxOutStaNumOfBytes, chip_id);
    mtlk_osal_lock_release(&hw_stats->lock);
  }
}

void __MTLK_IFUNC
mtlk_hw_get_priority_gpio_stats (mtlk_hw_t *hw, mtlk_priority_gpio_stats_t *pta_stats)
{
  hw_statistics_t    *hw_stats;
  uint16              chip_id = hw_mmb_get_chip_id(hw);

  MTLK_ASSERT(hw != NULL);

  hw_stats = &hw->hw_stats;
  mtlk_osal_lock_acquire(&hw_stats->lock);
  pta_stats->zigbeeStarvationTimerExpired = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.zigbeeStarvationTimerExpired, chip_id);
  pta_stats->btStarvationTimerExpired = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.btStarvationTimerExpired, chip_id);
  pta_stats->zigbeeDenyOverTime       = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.zigbeeDenyOverTime, chip_id);
  pta_stats->btDenyOverTime           = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.btDenyOverTime, chip_id);
  pta_stats->gpio0TxDemandLow         = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.gpio0TxDemandLow, chip_id);
  pta_stats->zigbeeGrnatNotUsed       = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.zigbeeGrnatNotUsed, chip_id);
  pta_stats->btGrnatNotUsed           = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.btGrnatNotUsed, chip_id);
  pta_stats->zigbeeGrnatUsed          = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.zigbeeGrnatUsed, chip_id);
  pta_stats->btGrnatUsed              = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.btGrnatUsed, chip_id);
  pta_stats->zigbeeTxOngoing          = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.zigbeeTxOngoing, chip_id);
  pta_stats->btTxOngoing              = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.btTxOngoing, chip_id);
  pta_stats->gpio                     = MTLK_GET_STATS(hw_stats->stats_data,  ptaStats.gpio, chip_id);
  pta_stats->priorityIsHigh           = MTLK_GET_STATS(hw_stats->stats_copy,  ptaStats.priorityIsHigh, chip_id);
  pta_stats->priorityIsLow            = MTLK_GET_STATS(hw_stats->stats_copy,  ptaStats.priorityIsLow, chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_hw_cca_stats_get (wave_radio_t *radio, int channel, wave_cca_stats_t *cca_stats)
{
  unsigned idx;
  hw_statistics_t *hw_stats;
  mtlk_hw_t *hw = wave_radio_get_hw(radio);

  MTLK_ASSERT(NULL != cca_stats);

  idx = compute_stat_index(radio, channel);
  if (idx != CHAN_IDX_ILLEGAL) {
    wave_drv_channel_stats_t  *drv_ch_stat  = &hw->chan_statistics[idx];
    wifi_channelStats_t       *wifi_ch_stat = &drv_ch_stat->wifi_chan_stats;
    hw_stats = &hw->hw_stats;
    mtlk_osal_lock_acquire(&hw_stats->lock);
    cca_stats->ccaActive = drv_ch_stat->chan_active_time;
    cca_stats->ccaIntf   = drv_ch_stat->chan_util_interf;
    cca_stats->ccaTx     = wifi_ch_stat->ch_utilization_busy_tx;
    cca_stats->ccaRx     = wifi_ch_stat->ch_utilization_busy_self;
    mtlk_osal_lock_release(&hw_stats->lock);
  }
}

void __MTLK_IFUNC
wave_hw_radio_usage_stats_get (mtlk_hw_t *hw, int radio_id, int channel, wave_radio_usage_stats_t *user_radio_stats)
{
  hw_statistics_t *hw_stats;
  unsigned idx;

  MTLK_ASSERT(NULL != hw);
  MTLK_ASSERT(NULL != user_radio_stats);
  MTLK_ASSERT(radio_id < GEN7_NUM_OF_BANDS);

  idx = compute_stat_index(__mtlk_hw_wave_radio_get(hw, radio_id), channel);
  if (idx != CHAN_IDX_ILLEGAL) {
    wave_radio_usage_stats_internal_t  drv_radio_stats;
    wave_drv_channel_stats_t          *drv_chan_stats   = &hw->chan_statistics[idx];
    wifi_channelStats_t                *wifi_chan_stats = &drv_chan_stats->wifi_chan_stats;
    uint64 value;

    hw_stats = &hw->hw_stats;
    mtlk_osal_lock_acquire(&hw_stats->lock);
    /* Read actual values */
    drv_radio_stats.busy_self       = wifi_chan_stats->ch_utilization_busy_self;
    drv_radio_stats.busy_rx         = wifi_chan_stats->ch_utilization_busy_rx;
    drv_radio_stats.busy_tx         = wifi_chan_stats->ch_utilization_busy_tx;
    drv_radio_stats.busy            = wifi_chan_stats->ch_utilization_busy;
    drv_radio_stats.busy_ext        = wifi_chan_stats->ch_utilization_busy_ext;
    drv_radio_stats.interfer        = drv_chan_stats->chan_util_interf;
    drv_radio_stats.time_on_channel = drv_chan_stats->chan_active_time;
    drv_radio_stats.idle            = drv_chan_stats->chan_idle_time;

#define _CALC_AND_STORE_(item) \
    value = drv_radio_stats.item ;   /* value in microseconds */ \
    do_div(value, 1000ul);           /* microseconds to milliseconds */ \
    user_radio_stats->item = value;  /* save to the user context */

    _CALC_AND_STORE_(time_on_channel);
    _CALC_AND_STORE_(busy);
    _CALC_AND_STORE_(busy_tx);
    _CALC_AND_STORE_(busy_rx);
    _CALC_AND_STORE_(busy_self);
    _CALC_AND_STORE_(interfer);
    _CALC_AND_STORE_(idle);
    _CALC_AND_STORE_(busy_ext);
#undef  _CALC_AND_STORE_
    mtlk_osal_lock_release(&hw_stats->lock);
  }
}

void __MTLK_IFUNC
mtlk_hw_get_avg_rssi (wave_rssi_avg_info_t *rssi_info, int8 *data_rssi)
{
  int i, abs_value;
  uint64 a = 0, b = 0;
  uint64 rssi_linear_avg = 0, smoothed_signal = 0, round_off = 0;
  uint32 average_count = 0;

  for (i=0; i<WAVE_STAT_MAX_ANTENNAS; i++) {
     abs_value = abs(data_rssi[i]);
     if (abs_value >= ARRAY_SIZE(dbm_linear_lup_table))
       continue;
     rssi_linear_avg += dbm_linear_lup_table[abs_value];
     average_count ++;
  }
  if (average_count == 0)
    return;

  do_div(rssi_linear_avg, average_count);
  rssi_info->avg_sample = rssi_linear_avg;

 if (rssi_info->last_sample == 0) {
   rssi_info->last_sample = rssi_info->avg_sample;
   smoothed_signal = rssi_info->avg_sample;
 } else {
   do_div(rssi_info->avg_sample, 10); /* for alpha 0.1 co-efficient */
   a = rssi_info->avg_sample;
   do_div(rssi_info->last_sample, 10); /* for 1 - alpha co-efficient */
   b = rssi_info->last_sample * 9;
   smoothed_signal = a + b;
   rssi_info->last_sample = smoothed_signal;
 }

 for (i=1; i<100; i++) { /* loop for 0 to -99dBM */
     if (smoothed_signal >= dbm_linear_lup_table[i]) {
        /* Round off, either to the previous index or next index */
       round_off = (dbm_linear_lup_table[i] + dbm_linear_lup_table[i-1]) / 2;
       if (smoothed_signal > round_off)
         rssi_info->avg_rssi_dbm = -(i);
       else
         rssi_info->avg_rssi_dbm = -(i - 1);
       break;
     }
  }
}

void __MTLK_IFUNC
wave_hw_stats_scan_lock_acquire(mtlk_hw_t *hw)
{
  hw_statistics_t *hw_stats;

  MTLK_ASSERT(hw);
  hw_stats = &hw->hw_stats;
  mtlk_osal_lock_acquire(&hw_stats->stats_scan_lock);
}

void __MTLK_IFUNC
wave_hw_stats_scan_lock_release(mtlk_hw_t *hw)
{
  hw_statistics_t *hw_stats;

  MTLK_ASSERT(hw);
  hw_stats = &hw->hw_stats;
  mtlk_osal_lock_release(&hw_stats->stats_scan_lock);
}

#ifdef CONFIG_WAVE_DEBUG

void __MTLK_IFUNC
wave_core_get_radio_rx_stats (mtlk_core_t *core, mtlk_wssa_drv_radio_rx_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->barMpduCount = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, rxCounters.bandCounts[radio_idx].barMpduCount, chip_id);
  stats->crcErrorCount = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, rxCounters.bandCounts[radio_idx].crcErrorCount, chip_id);
  stats->delCrcError = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, rxCounters.bandCounts[radio_idx].delCrcError, chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}
void __MTLK_IFUNC
wave_core_get_radio_baa_stats (mtlk_core_t *core, mtlk_wssa_drv_radio_baa_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->retryCount = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.retryCounts[radio_idx].retryCount , chip_id);
  stats->multipleRetryCount = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.retryCounts[radio_idx].multipleRetryCount , chip_id);
  stats->fwMngmntframesSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwMngmntframesSent[radio_idx], chip_id);
  stats->fwMngmntFramesConfirmed = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwMngmntFramesConfirmed[radio_idx], chip_id);
  stats->fwMngmntframesRecieved = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwMngmntframesRecieved[radio_idx], chip_id);
  stats->fwctrlFramesSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwctrlFramesSent[radio_idx], chip_id);
  stats->fwctrlFramesRecieved = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwctrlFramesRecieved[radio_idx], chip_id);
  stats->fwMulticastReplayedPackets = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwMulticastReplayedPackets[radio_idx], chip_id);
  stats->fwPairWiseMicFailurePackets = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.fwPairWiseMicFailurePackets[radio_idx], chip_id);
  stats->groupMicFailurePackets = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.groupMicFailurePackets[radio_idx], chip_id);
  stats->beaconProbeResponsePhyType = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.beaconProbeResponsePhyType[radio_idx], chip_id);
  stats->rxBasicReport = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.rxBasicReport[radio_idx], chip_id);
  stats->txFrameErrorCount = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, baaCounters.txFrameErrorCount[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_tsman_init_tid_gl_stats (mtlk_core_t *core, mtlk_wssa_drv_tsman_init_tid_gl_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->InitTidStatsEventAddbaRequestWasSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaRequestWasSent[radio_idx], chip_id);
  stats->InitTidStatsEventAddbaRequestConfiremd = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaRequestConfiremd[radio_idx], chip_id);
  stats->InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndValid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndValid[radio_idx], chip_id);
  stats->InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndInvalid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusSuccessAndInvalid[radio_idx], chip_id);
  stats->InitTidStatsEventAddbaResponseReceivedWithStatusFailure = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseReceivedWithStatusFailure[radio_idx], chip_id);
  stats->InitTidStatsEventAddbaResponseTimeout = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventAddbaResponseTimeout[radio_idx], chip_id);
  stats->InitTidStatsEventBaAgreementOpened = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBaAgreementOpened[radio_idx], chip_id);
  stats->InitTidStatsEventBarWasSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasSent[radio_idx], chip_id);
  stats->InitTidStatsEventBarWasDiscarded = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasDiscarded[radio_idx], chip_id);
  stats->InitTidStatsEventBarWasAnsweredWithAck = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasAnsweredWithAck[radio_idx], chip_id);
  stats->InitTidStatsEventBarWasAnsweredWithBa = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventBarWasAnsweredWithBa[radio_idx], chip_id);
  stats->InitTidStatsEventInactivityTimeout = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventInactivityTimeout[radio_idx], chip_id);
  stats->InitTidStatsEventDelbaWasSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventDelbaWasSent[radio_idx], chip_id);
  stats->InitTidStatsEventDelbaReceived = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventDelbaReceived[radio_idx], chip_id);
  stats->InitTidStatsEventCloseTid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventCloseTid[radio_idx], chip_id);
  stats->InitTidStatsEventOpenTid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorTidGlobalStats.InitiatorTidStatisticsEventOpenTid[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_tsman_init_sta_gl_stats (mtlk_core_t *core, mtlk_wssa_drv_tsman_init_sta_gl_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->InitStaStatsEventRemove = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventRemove[radio_idx], chip_id);
  stats->InitStaStatsEventAdd = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventAdd[radio_idx], chip_id);
  stats->InitStaStatsEventAddbaResponseReceivedWithIllegalTid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerInitiatorStationGlobalStats.InitiatorStationStatisticsEventAddbaResponseReceivedWithIllegalTid[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_tsman_rcpt_tid_gl_stats (mtlk_core_t *core, mtlk_wssa_drv_tsman_rcpt_tid_gl_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->RcpTidStatsEventAddbaRequestAccepted = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaRequestAccepted[radio_idx], chip_id);
  stats->RcpTidStatsEventAddbaRequestWasReceivedWithInvalidParameters = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaRequestWasReceivedWithInvalidParameters[radio_idx], chip_id);
  stats->RcpTidStatsEventAddbaResponseWasSentWithStatusSuccess = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaResponseWasSentWithStatusSuccess[radio_idx], chip_id);
  stats->RcpTidStatsEventAddbaResponseWasSentWithStatusFailure = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventAddbaResponseWasSentWithStatusFailure[radio_idx], chip_id);
  stats->RcpTidStatsEventBaAgreementOpened = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBaAgreementOpened[radio_idx], chip_id);
  stats->RcpTidStatsEventBaAgreementClosed = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBaAgreementClosed[radio_idx], chip_id);
  stats->RcpTidStatsEventBarWasReceivedWithBaAgreementOpened = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventBarWasReceivedWithBaAgreementOpened[radio_idx], chip_id);
  stats->RcpTidStatsEventInactivityTimeout = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventInactivityTimeout[radio_idx], chip_id);
  stats->RcpTidStatsEventDelbaReceived = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventDelbaReceived[radio_idx], chip_id);
  stats->RcpTidStatsEventDelbaWasSent = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventDelbaWasSent[radio_idx], chip_id);
  stats->RcpTidStatsEventIllegalAggregationWasReceived = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventIllegalAggregationWasReceived[radio_idx], chip_id);
  stats->RcpTidStatsEventCloseTid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientTidGlobalStats.RecipientTidStatisticsEventCloseTid[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_tsman_rcpt_sta_gl_stats (mtlk_core_t *core, mtlk_wssa_drv_tsman_rcpt_sta_gl_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->RcptStaStatsEventRemove = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventRemove[radio_idx], chip_id);
  stats->RcptStaStatsEventAdd = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventAdd[radio_idx], chip_id);
  stats->RcptStaStatsEventAddbaRequestReceivedWithIllegalTid = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, tsManagerRecipientStationGlobalStats.RecipientStationStatisticsEventAddbaRequestReceivedWithIllegalTid[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_radio_link_adapt_stats (mtlk_core_t *core, mtlk_wssa_drv_radio_link_adapt_stats_t *stats)
{
  int no_of_bands = 0;
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;
  unsigned cbw;

  if(_chipid_is_gen7(chip_id)) {
    no_of_bands = GEN7_NUM_OF_BANDS;
  } else {
    no_of_bands = NUM_OF_BW;
  }

  for (cbw=0; cbw < no_of_bands; cbw++) {
    stats->minPower[cbw] = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.minPower[radio_idx][cbw], chip_id);
    stats->maxPower[cbw] = MTLK_GET_STATS(hw_stats->stats_copy, linkAdaptationStats.maxPower[radio_idx][cbw], chip_id);
  }

  stats->bfReportReceivedCounter    = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, linkAdaptationStats.bfReportReceivedCounter[radio_idx], chip_id);
  stats->protectionSentCounter      = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, linkAdaptationStats.protectionSentCounter[radio_idx], chip_id);
  stats->protectionSucceededCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, linkAdaptationStats.protectionSucceededCounter[radio_idx], chip_id);

  for (cbw=0; cbw < no_of_bands; cbw++) {
    stats->rxDc[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationStats.rxDc[cbw], chip_id);
    stats->txLo[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationStats.txLo[cbw], chip_id);
    stats->rxIq[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationStats.rxIq[cbw], chip_id);
    stats->txIq[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationStats.txIq[cbw], chip_id);
  }
}

void __MTLK_IFUNC
wave_core_get_multicast_stats (mtlk_core_t *core, mtlk_wssa_drv_multicast_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->notEnoughClonePds = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, multicastStats.notEnoughClonePds , chip_id);
  stats->allClonesFinishedTransmission = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  multicastStats.allClonesFinishedTransmission, chip_id);
  stats->noStationsInGroup = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  multicastStats.noStationsInGroup, chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}
void __MTLK_IFUNC
wave_core_get_training_man_stats (mtlk_core_t *core, mtlk_wssa_drv_training_man_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->trainingTimerExpiredCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  trainingManagerStats.trainingTimerExpiredCounter, chip_id);
  stats->trainingStartedCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  trainingManagerStats.trainingStartedCounter, chip_id);
  stats->trainingFinishedSuccessfullyCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  trainingManagerStats.trainingFinishedSuccessfullyCounter, chip_id);
  stats->trainingFinishedUnsuccessfullyCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  trainingManagerStats.trainingFinishedUnsuccessfullyCounter, chip_id);
  stats->trainingNotStartedCounter = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot,  trainingManagerStats.trainingNotStartedCounter, chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}
void __MTLK_IFUNC
wave_core_get_grp_man_stats (mtlk_core_t *core, mtlk_wssa_drv_group_man_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->groupInfoDb = 0;
  mtlk_osal_lock_release(&hw_stats->lock);
}
void __MTLK_IFUNC
wave_core_get_general_stats (mtlk_core_t *core, mtlk_wssa_drv_general_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->calibrationMask = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.calibrationMask[radio_idx], chip_id);
  stats->onlineCalibrationMask = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.onlineCalibrationMask[radio_idx], chip_id);
  stats->dropReasonClassifier = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonClassifier[radio_idx], chip_id);
  stats->dropReasonDisconnect = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonDisconnect[radio_idx], chip_id);
  stats->dropReasonATF = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonATF[radio_idx], chip_id);
  stats->dropReasonTSFlush = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonTSFlush[radio_idx], chip_id);
  stats->dropReasonReKey = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonReKey[radio_idx], chip_id);
  stats->dropReasonSetKey = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonSetKey[radio_idx], chip_id);
  stats->dropReasonDiscard = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonDiscard[radio_idx], chip_id);
  stats->dropReasonDsabled = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonDsabled[radio_idx], chip_id);
  stats->dropReasonAggError = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.dropReasonAggError[radio_idx], chip_id);
  stats->defragStart = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.defragStart[radio_idx], chip_id);
  stats->defragEnd = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.defragEnd[radio_idx], chip_id);
  stats->defragTimeout = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.defragTimeout[radio_idx], chip_id);
  stats->classViolationErrors = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.classViolationErrors[radio_idx], chip_id);
  stats->rxhErrors = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.rxhErrors[radio_idx], chip_id);
  stats->rxSmps = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.rxSmps[radio_idx], chip_id);
  stats->rxOmn = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.rxOmn[radio_idx], chip_id);
  stats->txMngFromHost = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.txMngFromHost[radio_idx], chip_id);
  stats->txDataFromHost = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.txDataFromHost[radio_idx], chip_id);
  stats->cyclicBufferOverflow = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.cyclicBufferOverflow[radio_idx], chip_id);
  stats->beaconsTransmitted = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.beaconsTransmitted[radio_idx], chip_id);
  stats->debugStatistics = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.debugStatistics[radio_idx], chip_id);
  stats->exceedsETSIMaxSizeUnicast = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.exceedsETSIMaxSizeUnicast[radio_idx], chip_id);
  stats->exceedsETSIMaxSizeMulticast = MTLK_DIFF_STATS(hw_stats->stats_data, hw_stats->stats_snapshot, generalStats.exceedsETSIMaxSizeMulticast[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_dynamic_bw_stats (mtlk_core_t *core, mtlk_wssa_drv_dynamic_bw_stats_t *stats)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  unsigned radio_idx = wave_radio_id_get(wave_vap_radio_get(core->vap_handle));
  hw_statistics_t *hw_stats = &hw->hw_stats;

  mtlk_osal_lock_acquire(&hw_stats->lock);
  stats->dynamicBW20MHz = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, dynamicBwStats.dynamicBW20MHz[radio_idx], chip_id);
  stats->dynamicBW40MHz = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, dynamicBwStats.dynamicBW40MHz[radio_idx], chip_id);
  stats->dynamicBW80MHz = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot, dynamicBwStats.dynamicBW80MHz[radio_idx], chip_id);
  stats->dynamicBW160MHz = MTLK_DIFF_STATS(hw_stats->stats_copy, hw_stats->stats_snapshot,  dynamicBwStats.dynamicBW160MHz[radio_idx], chip_id);
  mtlk_osal_lock_release(&hw_stats->lock);
}

void __MTLK_IFUNC
wave_core_get_radio_la_mu_vht_stats (mtlk_core_t *core, mtlk_wssa_drv_radio_la_mu_vht_stats_t *stats, uint8 *mu)
{
  mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
  int chip_id = hw->card_info.chip_id;
  hw_statistics_t *hw_stats = &hw->hw_stats;
  unsigned cbw;

  stats->protectionSucceededCounter = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.protectionSucceededCounter[*mu], chip_id);
  stats->protectionSentCounter = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.protectionSentCounter[*mu], chip_id);
  stats->DataPhyMode  = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.DataPhyMode[*mu], chip_id);
  stats->dataBwLimit  = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.dataBwLimit[*mu], chip_id);
  stats->groupGoodput = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.groupGoodput[*mu], chip_id);
  stats->raState      = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.raState[*mu], chip_id);

  for (cbw=0; cbw < MAX_USP_IN_VHT_GROUP; cbw++) {
    stats->raStability[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.raStability[*mu][cbw], chip_id);
    stats->nssData[cbw]     = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.nssData[*mu][cbw], chip_id);
    stats->mcsData[cbw]     = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.mcsData[*mu][cbw], chip_id);
  }
  for (cbw=0; cbw < NUM_OF_BW; cbw++) {
    stats->powerData[cbw] = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.powerData[*mu][cbw], chip_id);
    stats->scpData[cbw]   = MTLK_GET_STATS(hw_stats->stats_data, linkAdaptationMuVhtStats.scpData[*mu][cbw], chip_id);
  }
}

#endif /* CONFIG_WAVE_DEBUG */
