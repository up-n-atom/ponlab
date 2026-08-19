/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
 * $Id:
 *
 * Platform Specific Data Base
 *
 */

#include "mtlkinc.h"
#include "mtlkdfdefs.h"

#include "mtlk_psdb.h"
#include "core.h"
#include "hw_mmb.h"

#include "mtlk_df.h"
#include "mtlk_df_fw.h"

#include "hw_hal.h"
#include "mtlk_card_types.h"
#include "core_config.h"

#define LOG_LOCAL_GID   GID_PSDB
#define LOG_LOCAL_FID   1

/*-------------------------------------------------*/
/*----- Country to RegDomain table ----------------*/

typedef struct {
    char    country[3];
    uint8   regd_code;
} _regd_country2domain_t;

/* FIXME" This table should be taken from PSD binary file ? */
static const _regd_country2domain_t country_regdcode_table[] =
{
    {"??", REGD_CODE_UNKNOWN  },
    {"AD", REGD_CODE_ETSI     },
    {"AE", REGD_CODE_UAE      },
    {"AF", REGD_CODE_ETSI     },
    {"AG", REGD_CODE_FCC      },
    {"AI", REGD_CODE_FCC      },
    {"AL", REGD_CODE_ETSI     },
    {"AM", REGD_CODE_ETSI     },
    {"AN", REGD_CODE_FCC      },
    {"AO", REGD_CODE_ETSI     },
    {"AQ", REGD_CODE_ETSI     },
    {"AR", REGD_CODE_FCC      },
    {"AS", REGD_CODE_ETSI     },
    {"AT", REGD_CODE_ETSI     },
    {"AU", REGD_CODE_AUSTRALIA},
    {"AW", REGD_CODE_FCC      },
    {"AX", REGD_CODE_ETSI     },
    {"AZ", REGD_CODE_ETSI     },
    {"BA", REGD_CODE_ETSI     },
    {"BB", REGD_CODE_FCC      },
    {"BD", REGD_CODE_MKK      },
    {"BE", REGD_CODE_ETSI     },
    {"BF", REGD_CODE_MKK      },
    {"BG", REGD_CODE_ETSI     },
    {"BH", REGD_CODE_ETSI     },
    {"BI", REGD_CODE_ETSI     },
    {"BJ", REGD_CODE_ETSI     },
    {"BL", REGD_CODE_FCC      },
    {"BM", REGD_CODE_FCC      },
    {"BN", REGD_CODE_MKK      },
    {"BO", REGD_CODE_FCC      },
    {"BR", REGD_CODE_BRAZIL   },
    {"BS", REGD_CODE_FCC      },
    {"BT", REGD_CODE_MKK      },
    {"BV", REGD_CODE_MKK      },
    {"BW", REGD_CODE_MKK      },
    {"BY", REGD_CODE_ETSI     },
    {"BZ", REGD_CODE_FCC      },
    {"CA", REGD_CODE_DOC      }, /* Canada */
    {"CB", REGD_CODE_DOC      }, /* Canada High Power (proprietary) */
    {"CC", REGD_CODE_MKK      },
    {"CD", REGD_CODE_MKK      },
    {"CF", REGD_CODE_ETSI     },
    {"CG", REGD_CODE_ETSI     },
    {"CH", REGD_CODE_ETSI     },
    {"CI", REGD_CODE_MKK      },
    {"CK", REGD_CODE_MKK      },
    {"CL", REGD_CODE_FCC      },
    {"CM", REGD_CODE_ETSI     },
    {"CN", REGD_CODE_CHINA    },
    {"CO", REGD_CODE_FCC      },
    {"CR", REGD_CODE_FCC      },
    {"CU", REGD_CODE_FCC      },
    {"CV", REGD_CODE_MKK      },
    {"CX", REGD_CODE_MKK      },
    {"CY", REGD_CODE_ETSI     },
    {"CZ", REGD_CODE_ETSI     },
    {"DE", REGD_CODE_ETSI     },
    {"DF", REGD_CODE_FCC      }, /* FCC default country (proprietary) */
    {"DJ", REGD_CODE_MKK      },
    {"DK", REGD_CODE_ETSI     },
    {"DM", REGD_CODE_FCC      },
    {"DO", REGD_CODE_FCC      },
    {"DZ", REGD_CODE_ETSI     },
    {"EC", REGD_CODE_FCC      },
    {"EE", REGD_CODE_ETSI     },
    {"EG", REGD_CODE_ETSI     },
    {"EH", REGD_CODE_ETSI     },
    {"ER", REGD_CODE_ETSI     },
    {"ES", REGD_CODE_ETSI     },
    {"ET", REGD_CODE_ETSI     },
    {"EU", REGD_CODE_ETSI     }, /* ETSI default country (proprietary) */
    {"FI", REGD_CODE_ETSI     },
    {"FJ", REGD_CODE_MKK      },
    {"FK", REGD_CODE_FCC      },
    {"FM", REGD_CODE_MKK      },
    {"FO", REGD_CODE_ETSI     },
    {"FR", REGD_CODE_ETSI     },
    {"GA", REGD_CODE_ETSI     },
    {"GB", REGD_CODE_ETSI     },
    {"GD", REGD_CODE_FCC      },
    {"GE", REGD_CODE_ETSI     },
    {"GF", REGD_CODE_ETSI     },
    {"GG", REGD_CODE_ETSI     },
    {"GH", REGD_CODE_MKK      },
    {"GI", REGD_CODE_ETSI     },
    {"GL", REGD_CODE_FCC      },
    {"GM", REGD_CODE_ETSI     },
    {"GN", REGD_CODE_ETSI     },
    {"GP", REGD_CODE_ETSI     },
    {"GQ", REGD_CODE_ETSI     },
    {"GR", REGD_CODE_ETSI     },
    {"GS", REGD_CODE_ETSI     },
    {"GT", REGD_CODE_FCC      },
    {"GU", REGD_CODE_MKK      },
    {"GW", REGD_CODE_ETSI     },
    {"GY", REGD_CODE_FCC      },
    {"HK", REGD_CODE_APAC     },
    {"HM", REGD_CODE_MKK      },
    {"HN", REGD_CODE_FCC      },
    {"HR", REGD_CODE_ETSI     },
    {"HT", REGD_CODE_FCC      },
    {"HU", REGD_CODE_ETSI     },
    {"ID", REGD_CODE_APAC     },
    {"IE", REGD_CODE_ETSI     },
    {"IL", REGD_CODE_ISRAEL   },
    {"IM", REGD_CODE_ETSI     },
    {"IN", REGD_CODE_APAC     },
    {"IO", REGD_CODE_MKK      },
    {"IQ", REGD_CODE_ETSI     },
    {"IR", REGD_CODE_ETSI     },
    {"IS", REGD_CODE_ETSI     },
    {"IT", REGD_CODE_ETSI     },
    {"JE", REGD_CODE_ETSI     },
    {"JM", REGD_CODE_FCC      },
    {"JO", REGD_CODE_ETSI     },
    {"JP", REGD_CODE_JAPAN    },
    {"KE", REGD_CODE_ETSI     },
    {"KG", REGD_CODE_ETSI     },
    {"KH", REGD_CODE_ETSI     },
    {"KI", REGD_CODE_ETSI     },
    {"KM", REGD_CODE_ETSI     },
    {"KN", REGD_CODE_FCC      },
    {"KP", REGD_CODE_MKK      },
    {"KR", REGD_CODE_KOREA    },
    {"KW", REGD_CODE_ETSI     },
    {"KY", REGD_CODE_FCC      },
    {"KZ", REGD_CODE_ETSI     },
    {"LA", REGD_CODE_MKK      },
    {"LB", REGD_CODE_ETSI     },
    {"LC", REGD_CODE_FCC      },
    {"LI", REGD_CODE_ETSI     },
    {"LK", REGD_CODE_MKK      },
    {"LR", REGD_CODE_ETSI     },
    {"LS", REGD_CODE_ETSI     },
    {"LT", REGD_CODE_ETSI     },
    {"LU", REGD_CODE_ETSI     },
    {"LV", REGD_CODE_ETSI     },
    {"LY", REGD_CODE_ETSI     },
    {"MA", REGD_CODE_ETSI     },
    {"MC", REGD_CODE_ETSI     },
    {"MD", REGD_CODE_ETSI     },
    {"ME", REGD_CODE_ETSI     },
    {"MF", REGD_CODE_FCC      },
    {"MG", REGD_CODE_ETSI     },
    {"MH", REGD_CODE_MKK      },
    {"MK", REGD_CODE_ETSI     },
    {"ML", REGD_CODE_ETSI     },
    {"MM", REGD_CODE_ETSI     },
    {"MN", REGD_CODE_ETSI     },
    {"MO", REGD_CODE_ETSI     },
    {"MP", REGD_CODE_MKK      },
    {"MQ", REGD_CODE_ETSI     },
    {"MR", REGD_CODE_ETSI     },
    {"MS", REGD_CODE_ETSI     },
    {"MT", REGD_CODE_ETSI     },
    {"MU", REGD_CODE_ETSI     },
    {"MV", REGD_CODE_MKK      },
    {"MW", REGD_CODE_ETSI     },
    {"MX", REGD_CODE_FCC      },
    {"MY", REGD_CODE_APAC     },
    {"MZ", REGD_CODE_ETSI     },
    {"NA", REGD_CODE_ETSI     },
    {"NC", REGD_CODE_MKK      },
    {"NE", REGD_CODE_ETSI     },
    {"NF", REGD_CODE_MKK      },
    {"NG", REGD_CODE_ETSI     },
    {"NI", REGD_CODE_FCC      },
    {"NL", REGD_CODE_ETSI     },
    {"NO", REGD_CODE_ETSI     },
    {"NP", REGD_CODE_MKK      },
    {"NR", REGD_CODE_MKK      },
    {"NU", REGD_CODE_MKK      },
    {"NZ", REGD_CODE_MKK      },
    {"OM", REGD_CODE_ETSI     },
    {"PA", REGD_CODE_FCC      },
    {"PE", REGD_CODE_FCC      },
    {"PF", REGD_CODE_ETSI     },
    {"PG", REGD_CODE_ETSI     },
    {"PH", REGD_CODE_APAC     },
    {"PK", REGD_CODE_ETSI     },
    {"PL", REGD_CODE_ETSI     },
    {"PM", REGD_CODE_FCC      },
    {"PN", REGD_CODE_MKK      },
    {"PR", REGD_CODE_FCC      },
    {"PT", REGD_CODE_ETSI     },
    {"PW", REGD_CODE_MKK      },
    {"PY", REGD_CODE_FCC      },
    {"QA", REGD_CODE_ETSI     },
    {"RE", REGD_CODE_ETSI     },
    {"RO", REGD_CODE_ETSI     },
    {"RS", REGD_CODE_ETSI     },
    {"RU", REGD_CODE_ETSI     },
    {"RW", REGD_CODE_ETSI     },
    {"SA", REGD_CODE_ETSI     },
    {"SB", REGD_CODE_MKK      },
    {"SC", REGD_CODE_MKK      },
    {"SD", REGD_CODE_ETSI     },
    {"SE", REGD_CODE_ETSI     },
    {"SG", REGD_CODE_SINGAPORE},
    {"SH", REGD_CODE_ETSI     },
    {"SI", REGD_CODE_ETSI     },
    {"SJ", REGD_CODE_ETSI     },
    {"SK", REGD_CODE_ETSI     },
    {"SL", REGD_CODE_ETSI     },
    {"SM", REGD_CODE_ETSI     },
    {"SN", REGD_CODE_ETSI     },
    {"SO", REGD_CODE_ETSI     },
    {"SR", REGD_CODE_FCC      },
    {"ST", REGD_CODE_ETSI     },
    {"SV", REGD_CODE_FCC      },
    {"SY", REGD_CODE_ETSI     },
    {"SZ", REGD_CODE_ETSI     },
    {"TC", REGD_CODE_FCC      },
    {"TD", REGD_CODE_ETSI     },
    {"TF", REGD_CODE_MKK      },
    {"TG", REGD_CODE_ETSI     },
    {"TH", REGD_CODE_MKK      },
    {"TJ", REGD_CODE_ETSI     },
    {"TK", REGD_CODE_MKK      },
    {"TL", REGD_CODE_MKK      },
    {"TM", REGD_CODE_ETSI     },
    {"TN", REGD_CODE_ETSI     },
    {"TO", REGD_CODE_ETSI     },
    {"TR", REGD_CODE_ETSI     },
    {"TT", REGD_CODE_ETSI     },
    {"TV", REGD_CODE_MKK      },
    {"TW", REGD_CODE_FCC      },
    {"TZ", REGD_CODE_ETSI     },
    {"UA", REGD_CODE_ETSI     },
    {"UG", REGD_CODE_FCC      },
    {"UM", REGD_CODE_FCC      },
    {"US", REGD_CODE_FCC      },
    {"UY", REGD_CODE_FCC      },
    {"UZ", REGD_CODE_ETSI     },
    {"VA", REGD_CODE_ETSI     },
    {"VC", REGD_CODE_FCC      },
    {"VE", REGD_CODE_FCC      },
    {"VG", REGD_CODE_FCC      },
    {"VI", REGD_CODE_FCC      },
    {"VN", REGD_CODE_MKK      },
    {"VU", REGD_CODE_MKK      },
    {"WF", REGD_CODE_ETSI     },
    {"WS", REGD_CODE_MKK      },
    {"YE", REGD_CODE_ETSI     },
    {"YT", REGD_CODE_ETSI     },
    {"ZA", REGD_CODE_ETSI     },
    {"ZM", REGD_CODE_ETSI     },
    {"ZW", REGD_CODE_ETSI     },
};

uint32 __MTLK_IFUNC
mtlk_psdb_country_to_regd_code (mtlk_core_t *core, const char *country)
{
    unsigned i;
    wave_radio_t *radio;
    uint32 regd_code = REGD_CODE_UNKNOWN;   /* not found */

    MTLK_ASSERT(country);

    radio = wave_vap_radio_get(core->vap_handle);
    MTLK_ASSERT(NULL != radio);

    for (i = 0; i < MTLK_ARRAY_SIZE(country_regdcode_table); i++) {
        if (country[0] == country_regdcode_table[i].country[0] &&
            country[1] == country_regdcode_table[i].country[1])
        {
            regd_code =  country_regdcode_table[i].regd_code;
            if ( regd_code == REGD_CODE_FCC  && MTLK_HW_BAND_6_GHZ == core_cfg_get_freq_band_cur(core)){
                if (IEEE80211_6GHZ_CTRL_REG_SP_AP == WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_6GHZ_OPER_POWER_MODE) ||
                    IEEE80211_6GHZ_CTRL_REG_ISP_AP == WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_6GHZ_OPER_POWER_MODE))
                    regd_code = REGD_CODE_FCC_SP;
                else
                    regd_code = REGD_CODE_FCC_LPI;
            }
        }
    }
    return regd_code;
}

BOOL __MTLK_IFUNC
mtlk_psdb_country_is_supported (mtlk_core_t *core, const char *country)
{
    return (REGD_CODE_UNKNOWN != mtlk_psdb_country_to_regd_code(core, country));
}

/*-------------------------------------------------*/
/*--- Platform Specific Data definitions ----------*/

#define PSDB_FILE_NAME "PSD.bin"

/* Firmware specific */
#define PSDB_PLATFORM_MARKER        0xAAAA5555 /* Start platform's data marker */
#define PSDB_TABLE_MARKER           0x66669999 /* Start tables marker */

/* Firmware related definitions */
#define PSDB_MAX_NUM_TABLE_WORDS    (PROGMODEL_CHUNK_SIZE / 4)  /* words per table */


#define NUM_WORDS_PER_FIELDS_BLOCK  2
#define NUM_WORDS_PER_REGD_ENTRY    5

#define PSDB_MAX_NUM_FBLOCKS    ((NUM_PLATFORM_DATA_FIELDS - 2) / NUM_WORDS_PER_FIELDS_BLOCK) /* 2 words for sizes */

/* Field IDs of table pointers */
#define PSDB_FIELD_HW_POWER_LIMIT   0x010000 /* Regulatory Domain */

/*
* In WAVE700, a new Power Limit Damage table has been introduced in PSD file.
* This table contains a power limit per band, in half dBs, that could not be overpassed to avoid damaging the HW of the chip.
*/
#define PSDB_FIELD_POWER_LIMIT_DAMAGE 0x120000   /* Power Limit Damage table */
#define PSDB_FIELD_POWER_LIMIT_DAMAGE_NROWS   1  /* table size: nrows */
#define PSDB_FIELD_POWER_LIMIT_DAMAGE_NCOLS   3  /* table size: ncols */

/*
* This table contains antenna gain per frequency, in 1/8 dBi unit. (introduced in WAVE700)
*/
#define PSDB_FIELD_ANTENNA_GAIN 0x370000   /* Antenna Gain table */
#define PSDB_FIELD_ANTENNA_GAIN_NCOLS   2  /* table size: ncols */
#define PSDB_FIELD_ANTENNA_GAIN_W700_NROWS   6  /* table size: nrows */
#define PSDB_FIELD_ANTENNA_GAIN_W600_NROWS   2  /* table size: nrows */

#define PSDB_FIELD_BEAMFORM_CAPS    0x130000 /* BfSupportedModesBitMap */
#define PSDB_FIELD_BEAMFORM_NROWS   1        /*  table size: nrows */
#define PSDB_FIELD_BEAMFORM_NCOLS   1        /*  table size: ncols */

#define PSDB_FIELD_POWER_ADAPT_11B  0x140000
#define PSDB_FIELD_POWER_ADAPT_11AG 0x150000
#define PSDB_FIELD_POWER_ADAPT_11N  0x160000
#define PSDB_FIELD_POWER_ADAPT_11AC 0x170000
#define PSDB_FIELD_POWER_ADAPT_11AX 0x300000
/*
* In WAVE700 family same PSD power offset table is used for 11AC, 11AX and 11BE
* Max MCS Value allowed in each standard generation must be taken into account for storing
* the power offsets
*
* Power Adaptation Table in W700 is per band so we have 3 tables
*/
#define PSDB_FIELD_POWER_ADAPT_GEN7_2G 0x380000
#define PSDB_FIELD_POWER_ADAPT_GEN7_5G 0x390000
#define PSDB_FIELD_POWER_ADAPT_GEN7_6G 0x3A0000

#define PSDB_FIELD_ANT_MASK_PER_CHANNEL 0x2A0000
#define PSDB_AMPC_TABLE_NCOLS       2 /* Number of Columns in Antenna mask per channel table */

/* CDB configuration table: ID and sizes */
#define PSDB_FIELD_CDB_CFG          0x2F0000
#define PSDB_FIELD_CDB_CFG_NROWS    1
#define PSDB_FIELD_CDB_CFG_NCOLS    WAVE_CARD_RADIO_NUM_MAX_G6
#define PSDB_FIELD_CTB_CFG_NCOLS    WAVE_CARD_RADIO_NUM_MAX_G7
#define PSDB_ZWDFS_ANT_MSK          0x10

/* CCA Thresholds table */
#define PSDB_FIELD_CCA_TH_CFG       0x330000

/* All other tables IDs should be greater than PSDB_FIELD_HW_POWER_LIMIT */
#define PSDB_FIELD_TABLE_ID_MIN     PSDB_FIELD_HW_POWER_LIMIT

/* CDB configuration table */

static const char * _wave_cdb_cfg_strings[WAVE_CDB_CFG_LAST] = {
  "CDB_CFG_INVALID",
  "CDB_CFG_2G",
  "CDB_CFG_5G",
  "CDB_CFG_5G_LOW",
  "CDB_CFG_5G_HIGH",
  "CDB_CFG_ALTERN",
  "CDB_CFG_6G_UNII5",
  "CDB_CFG_6G",
  "CDB_CFG_NOT_SET"
};

static __INLINE const char *
__wave_cdb_cfg_to_string (wave_cdb_cfg_e config)
{
  if (wave_cdb_cfg_is_unknown(config)) {
    return "CDB_CFG_OUT_OF_RANGE";
  } else {
    return _wave_cdb_cfg_strings[config];
  }
}

const char * __MTLK_IFUNC
wave_cdb_cfg_to_string (wave_cdb_cfg_e config)
{
  return __wave_cdb_cfg_to_string (config);
}

static __INLINE psdb_cdb_cfg_table_t *
__wave_psdb_get_cdb_cfg_table (hw_psdb_t *psdb)
{
  return &psdb->cdb_cfg_table;
}

static __INLINE wave_ant_params_t *
__wave_psdb_get_card_ant_params (hw_psdb_t *psdb)
{
  psdb_cdb_cfg_table_t *cdb_table;

  cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);

  return &cdb_table->card_ant_cfg;
}

wave_ant_params_t *
wave_psdb_get_card_ant_params (hw_psdb_t *psdb)
{
  MTLK_ASSERT(psdb);
  return __wave_psdb_get_card_ant_params(psdb);
}

static psdb_radio_cfg_t *
__wave_psdb_cdb_get_radio_cfg (hw_psdb_t *psdb, wave_uint idx)
{
  psdb_cdb_cfg_table_t  *cdb_table;
  psdb_radio_cfg_t      *radio_cfg = NULL;

  cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
  if (wave_psdb_cdb_cfg_idx_is_valid(cdb_table, idx)) {
    radio_cfg = &cdb_table->radio_cfg[idx];
  }

  return radio_cfg;
}

static __INLINE wave_ant_params_t *
__wave_psdb_get_radio_ant_params (hw_psdb_t *psdb, wave_uint idx)
{
  psdb_radio_cfg_t   *cfg = NULL;
  wave_ant_params_t  *params = NULL;

  cfg = __wave_psdb_cdb_get_radio_cfg(psdb, idx);
  if (cfg) {
    params = &cfg->radio_ant_cfg;
  }
  return params;
}

const wave_ant_params_t * __MTLK_IFUNC
wave_psdb_get_radio_ant_params (hw_psdb_t *psdb, unsigned radio_idx)
{
  MTLK_ASSERT(psdb);
  return __wave_psdb_get_radio_ant_params(psdb, radio_idx);
}

int __MTLK_IFUNC
wave_psdb_get_radio_cca_th_params (hw_psdb_t *psdb, unsigned radio_idx, iwpriv_cca_th_t *params)
{
  psdb_cdb_cfg_table_t *cdb_table;
  mtlk_hw_band_e band;
  int i;

  MTLK_ASSERT(params);

  cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
  if (cdb_table->cca_th_is_filled &&
      wave_psdb_cdb_cfg_idx_is_valid(cdb_table, radio_idx)) {
    band = cdb_table->radio_cfg[radio_idx].wave_band;
    if (band >= MTLK_HW_BAND_MAX) {
        ELOG_D("Out of index in cca_th_params[] (band=%u)", band);
        return MTLK_ERR_UNKNOWN;
    }
    params->is_updated = 1;
    for (i = 0; i < MTLK_CCA_TH_PARAMS_LEN; i++) {
      params->values[i] = cdb_table->cca_th_params[band][i];
    }

    return MTLK_ERR_OK;
  } else {
    return MTLK_ERR_NOT_SUPPORTED;
  }
}

static __INLINE BOOL
__wave_psdb_is_radio_band_supported (hw_psdb_t *psdb, unsigned radio_idx, mtlk_hw_band_e band)
{
  psdb_radio_cfg_t   *cfg = NULL;
  int                 res = FALSE;

  cfg = __wave_psdb_cdb_get_radio_cfg(psdb, radio_idx);
  if (cfg) {
    if (band == cfg->wave_band) {
      res = TRUE;
    }
  }
  return res;
}

BOOL __MTLK_IFUNC
wave_psdb_is_radio_band_supported (hw_psdb_t *psdb, unsigned radio_idx, mtlk_hw_band_e band)
{
  return __wave_psdb_is_radio_band_supported(psdb, radio_idx, band);
}

static __INLINE wave_int
__wave_psdb_cdb_get_radio_idx_by_chan (psdb_cdb_cfg_table_t *cdb_table, int chan_num)
{
  /* compare with last channel for radio 0, all above are for radio 1 */
  return (chan_num > cdb_table->radio_chan_threshod) ? 1 : 0;
}

static __INLINE wave_ant_params_t *
__wave_psdb_cdb_get_ant_params_by_chan (psdb_cdb_cfg_table_t *cdb_table, int chan_num)
{
  wave_ant_params_t    *params = NULL;
  wave_int              idx;

  idx    = __wave_psdb_cdb_get_radio_idx_by_chan(cdb_table, chan_num);
  if (wave_psdb_cdb_cfg_idx_is_valid(cdb_table, idx)) {
    params = &cdb_table->radio_cfg[idx].radio_ant_cfg;
  }

  return params;
}

static wave_ant_params_t *
_wave_psdb_get_ant_params_by_chan (hw_psdb_t *psdb, int chan_num)
{
  return __wave_psdb_cdb_get_ant_params_by_chan(__wave_psdb_get_cdb_cfg_table(psdb), chan_num);
}

static uint8
_wave_psdb_get_tx_ant_factor_by_chan (hw_psdb_t *psdb, int chan_num)
{
  uint8               tx_ant_factor = 0;
  wave_ant_params_t  *params;

  params = _wave_psdb_get_ant_params_by_chan (psdb, chan_num);
  if (params) {
    tx_ant_factor = params->tx_ant_factor;
  }

  return tx_ant_factor;
}

/* 6GHz band support */
static uint8
_wave_psdb_get_tx_ant_factor_by_chan_6g (hw_psdb_t *psdb, int chan_num)
{
  uint8                   tx_ant_factor = 0;
  wave_ant_params_t       * params;
  psdb_cdb_cfg_table_t    * cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);

  /* TODO: Dual band configuration with 6GHz to be handled */

  /*  params = _wave_psdb_get_ant_params_by_chan (psdb, chan_num); */ 
  params  =  &cdb_table->radio_cfg[0].radio_ant_cfg;

  if (params) {
    tx_ant_factor = params->tx_ant_factor;
  }

  return tx_ant_factor;
}

/*-------------------------------------------------*/
/*--- External API ---*/

uint32 count_bits_set (uint32 v);

/*-------------------------------------------------*/
/* Misc utils */

/* Copying word array */
static void wordcpy(uint32 *dest, uint32 *src, int n)
{
    uint32 w0, w1;

    ILOG3_PPD("%p %p size %d", dest, src, n);

    /* unroll 2 */
    while (n >= 2 ) {
        w0 = *src++;
        w1 = *src++;
        *dest++ = w0;
        *dest++ = w1;
        n -= 2;
    }

    /* last word */
    if (n != 0) {
        *dest = *src;
    }
}

/* Copying word array with endian convertion */
static void wordcpy_mac_to_host(uint32 *dest, uint32 *src, int n)
{
    uint32 w0, w1;

    ILOG3_PPD("%p %p size %d", dest, src, n);

    /* unroll 2 */
    while (n >= 2 ) {
        w0 = MAC_TO_HOST32(*src++);
        w1 = MAC_TO_HOST32(*src++);
        *dest++ = w0;
        *dest++ = w1;
        n -= 2;
    }

    /* last word */
    if (n != 0) {
        *dest = MAC_TO_HOST32(*src);
    }
}

/*-------------------------------------------------*/

#define  PSDB_MEM_DEBUG
#define  PSDB_TABLE_DEBUG

#define PSDB_ALLOC(size)    _psdb_alloc(size, __LINE__)
#define PSDB_FREE(buf)      _psdb_free(buf, __LINE__)

#ifdef  PSDB_MEM_DEBUG
static uint32   _alloc_count = 0;
static uint32   _alloc_size  = 0;
#endif

static void *_psdb_alloc (uint32 size, unsigned lnum)
{
    void *ptr;

    ptr = mtlk_osal_mem_alloc(size, MTLK_MEM_TAG_PSDB);
    if(!ptr) {
        ELOG_D("Line %d: Can't allocate memory", lnum);
    }

#ifdef  PSDB_MEM_DEBUG
    if(!ptr)
        return NULL;

    _alloc_count++;
    _alloc_size += size;
    ILOG3_DDPDD("Line %d: alloc %d, ptr 0x%p, size %d, total %d",
                lnum, _alloc_count, ptr, size, _alloc_size);
#endif

    return ptr;
}

static void _psdb_free (void *buf, unsigned lnum)
{
#ifdef  PSDB_MEM_DEBUG
    ILOG3_DDP("Line %d: free %d, ptr 0x%p", lnum, _alloc_count, buf);
    if(!buf)
        return;

    _alloc_count--;
#endif

    if (buf) mtlk_osal_mem_free(buf);
}

#ifdef  PSDB_TABLE_DEBUG

static __INLINE BOOL _psdb_check_snprintf (int res, size_t maxsize)
{
  return (res < 0 || (size_t)res >= maxsize) ? FALSE : TRUE;
}

static void
_psdb_dump_array (uint32 *wbuf, int nrows, int ncols)
{
    uint8   line[6 + (8 * 9) + 1];
    int     i, j, pos, res;
    uint32  *wp;

    if (nrows <= 0 || ncols <= 0 || ncols > 8)
        return;

    for (i = 0; i < nrows; i++) {
        wp = wbuf;
        res = mtlk_snprintf(line, sizeof(line), "%4d:", i);
        if (!_psdb_check_snprintf(res, sizeof(line)))
            break;
        pos = res;
        for (j = 0; j < ncols; j++) {
            res = mtlk_snprintf(line + pos, sizeof(line) - pos, " %08X", MAC_TO_HOST32(wbuf[j]));
            if (!_psdb_check_snprintf(res, sizeof(line) - pos))
                break;
            pos += res;
        }
        ILOG3_S("%s", line);
        wbuf += ncols;
    }
}

static void
_psdb_dump_table (psdb_any_table_t *db_table)
{
    int     nrows, ncols;

    nrows = db_table->nrows;
    ncols = db_table->ncols;

    ILOG3_DDD("Table: id 0x%X, nrows %d, ncols %d", db_table->id, nrows, ncols);
    _psdb_dump_array(&db_table->data.words[0], nrows, ncols);
}

#endif /* PSDB_TABLE_DEBUG */


/* Allocate and copy table
 */
static psdb_any_table_t *
psdb_copy_table(uint32 *wbuf, uint32 wsize, uint32 id, uint32 b_offs, uint32 marker)
{
    psdf_any_table_t    *f_table;
    psdb_any_table_t    *db_table = NULL;
    uint32               nrows, ncols, nwords;
    uint32               w_offs, size;

    /* Fileds blocks are stored in MAC endian */
    ILOG2_DD("Table: id 0x%X, offs 0x%04X", id, b_offs);

    w_offs = b_offs  / sizeof(uint32);      /* offset in words */
    if ((w_offs * sizeof(uint32)) != b_offs) {
        ELOG_D("Table's offset is not aligned to word: 0x%04X", b_offs);
        goto ERROR;
    } else
    if (w_offs > wsize) {
        ELOG_D("Table's offset too big: 0x%04X", b_offs);
        goto ERROR;
    }

    /* pointer to the table */
    f_table = (psdf_any_table_t *)&wbuf[w_offs];

    /* table marker */
    if (marker != MAC_TO_HOST32(f_table->marker)) {
        ELOG_D("Wrong Table's Marker: 0x%08X", MAC_TO_HOST32(f_table->marker));
        goto ERROR;
    }

    /* table dimensions and size in words */
    nrows   = MAC_TO_HOST32(f_table->data.nrows);
    ncols   = MAC_TO_HOST32(f_table->data.ncols);
    nwords  = nrows * ncols;

    /* nrows and ncols should be taken into account because they will be passed to FW */
    nwords += sizeof(psdf_table_data_t) / sizeof(uint32);

    /* Check offset */
    if ((w_offs + (sizeof(psdf_any_table_t) / sizeof(uint32))) > wsize) {
        ELOG_D("Wrong Table's header: size too big (%d)", nwords);
        goto ERROR;
    }

    /* Check size. Is not related to HW_POWER_LIMIT table due to it will not be passed to FW */
    if ((id != PSDB_FIELD_HW_POWER_LIMIT) && (nwords > PSDB_MAX_NUM_TABLE_WORDS)) {
        ELOG_D("Table's nof_words too big: %d", nwords);
        goto ERROR;
    }

    size = nwords * sizeof(uint32) + sizeof(psdb_any_table_t) - sizeof(psdf_table_data_t);
    db_table = PSDB_ALLOC(size);
    if(!db_table) {
        goto ERROR;
    }

    db_table->offs      = b_offs;
    db_table->id        = id;
    db_table->nrows     = nrows;
    db_table->ncols     = ncols;
    db_table->nof_words = nwords;

    /* Copy Table's content without endian convertion */
    wordcpy((uint32 *)&db_table->data, (uint32 *)&f_table->data, nwords);

    ILOG2_PDDDD("ptr 0x%p: offs 0x%X, id 0x%X, sizes %ux%u",
                db_table, db_table->offs, id, nrows, ncols);

#ifdef  PSDB_TABLE_DEBUG
    _psdb_dump_table(db_table);
#endif /* PSDB_TABLE_DEBUG */

    return db_table;

ERROR:
    return NULL;
}

static psdf_fields_array_t *
psdb_search_fields_array (uint32 *wbuf, uint32 wsize, uint32 chip_id, uint32 chip_type,  uint32 hw_type, uint32 hw_rev)
{
    psdf_fields_array_t *fdp;
    psdf_fields_block_t *fblocks;
    uint32              *wp = wbuf;
    uint32               word;
    uint32               nrows, i;
    BOOL                 found_type, found_rev, found_chipType;

    /* Convert to MAC endian before search */
    hw_type = HOST_TO_MAC32(hw_type);
    hw_rev  = HOST_TO_MAC32(hw_rev);

    while((wp - wbuf) < (ptrdiff_t)wsize)
    {
        word = MAC_TO_HOST32(*wp);
        if (word == PSDB_TABLE_MARKER) {
            /* Platforms data should not be located after this marker */
            break;
        } else
        if (word != PSDB_PLATFORM_MARKER) {
            wp++;
            continue;
        }

        /* Fields block */
        ILOG2_D("Fields block offset 0x%04X", (wp - wbuf) * sizeof(*wp)); /* in bytes */
        fdp   = (psdf_fields_array_t *)wp;
        word  = MAC_TO_HOST32(fdp->table.data.ncols);
        nrows = MAC_TO_HOST32(fdp->table.data.nrows);
        if (NUM_WORDS_PER_FIELDS_BLOCK != word)
        {
            ELOG_D("Wrong Fields data size (ncols %d)", word);
            goto ERROR;
        } else
        if (HOST_TO_MAC32(PSDB_FIELD_CHIP_ID)  != fdp->chip_id.id ||
            HOST_TO_MAC32(PSDB_FIELD_CHIP_REV) != fdp->chip_rev.id)
        {
            ELOG_V("Wrong Fields data header");
            goto ERROR;
        }

        /* Check for ChipID */
        if (MAC_TO_HOST32(fdp->chip_id.value) != chip_id) {
            wp++;
            continue;
        }

        /* Check for HW type, HW revision and Chip Type */
        found_type = FALSE;
        found_rev  = FALSE;
        found_chipType  = FALSE;
        for (i = 0; i < nrows; i++) {
            fblocks = &fdp->table.data.fblocks[i];

            if (HOST_TO_MAC32(PSDB_FIELD_HW_TYPE) == fblocks->id) {
                if (hw_type == fblocks->value) { /* Both are in MAC endian */
                    if (TRUE == found_rev && TRUE == found_chipType) { /* If other two already found */
                        return fdp;          /*  then search completed */
                    } else {
                        found_type = TRUE;
                    }
                }
            } else
            if (HOST_TO_MAC32(PSDB_FIELD_HW_REV) == fblocks->id) {
                if (hw_rev == fblocks->value) { /* Both are in MAC endian */
                    if (TRUE == found_type && TRUE == found_chipType) { /* If other two already found */
                        return fdp;           /*  then search completed */
                    } else {
                        found_rev = TRUE;
                    }
                }
            }
            // TODO jpastor, reusing RF_CHIP_ID for chip type, check if it is possible or not
            if (HOST_TO_MAC32(PSDB_FIELD_RF_CHIP_ID) == fblocks->id) {
                if (chip_type == fblocks->value) { /* Both are in MAC endian */
                    if (TRUE == found_type && TRUE == found_rev) { /* If other two already found */
                        return fdp;           /*  then search completed */
                    } else {
                        found_chipType = TRUE;
                    }
                }
            }
        } /* for */

        /* seek to next fields block */
        wp += word * nrows; /* word = ncols */
    } /* while */

ERROR:
    return NULL;
}

/* Conversion from 2.4 and 5 GHz channel number to its index in our array of channel scan support_data */
extern const uint8 channum2idx_2_5[256];    /* FIXME: size should be taken from scan_support.c */

/* Conversion from channel number to all channel-freq-support index */
unsigned __MTLK_IFUNC channum2chanidx(int num)
{
  unsigned idx;

  if ((num < 0) || (num >= ARRAY_SIZE(channum2idx_2_5)))
    return CHAN_IDX_ILLEGAL;

  idx = channum2idx_2_5[(u8) num];
  /* Note: size of array pw_by_chan of struct psdb_regd_parsed_t must fit to idx value */
  return (idx > NUM_TOTAL_CHAN_FREQS) ? CHAN_IDX_ILLEGAL : idx;
}

/* 6GHz band support update */
/* Conversion from channel number to all channel-freq-support index for 6GHz band *
 * chan num = (2*index)+1: the chan nums 223,231 are not allowed                  */
static __INLINE
BOOL mtlk_is_6g_channum_valid(int num)
{
  if ((num < 0) || (num > WAVE_LAST_6G_CHAN) ||
      (num == INVALID_6G_CHAN_NUM_223) ||
      (num == INVALID_6G_CHAN_NUM_231))
    return FALSE;
  else
    return TRUE;
}

/* idx returned is 1(ch 1), 2(ch 3),..., 117(ch 233). Idx 0 is treated as illegal */
unsigned channum6g2chanidx(int num)
{
  unsigned idx = 0;

  if (!mtlk_is_6g_channum_valid(num))
    return CHAN_IDX_ILLEGAL;

  for (idx = 0; idx < MAX_CHAN_IDX_6G_FULL; idx++) {
    if (num == ((2 * idx) + 1))
      break;
  }

  return ((idx > MAX_CHAN_IDX_6G_FULL - 1) ? CHAN_IDX_ILLEGAL : (idx+1));
}

static psdb_phy_cw_t
_get_regd_entry_phy_cw (psdf_regd_entry_t   *regd_entry)
{
    psdb_chanwidth_t     chanwidth;
    psdb_phy_mode_t      phy_mode;
    psdb_phy_cw_t        phy_cw_mode;

    chanwidth   = regd_entry->chanwidth;
    phy_mode    = regd_entry->phy_mode;
    phy_cw_mode = PSDB_PHY_CW_LAST;     /* Illegal mode */

    /* Only 20 MHz in 11B mode or 11AG*/
    if (PSDB_PHY_MODE_11B == phy_mode) {
        if (PSDB_CHANWIDTH_20 == chanwidth) {
            return PSDB_PHY_CW_11B;
        }
    } else if (PSDB_PHY_MODE_11AG == phy_mode) {
        if (PSDB_CHANWIDTH_20 == chanwidth) {
            return PSDB_PHY_CW_AG_20;
        }
    }

    /* 20/40/80 MHz in other modes */
    /* 1. Checking the mode */
    if (PSDB_PHY_MODE_OFDM == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_OFDM_20;
    } else if (PSDB_PHY_MODE_MU == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_MU_20;
    } else if (PSDB_PHY_MODE_BF == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_BF_20;
    } else if (PSDB_PHY_MODE_11AX == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_AX_20;
    } else if (PSDB_PHY_MODE_11N == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_N_20;
    } else if (PSDB_PHY_MODE_BF_11AX == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_BF_AX_20;
    } else if (PSDB_PHY_MODE_11BE == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_BE_20;
    } else if (PSDB_PHY_MODE_BF_11BE == phy_mode) {
        phy_cw_mode = PSDB_PHY_CW_BF_BE_20;
    } else {
        return PSDB_PHY_CW_LAST; /* Illegal mode */
    }

    /* 2. Checking the chanwidth */
    if (PSDB_CHANWIDTH_320 == chanwidth) {
      phy_cw_mode += (PSDB_PHY_CW_OFDM_320 - PSDB_PHY_CW_OFDM_20);
    } else if (PSDB_CHANWIDTH_160 == chanwidth) {
        phy_cw_mode += (PSDB_PHY_CW_OFDM_160 - PSDB_PHY_CW_OFDM_20);
    } else if (PSDB_CHANWIDTH_80 == chanwidth) {
        phy_cw_mode += (PSDB_PHY_CW_OFDM_80 - PSDB_PHY_CW_OFDM_20);
    } else if (PSDB_CHANWIDTH_40 == chanwidth) {
        phy_cw_mode += (PSDB_PHY_CW_OFDM_40 - PSDB_PHY_CW_OFDM_20);
    } else if (PSDB_CHANWIDTH_20 != chanwidth) {
        phy_cw_mode = PSDB_PHY_CW_LAST;     /* Illegal mode */
    }

    return phy_cw_mode;
}

/* Parsing the whole table */
static int
psdb_parse_regd_table (mtlk_hw_t *hw, hw_psdb_t *psdb, psdb_any_table_t *regd_table, uint32 hw_rev)
{
    psdf_regd_entry_t   *regd_entry;
    psdb_phy_cw_t        phy_cw_mode;
    uint32               tx_ant_factor = 0;
    int                  chan_freq, chan_num;
    int                  i, nof_entries;
    unsigned             idx;
    bool                 freq6g_in_regd = false;

    MTLK_ASSERT(NULL != regd_table);

    /* Table content is REGD entries, words are in host endian */
    nof_entries  = regd_table->nrows;
    ILOG0_DDDDD("PSD: band_support_2g = %d, band_support_5g =%d, band_support_6g = %d, nrows = %d, ncols = %d",
                __wave_psdb_get_cdb_cfg_table(psdb)->band_support_2G,
                __wave_psdb_get_cdb_cfg_table(psdb)->band_support_5G,
                __wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G,
                regd_table->nrows, regd_table->ncols);
    for (i = 0; i < nof_entries; i++) {
        regd_entry = &regd_table->data.regd_entries[i];
        chan_freq = regd_entry->chan_freq;
        chan_num = ieee80211_frequency_to_channel(chan_freq);
        /* 6GHz support: channel number to index conversion handled differently for 6GHz and 2.4/5GHz bands */
        if(WAVE_FREQ_IS_6G(chan_freq)) {
          freq6g_in_regd = true;
          idx = channum6g2chanidx(chan_num);
          if (!mtlk_hw_type_is_gen7(hw) && (hw_rev < HW_VERS_600_GPB665_wrx665_6G) && (idx > MAX_CHAN_IDX_6G_UNII5))
            idx = CHAN_IDX_ILLEGAL;
        } else {
          idx = channum2chanidx(chan_num);
        }

        ILOG1_DDDDDDDD("[%2d] = regd 0x%02X, chan %3d (%4d MHz), chan_idx %2d, bw %d, mode %d, pw %3d",
                       i, regd_entry->regd_code, chan_num, chan_freq, idx,
                       regd_entry->chanwidth, regd_entry->phy_mode, regd_entry->pw_limit);

        if (CHAN_IDX_ILLEGAL == idx) {
            /* an attempt to set this channel will be rejected, but it is Ok for the parsing */
            WLOG_DDD("WARNING: Illegal/Unsupported chan num %d (%d MHz) idx %d", chan_num, chan_freq, idx);
            /* goto ERROR; */
        }

        phy_cw_mode = _get_regd_entry_phy_cw(regd_entry);
        if (!mtlk_hw_type_is_gen7(hw) && PSDB_PHY_CW_LAST == phy_cw_mode) {  /* Illegal */
            goto ERR_PHY_CW;
        }

        /* Check for max power limit value */
        tx_ant_factor = _wave_psdb_get_tx_ant_factor_by_chan(psdb, chan_num);

        if (MAX_UINT8 < regd_entry->pw_limit ||
            MAX_UINT8 < (regd_entry->pw_limit + tx_ant_factor)) {
            ELOG_DDD("ERROR: power limit (%d) + antennas factor (%d) > max allowed (%d)",
                     regd_entry->pw_limit, tx_ant_factor, MAX_UINT8);
            goto ERROR;
        }
    }
    if (__wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G && !freq6g_in_regd) {
      ELOG_V("ERROR: No 6G frequency found in the Regd Table");
      goto ERROR;
    }

    return MTLK_ERR_OK;

ERR_PHY_CW:
    ELOG_DD("Illegal pair (BandWidth %d / PhyMode %d)", regd_entry->chanwidth, regd_entry->phy_mode);

ERROR:
    return MTLK_ERR_UNKNOWN;
}

int
psdb_get_field_val (hw_psdb_t *psdb, uint32 id, uint32 *val)
{
    psdb_fields_array_t *fields_data = psdb->fields_data;   /* Pointer to Fields array */
    unsigned i;

    if (!fields_data) return MTLK_ERR_NO_ENTRY;

    for (i = 0; i < fields_data->nrows; i++) {
        if (fields_data->data.fblocks[i].id == HOST_TO_MAC32(id)) {
          *val = MAC_TO_HOST32(fields_data->data.fblocks[i].value);
          return MTLK_ERR_OK;
        }
    }

    return MTLK_ERR_NO_ENTRY;
}

int
psdb_get_hw_limits (mtlk_core_t *core, const char *country, int chan_freq, psdb_pw_limits_t *pw_limits)
{
    hw_psdb_t           *psdb;
    psdb_any_table_t    *regd_table;    /* Source file RegDomain table */
    psdb_regd_parsed_t  *regd_parsed;   /* Parsed RegDomain table */
    psdf_regd_entry_t   *regd_entry;
    psdb_phy_cw_t        phy_cw_mode;
    uint32               tx_ant_factor = 0;

    uint32               regd_code;
    int                  chan_num;
    unsigned             chan_idx;
    unsigned             i;
    BOOL                 freq_found = FALSE;

    psdb = mtlk_core_get_psdb(core);
    MTLK_ASSERT(NULL != psdb);

    regd_table  = psdb->regd_table;
    regd_parsed = psdb->regd_parsed;
    if (NULL == regd_table || NULL == regd_parsed) {
        ILOG2_V("PSDB data missed");
        memset(pw_limits, 0, sizeof(psdb_pw_limits_t));

        return MTLK_ERR_NOT_SUPPORTED;
    }

    /* RegDomainCode */
    regd_code = mtlk_psdb_country_to_regd_code(core, country);

    /* Check for Country and RegDomainCode changes */
    if (regd_parsed->country[0] == country[0] &&
        regd_parsed->country[1] == country[1] &&
        regd_code == regd_parsed->regd_code) {
        ILOG3_V("Country and RegDomainCode are not changed");
        goto READY;
    }

    regd_parsed->country[0] = country[0];
    regd_parsed->country[1] = country[1];

    /* Clear and refill the power table */
    memset(regd_parsed->pw_by_chan, 0, sizeof(regd_parsed->pw_by_chan));
    memset(regd_parsed->pw_by_chan_6g, 0, sizeof(regd_parsed->pw_by_chan_6g));

    /* Table content is REGD entries, all words are in host endian */
    regd_table = psdb->regd_table;
    for (i = 0; i < regd_table->nrows; i++) {
        regd_entry = &regd_table->data.regd_entries[i];

        /* all channels for this RegDomain code */
        if (regd_code != regd_entry->regd_code) {
            continue;
        }

        if ((int)regd_entry->chan_freq == chan_freq)
          freq_found = TRUE;
        chan_num = ieee80211_frequency_to_channel(regd_entry->chan_freq);
        phy_cw_mode = _get_regd_entry_phy_cw(regd_entry);
        /* 6GHz support: power limit tables are updated differently for 6GHz and 2.4/5GHz bands */
        if(WAVE_FREQ_IS_6G(regd_entry->chan_freq)) {
          chan_idx = channum6g2chanidx(chan_num);
          regd_parsed->pw_by_chan_6g[chan_idx].pw_limits[phy_cw_mode] = regd_entry->pw_limit;
        } else {
          chan_idx = channum2chanidx(chan_num);
          regd_parsed->pw_by_chan[chan_idx].pw_limits[phy_cw_mode] = regd_entry->pw_limit;
        }
    }

    if (!freq_found)
      ILOG0_D("Warning: Channel Frequency %d NOT found in the REGD table!", chan_freq);

READY:
    chan_num = ieee80211_frequency_to_channel(chan_freq);

    /* 6GHz support: power limits are updated differently for 6GHz and 2.4/5GHz bands */

    if (WAVE_FREQ_IS_6G(chan_freq)) {
      chan_idx = channum6g2chanidx(chan_num);
      *pw_limits = regd_parsed->pw_by_chan_6g[chan_idx];
      tx_ant_factor = _wave_psdb_get_tx_ant_factor_by_chan_6g(psdb, chan_num);
    } else { 
    /*5/2.4 GHz bands */
      chan_idx = channum2chanidx(chan_num);
      *pw_limits = regd_parsed->pw_by_chan[chan_idx];   /* struct copy */

       tx_ant_factor = _wave_psdb_get_tx_ant_factor_by_chan(psdb, chan_num);
    }

    /* Add antenna factor to each limit which is set, i.e. non zero */
    /* Note that overflow was checked during table parsing */

#define _PWL_FACTOR_APLLY(value, factor)  if(0 != (value)) { (value) += (factor); }

    for (i = 0; i < MTLK_ARRAY_SIZE(pw_limits->pw_limits); i++) {
      _PWL_FACTOR_APLLY(pw_limits->pw_limits[i], tx_ant_factor);
    }

#undef _PWL_FACTOR_APLLY

    /* If the MU/BF/AX/N/AG values are not configured, we need to use the OFDM
     * values instead. If BF_AX values are not configured, we need to use the BF
     */

#ifdef MTLK_WAVE_700
    for (i = 0; i < MAXIMUM_BANDWIDTHS_GEN7; i++) {
      if (pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i] == 0) {
        pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
      }
      if (pw_limits->pw_limits[PSDB_PHY_CW_BE_20 + i] == 0) {
        pw_limits->pw_limits[PSDB_PHY_CW_BE_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
      }
      if (pw_limits->pw_limits[PSDB_PHY_CW_BF_BE_20 + i] == 0) {
        pw_limits->pw_limits[PSDB_PHY_CW_BF_BE_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i];
      }
    }
#endif

    for (i = 0; i < MAXIMUM_BANDWIDTHS_GEN6; i++) {
        if (pw_limits->pw_limits[PSDB_PHY_CW_MU_20 + i] == 0) {
            pw_limits->pw_limits[PSDB_PHY_CW_MU_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
        }
        if (pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i] == 0) {
            pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
        }
        if (pw_limits->pw_limits[PSDB_PHY_CW_BF_AX_20 + i] == 0) {
            pw_limits->pw_limits[PSDB_PHY_CW_BF_AX_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_BF_20 + i];
        }
        if (pw_limits->pw_limits[PSDB_PHY_CW_AX_20 + i] == 0) {
            pw_limits->pw_limits[PSDB_PHY_CW_AX_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
        }
        if (i < MAXIMUM_BANDWIDTHS_11N) {
            if (pw_limits->pw_limits[PSDB_PHY_CW_N_20 + i] == 0) {
                pw_limits->pw_limits[PSDB_PHY_CW_N_20 + i] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20 + i];
            }
        }
    }
    if (pw_limits->pw_limits[PSDB_PHY_CW_AG_20] == 0) {
        pw_limits->pw_limits[PSDB_PHY_CW_AG_20] = pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20];
    }

#define _CHAR_TO_PRINT_(c)  isprint(c) ? (c) : '.'

    ILOG1_CCDD("Country '%c%c' (0x%02X, 0x%02X)",
               _CHAR_TO_PRINT_(regd_parsed->country[0]),
               _CHAR_TO_PRINT_(regd_parsed->country[1]),
               regd_parsed->country[0], regd_parsed->country[1]);

    ILOG1_DDD("domain 0x%02X, chan_freq %d, chan_num %d",
              regd_parsed->regd_code, chan_freq, chan_num);

    ILOG2_DDDDDD("antennas factor: %d, 11B/OFDM power limits: %u %u %u %u %u",
                 tx_ant_factor, pw_limits->pw_limits[PSDB_PHY_CW_11B],
                 pw_limits->pw_limits[PSDB_PHY_CW_OFDM_20], pw_limits->pw_limits[PSDB_PHY_CW_OFDM_40],
                 pw_limits->pw_limits[PSDB_PHY_CW_OFDM_80], pw_limits->pw_limits[PSDB_PHY_CW_OFDM_160]);

#ifdef MTLK_WAVE_700
    ILOG1_DDDDD(" 11BE power limits: 20:%u ,40:%u ,80:%u ,160:%u ,320:%u",
                 pw_limits->pw_limits[PSDB_PHY_CW_BE_20],
                 pw_limits->pw_limits[PSDB_PHY_CW_BE_40], pw_limits->pw_limits[PSDB_PHY_CW_BE_80],
                 pw_limits->pw_limits[PSDB_PHY_CW_BE_160], pw_limits->pw_limits[PSDB_PHY_CW_BE_320]);
#endif

    return MTLK_ERR_OK;
}

hw_psdb_t *
mtlk_core_get_psdb (mtlk_core_t *core)
{
    mtlk_hw_t *hw;

    MTLK_ASSERT(core);
    hw = mtlk_vap_get_hw(core->vap_handle);
    MTLK_ASSERT(hw);
    return mtlk_hw_get_psdb(hw);
}

int
core_get_psdb_hw_limits (mtlk_core_t *core, const char *country, int chan_freq, psdb_pw_limits_t *pw_limits)
{
    MTLK_ASSERT(core);
    return psdb_get_hw_limits(core, country, chan_freq, pw_limits);
}

int __MTLK_IFUNC
mtlk_psdb_get_rate_power_list (mtlk_core_t *core, mtlk_clpb_t *clpb, mtlk_hw_band_e bandId)
{
    hw_psdb_t             *psdb;
    mtlk_rate_pw_entry_t  *entry;
    int                   res;
    unsigned              i;
    mtlk_rate_pw_entry_t *pwRateGen7 = NULL;

    MTLK_ASSERT(core);
    MTLK_ASSERT(clpb);

    psdb = mtlk_core_get_psdb(core);
    MTLK_ASSERT(psdb);

    /*
     * In WAVE700 family same PSD power offset table is used for 11AC, 11AX and 11BE
     * Max MCS Value allowed in each standard generation must be taken into account for storing
     * the power offsets
     *
     * For Gen7 values of 11be are band dependent and rehused for 11ac and 11ax phy modes
     * Overwrite the 11ac, 11ax phy modes with the values of 11 be of bandId (input argument)
    */
    MTLK_ASSERT(bandId < MTLK_HW_BAND_MAX);
    if (bandId == MTLK_HW_BAND_2_4_GHZ) pwRateGen7 = psdb->rate_pw_table->rates_gen7_2G;
    else if (bandId == MTLK_HW_BAND_5_2_GHZ) pwRateGen7 = psdb->rate_pw_table->rates_gen7_5G;
    else pwRateGen7 = psdb->rate_pw_table->rates_gen7_6G;

    MTLK_ASSERT(pwRateGen7);

    if (pwRateGen7->phy_mode == PHY_MODE_11BE) {
      /* If 11be phy Mode is present in PSD means we are in gen7 and we need to overwrite 11ac, 11ax modes */
      for (i = 0; i < PSDB_RATES_GEN7; i++) {
        if ((pwRateGen7[i].mcs < NUM_RATE_MCS_AC) && (i<PSDB_RATES_AC)) { // 11AC MCS0-9
          psdb->rate_pw_table->rates_11ac[i].mcs = pwRateGen7[i].mcs;
          psdb->rate_pw_table->rates_11ac[i].band = pwRateGen7[i].band;
          psdb->rate_pw_table->rates_11ac[i].nss = pwRateGen7[i].nss;
          psdb->rate_pw_table->rates_11ac[i].pw_offs = pwRateGen7[i].pw_offs;
          psdb->rate_pw_table->rates_11ac[i].phy_mode = PHY_MODE_AC;

          psdb->rate_pw_table->rates_mu[i].mcs = pwRateGen7[i].mcs;
          psdb->rate_pw_table->rates_mu[i].band = pwRateGen7[i].band;
          psdb->rate_pw_table->rates_mu[i].nss = pwRateGen7[i].nss;
          psdb->rate_pw_table->rates_mu[i].pw_offs = pwRateGen7[i].pw_offs;
          psdb->rate_pw_table->rates_mu[i].phy_mode = PHY_MODE_11AX_MU_DL;

          psdb->rate_pw_table->rates_bfac[i].mcs = pwRateGen7[i].mcs;
          psdb->rate_pw_table->rates_bfac[i].band = pwRateGen7[i].band;
          psdb->rate_pw_table->rates_bfac[i].nss = pwRateGen7[i].nss;
          psdb->rate_pw_table->rates_bfac[i].pw_offs = pwRateGen7[i].pw_offs;
          psdb->rate_pw_table->rates_bfac[i].phy_mode = PHY_MODE_11AC_BF_DL;
        }

        if ((pwRateGen7[i].mcs < NUM_RATE_MCS_AX) && (i<PSDB_RATES_AX)) { // 11AX MCS0-11
          psdb->rate_pw_table->rates_11ax[i].mcs = pwRateGen7[i].mcs;
          psdb->rate_pw_table->rates_11ax[i].band = pwRateGen7[i].band;
          psdb->rate_pw_table->rates_11ax[i].nss = pwRateGen7[i].nss;
          psdb->rate_pw_table->rates_11ax[i].pw_offs = pwRateGen7[i].pw_offs;
          psdb->rate_pw_table->rates_11ax[i].phy_mode = PHY_MODE_AX;

          psdb->rate_pw_table->rates_bf[i].mcs = pwRateGen7[i].mcs;
          psdb->rate_pw_table->rates_bf[i].band = pwRateGen7[i].band;
          psdb->rate_pw_table->rates_bf[i].nss = pwRateGen7[i].nss;
          psdb->rate_pw_table->rates_bf[i].pw_offs = pwRateGen7[i].pw_offs;
          psdb->rate_pw_table->rates_bf[i].phy_mode = PHY_MODE_11AX_BF_DL;        
        }

        /* 11be BF */
        psdb->rate_pw_table->rates_bfbe[i].mcs = pwRateGen7[i].mcs;
        psdb->rate_pw_table->rates_bfbe[i].band = pwRateGen7[i].band;
        psdb->rate_pw_table->rates_bfbe[i].nss = pwRateGen7[i].nss;
        psdb->rate_pw_table->rates_bfbe[i].pw_offs = pwRateGen7[i].pw_offs;
        psdb->rate_pw_table->rates_bfbe[i].phy_mode = PHY_MODE_BF_11BE;
      }
    }
    /* create returned data */
    for (i = 0; i < MTLK_ARRAY_SIZE(psdb->rate_pw_table->rates_all); i++) {
      entry = &psdb->rate_pw_table->rates_all[i];
      /* TBD: check for mode supported */

      if (MTLK_ERR_OK != (res = mtlk_clpb_push(clpb, entry, sizeof(*entry)))) {
        goto err_push;
      }
    }

    return MTLK_ERR_OK;

err_push:
    mtlk_clpb_purge(clpb);
    return res;
}

int __MTLK_IFUNC
mtlk_psdb_get_damage_power_limit (mtlk_core_t *core, mtlk_clpb_t *clpb, mtlk_hw_band_e bandId, uint8 max_ant_gain)
{
    hw_psdb_t *psdb;
    int       res;
    unsigned  limit;

    MTLK_ASSERT(core);
    MTLK_ASSERT(clpb);

    psdb = mtlk_core_get_psdb(core);
    MTLK_ASSERT(psdb);

    if (psdb->pw_limit_damage_table) {
      /* Power Limit is in half dBs but Tx Power target is in eighth, so convert value to correct format as limit is in dBm*/
      limit = STAT_PW_TO_POWER(psdb->pw_limit_damage_table->pw_limits[bandId]) + max_ant_gain; 
    } else {
      /* If Power Limit table does not exist, set default value to 0 */
      limit = 0;
    }

    if (MTLK_ERR_OK != (res = mtlk_clpb_push(clpb, &limit, sizeof(limit)))) {
      goto err_push;
    }

    return MTLK_ERR_OK;

err_push:
    mtlk_clpb_purge(clpb);
    return res;
}

uint32 __MTLK_IFUNC
wave_psdb_get_ant_gain_by_freq (hw_psdb_t *psdb, uint32 freq)
{
  psdb_ant_gain_per_freq_table_t *agpf;
  uint32  gain = 0;

  agpf = wave_psdb_get_ant_gain_per_freq_table(psdb);
  if (agpf) {
    psdb_gain_per_freq_t *entry;
    unsigned i;
    for (i = 0; i < agpf->nof_entries; i++) {
      entry = &agpf->entries[i];
      if (freq < entry->freq) {
        gain = entry->gain;
        break;
      }
    }
  }  else { /* Use default gain if table missing */
    gain = 0;
  }

  return gain;
}

int __MTLK_IFUNC
mtlk_psdb_get_ant_gain (mtlk_core_t *core, mtlk_clpb_t *clpb, int chan, mtlk_hw_band_e bandId)
{
    hw_psdb_t *psdb;
    int       res;
    unsigned  gain;
    uint32 regd_code;

    MTLK_ASSERT(core);
    MTLK_ASSERT(clpb);

    psdb = mtlk_core_get_psdb(core);
    MTLK_ASSERT(psdb);
    regd_code = wave_core_cfg_get_regd_region(core);
    // Antenna Gain must only be taken into account in ETSI (all bands) and in FCC_LPI (only in 6Ghz)
    if ((regd_code == REG_DOMAIN_ETSI) || ((bandId == MTLK_HW_BAND_6_GHZ) && (REGD_CODE_FCC_LPI == core_cfg_get_regd_code(core)))) {
      gain = wave_psdb_get_ant_gain_by_freq(psdb, channel_to_frequency(chan,bandId));
    }
    else {
      gain = 0;
    }

    if (MTLK_ERR_OK != (res = mtlk_clpb_push(clpb, &gain, sizeof(gain)))) {
      goto err_push;
    }

    return MTLK_ERR_OK;

err_push:
    mtlk_clpb_purge(clpb);
    return res;
}

/* Beamforming capabilities related API */

uint32
psdb_get_bf_capabilities (hw_psdb_t *psdb)
{
    MTLK_ASSERT(psdb);
    return psdb->beamform_caps;
}

uint32
core_get_psdb_bf_capabilities (mtlk_core_t *core)
{
    MTLK_ASSERT(core);
    return psdb_get_bf_capabilities(mtlk_core_get_psdb(core));
}

BOOL
psdb_get_bf_explicit_cap (hw_psdb_t *psdb)
{
    MTLK_ASSERT(psdb);
    return (0 != MTLK_BFIELD_GET(psdb_get_bf_capabilities(psdb), PSDB_BF_CAPS_EXPLICIT_SUPPORT));
}

int __MTLK_IFUNC
mtlk_psdb_get_hw_limits_list (mtlk_core_t *core, mtlk_clpb_t *clpb)
{
    mtlk_hw_limits_stat_entry_t stat_entry;
    psdb_any_table_t  *regd_table;
    psdf_regd_entry_t *regd_entries;
    mtlk_hw_t         *hw;
    int                res = MTLK_ERR_UNKNOWN;
    unsigned           i;

    MTLK_ASSERT(core);
    MTLK_ASSERT(clpb);

    hw = mtlk_vap_get_hw(core->vap_handle);
    MTLK_ASSERT(hw);

    regd_table = mtlk_hw_get_psdb_regd_table(hw);
    if (!regd_table) {
          WLOG_V("No HW limits available for this platform");
          return MTLK_ERR_UNKNOWN;
    }

    /* create returned data */
    regd_entries = regd_table->data.regd_entries;
    for (i = 0; i < regd_table->nrows; i++) {
      stat_entry.reg_domain = regd_entries[i].regd_code;
      stat_entry.freq       = regd_entries[i].chan_freq;
      stat_entry.spectrum   = regd_entries[i].chanwidth;
      stat_entry.phy_mode   = regd_entries[i].phy_mode;
      stat_entry.tx_lim     = regd_entries[i].pw_limit;

      if (MTLK_ERR_OK != (res = mtlk_clpb_push(clpb, &stat_entry, sizeof(stat_entry)))) {
        goto err_push;
      }
    }

    return MTLK_ERR_OK;

err_push:
    mtlk_clpb_purge(clpb);
    return res;
}

/* ********** DEBUG APIs ********** */
#ifdef CONFIG_WAVE_DEBUG
BOOL
core_get_psdb_bf_explicit_cap (mtlk_core_t *core)
{
    MTLK_ASSERT(core);
    return (0 != MTLK_BFIELD_GET(core_get_psdb_bf_capabilities(core), PSDB_BF_CAPS_EXPLICIT_SUPPORT));
}
#endif

static int
_mtlk_psdb_parse_rate_pw_table (psdb_rate_pw_table_t *rate_pw_table, psdb_any_table_t *src_table, uint32 fld_id)
{
    psdf_rate_pw_entry_t  *psdb_entry;
    mtlk_rate_pw_entry_t  *fill_entry = NULL;
    uint32                 ncols = 0, nrows = 0, phy_mode = 0, i;
    mtlk_hw_band_e         band  = MTLK_HW_BAND_NONE;

    MTLK_ASSERT(rate_pw_table);
    MTLK_ASSERT(src_table);

    ILOG2_DDD("Table ID 0x%08X sizes %dx%d", fld_id, src_table->nrows, src_table->ncols);

    /* Check for table sizes */
#define NWORDS_PER_ENTRY  (sizeof (psdf_rate_pw_entry_t) / sizeof (uint32))

    /* expected sizes */
    switch (fld_id) {
    case PSDB_FIELD_POWER_ADAPT_11B:
      phy_mode = PHY_MODE_B;  fill_entry = rate_pw_table->rates_11b;  nrows = PSDB_RATES_B;  ncols = NWORDS_PER_ENTRY;
      break;
    case PSDB_FIELD_POWER_ADAPT_11AG:
      phy_mode = PHY_MODE_AG; fill_entry = rate_pw_table->rates_11ag; nrows = PSDB_RATES_AG; ncols = NWORDS_PER_ENTRY;
      break;
    case PSDB_FIELD_POWER_ADAPT_11N:
      phy_mode = PHY_MODE_N;  fill_entry = rate_pw_table->rates_11n;  nrows = PSDB_RATES_N;  ncols = NWORDS_PER_ENTRY;
      break;
    case PSDB_FIELD_POWER_ADAPT_11AC: /* One row per all NSS */
      phy_mode = PHY_MODE_AC; fill_entry = rate_pw_table->rates_11ac;
      nrows = (PSDB_RATES_AC    / PHY_STAT_RATE_NSS_MAX);
      ncols = (NWORDS_PER_ENTRY * PHY_STAT_RATE_NSS_MAX);
      break;
    case PSDB_FIELD_POWER_ADAPT_11AX: /* One row per all NSS */
      phy_mode = PHY_MODE_AX; fill_entry = rate_pw_table->rates_11ax;
      nrows = (PSDB_RATES_AX / PHY_STAT_RATE_NSS_MAX);
      ncols = (NWORDS_PER_ENTRY * PHY_STAT_RATE_NSS_MAX);
      break;
    /*
    * In WAVE700 family same PSD power offset table is used for 11AC, 11AX and 11BE
    * Max MCS Value allowed in each standard generation must be taken into account for storing 
    * the power offsets 
    * 
    * Power Adaptation Table in W700 is per band so we have 3 tables 
    */  
    case PSDB_FIELD_POWER_ADAPT_GEN7_2G: /* One row per all NSS */
      phy_mode = PHY_MODE_11BE; fill_entry = rate_pw_table->rates_gen7_2G;
      nrows = (PSDB_RATES_GEN7 / PHY_STAT_RATE_NSS_MAX);
      ncols = (NWORDS_PER_ENTRY * PHY_STAT_RATE_NSS_MAX);
      band = MTLK_HW_BAND_2_4_GHZ;
      break;
    case PSDB_FIELD_POWER_ADAPT_GEN7_5G: /* One row per all NSS */
      phy_mode = PHY_MODE_11BE; fill_entry = rate_pw_table->rates_gen7_5G;
      nrows = (PSDB_RATES_GEN7 / PHY_STAT_RATE_NSS_MAX);
      ncols = (NWORDS_PER_ENTRY * PHY_STAT_RATE_NSS_MAX);
      band = MTLK_HW_BAND_5_2_GHZ;
      break;   
    case PSDB_FIELD_POWER_ADAPT_GEN7_6G: /* One row per all NSS */
      phy_mode = PHY_MODE_11BE; fill_entry = rate_pw_table->rates_gen7_6G;
      nrows = (PSDB_RATES_GEN7 / PHY_STAT_RATE_NSS_MAX);
      ncols = (NWORDS_PER_ENTRY * PHY_STAT_RATE_NSS_MAX);
      band = MTLK_HW_BAND_6_GHZ;
      break;
    default: /* Never */
      break;
    }

    if ((nrows != src_table->nrows) || (ncols != src_table->ncols)) {
      ELOG_DDDDD("Table ID 0x%08X has wrong sizes %dx%d (%dx%d expected)",
                 fld_id, src_table->nrows, src_table->ncols, nrows, ncols);
      return MTLK_ERR_PARAMS;
    }

    /* Fill the table */
    MTLK_ASSERT(fill_entry);
    psdb_entry = &src_table->data.rate_pw_entries[0];
    for (i = 0; i < nrows; i++) {
      if ((PHY_MODE_AC == phy_mode) || (PHY_MODE_AX == phy_mode) || (PHY_MODE_11BE == phy_mode)) {
        uint32 n;
        for (n = 1; n <= PHY_STAT_RATE_NSS_MAX; n++) {
          fill_entry->phy_mode = phy_mode;
          fill_entry->pw_offs  = STAT_PW_TO_POWER(MAC_TO_HOST32(psdb_entry->pw_offs));
          fill_entry->mcs = i;
          fill_entry->nss = n;
          fill_entry->band = band;
          if(PHY_MODE_AC == phy_mode) {
            (fill_entry+PSDB_RATES_AC)->phy_mode = PHY_MODE_11AX_MU_DL;
            (fill_entry+PSDB_RATES_AC)->pw_offs  = STAT_PW_TO_POWER(MAC_TO_HOST32(psdb_entry->pw_offs));
            (fill_entry+PSDB_RATES_AC)->mcs = i;
            (fill_entry+PSDB_RATES_AC)->nss = n;
            (fill_entry+PSDB_RATES_AC+PSDB_RATES_AC)->phy_mode = PHY_MODE_11AC_BF_DL;
            (fill_entry+PSDB_RATES_AC+PSDB_RATES_AC)->pw_offs  = STAT_PW_TO_POWER(MAC_TO_HOST32(psdb_entry->pw_offs));
            (fill_entry+PSDB_RATES_AC+PSDB_RATES_AC)->mcs = i;
            (fill_entry+PSDB_RATES_AC+PSDB_RATES_AC)->nss = n;
          }
          if(PHY_MODE_AX == phy_mode) {
            (fill_entry+PSDB_RATES_AX)->phy_mode = PHY_MODE_11AX_BF_DL;
            (fill_entry+PSDB_RATES_AX)->pw_offs  = STAT_PW_TO_POWER(MAC_TO_HOST32(psdb_entry->pw_offs));
            (fill_entry+PSDB_RATES_AX)->mcs = i;
            (fill_entry+PSDB_RATES_AX)->nss = n;
          }
          ++psdb_entry;
          ++fill_entry;
        }
      } else { /* B/AG/N */
        fill_entry->phy_mode = phy_mode;
        fill_entry->pw_offs  = STAT_PW_TO_POWER(MAC_TO_HOST32(psdb_entry->pw_offs));
        fill_entry->mcs = i;
        fill_entry->nss = BITRATE_MCS_TO_NSS(i);
        ++psdb_entry;
        ++fill_entry;
      }
    }

#undef NWORDS_PER_ENTRY

    return MTLK_ERR_OK;
}

static int
_mtlk_psdb_check_table_sizes (const char *name, psdb_any_table_t *table, uint32 nrows, uint32 ncols)
{
    /* Zero value of input nrows/ncols means don't check this size */
    /* In this case, use table's size instead to be equal */
    if (0 == nrows) nrows = table->nrows;
    if (0 == ncols) ncols = table->ncols;

    if ((nrows != table->nrows) || (ncols != table->ncols)) {
      ELOG_SDDDDD("%s Table ID 0x%08X has wrong sizes %dx%d (%dx%d expected)",
                                name, table->id, table->nrows, table->ncols, nrows, ncols);
      return MTLK_ERR_PARAMS;
    }

    return MTLK_ERR_OK;
}

static int
_mtlk_psdb_parse_cdb_cfg_table (mtlk_hw_t *hw, hw_psdb_t *psdb, psdb_any_table_t *table, bool band6g_supported)
{
  psdb_cdb_cfg_table_t  *cdb_table;
  wave_cdb_cfg_e         cfg;
  int res = MTLK_ERR_OK;
  unsigned i;
  uint32 psd_ncols = PSDB_FIELD_CDB_CFG_NCOLS;

  MTLK_ASSERT(hw);

  if (mtlk_hw_type_is_gen7(hw)) {
  /* Skip the table size check for WAVE700 EMUL/FPGA DUT */
    psd_ncols = (wave_hw_mmb_card_is_asic(hw) ? PSDB_FIELD_CTB_CFG_NCOLS : 0);
  }

  res = _mtlk_psdb_check_table_sizes("CDB configuration",
                                     table, PSDB_FIELD_CDB_CFG_NROWS,
                                     psd_ncols);
  if (MTLK_ERR_OK != res) {
    return res;
  }

  for (i = 0; i < table->ncols; i++) {
    cfg = MAC_TO_HOST32(table->data.words[i]);

    /* TODO: FIX ME: 6GHz band support hack *
     * 624B0 card can be operated on 5GHz band OR          *
     * recalibrated to operate on 6GHz band (upto 6.4GHz). *
     * Hence the CDB configuration from the PSD file       *
     * cannot be distinguished between 5GHz or 6GHz band.  *
     * The band information from the CAL file is used here *
     * to distinguish between the 5GHz and 6GHz bands      *
     */
    if((i == 0) && (cfg == WAVE_CDB_CFG_5G) && (band6g_supported))
      cfg = WAVE_CDB_CFG_6G_UNII5;

    ILOG0_DDS("CDB config[%d] = %d (%s)", i, cfg, wave_cdb_cfg_to_string(cfg));
    if (wave_cdb_cfg_is_unknown(cfg)) {
      ELOG_DD("Unknown CDB config[%d] = %d", i, cfg);
      return MTLK_ERR_PARAMS;
    }
    cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
    cdb_table->radio_cfg[i].cdb_config = cfg;
  }

  return MTLK_ERR_OK;
}

static int
_mtlk_psdb_parse_cca_th_table (hw_psdb_t *psdb, psdb_any_table_t *table)
{
    psdb_cdb_cfg_table_t  *cdb_table;
    int res = MTLK_ERR_OK;
    int i;
    int32 *ptr_val = NULL;

    res = _mtlk_psdb_check_table_sizes("CcaTh configuration",
              table, MTLK_CCA_TH_PARAMS_LEN, 0);
    if (MTLK_ERR_OK != res) {
      return res;
    }

    /* Apply endian conversion to the table content */
    wordcpy_mac_to_host((uint32 *)&table->data, (uint32 *)&table->data, table->nof_words);

    cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
  
    /* Copy the CCA thresholds of 5GHz band to the 6GHz band if CCA thresholds are not available for 6GHz in PSD */
 
    ILOG1_DD("CCA Thresholds table: nrows = %d, ncols = %d", table->nrows, table->ncols);
    ptr_val = &table->data.cca_th_entries[0].value_2G;

    for (i = 0; i < MTLK_CCA_TH_PARAMS_LEN; i++) {
      cdb_table->cca_th_params[MTLK_HW_BAND_2_4_GHZ][i] = (int8)(*ptr_val++);
      cdb_table->cca_th_params[MTLK_HW_BAND_5_2_GHZ][i] = (int8)(*ptr_val++);

     if (table->ncols == MTLK_HW_BAND_MAX)
       cdb_table->cca_th_params[MTLK_HW_BAND_6_GHZ][i] = (int8)(*ptr_val++);
     else
       cdb_table->cca_th_params[MTLK_HW_BAND_6_GHZ][i] = cdb_table->cca_th_params[MTLK_HW_BAND_5_2_GHZ][i];
     
     ILOG1_DDDD("CCA_TH[%d]: 2G: %d,  5G: %d, 6G: %d", i,
                 cdb_table->cca_th_params[MTLK_HW_BAND_2_4_GHZ][i],
                 cdb_table->cca_th_params[MTLK_HW_BAND_5_2_GHZ][i],
                 cdb_table->cca_th_params[MTLK_HW_BAND_6_GHZ][i]);
    }

    return MTLK_ERR_OK;
}

/* Update CDB config after all parsing completed */

#define _HW_ANT_MAX_NUM_    (5)
#define _HW_ANT_MASK_(n)    (1 << (1 * (n))) /* One bit  Antenna Mask */
#define _HW_ANT_SMSK_(n)    (1 << (2 * (n))) /* Two bits AntSelection Mask */

uint16 __MTLK_IFUNC
wave_psdb_ant_mask_to_ant_sel_mask (uint8 ant_mask)
{
  uint16 sel_mask = 0;
  uint8 i;

  for (i = 0; i < _HW_ANT_MAX_NUM_; i++) {
    if (ant_mask &  _HW_ANT_MASK_(i)) {
        sel_mask |= _HW_ANT_SMSK_(i);
    }
  }

  return sel_mask;
}

/* FIXME: should be taken from where ? */
#define WAVE_CHAN_NONE_LOWER    0
#define WAVE_CHAN_NONE_UPPER  255
#define WAVE_CHAN_2G_LOWER      1
#define WAVE_CHAN_2G_UPPER     14
#define WAVE_CHAN_5G_LOWER     36
#define WAVE_CHAN_5GL_UPPER    64
#define WAVE_CHAN_5GH_LOWER   100
#define WAVE_CHAN_5G_UPPER    165

#define WAVE_CHAN_5GL_LOWER      WAVE_CHAN_5G_LOWER
#define WAVE_CHAN_5GH_UPPER      WAVE_CHAN_5G_UPPER

static struct cdb_cfg_radio_params {
  mtlk_hw_band_e  wave_band;
  uint8       lower_chan;
  uint8       upper_chan;
} cdb_cfg_radio_params_table[WAVE_CDB_CFG_LAST] = {
  [WAVE_CDB_CFG_INVALID]  = { MTLK_HW_BAND_NONE   , WAVE_CHAN_NONE_LOWER, WAVE_CHAN_NONE_UPPER },
  [WAVE_CDB_CFG_2G     ]  = { MTLK_HW_BAND_2_4_GHZ, WAVE_CHAN_2G_LOWER,   WAVE_CHAN_2G_UPPER   },
  [WAVE_CDB_CFG_5G     ]  = { MTLK_HW_BAND_5_2_GHZ, WAVE_CHAN_5G_LOWER,   WAVE_CHAN_5G_UPPER   },
  [WAVE_CDB_CFG_5G_LOW ]  = { MTLK_HW_BAND_5_2_GHZ, WAVE_CHAN_5GL_LOWER,  WAVE_CHAN_5GL_UPPER  },
  [WAVE_CDB_CFG_5G_HIGH]  = { MTLK_HW_BAND_5_2_GHZ, WAVE_CHAN_5GH_LOWER,  WAVE_CHAN_5GH_UPPER  },
  [WAVE_CDB_CFG_ALTERN ]  = { MTLK_HW_BAND_NONE   , WAVE_CHAN_NONE_LOWER, WAVE_CHAN_NONE_UPPER },    /* TBD */
  [WAVE_CDB_CFG_6G_UNII5] = { MTLK_HW_BAND_6_GHZ  , WAVE_CHAN_6G_LOWER,   WAVE_CHAN_6G_UPPER_UNII5}, /* U-NII-5 6GHz support */
  [WAVE_CDB_CFG_6G]       = { MTLK_HW_BAND_6_GHZ  , WAVE_CHAN_6G_LOWER,   WAVE_CHAN_6G_UPPER},       /* Full 6GHz support */
  [WAVE_CDB_CFG_NOT_SET]  = { MTLK_HW_BAND_NONE   , WAVE_CHAN_NONE_LOWER, WAVE_CHAN_NONE_UPPER }
};

static int
_mtlk_psdb_update_cdb_cfg_table (mtlk_hw_api_t *hw_api, hw_psdb_t *psdb, uint32 hw_rev)
{
  psdb_cdb_cfg_table_t  *cdb_table;
  psdb_radio_cfg_t      *radio_cfg;
  int i;
  uint8 max_radio = WAVE_CARD_RADIO_NUM_MAX;

  if (mtlk_hw_type_is_gen6_d2(hw_api->hw)) {
    if (wave_hw_get_eeprom_rx_ant_num(hw_api->hw) != MAX_NUM_RX_ANTENNAS_665)
      max_radio = WAVE_CARD_RADIO_NUM_MIN;
    else
      max_radio = WAVE_CARD_RADIO_NUM_MAX_G6;
  }
  /* Use card config of 1st radio if CDB config was not set (missing) */
  cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
  if (mtlk_hw_type_is_gen6(hw_api->hw) && mtlk_mmb_drv_get_disable_zwdfs() && cdb_table->zwdfs_ant_mask != 0) {
    max_radio = WAVE_CARD_RADIO_NUM_MIN;
  }
  radio_cfg = &cdb_table->radio_cfg[0];

  if (wave_cdb_cfg_is_not_set(radio_cfg->cdb_config)) {
    int idx = 0;

    WLOG_DDD("PSDB CDB config table missing. Use card config: band support 2.4G:%d  5G:%d  6G:%d",
            (int)cdb_table->band_support_2G, (int)cdb_table->band_support_5G, (int)cdb_table->band_support_6G);

    /* Card band support */
    if ((!cdb_table->band_support_2G) && (!cdb_table->band_support_5G) && (!cdb_table->band_support_6G)) {
      /* None bands supported - configure 1 radio */
      radio_cfg = &cdb_table->radio_cfg[idx++];
      radio_cfg->radio_ant_cfg = cdb_table->card_ant_cfg; /* struct copy */
      radio_cfg->cdb_config = WAVE_CDB_CFG_INVALID;
    } else { /* supported */
      if (cdb_table->band_support_2G) {
        radio_cfg = &cdb_table->radio_cfg[idx++];
        radio_cfg->cdb_config = WAVE_CDB_CFG_2G;
      }
      if (cdb_table->band_support_5G) {
        radio_cfg = &cdb_table->radio_cfg[idx++];
        radio_cfg->cdb_config = WAVE_CDB_CFG_5G;
      }
      if (cdb_table->band_support_6G) {
        radio_cfg = &cdb_table->radio_cfg[idx++];
        radio_cfg->cdb_config = ((hw_rev == HW_VERS_600_GPB665_wrx665_6G) ?
                                 WAVE_CDB_CFG_6G : WAVE_CDB_CFG_6G_UNII5);
        /* To-do: need to add support for WAVE700 for 6G  */
      }
    }
    cdb_table->nof_entries = idx; /* filled radios */
  } else {
    /* CDB table was parsed */
    for(i = 0; i < max_radio; i++) {
      radio_cfg = &cdb_table->radio_cfg[i];
      if (wave_cdb_cfg_is_invalid(radio_cfg->cdb_config) || wave_cdb_cfg_is_not_set(radio_cfg->cdb_config)) {
        break;
      }
      cdb_table->nof_entries = (i + 1);
    }
    /* Make sure that minimum one valid radio is present */
    if (cdb_table->nof_entries == 0) {
      ELOG_V("There is no valid radio present");
      return MTLK_ERR_PARAMS;
    }
  }

  /* Update filled radio configs */
  for (i = 0; i < cdb_table->nof_entries; i++) {
    struct cdb_cfg_radio_params *radio_params;
    wave_ant_params_t           *radio_ant_cfg;

    radio_cfg     = &cdb_table->radio_cfg[i];
    radio_ant_cfg = &radio_cfg->radio_ant_cfg;
    radio_params  = &cdb_cfg_radio_params_table[radio_cfg->cdb_config];

    radio_cfg->lower_chan = radio_params->lower_chan;
    radio_cfg->upper_chan = radio_params->upper_chan;
    radio_cfg->wave_band  = radio_params->wave_band;

    /* Set Band support if is it not set before */
    if (radio_cfg->wave_band == MTLK_HW_BAND_NONE) {
        radio_cfg->wave_band  = radio_params->wave_band;
    }

    radio_ant_cfg->tx_ant_mask     = wave_psdb_get_ant_mask_by_channel(psdb, radio_params->lower_chan);
    radio_ant_cfg->tx_ant_num      = count_bits_set(radio_ant_cfg->tx_ant_mask);
    radio_ant_cfg->tx_ant_factor   = mtlk_antennas_factor(radio_ant_cfg->tx_ant_num);
    radio_ant_cfg->tx_ant_sel_mask = (uint8)wave_psdb_ant_mask_to_ant_sel_mask(radio_ant_cfg->tx_ant_mask);

    if (mtlk_hw_type_is_gen6_d2(hw_api->hw))
    {
      radio_ant_cfg->rx_ant_mask     = wave_psdb_get_card_ant_params(psdb)->rx_ant_mask;
      radio_ant_cfg->rx_ant_num      = count_bits_set(radio_ant_cfg->rx_ant_mask);
      radio_ant_cfg->rx_ant_sel_mask = wave_psdb_ant_mask_to_ant_sel_mask(radio_ant_cfg->rx_ant_mask);
    } else {
    /* Use TX params for RX too */
      radio_ant_cfg->rx_ant_mask     = radio_ant_cfg->tx_ant_mask;
      radio_ant_cfg->rx_ant_num      = radio_ant_cfg->tx_ant_num;
      radio_ant_cfg->rx_ant_sel_mask = radio_ant_cfg->tx_ant_sel_mask;
    }

    ILOG0_DDDD("[%d] band %u, channels %u ... %u", i,
               radio_cfg->wave_band, radio_cfg->lower_chan, radio_cfg->upper_chan);

    ILOG0_DDDDD("[%d] Antennas number %u, mask %02X, sel_mask %02X, factor %u", i,
                radio_ant_cfg->tx_ant_num,
                radio_ant_cfg->tx_ant_mask,
                radio_ant_cfg->tx_ant_sel_mask,
                radio_ant_cfg->tx_ant_factor);
  }

  return MTLK_ERR_OK;
}

wave_cdb_cfg_e __MTLK_IFUNC
wave_psdb_get_radio_cdb_config (hw_psdb_t *psdb, unsigned idx)
{
  wave_cdb_cfg_e        config;
  psdb_cdb_cfg_table_t *cdb_table;

  MTLK_ASSERT(psdb);

  cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
  if (wave_psdb_cdb_cfg_idx_is_valid(cdb_table, idx)) {
    config = cdb_table->radio_cfg[idx].cdb_config;
  } else {
    config = WAVE_CDB_CFG_INVALID;
  }

  return config;
}

/* Use TX antennas mask of card as default mask */
static uint32
_wave_psdb_get_default_ant_mask (hw_psdb_t *psdb)
{
  return wave_psdb_get_card_ant_params(psdb)->tx_ant_mask;
}

static int
_mtlk_psdb_parse_ant_mask_per_chan_table (hw_psdb_t *psdb, psdb_any_table_t *src_table)
{
  psdb_ant_mask_per_chan_table_t *ampc;
  uint32    nof_entries, i;
  int       res = MTLK_ERR_OK;

  MTLK_ASSERT(psdb);
  MTLK_ASSERT(src_table);


  if (psdb->ant_mask_per_chan) {
    ELOG_V("Duplicate Antenna Mask per Channel table");
    return MTLK_ERR_PARAMS;
  }

  res = _mtlk_psdb_check_table_sizes("Antenna mask per channel",
                                     src_table, 0, PSDB_AMPC_TABLE_NCOLS);
  if (MTLK_ERR_OK != res) {
    return res;
  }

  /* Allocate a table for Driver purpose */
  nof_entries = src_table->nrows;
  ampc = PSDB_ALLOC(sizeof(*ampc) + (nof_entries * sizeof(ampc->entries[0])));
  if (!ampc) {
    return MTLK_ERR_NO_MEM;
  }

  psdb->ant_mask_per_chan = ampc;
  ampc->nof_entries = nof_entries;
  for (i = 0; i < ampc->nof_entries; i++) {
    psdb_mask_per_chan_t *entry = &ampc->entries[i];
    entry->upper_channel = MAC_TO_HOST32(src_table->data.mask_per_chan_entries[i].upper_channel);
    entry->ant_mask      = MAC_TO_HOST32(src_table->data.mask_per_chan_entries[i].ant_mask);
    ILOG0_DD("Channel until %3u (including) mask is 0x%02X",
             entry->upper_channel, entry->ant_mask);
    /* All values should by uint8 */
    if (MAX_UINT8 < (entry->upper_channel | entry->ant_mask)) {
      ELOG_DD("Atennas Mask per Channel: value of Channel (%u) or/and Mask (%u) too big",
             entry->upper_channel, entry->ant_mask);
      return MTLK_ERR_PARAMS;
    }
  }

  return res;
}

static int
_mtlk_psdb_parse_pw_limit_damage_table (hw_psdb_t *psdb, psdb_any_table_t *src_table)
{
  psdb_pw_limit_damage_table_t *pld;
  uint8    nof_entries;
  int       res = MTLK_ERR_OK;

  MTLK_ASSERT(psdb);
  MTLK_ASSERT(src_table);

  if (psdb->pw_limit_damage_table) {
    ELOG_V("Duplicate Power Limit Damage table");
    return MTLK_ERR_PARAMS;
  }

  res = _mtlk_psdb_check_table_sizes("Power Limit Damage",
                                     src_table, PSDB_FIELD_POWER_LIMIT_DAMAGE_NROWS, PSDB_FIELD_POWER_LIMIT_DAMAGE_NCOLS);
  if (MTLK_ERR_OK != res) {
    return res;
  }

  /* Allocate a table for Driver purpose */
  nof_entries = src_table->ncols;
  if (nof_entries != MTLK_HW_BAND_MAX) {
    ELOG_V("Wrong size for Power Limit Damage table");
    return MTLK_ERR_PARAMS;
  }
  pld = PSDB_ALLOC(sizeof(*pld) + (nof_entries * sizeof(pld->pw_limits[0])));
  if (!pld) {
    return MTLK_ERR_NO_MEM;
  }

  psdb->pw_limit_damage_table = pld;
  pld->pw_limits[MTLK_HW_BAND_2_4_GHZ] = MAC_TO_HOST32(src_table->data.words[0]);
  pld->pw_limits[MTLK_HW_BAND_5_2_GHZ] = MAC_TO_HOST32(src_table->data.words[1]);
  pld->pw_limits[MTLK_HW_BAND_6_GHZ]   = MAC_TO_HOST32(src_table->data.words[2]);

  return res;
}

static int 
_mtlk_psdb_round_to_upper_half_db(int power)
{
  if ((power % 8) > 4)
  {
     power =((power >> 3) * 8) + 8;
  }
  else if ((power % 8 ) > 0)
  {
     power =((power >> 3) * 8) + 4;
  }
  return power;
}

static int
_mtlk_psdb_parse_ant_gain_per_freq_table (mtlk_hw_t *hw, hw_psdb_t *psdb, psdb_any_table_t *src_table)
{
  psdb_ant_gain_per_freq_table_t *agpf;
  uint32    nof_entries, i;
  int       res = MTLK_ERR_OK;
  uint32    psd_nrows = PSDB_FIELD_ANTENNA_GAIN_W600_NROWS;

  MTLK_ASSERT(psdb);
  MTLK_ASSERT(src_table);
  MTLK_ASSERT(hw);

  if (psdb->ant_gain_per_freq) {
    ELOG_V("Duplicate Antenna Gain table");
    return MTLK_ERR_PARAMS;
  }

  if (mtlk_hw_type_is_gen7(hw)) {
     psd_nrows = PSDB_FIELD_ANTENNA_GAIN_W700_NROWS;
  }

  res = _mtlk_psdb_check_table_sizes("Antenna gain per frequency",
                                     src_table, psd_nrows, PSDB_FIELD_ANTENNA_GAIN_NCOLS);
  if (MTLK_ERR_OK != res) {
    return res;
  }

  /* Allocate a table for Driver purpose */
  nof_entries = src_table->nrows;
  agpf = PSDB_ALLOC(sizeof(*agpf) + (nof_entries * sizeof(agpf->entries[0])));
  if (!agpf) {
    return MTLK_ERR_NO_MEM;
  }

  psdb->ant_gain_per_freq = agpf;
  agpf->nof_entries = nof_entries;
  for (i = 0; i < agpf->nof_entries; i++) {
    psdb_gain_per_freq_t *entry = &agpf->entries[i];
    entry->freq = MAC_TO_HOST32(src_table->data.gain_per_freq_entries[i].freq);
    entry->gain = _mtlk_psdb_round_to_upper_half_db(MAC_TO_HOST32(src_table->data.gain_per_freq_entries[i].gain));
    ILOG2_DD("Antenna gain until frequency %u Hz (including) is %u", entry->freq, entry->gain);
  }

  return res;
}

uint8 __MTLK_IFUNC
wave_psdb_get_hw_radio_band_cfg(mtlk_hw_t* hw)
{
  const wave_ant_params_t *params = NULL;
  uint8 ant_num_radio_0 = 0, ant_num_radio_1 = 0;
  unsigned nof_radios;

  hw_psdb_t *psdb = mtlk_hw_get_psdb(hw);

  MTLK_ASSERT(psdb);

  wave_psdb_cfg_radio_number_get(psdb, &nof_radios);

  if (nof_radios == 1)
    return WAVE_HW_RADIO_BAND_CFG_SB;

  MTLK_ASSERT((nof_radios == WAVE_CARD_RADIO_NUM_MAX_G6 || nof_radios == WAVE_CARD_RADIO_NUM_MAX_G7));

  if(mtlk_hw_type_is_gen7(hw))
  {
    //TODO Dual band support 4x4
    if (nof_radios == WAVE_CARD_RADIO_NUM_MAX_G7)
      return WAVE_HW_RADIO_BAND_CFG_CTB;
    else if (nof_radios == WAVE_CARD_RADIO_NUM_MAX_G6)
      return WAVE_HW_RADIO_BAND_CFG_DB_4x4;
  }

  params = wave_psdb_get_radio_ant_params(psdb, 0);
  if (!params)
    goto end;
  ant_num_radio_0 = params->tx_ant_num;
  params = wave_psdb_get_radio_ant_params(psdb, 1);
  if (!params)
    goto end;
  ant_num_radio_1 = params->tx_ant_num;

  if (__wave_psdb_get_cdb_cfg_table(psdb)->zwdfs_ant_mask)
    return WAVE_HW_RADIO_BAND_CFG_SCAN;
  else if (ant_num_radio_0 == 2 && ant_num_radio_1 == 2)
    return WAVE_HW_RADIO_BAND_CFG_DB_2x2;
  else if (ant_num_radio_0 == 3 && ant_num_radio_1 == 1)
    return WAVE_HW_RADIO_BAND_CFG_DB_3x1;
  else if (ant_num_radio_0 == 4 && ant_num_radio_1 == 1)
    return WAVE_HW_RADIO_BAND_CFG_DB_4x1;

end:
  return WAVE_HW_RADIO_BAND_CFG_UNSUPPORTED;
}

uint32 __MTLK_IFUNC
wave_psdb_get_ant_mask_by_channel (hw_psdb_t *psdb, uint32 channel)
{
  psdb_ant_mask_per_chan_table_t *ampc;
  uint32  mask = 0;

  ampc = wave_psdb_get_ant_mask_per_chan_table(psdb);
  if (ampc) {
    psdb_mask_per_chan_t *entry;
    unsigned i;
    for (i = 0; i < ampc->nof_entries; i++) {
      entry = &ampc->entries[i];
      if (channel < entry->upper_channel) {
        mask = entry->ant_mask;
        break;
      }
    }
  } else { /* Use default mask if table missing */
    mask = _wave_psdb_get_default_ant_mask(psdb);
  }

  return mask;
}

/* Copy and Parse Fields array, allocate all the tables, fill psdb */
static int
psdb_parse_fields_array (mtlk_hw_api_t *hw_api, uint32 *wbuf, uint32 wsize, uint32 offs,
                         hw_psdb_t *psdb, bool band6g_supported, uint32 chip_id)
{
    wave_ant_params_t   *ant_params;
    psdb_fields_array_t *fld;
    psdb_any_table_t    *tmptab;
    int                  nfld = 0;                  /* Number of fileds */
    int                  ntab = 0;                  /* Number of tables */
    uint32               fld_id, fld_val;
    int                  size;
    int                  i;
    uint8                max_rx_ant = 0;

    MTLK_STATIC_ASSERT(NUM_WORDS_PER_FIELDS_BLOCK == (sizeof(psdf_fields_block_t) / sizeof(uint32)));
    MTLK_STATIC_ASSERT(NUM_WORDS_PER_REGD_ENTRY   == (sizeof(psdf_regd_entry_t)   / sizeof(uint32)));

    MTLK_ASSERT(NULL != psdb);

    if (_chipid_is_gen6_d2(chip_id)) {
      max_rx_ant = MAX_NUM_RX_ANTENNAS_665;
    } else {
      max_rx_ant = MAX_NUM_RX_ANTENNAS;
    }

    psdb->regd_table = NULL;
    psdb->nof_tables = 0;

    psdb->cdb_cfg_table.cca_th_is_filled = FALSE;

    /* Copy Fields array similar to any table, specify MARKER as ID */
    fld = (psdb_fields_array_t *)psdb_copy_table(wbuf, wsize, PSDB_PLATFORM_MARKER, offs, PSDB_PLATFORM_MARKER);
    if (!fld) {
        goto ERROR;
    }

    psdb->fields_data = fld;

    /* Only single_word fields have to be stored and passed to FW */
    /* Calculate number of such words, all others are tables */
    for (i = 0; i < (int)fld->nrows; i++) {
        fld_id  = MAC_TO_HOST32(fld->data.fblocks[i].id);
        fld_val = MAC_TO_HOST32(fld->data.fblocks[i].value);

        if (fld_id < PSDB_FIELD_TABLE_ID_MIN) {
            psdf_fields_block_t  tmp;
            /* Put at beginning i.e. swap if it is not in place */
            if (i != nfld) {
                ILOG1_DD("Swap fields %d and %d", i, nfld);
                tmp = fld->data.fblocks[nfld];
                fld->data.fblocks[nfld] = fld->data.fblocks[i];
                fld->data.fblocks[i] = tmp;
            }
            ++nfld;
        } else {
            if (0 == fld_val) { /* skip empty table */
                continue;
            }
            ++ntab;
        }
    }

    ILOG0_DDD("Parsed %d items, found %d fields and %d tables", fld->nrows, nfld, ntab);

    if (PSDB_MAX_NUM_FBLOCKS < nfld) {
        ELOG_D("Wrong Fields data size (nrows %d)", nfld);
        goto ERROR;
    }

    fld->data.nrows = HOST_TO_MAC32(nfld);
    fld->nof_words = (nfld * fld->ncols) + 2; /* with sizes */

    /* Allocate Rate Power table */
    psdb->rate_pw_table = PSDB_ALLOC(sizeof(psdb_rate_pw_table_t));
    if (!psdb->rate_pw_table) {
        goto ERROR;
    }
    memset(psdb->rate_pw_table, -1, sizeof(psdb_rate_pw_table_t));

    /* Reduce num_of_tables because some of tables will not be passed to FW */
    i = ntab; /* store ntab for error ptintout */
    --ntab;   /* minus one for RegDomain table */
    if (ntab < 0) {
      ELOG_D("Number of tables (%d) too small", i);
      goto ERROR;
    }

    /* Create tables array */
    size = (ntab + 1) * sizeof(psdb_any_table_t *); /* plus 1 for EndOfTable */
    psdb->tables = (psdb_any_table_t **)PSDB_ALLOC(size);
    if (!psdb->tables) {
        goto ERROR;
    }

    memset((void *)psdb->tables, 0, size); /* fill with NULL */
    psdb->nof_tables = ntab;

    ant_params = wave_psdb_get_card_ant_params(psdb);

    /* Search for some params and tables */
    ntab = 0;
    for (i = 0; i < (int)fld->nrows; i++) {
        fld_id  = MAC_TO_HOST32(fld->data.fblocks[i].id);
        fld_val = MAC_TO_HOST32(fld->data.fblocks[i].value);

        if (fld_id >= PSDB_FIELD_TABLE_ID_MIN) {
            ILOG2_DDD("Fields block %d: table id 0x%X, offs 0x%X", i, fld_id, fld_val);

            if (0 == fld_val) { /* skip empty table */
                continue;
            }

            /* Allocate and copy new table */
            tmptab = psdb_copy_table(wbuf, wsize, fld_id, fld_val, PSDB_TABLE_MARKER);
            if (!tmptab) {
                goto FREE_TABS;
            } else if (fld_id == PSDB_FIELD_HW_POWER_LIMIT) { /* RegDomain Table */
                /* this table is only for Driver purpose, i.e. will not be sent to FW */

                psdb_regd_parsed_t  *regd_parsed;

                ILOG1_D("RegDomain table offs 0x%04X", fld_val);

                if (psdb->regd_table) {
                        ELOG_V("Duplicated RegDomain table");
                        PSDB_FREE(tmptab);
                        goto FREE_TABS;
                }

                psdb->regd_table = tmptab;

                /* Check table sizes */
                if (NUM_WORDS_PER_REGD_ENTRY != tmptab->ncols) {
                        ELOG_D("Wrong RegDomain table ncols %d", tmptab->ncols);
                        goto FREE_TABS;
                }

                /* Apply endian convertion to the table content */
                wordcpy_mac_to_host((uint32 *)&tmptab->data, (uint32 *)&tmptab->data, tmptab->nof_words);

                /* Allocate RegDomain parsed table */
                regd_parsed = PSDB_ALLOC(sizeof(psdb_regd_parsed_t));
                if(!regd_parsed) {
                    goto FREE_TABS;
                }

                memset(regd_parsed, 0, sizeof(psdb_regd_parsed_t));
                psdb->regd_parsed = regd_parsed;
                ILOG0_DD("Regd Table: nrows = %d, ncols = %d", tmptab->nrows, tmptab->ncols);

            } else if (fld_id == PSDB_FIELD_CCA_TH_CFG) { /* CCA Thresholds table */
                /* this table is only for Driver purpose, i.e. will not be sent to FW */
                int res = _mtlk_psdb_parse_cca_th_table(psdb, tmptab);
                PSDB_FREE(tmptab);
                --psdb->nof_tables;
                if (MTLK_ERR_OK == res) {
                    psdb->cdb_cfg_table.cca_th_is_filled = TRUE;
                } else {
                    psdb->cdb_cfg_table.cca_th_is_filled = FALSE;
                    goto FREE_TABS;
                }
                ILOG0_D("psdb->cdb_cfg_table.cca_th_is_filled = %d", psdb->cdb_cfg_table.cca_th_is_filled);
            } else { /* add all other tables into the tables array for sending to FW */
                /* checking of ntab is not necessary because fields array has a proper size */
                psdb->tables[ntab++] = tmptab;

                if (PSDB_FIELD_BEAMFORM_CAPS == fld_id) { /* Beamforming capabilities */
                    /* Check table sizes */
                    if (PSDB_FIELD_BEAMFORM_NROWS != tmptab->nrows ||
                        PSDB_FIELD_BEAMFORM_NCOLS != tmptab->ncols ) {
                            ELOG_DD("Wrong Beamforming table sizes: nrows=%d, ncols=%d", tmptab->nrows, tmptab->ncols);
                            goto FREE_TABS;
                    }

                    /* Store single word BF capabilities */
                    psdb->beamform_caps = MAC_TO_HOST32(tmptab->data.words[0]);

                } else if ((PSDB_FIELD_POWER_ADAPT_11B == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_11AG == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_11N == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_11AC == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_11AX == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_GEN7_2G == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_GEN7_5G == fld_id) ||
                           (PSDB_FIELD_POWER_ADAPT_GEN7_6G == fld_id)) {
                  if (MTLK_ERR_OK != _mtlk_psdb_parse_rate_pw_table(psdb->rate_pw_table, tmptab, fld_id)) {
                    goto FREE_TABS;
                  }
                } else if (PSDB_FIELD_CDB_CFG == fld_id) {
                  if (MTLK_ERR_OK != _mtlk_psdb_parse_cdb_cfg_table(hw_api->hw, psdb, tmptab, band6g_supported)) {
                    ELOG_V("Failed to parse CDB cfg table");
                    goto FREE_TABS;
                  }
                } else if (PSDB_FIELD_ANT_MASK_PER_CHANNEL == fld_id) {
                  if (MTLK_ERR_OK != _mtlk_psdb_parse_ant_mask_per_chan_table(psdb, tmptab)) {
                    ELOG_V("Failed to parse Antenna Mask per Channel table");
                    goto FREE_TABS;
                  }
                } else if (PSDB_FIELD_POWER_LIMIT_DAMAGE == fld_id) {
                  if (MTLK_ERR_OK != _mtlk_psdb_parse_pw_limit_damage_table(psdb, tmptab)) {
                    ELOG_V("Failed to parse Power Limit Damage table");
                    goto FREE_TABS;
                  }
                } else if (PSDB_FIELD_ANTENNA_GAIN == fld_id) {
                  if (MTLK_ERR_OK != _mtlk_psdb_parse_ant_gain_per_freq_table(hw_api->hw, psdb, tmptab)) {
                    ELOG_V("Failed to parse Antenna Gain table");
                    goto FREE_TABS;
                  }
                }
            }
        } else if (fld_id == PSDB_FIELD_BAND_SUPPORT_2G) {
            ILOG0_D("PSDB_FIELD_BAND_SUPPORT_2G = %d", fld_val);
            __wave_psdb_get_cdb_cfg_table(psdb)->band_support_2G = !!fld_val; /* BOOL */
        } else if (fld_id == PSDB_FIELD_BAND_SUPPORT_5G) {
            ILOG0_D("PSDB_FIELD_BAND_SUPPORT_5G = %d", fld_val);
            __wave_psdb_get_cdb_cfg_table(psdb)->band_support_5G = !!fld_val; /* BOOL */
        } else if (fld_id == PSDB_FIELD_BAND_SUPPORT_6G) {
            ILOG0_D("PSDB_FIELD_BAND_SUPPORT_6G = %d", fld_val);
            __wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G = !!fld_val; /* BOOL */
        } else if (fld_id == PSDB_FIELD_TX_ANTENNAS_MASK) {
            ant_params->tx_ant_mask = fld_val;
            ant_params->tx_ant_num  = count_bits_set(fld_val);
        } else if (fld_id == PSDB_FIELD_RX_ANTENNAS_MASK) {
            ant_params->rx_ant_mask = fld_val;
            ant_params->rx_ant_num  = count_bits_set(fld_val);
        } else if (fld_id == PSDB_FIELD_TX_ANT_SEL_MASK) {
            ant_params->tx_ant_sel_mask = fld_val;
        } else if (fld_id == PSDB_FIELD_RX_ANT_SEL_MASK) {
            ant_params->rx_ant_sel_mask = fld_val;
        } else if (fld_id == PSDB_FIELD_ZWDFS_ANT_MASK) {
            ILOG0_D("PSDB_FIELD_ZWDFS_ANT_MASK = 0x%02X", fld_val);
            __wave_psdb_get_cdb_cfg_table(psdb)->zwdfs_ant_mask = fld_val;
        }
     }

    ILOG0_DDDD("PSDB: TX/RX antennas number %d/%d, mask 0x%02X/0x%02X",
                ant_params->tx_ant_num,  ant_params->rx_ant_num,
                ant_params->tx_ant_mask, ant_params->rx_ant_mask);

    if (0 == ant_params->tx_ant_num || ant_params->tx_ant_num > MAX_NUM_TX_ANTENNAS ||
        0 == ant_params->rx_ant_num || ant_params->rx_ant_num > max_rx_ant) {
        ELOG_DD("PSDB: TX/RX antennas number is zero or too big (max number %d/%d)",
                MAX_NUM_TX_ANTENNAS, max_rx_ant);
        goto FREE_TABS;
    }

    /* Calculate antennas factor depending on TX antennas number */
    /* We are sure that TX antennas number is in allowed range */
    {
        ant_params->tx_ant_factor = mtlk_antennas_factor(ant_params->tx_ant_num);
    }

    /* RegDomain Table must be present */
    if (!psdb->regd_table) {
        ELOG_V("RegDomain table not found");
        goto FREE_TABS;
    }

    if (!psdb->cdb_cfg_table.cca_th_is_filled) {
        ILOG0_V("PSD File does not contain CCA Thresholds table");
    }

    ILOG1_D("PSDB: %d general tables found", ntab);

    for (i = 0; i < ntab; i++) {
        ILOG3_DP("table[%d] = 0x%p", i, (void *)psdb->tables[i]);
    }

    return MTLK_ERR_OK;

FREE_TABS:
    /* All already allocated tables will be freed during cleanup */

ERROR:
    return MTLK_ERR_NO_MEM;
}

/*-----------------------------------------------*/

void __MTLK_IFUNC
mtlk_psdb_data_cleanup (hw_psdb_t *psdb)
{
    unsigned i;

    ILOG1_V("Cleanup");

    MTLK_ASSERT(NULL != psdb);

    PSDB_FREE(psdb->fields_data);
    PSDB_FREE(psdb->rate_pw_table);
    PSDB_FREE(psdb->regd_table);
    PSDB_FREE(psdb->regd_parsed);
    PSDB_FREE(psdb->ant_mask_per_chan);
    PSDB_FREE(psdb->pw_limit_damage_table);
    PSDB_FREE(psdb->ant_gain_per_freq);

    if (psdb->tables) {
      for (i = 0; i < psdb->nof_tables; i++) {
          PSDB_FREE(psdb->tables[i]);
      }

      PSDB_FREE(psdb->tables);
    }

    /* Clean internal data to prevent the attempts to free already freed buffers */
    memset(psdb, 0, sizeof(*psdb));
}

static void
_mtlk_psdb_cdb_init_radio_cfg (psdb_radio_cfg_t *cfg)
{
    cfg->cdb_config = WAVE_CDB_CFG_NOT_SET;
    cfg->wave_band  = MTLK_HW_BAND_NONE;
}

static void
_psdb_init_default (hw_psdb_t *psdb)
{
    psdb_cdb_cfg_table_t  *cdb_table;
    uint32  word;

    /* Beamforming capabilities */
    word = 0;
    MTLK_BFIELD_SET(word, PSDB_BF_CAPS_EXPLICIT_SUPPORT, 1); /* enable */
    psdb->beamform_caps = word;

    /* CDB config */
    psdb->cdb_cfg_table.radio_chan_threshod = WAVE_CHAN_NONE_UPPER;
    psdb->cdb_cfg_table.band_support_2G = FALSE;
    psdb->cdb_cfg_table.band_support_5G = FALSE;
    psdb->cdb_cfg_table.band_support_6G = FALSE;
    psdb->cdb_cfg_table.nof_entries = 0;
    psdb->cdb_cfg_table.zwdfs_ant_mask = 0x0;

    /* Set CDB config to UNKNOWN for both radios */
    MTLK_STATIC_ASSERT(WAVE_CARD_RADIO_NUM_MAX == ARRAY_SIZE(psdb->cdb_cfg_table.radio_cfg));
    cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
    _mtlk_psdb_cdb_init_radio_cfg(&cdb_table->radio_cfg[0]);
    _mtlk_psdb_cdb_init_radio_cfg(&cdb_table->radio_cfg[1]);
	_mtlk_psdb_cdb_init_radio_cfg(&cdb_table->radio_cfg[2]);
}

static void
_psdb_print_info (hw_psdb_t *psdb)
{
    ILOG0_DD("PSDB Beamforming capabilities: 0x%02X, bf_explicit=%d",
            psdb_get_bf_capabilities(psdb),
            psdb_get_bf_explicit_cap(psdb));
}

static int
_psdb_zwdfs_verify_params (hw_psdb_t *psdb, uint32 chip_id)
{
    psdb_cdb_cfg_table_t  *cdb_table = __wave_psdb_get_cdb_cfg_table(psdb);
    BOOL                  found_matching_radio = FALSE;
    uint                  i;

    for (i = 0; i < cdb_table->nof_entries; i++) {
        psdb_radio_cfg_t   *radio_cfg;
        wave_ant_params_t  *radio_ant_cfg;

        radio_cfg     = &cdb_table->radio_cfg[i];
        radio_ant_cfg = &radio_cfg->radio_ant_cfg;

        if (_chipid_is_gen6_d2(chip_id)) {
          if (cdb_table->zwdfs_ant_mask && (i == 1) &&
              cdb_table->band_support_5G)
            radio_ant_cfg->rx_ant_mask &= PSDB_ZWDFS_ANT_MSK;
        }

        if (cdb_table->band_support_5G &&
            cdb_table->zwdfs_ant_mask == radio_ant_cfg->rx_ant_mask)
            found_matching_radio = TRUE;

        if ((cdb_table->zwdfs_ant_mask & radio_ant_cfg->rx_ant_mask) == 0) {
            ELOG_DDD("PSDB: zwdfs ant is configured (0x%2X) but is not contained in ant mask of radio%d : 0x%2X",
                     cdb_table->zwdfs_ant_mask, i, radio_ant_cfg->rx_ant_mask);
            return MTLK_ERR_PARAMS;
        }
    }

    if (found_matching_radio == FALSE) {
        ELOG_V("PSDB: zwdfs_ant_mask is configured, but could not find matching radio");
        return MTLK_ERR_PARAMS;
    }

    return MTLK_ERR_OK;
}

int  __MTLK_IFUNC
mtlk_psdb_file_read_and_parse(mtlk_hw_t* hw, mtlk_hw_api_t *hw_api, hw_psdb_t *psdb,
                              uint32 chip_id, uint32 chip_type, uint32 hw_type,
                              uint32 hw_rev, bool band6g_supported)
{
    mtlk_df_fw_file_buf_t   fb;
    mtlk_fw_info_t          fw_info;
    psdf_fields_array_t     *fdp;
    uint32                  wsize, *wbuf;
    uint32                  value;
    int                     res;

    MTLK_ASSERT(psdb);

    /* Clear whole psdb struct */
    memset(psdb, 0, sizeof(*psdb));

    _psdb_init_default(psdb);

    memset(&fw_info, 0, sizeof (fw_info));
    wave_strcopy(fw_info.fname, PSDB_FILE_NAME, sizeof(fw_info.fname));

    res = mtlk_hw_load_file(hw_api, fw_info.fname, &fb);
    if (res != MTLK_ERR_OK) {
      ELOG_S("Can't load required file '%s' --> rollback!", fw_info.fname);
      return MTLK_ERR_UNKNOWN;
    }

    ILOG0_SD("Loaded '%s' of %d bytes", fw_info.fname, fb.size);
    mtlk_dump(3, fb.buffer, MIN(512, fb.size), "Head of file");

    wsize = fb.size / sizeof(uint32);
    if (fb.size != (wsize * sizeof(uint32))) {
        ELOG_D("Wrong file len %d", fb.size);
        res = MTLK_ERR_UNKNOWN;
        goto unload;
    }

    if (fb.size < (sizeof(psdf_ver_info_t) + sizeof(psdf_fields_array_t))) {
        ELOG_V("File size too small");
        goto unload;
    }

    wbuf = (uint32 *)fb.buffer; /* word ptr */

    /* Info */
    {
        psdf_ver_info_t *info = (psdf_ver_info_t *)wbuf;
        uint32           ver_num;

        ver_num = MAC_TO_HOST32(info->ver_num);
        ILOG0_DDDD("PSDB Info: version %d (0x%08x), revision %x%x",
            ver_num, ver_num,
            MAC_TO_HOST32(info->rev_hash_msb),
            MAC_TO_HOST32(info->rev_hash_lsb));
    }

#ifdef  PSDB_MEM_DEBUG
    _alloc_count = 0;
    _alloc_size  = 0;
#endif

    /* Look for Fields data */
    ILOG0_DDDD("Look for chip_id 0x%04X, chip_type 0x%02X, hw_type 0x%02X, hw_rev 0x%02X", chip_id, chip_type, hw_type, hw_rev);
    fdp = psdb_search_fields_array(wbuf, wsize, chip_id, chip_type, hw_type, hw_rev);
    if (!fdp) {
        ELOG_V("Fields data NOT found");

        res = MTLK_ERR_UNKNOWN;
        goto unload;
    }

    ILOG2_D("Fields data found at offs 0x%04X", (void *)fdp - (void *)wbuf);

    /* Check and store Chip_Revision */
    value = MAC_TO_HOST32(fdp->chip_rev.value);
    if (MAX_UINT8 < value) {
        ELOG_D("PSDB: Chip_Revision value (%u) is not fit to uint8", value);
        res = MTLK_ERR_PARAMS;
        goto unload;
    }

    psdb->chip_rev = value;

    res = psdb_parse_fields_array (hw_api, wbuf, wsize, (void *)fdp - (void *)wbuf, psdb, band6g_supported, chip_id);
    if (MTLK_ERR_OK != res) { goto cleanup; }

    if ((band6g_supported && !(__wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G)) ||
        (!band6g_supported && (__wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G)))
      ILOG0_V("WARNING: 6G band support inconsistency between the EEPROM and PSD information!");
    /* Update CDB config after all fields/tables parsing */
    res = _mtlk_psdb_update_cdb_cfg_table(hw_api, psdb, hw_rev);
    if (MTLK_ERR_OK != res) { goto cleanup; }

    if(!mtlk_hw_type_is_gen7(hw))
    {
      if (__wave_psdb_get_cdb_cfg_table(psdb)->zwdfs_ant_mask &&
          (psdb->cdb_cfg_table.nof_entries == 2)) {
          /* ZWDFS CDB: Verify zwdfs config if the nof_entries is 2 on CDB mode */
          res = _psdb_zwdfs_verify_params(psdb, chip_id);
          if (MTLK_ERR_OK != res) { goto cleanup; }
      }
    }

    /* Check Consistency between chip_type in efuses and PSD band config in gen7*/
    if(mtlk_hw_type_is_gen7(hw))
    {
      if(mtlk_is_gen7_triple_band(hw))
      {
        if(!__wave_psdb_get_cdb_cfg_table(psdb)->band_support_2G ||
           !__wave_psdb_get_cdb_cfg_table(psdb)->band_support_5G ||
           !__wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G)
        {
          ELOG_V("PSDB: Inconstency between chip_type efuse and PSD");
          res = MTLK_ERR_PARAMS;
          goto unload;
        }
      }
      else if(mtlk_is_gen7_dual_band(hw))
      {
        if(!__wave_psdb_get_cdb_cfg_table(psdb)->band_support_2G ||
           !__wave_psdb_get_cdb_cfg_table(psdb)->band_support_5G ||
           __wave_psdb_get_cdb_cfg_table(psdb)->band_support_6G)
        {
          ELOG_V("PSDB: Inconstency between chip_type efuse and PSD");
          res = MTLK_ERR_PARAMS;
          goto unload;
        }        
      }
    }
     /* We sure that RegDomain table is found, so parse it */
    res = psdb_parse_regd_table(hw_api->hw, psdb, psdb->regd_table, hw_rev);

cleanup:
    if (MTLK_ERR_OK != res) {
        ELOG_V("PSDB parse error");
        mtlk_psdb_data_cleanup(psdb);
    }

unload:
    mtlk_hw_unload_file(hw_api, &fb);

    if (MTLK_ERR_OK == res) {
        _psdb_print_info(psdb);
    }

    return res;
}


int __MTLK_IFUNC
mtlk_hw_psdb_send_fields (mtlk_hw_t *hw, mtlk_txmm_t *txmm, uint32 *wbuf, int nof_words);

int __MTLK_IFUNC
mtlk_hw_psdb_send_table (mtlk_hw_t *hw, mtlk_txmm_t *txmm, uint32 table_id, uint32 *wbuf, int nof_words);


int  __MTLK_IFUNC
mtlk_psdb_load_data (mtlk_hw_t *hw, mtlk_txmm_t *txmm, hw_psdb_t *psdb)
{
    psdb_fields_array_t *fields;
    psdb_any_table_t    *table;
    int                  res = MTLK_ERR_FW;
    unsigned             i;

    MTLK_ASSERT(NULL != hw);
    MTLK_ASSERT(NULL != txmm);
    MTLK_ASSERT(NULL != psdb);

    fields = psdb->fields_data;

    if (NULL == fields) {
      WLOG_V("PSDB data missed, continue");
      res = MTLK_ERR_OK;
      goto FINISH;
    }

    res = mtlk_hw_psdb_send_fields(hw, txmm, (uint32 *)&fields->data, fields->nof_words);
    if (MTLK_ERR_OK != res) {
        ELOG_D("PSDB Fields data loading failure, res %d", res);
        goto FINISH;
    }

    ILOG1_D("Num of PSDB tables for loading: %d", psdb->nof_tables);

    if (0 == psdb->nof_tables) {
        return MTLK_ERR_OK;
    }

    MTLK_ASSERT(NULL != psdb->tables);

    for (i = 0; i < psdb->nof_tables; i++) {
        table = psdb->tables[i];
        MTLK_ASSERT(NULL != table);

        res = mtlk_hw_psdb_send_table(hw, txmm, table->id, (uint32 *)&table->data, table->nof_words);
        if (MTLK_ERR_OK != res) {
            ELOG_DD("PSDB table %d loading failure, res %u", i, res);
            goto FINISH;
        }
    }

FINISH:
    return res;
}
