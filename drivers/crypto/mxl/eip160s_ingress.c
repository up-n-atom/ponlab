// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2024 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/random.h>

#include <net/eip160.h>
#include "eip160s_struct.h"
#include "eip160s_logging.h"
#include "eip160s_internal.h"

static int _macsec_detect_ing_engine(struct eip160_pdev *pdev)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _eip160_extcfg _ext_cfg;
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    _module_base = ING_SYSTEM_CONTROL_MODULE_BASE;
    _module_offset =  EIPI_EIP160_VERSION;
    _absreg_addr = _module_base + _module_offset;
    /* Check EIP160 number and it's one's complement */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160_VERSION read fails\n");
        goto END;
    }

    if ((_data & 0xFFFFu) != EIP160_NR_CNR) {
        LOG_CRIT("EIP160 number mismatch\n");
        ret = -1;
        goto END;
    }

    _module_offset = EIPI_EIP160_CONFIG2;
    _absreg_addr = _module_base + _module_offset;
    /* Gt EIP160_CONFIG2 data */
    ret = IG_REG_READ(pdev, _absreg_addr,  &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160_CONFIG2 word get failed\n");
        goto END;
    }

    _ext_cfg.raw_eip160_extcfg = _data;

    if (_ext_cfg.ingress_only != 1) {
        LOG_CRIT("It is not configured for ingress only\n");
        ret = -1;
        goto END;
    }

    _module_base = EIP_160S_IG_TR_PAGE0_BASE;
    _module_offset =  0x0;
    _absreg_addr = _module_base + _module_offset;

    /* Read and Write test */
    ret = IG_REG_WRITE(pdev, _absreg_addr, 0xfedcba98);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160 test write failed\n");
        goto END;
    }

    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160 test read failed\n");
        goto END;
    }

    if (_data != 0xfedcba98) {
        LOG_CRIT("EIP160 test write & read failed\n");
        ret = -1;
        goto END;
    }

    /* Reset */
    ret = IG_REG_WRITE(pdev, _absreg_addr, 0x0);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160 test reset failed\n");
        ret = -1;
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_detect_ing_crypto(struct eip160_pdev *pdev)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;
    _module_offset =  EIPI_EIP62_VERSION;
    _absreg_addr = _module_base + _module_offset;
    /* Check EIP62 number and it's one's complement */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62_VERSION read failed\n");
        goto END;
    }

    if ((_data & 0xFFFFu) != EIP62_NR_CNR) {
        LOG_CRIT("EIP62 number mismatch\n");
        ret = -1;
        goto END;
    }

    _module_offset =  EIPI_EIP62_SEQ_NUM_THRESHOLD;
    _absreg_addr = _module_base + _module_offset;
    /* Read and Write test */
    ret = IG_REG_WRITE(pdev, _absreg_addr, 0x89abcdef);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62 test write failed\n");
        goto END;
    }

    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62 test read failed\n");
        goto END;
    }

    if (_data != 0x89abcdef) {
        LOG_CRIT("EIP160 test write & read failed\n");
        ret = -1;
        goto END;
    }

    /* Reset */
    ret = IG_REG_WRITE(pdev, _absreg_addr, 0x0);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62 test reset failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_clr_ing_trans_rec_mem(struct eip160_pdev *pdev)
{
    int i, j, ret = 0;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    LOG_INFO("Entering func :- %s\n", __func__);

    _module_base = EIP_160S_IG_TR_PAGE0_BASE;

    /* Clear Transform Record RAM */
    for (i = 0; i < (pdev->ig_nr_of_sas); i++) {
        _module_offset = ((i > 127) ? 0x10000 : 0) + (i * ING_MAX_TR_SIZE_BYTES);
        _absreg_addr = _module_base + _module_offset;

        for (j = 0; j < 2; j++) { /* No-Need init full segment 'ING_MAX_TR_SIZE_WORDS' */
            ret = IG_REG_WRITE(pdev, _absreg_addr + (j * 4), 0x0);

            if (ret < 0) {
                LOG_CRIT("ERROR: TR mem init failed\n");
                goto END;
            }
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_cfg_ing_ctx_size_ud_ctrl(struct eip160_pdev *pdev)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _ctx_ctrl = 0;
    LOG_INFO("Entering func :- %s\n", __func__);

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;
    _module_offset =  EIPI_EIP62_CONTEXT_CTRL;
    _absreg_addr = _module_base + _module_offset;
    /* Get EIP62_CONTEXT_CTRL */
    ret = IG_REG_READ(pdev, _absreg_addr, &_ctx_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: Get EIP62_CONTEXT_CTRL failed\n");
        goto END;
    }

    REG_FIELD_REPL(_ctx_ctrl, ING_MAX_TR_SIZE_WORDS, EIPI_EIP62_CONTEXT_CTRL_CONTEXT_SIZE);
    /* Set EIP62_CONTEXT_CTRL.context_size */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _ctx_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: Set context_size failed\n");
        goto END;
    }

	_module_offset =  EIPI_EIP62_CONTEXT_UPD_CTRL;
	_absreg_addr = _module_base + _module_offset;
	/* Set 'EIP62_CONTEXT_UPD_CTRL.block_context_updates' to do not update the
		context for bad packets */
	ret = IG_REG_WRITE(pdev, _absreg_addr, 0x3);

	if (ret < 0) {
		LOG_CRIT("ERROR: EIP62_CONTEXT_UPD_CTRL set failed\n");
		goto END;
	}

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_cfg_ing_misc_flow_ctrl(struct eip160_pdev *pdev)
{
    int ret;
    struct misc_ctrl misc_ctrl = {
        .mc_latency_fix = 72,
        .static_bypass = 0,
        .nm_macsec_en = 1,
        .validate_frames = SAM_FCA_VALIDATE_STRICT,
        .sectag_after_vlan = 1,
    };

    LOG_INFO("Entering func :- %s\n", __func__);

    /* Set 'MISC_CONTROL' to do not update the context for bad packets */
    ret = eip160_cfg_ing_misc_flow_ctrl(pdev, &misc_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_cfg_ing_misc_flow_ctrl failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_cfg_ing_sanm_flow_ctrl(struct eip160_pdev *pdev)
{
    int ret;
    struct sanm_flow_ctrl nm_xcp = {
        .pkt_type = SA_NM_SECY_ALL_TAGGED,
        {
            .flow_type = SA_NM_FCA_FLOW_BYPASS,
            .dest_port = SA_NM_FCA_DPT_CONTROL,
            .drop_non_reserved = 0,
            .drop_action = SA_NM_FCA_DROP_AS_CRC,
        },
        {
            .flow_type = SA_NM_FCA_FLOW_BYPASS,
            .dest_port = SA_NM_FCA_DPT_CONTROL,
            .drop_non_reserved = 0,
            .drop_action = SA_NM_FCA_DROP_AS_CRC,
        },
    };

    LOG_INFO("Entering func :- %s\n", __func__);

    /* Set 'SA_NM_FLOW_NCP, SA_NM_FLOW_CP' flow control action */
    ret = eip160_cfg_ing_sanm_flow_ctrl(pdev, &nm_xcp);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_cfg_ing_sanm_flow_ctrl failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_clr_ing_cntrs_thold_sum(struct eip160_pdev *pdev)
{
    int ret;
    struct cntrs_thold_sum thold_sum = {
        .gl_cntr_sum = 0x3FFF,
#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
        .vlan_cntr_sum = {0xFFFF00FF, 0x000000FF},
#endif
        .psa_cntr_vec = {0xFFFFFFFF},
        .psa_cntr_sum = {0x7FF},
    };

    LOG_INFO("Entering func :- %s\n", __func__);

    /* Set SA expired/pn-thr/psa summary */
    ret = eip160_clr_ing_cntrs_thold_sum(pdev, &thold_sum);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_clr_ing_cntrs_thold_sum failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_ck_ing_dev_sync_stat(struct eip160_pdev *pdev)
{
    int i = 0, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _sync_done;
    LOG_INFO("Entering func :- %s\n", __func__);

    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset =  EIPI_COUNT_CONTROL;
    _absreg_addr = _module_base + _module_offset;

    do {
        /* Check 'COUNT_CONTROL.reset_all' for sunc done */
        ret = IG_REG_READ(pdev, _absreg_addr, &_sync_done);

        if (ret < 0) {
            LOG_CRIT("ERROR: COUNT_CONTROL get failed\n");
            goto END;
        }

        _sync_done = REG_FIELD_GET(_sync_done, EIPI_COUNT_CONTROL_RESET_ALL);
    } while ((_sync_done) && (++i < EIP160_MAX_SYNC_RETRY_COUNT));

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_cfg_ing_cntrs_ctrl(struct eip160_pdev *pdev)
{
    int ret;
    struct cntrs_ctrl cnt_ctrl = {
        .reset_all = 0,
        .saturate_cntrs = 1,
        .auto_cntr_reset = 1,
        .reset_summary = 0,
        .psa_inc_en = 0x7ff,
        .vlan_inc_en = 0,
        .global_inc_en = 0x3fff,
    };

    LOG_INFO("Entering func :- %s\n", __func__);

    /* Set 'COUNT_CONTROL' to set control options */
    ret = eip160_cfg_ing_cntrs_ctrl(pdev, &cnt_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_cfg_ing_cntrs_ctrl failed\n");
        goto END;
    }

    cnt_ctrl.reset_all = 1;
    /* Set 'COUNT_CONTROL' to set control options */
    ret = eip160_cfg_ing_cntrs_ctrl(pdev, &cnt_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_cfg_ing_cntrs_ctrl failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

static int _macsec_cfg_ing_ccknm_flow_ctrl(struct eip160_pdev *pdev)
{
    int ret;
    struct ccknm_flow_ctrl cc_ctrl = {
        .nm_act_ncp = 1,
        .nm_act_cp = 1,
    };

    LOG_INFO("Entering func :- %s\n", __func__);

    /* Set 'IG_CC_CONTRO' for non-match action of ctrl & non-ctrl frames */
    ret = eip160_cfg_ing_ccknm_flow_ctrl(pdev, &cc_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_cfg_ing_ccknm_flow_ctrl failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

int eip160_init_ing_dev(struct eip160_pdev *pdev)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

#if defined(NO_TARGET_TEST) && NO_TARGET_TEST
    return 1;
#endif

    ret = _macsec_detect_ing_engine(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_detect_ing_engine failed\n");
        goto END;
    }

    ret = _macsec_detect_ing_crypto(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_detect_ing_crypto failed\n");
        goto END;
    }

    _module_base = ING_SYSTEM_CONTROL_MODULE_BASE;
    _module_offset =  EIPI_EIP160_CONFIG;
    _absreg_addr = _module_base + _module_offset;
    /* Read max no.of SA supported */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP160_CONFIG read failed\n");
        goto END;
    }

    pdev->ig_nr_of_sas = REG_FIELD_GET(_data, EIPI_EIP160_CONFIG_NR_OF_SAS);
    ret = _macsec_clr_ing_trans_rec_mem(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_clr_ing_trans_rec_mem failed\n");
        goto END;
    }

    ret = _macsec_cfg_ing_cntrs_ctrl(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_cfg_ing_cntrs_ctrl failed\n");
        goto END;
    }

    ret = _macsec_cfg_ing_ctx_size_ud_ctrl(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_cfg_ing_ctx_size failed\n");
        goto END;
    }

    ret = _macsec_cfg_ing_misc_flow_ctrl(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_cfg_ing_misc_flow_ctrl failed\n");
        goto END;
    }

    ret = _macsec_cfg_ing_sanm_flow_ctrl(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_cfg_ing_sanm_flow_ctrl failed\n");
        goto END;
    }

    ret = _macsec_cfg_ing_ccknm_flow_ctrl(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_cfg_ing_ccknm_flow_ctrl failed\n");
        goto END;
    }

    ret = _macsec_clr_ing_cntrs_thold_sum(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_clr_ing_cntrs_thold_sum failed\n");
        goto END;
    }

    ret = _macsec_ck_ing_dev_sync_stat(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_ck_ing_dev_sync_stat failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_init_ing_dev);

int eip160_get_ing_hw_cap(struct eip160_pdev *pdev, struct eip160_hw_cap *hw_cap)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data[3];
    struct _eip160_config eip_cfg;
    struct _eip160_extcfg ext_cfg;
	struct _eip62_config ten_cfg;
    struct _eip201_config aic_opt;
    struct _eip_eng_ver eip_ver, aic_ver, ten_ver;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || hw_cap == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }
    
    _module_base = ING_SYSTEM_CONTROL_MODULE_BASE;
    
    _module_offset = EIPI_EIP160_CONFIG2;
    _absreg_addr = _module_base + _module_offset;
	for (i = 0; i < 3; i++) {
	    /* Gt EIP160_CONFIG2, EIP160_CONFIG, EIP160_VERSION data */
	    ret = IG_REG_READ(pdev, _absreg_addr + i * 4,  &_data[i]);

	    if (ret < 0) {
	        LOG_CRIT("ERROR: EIP160 cap word get failed for iteration %d\n", i);
	        goto END;
	    }
	}

    ext_cfg.raw_eip160_extcfg = _data[0];
    eip_cfg.raw_eip160_cfg = _data[1];
    eip_ver.raw_engxxx_ver = _data[2];

    hw_cap->eip_cfg.sa_counters = ext_cfg.sa_counters;
    hw_cap->eip_cfg.vl_counters = ext_cfg.vl_counters;
    hw_cap->eip_cfg.global_counters = ext_cfg.global_counters;
    hw_cap->eip_cfg.saturate_cntrs = ext_cfg.saturate_cntrs;
    hw_cap->eip_cfg.auto_cntr_reset = ext_cfg.auto_cntr_reset;
    hw_cap->eip_cfg.sa_octet_ctrs = ext_cfg.sa_octet_ctrs;
    hw_cap->eip_cfg.vl_octet_ctrs = ext_cfg.vl_octet_ctrs;
    hw_cap->eip_cfg.glob_octet_ctrs = ext_cfg.glob_octet_ctrs;
    hw_cap->eip_cfg.ingress_only = ext_cfg.ingress_only;
    hw_cap->eip_cfg.egress_only = ext_cfg.egress_only;
    hw_cap->eip_cfg.tag_bypass = ext_cfg.tag_bypass;

    hw_cap->eip_cfg.nr_of_sas = eip_cfg.nr_of_sas;
    hw_cap->eip_cfg.consist_checks = eip_cfg.consist_checks;
    hw_cap->eip_cfg.match_sci = eip_cfg.match_sci;

    hw_cap->eip_ver.eip_nr = eip_ver.eip_nr;
    hw_cap->eip_ver.eip_nr_complement = eip_ver.eip_nr_complement;
    hw_cap->eip_ver.patch_level = eip_ver.patch_level;
    hw_cap->eip_ver.minor_version = eip_ver.minor_version;
    hw_cap->eip_ver.major_version = eip_ver.major_version;

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;
    
    _module_offset = EIPI_EIP62_TYPE_REG;
    _absreg_addr = _module_base + _module_offset;
	for (i = 0; i < 2; i++) {
	    /* Gt EIP62_CONFIG, EIP62_VERSION data */
	    ret = IG_REG_READ(pdev, _absreg_addr + i * 4,  &_data[i]);

	    if (ret < 0) {
	        LOG_CRIT("ERROR: EIP62 cap word get failed for iteration %d\n", i);
	        goto END;
	    }
	}

    ten_cfg.raw_eip62_cfg = _data[0];
    ten_ver.raw_engxxx_ver = _data[1];

    hw_cap->ten_ver.eip_nr = ten_ver.eip_nr;
    hw_cap->ten_ver.eip_nr_complement = ten_ver.eip_nr_complement;
    hw_cap->ten_ver.patch_level = ten_ver.patch_level;
    hw_cap->ten_ver.minor_version = ten_ver.minor_version;
    hw_cap->ten_ver.major_version = ten_ver.major_version;

    hw_cap->ten_cfg.minor_revision = ten_cfg.minor_revision;
    hw_cap->ten_cfg.major_revision = ten_cfg.major_revision;
    hw_cap->ten_cfg.fpga_solution = ten_cfg.fpga_solution;
    hw_cap->ten_cfg.gf_sboxes = ten_cfg.gf_sboxes;
    hw_cap->ten_cfg.lookup_sboxes = ten_cfg.lookup_sboxes;
    hw_cap->ten_cfg.macsec_aes_only = ten_cfg.macsec_aes_only;
    hw_cap->ten_cfg.aes_present = ten_cfg.aes_present;
    hw_cap->ten_cfg.aes_fb = ten_cfg.aes_fb;
    hw_cap->ten_cfg.aes_speed = ten_cfg.aes_speed;
    hw_cap->ten_cfg.aes192 = ten_cfg.aes192;
    hw_cap->ten_cfg.aes256 = ten_cfg.aes256;
    hw_cap->ten_cfg.eop_param_bits = ten_cfg.eop_param_bits;
    hw_cap->ten_cfg.sec_offset = ten_cfg.sec_offset;
    hw_cap->ten_cfg.ghash_present = ten_cfg.ghash_present;
    hw_cap->ten_cfg.ghash_speed = ten_cfg.ghash_speed;

    _module_base = ING_EIP201_REGISTERS_MODULE_BASE;
    _module_offset = EIPI_AIC_OPTIONS;
    _absreg_addr = _module_base + _module_offset;
	for (i = 0; i < 2; i++) {
	    /* Gt AIC_OPTIONS, AIC_VERSION data */
	    ret = IG_REG_READ(pdev, _absreg_addr + i * 4,  &_data[i]);

	    if (ret < 0) {
	        LOG_CRIT("ERROR: EIP201 cap word get failed for iteration %d\n", i);
	        goto END;
	    }
	}

    aic_opt.raw_aic_opt = _data[0];
    aic_ver.raw_engxxx_ver = _data[1];

    hw_cap->aic_opt.number_of_inputs = aic_opt.number_of_inputs;

    hw_cap->aic_ver.eip_nr = aic_ver.eip_nr;
    hw_cap->aic_ver.eip_nr_complement = aic_ver.eip_nr_complement;
    hw_cap->aic_ver.patch_level = aic_ver.patch_level;
    hw_cap->aic_ver.minor_version = aic_ver.minor_version;
    hw_cap->aic_ver.major_version = aic_ver.major_version;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_hw_cap);

/* Populate Ingress CCW */
static int _macsec_pop_ing_ccw(struct ctx_ctrl_word *ccw, u32 *_ccw)
{
    int ret = -EINVAL;
    LOG_INFO("Entering func :- %s\n", __func__);

    ret = _macsec_pop_ccw(INGRESS_DIR, ccw, _ccw);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_build_ccw failed\n");
    }

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}

int eip160_cfg_ing_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par)
{
    int i, _ctx_id = 0, ret = -EINVAL;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _key_len;
    u32 le_key[MACSEC_CA256_KEY_LEN_WORD], le_hkey[MACSEC_HKEY_LEN_WORD];
    u32 le_iv[MACSEC_SCI_LEN_WORD], le_ctx_salt[MACSEC_CSALT_LEN_WORD];
    u32 _ccw, *_tr = NULL;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sa_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

#if !defined(NO_TARGET_TEST)
    /* Validate SA rule num range */
    if (sa_par->sa_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (sa_index) - SA rule num\n");
        return -EINVAL;
    }
#endif

    ret = _macsec_pop_ing_ccw(&sa_par->ccw, &_ccw);

    if (ret < 0) {
        LOG_CRIT("ERROR: api _macsec_pop_ing_ccw failed\n");
        ret = -EINVAL;
        goto END;
    }

    _key_len = (sa_par->ccw.ca_type == CA_AES_CTR_128) ? 16 : 32;

    ret = _macsec_pop_key(&sa_par->cp, le_key);
#ifdef __KERNEL__
    derive_aes_hkey(sa_par->cp.key, sa_par->cp.hkey, _key_len);
#endif
#if PRINT_DEBUG
    {
    char keystr[MACSEC_KEY_LEN_BYTE*2+1] = "\0";
    char hkeystr[MACSEC_KEY_LEN_BYTE*2+1] = "\0";
    for (i = 0; i < _key_len; i++)
	    sprintf(keystr + 2*i, "%02X", sa_par->cp.key[i]);
    for (i = 0; i < _key_len; i++)
	    sprintf(hkeystr + 2*i, "%02X", sa_par->cp.hkey[i]);
    pr_err("key = %s, h-key = %s\n", keystr, hkeystr);
    }
#endif
    _macsec_pop_hkey(&sa_par->cp, le_hkey);
    _macsec_pop_sci(&sa_par->cp, le_iv);
    _macsec_pop_csalt(&sa_par->cp, le_ctx_salt);

    _tr = os_mem_alloc(ING_MAX_TR_SIZE_BYTES);

    if (!_tr) {
        LOG_CRIT("Memory alloc failed :- %s\n", __func__);
        ret = -ENOMEM;
        goto END;
    }

    memset(_tr, 0, ING_MAX_TR_SIZE_BYTES);

    /* Generate unique context_id */
#ifdef __KERNEL__
    get_random_bytes(&_ctx_id, 4);
#else
    _ctx_id = ((rand() & 0xff) << 0) | ((rand() & 0xff) << 8) |
              ((rand() & 0xff) << 16) | ((rand() & 0xff) << 24);
#endif
    _ctx_id = _ctx_id & (~0x000000FF);
    _ctx_id = _ctx_id | (sa_par->sa_index);

    /* Build TR */
    if (sa_par->ccw.ca_type == CA_AES_CTR_128) {
        if (sa_par->ccw.sn_type == SN_32_BIT) {
            struct _ing_tr_32bpn_128bak *target_tr =
                (struct _ing_tr_32bpn_128bak *)_tr;
            target_tr->ccw = _ccw;
            target_tr->ctx_id = _ctx_id;
            memcpy(target_tr->key, (u8 *)le_key, MACSEC_CA128_KEY_LEN_BYTE);
            memcpy(target_tr->hkey, (u8 *)le_hkey, MACSEC_HKEY_LEN_BYTE);

            if (sa_par->pn_rc.seq0 != 0)
                target_tr->seq = sa_par->pn_rc.seq0; /* Must be 1 for Ingress */
            else
                target_tr->seq = 1; /* Must be 1 for Ingress */

            target_tr->mask = sa_par->pn_rc.mask;
            target_tr->iv0 = le_iv[0];
            target_tr->iv1 = le_iv[1];
        } else {
            struct _ing_tr_64bpn_128bak *target_tr =
                (struct _ing_tr_64bpn_128bak *)_tr;
            target_tr->ccw = _ccw;
            target_tr->ctx_id = _ctx_id;
            memcpy(target_tr->key, (u8 *)le_key, MACSEC_CA128_KEY_LEN_BYTE);
            memcpy(target_tr->hkey, (u8 *)le_hkey, MACSEC_HKEY_LEN_BYTE);

            if (sa_par->pn_rc.seq0 != 0)
                target_tr->seq0 = sa_par->pn_rc.seq0; /* Must be 1 for Ingress */
            else
                target_tr->seq0 = 1; /* Must be 1 for Ingress */

            target_tr->seq1 = sa_par->pn_rc.seq1; /* Must be 0 for Ingress */

            if (sa_par->pn_rc.mask > 0x40000000) /* According to IEEE 802.1AEbw */
                target_tr->mask = 0x40000000;
            else
                target_tr->mask = sa_par->pn_rc.mask;

            target_tr->iv0 = le_ctx_salt[0];
            target_tr->iv1 = le_ctx_salt[1];
            target_tr->iv2 = le_ctx_salt[2];
        }
    } else {
        if (sa_par->ccw.sn_type == SN_32_BIT) {
            struct _ing_tr_32bpn_256bak *target_tr =
                (struct _ing_tr_32bpn_256bak *)_tr;
            target_tr->ccw = _ccw;
            target_tr->ctx_id = _ctx_id;
            memcpy(target_tr->key, (u8 *)le_key, MACSEC_CA256_KEY_LEN_BYTE);
            memcpy(target_tr->hkey, (u8 *)le_hkey, MACSEC_HKEY_LEN_BYTE);

            if (sa_par->pn_rc.seq0 != 0)
                target_tr->seq = sa_par->pn_rc.seq0; /* Must be 1 for Ingress */
            else
                target_tr->seq = 1; /* Must be 1 for Ingress */

            target_tr->mask = sa_par->pn_rc.mask;
            target_tr->iv0 = le_iv[0];
            target_tr->iv1 = le_iv[1];
        } else {
            struct _ing_tr_64bpn_256bak *target_tr =
                (struct _ing_tr_64bpn_256bak *)_tr;
            target_tr->ccw = _ccw;
            target_tr->ctx_id = _ctx_id;
            memcpy(target_tr->key, (u8 *)le_key, MACSEC_CA256_KEY_LEN_BYTE);
            memcpy(target_tr->hkey, (u8 *)le_hkey, MACSEC_HKEY_LEN_BYTE);

            if (sa_par->pn_rc.seq0 != 0)
                target_tr->seq0 = sa_par->pn_rc.seq0; /* Must be 1 for Ingress */
            else
                target_tr->seq0 = 1; /* Must be 1 for Ingress */

            target_tr->seq1 = sa_par->pn_rc.seq1; /* Must be 0 for Ingress */

            if (sa_par->pn_rc.mask > 0x40000000) /* According to IEEE 802.1AEbw */
                target_tr->mask = 0x40000000;
            else
                target_tr->mask = sa_par->pn_rc.mask;

            target_tr->iv0 = le_ctx_salt[0];
            target_tr->iv1 = le_ctx_salt[1];
            target_tr->iv2 = le_ctx_salt[2];
        }
    }

    _module_base = EIP_160S_IG_TR_PAGE0_BASE + ((sa_par->sa_index > 127) ? 0x10000 : 0);
    _module_offset = (sa_par->sa_index) * ING_MAX_TR_SIZE_BYTES;
    _absreg_addr = _module_base + _module_offset;

#if defined(NO_TARGET_TEST) && NO_TARGET_TEST
    for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
        pr_info("TRec[%02d] = %8x\n", i, _tr[i]);
    }
#else
    /* Set TR's data */
    for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _tr[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: TR's write fails for iteration %d\n", i);
            goto END;
        }
    }
#endif
#if PRINT_DEBUG
    for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
        pr_err("Tr-Par[%02d] = %8x\n", i, _tr[i]);
    }
#endif

END:
    if (_tr != NULL) {
        memzero_explicit(_tr, ING_MAX_TR_SIZE_BYTES);
        os_mem_free(_tr);
    }

#if !defined(NO_TARGET_TEST)
    
    ret = 0;
#endif

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sa_parms);

int eip160_get_ing_sa_parms(struct eip160_pdev *pdev,
                             struct transform_rec *sa_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data, _ca_type, _sn_type;
    u32 *_key, *_hkey, *_sci, *_csalt;
    struct _raw_trans_rec _trans_rec;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sa_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sa_par->sa_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (sa_index) - SA rule num\n");
        return -EINVAL;
    }

    memset(_trans_rec.raw_trans_rec, 0, EGR_MAX_TR_SIZE_BYTES);
    _module_base = EIP_160S_IG_TR_PAGE0_BASE + ((sa_par->sa_index > 127) ? 0x10000 : 0);
    _module_offset = ((sa_par->sa_index) % 128) * (ING_MAX_TR_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Get TR's data */
    for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: TR read fails for iteration %d\n", i);
            goto END;
        }

        _trans_rec.raw_trans_rec[i] = _data;
    }

#if PRINT_DEBUG
    for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
        pr_err("Tr-Par[%02d] = %8x\n", i, _trans_rec.raw_trans_rec[i]);
    }
#endif

    _sn_type = (_trans_rec.raw_trans_rec[0] >> 28) & 0x3;
    _ca_type = (_trans_rec.raw_trans_rec[0] >> 17) & 0x7;
    sa_par->ccw.ca_type = _ca_type;
    sa_par->ccw.sn_type = _sn_type;

    if (_ca_type == CA_AES_CTR_128) {
        if (_sn_type == SN_32_BIT) {
            struct _ing_tr_32bpn_128bak *target_tr =
                (struct _ing_tr_32bpn_128bak *) _trans_rec.raw_trans_rec;
            _key = (u32 *)target_tr->key;
            _hkey = (u32 *)target_tr->hkey;
            _sci = (u32 *)&target_tr->iv0;
            sa_par->pn_rc.seq0 = target_tr->seq;
            sa_par->pn_rc.mask = target_tr->mask;
        } else if (_sn_type == SN_64_BIT) {
            struct _ing_tr_64bpn_128bak *target_tr =
                (struct _ing_tr_64bpn_128bak *) _trans_rec.raw_trans_rec;
            _key = (u32 *)target_tr->key;
            _hkey = (u32 *)target_tr->hkey;
            _csalt = (u32 *)&target_tr->iv0;
            sa_par->pn_rc.seq0 = target_tr->seq0;
            sa_par->pn_rc.seq1 = target_tr->seq1;
            sa_par->pn_rc.mask = target_tr->mask;
        } else {
            LOG_CRIT("\nERROR: Invalid SN type.\n");
            goto END;
        }
    } else if (_ca_type == CA_AES_CTR_256) {
        if (_sn_type == SN_32_BIT) {
            struct _ing_tr_32bpn_256bak *target_tr =
                (struct _ing_tr_32bpn_256bak *) _trans_rec.raw_trans_rec;
            _key = (u32 *)target_tr->key;
            _hkey = (u32 *)target_tr->hkey;
            _sci = (u32 *)&target_tr->iv0;
            sa_par->pn_rc.seq0 = target_tr->seq;
            sa_par->pn_rc.mask = target_tr->mask;
        } else if (_sn_type == SN_64_BIT) {
            struct _ing_tr_64bpn_256bak *target_tr =
                (struct _ing_tr_64bpn_256bak *) _trans_rec.raw_trans_rec;
            _key = (u32 *)target_tr->key;
            _hkey = (u32 *)target_tr->hkey;
            _csalt = (u32 *)&target_tr->iv0;
            sa_par->pn_rc.seq0 = target_tr->seq0;
            sa_par->pn_rc.seq1 = target_tr->seq1;
            sa_par->pn_rc.mask = target_tr->mask;
        } else {
            LOG_CRIT("\nERROR: Invalid SN type.\n");
            goto END;
        }
    } else {
        LOG_CRIT("\nERROR: Invalid CA type.\n");
        goto END;
    }

    _macsec_copy_key(&sa_par->cp, _key);
    _macsec_copy_hkey(&sa_par->cp, _hkey);
    if (_sn_type == SN_32_BIT) {
        _macsec_copy_sci(&sa_par->cp, _sci);
    } else if (_sn_type == SN_64_BIT) {
        _macsec_copy_csalt(&sa_par->cp, _csalt);
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sa_parms);

int eip160_clr_ing_sa_parms(struct eip160_pdev *pdev,
                             const struct clr_trans_rec *clr_tr)
{
    int i, ret = -EINVAL;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || clr_tr == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (clr_tr->sa_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (sa_index) - SA rule num\n");
        return -EINVAL;
    }

    ret = eip160_ck_ing_sam_in_flight(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_ck_ing_sam_in_flight failed\n");
        goto END;
    }

    _module_base = EIP_160S_IG_TR_PAGE0_BASE + ((clr_tr->sa_index > 127) ? 0x10000 : 0);
    _module_offset = (clr_tr->sa_index) * ING_MAX_TR_SIZE_BYTES;
    _absreg_addr = _module_base + _module_offset;

    /* Set TR's data */
    for (i = 0; i < 2; i++) { /* No-Need init full segment 'ING_MAX_TR_SIZE_WORDS' */
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, 0x0);

        if (ret < 0) {
            LOG_CRIT("ERROR: TR's write fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_clr_ing_sa_parms);

int eip160_get_ing_sa_next_pn(struct eip160_pdev *pdev,
                              struct sa_next_pn *next_pn)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data, _sn_type, _ca_type;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || next_pn == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (next_pn->sa_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (sa_index) - SA rule num\n");
        return -EINVAL;
    }

    _module_base = EIP_160S_IG_TR_PAGE0_BASE + ((next_pn->sa_index > 127) ? 0x10000 : 0);
    _module_offset = ((next_pn->sa_index) % 128) * (ING_MAX_TR_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;
    /* Get TR's CCW */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: CCW read failed\n");
        goto END;
    }

    _sn_type = REG_FIELD_GET(_data, CCW_EIPX_TR_CCW_SN);
    _ca_type = REG_FIELD_GET(_data, CCW_EIPX_TR_CCW_CA);

    /* Compute Current Sequence Number word offset */
    if (_ca_type == CA_AES_CTR_128) {
        //for struct _egr_tr_32bpn_128bak, struct _egr_tr_64bpn_128bak;
        _module_offset += 10 * 4;
    } else {
        //for struct _egr_tr_32bpn_256bak, struct _egr_tr_64bpn_256bak
        _module_offset += 14 * 4;
    }

    _absreg_addr = _module_base + _module_offset;
    /* Get TR's SN-0 */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: SN-0 read failed\n");
        goto END;
    }

    next_pn->next_pn = _data;

    if (_sn_type == SN_64_BIT) {
        /* Get TR's SN-1 */
        ret = IG_REG_READ(pdev, _absreg_addr + 4, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: SN-1 read failed\n");
            goto END;
        }

        next_pn->next_pn |= ((u64)_data << 32);
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sa_next_pn);

int eip160_cfg_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 const struct sam_flow_ctrl *sam_fca)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _sam_flow_ctrl _flow_ctrl;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sam_fca == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sam_fca->flow_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (flow_index) - SA rule num\n");
        return -EINVAL;
    }

    /* Validate Flow Control Action params */
    if ((sam_fca->flow_type > SAM_FCA_FLOW_EGRESS) ||
            (sam_fca->dest_port > SAM_FCA_DPT_UNCONTROL) ||
            (sam_fca->drop_action > SAM_FCA_DROP_AS_INTERNAL) ||
            (sam_fca->validate_frames > SAM_FCA_VALIDATE_STRICT)) {
        LOG_ERR("WARN: Invalid input Flow Control Action parameter\n");
        return -EINVAL;
    }

    _flow_ctrl.dec_enc.raw_fca_macsec = _flow_ctrl.cabp_len.raw_fca_cac = 0;
    _flow_ctrl.dec_enc.flow_type = sam_fca->flow_type;
    _flow_ctrl.dec_enc.dest_port = sam_fca->dest_port;
    _flow_ctrl.dec_enc.drop_non_reserved = sam_fca->drop_non_reserved;
    _flow_ctrl.dec_enc.flow_crypt_auth = sam_fca->flow_crypt_auth;
    _flow_ctrl.dec_enc.drop_action = sam_fca->drop_action;
    _flow_ctrl.dec_enc.sa_index = sam_fca->sa_index;
    _flow_ctrl.dec_enc.sa_in_use = sam_fca->sa_in_use;

    if ((sam_fca->flow_type == SAM_FCA_FLOW_BYPASS) && (sam_fca->flow_crypt_auth == 1)) {
        _flow_ctrl.crypt_auth.iv = sam_fca->iv;
        _flow_ctrl.crypt_auth.conf_offset_7 = sam_fca->conf_offset_7;
        _flow_ctrl.crypt_auth.icv_verify = sam_fca->icv_verify;
        _flow_ctrl.crypt_auth.conf_offset = sam_fca->conf_offset;
        _flow_ctrl.crypt_auth.encrypt_frames = sam_fca->encrypt_frames;
    } else if (sam_fca->flow_type == SAM_FCA_FLOW_INGRESS) {
        _flow_ctrl.dec_enc.rep_prot_fr = sam_fca->replay_protect;
        _flow_ctrl.dec_enc.vlevel_es_scb = sam_fca->validate_frames;
        _flow_ctrl.dec_enc.conf_offset = sam_fca->conf_offset;
        _flow_ctrl.dec_enc.encrypt_frames = sam_fca->encrypt_frames;
    }

    _module_base = ING_FLOW_CONTROL_PAGE0_MODULE_BASE +
                   ((sam_fca->flow_index > 127) ? 0x10000 : 0);
    _module_offset = (sam_fca->flow_index) * IE_MAX_SAM_FCA_BYTES;
    _absreg_addr = _module_base + _module_offset;

    /* Set Flow Ctrl Action data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _flow_ctrl.dec_enc.raw_fca_macsec);

    if (ret < 0) {
        LOG_CRIT("ERROR: Flow Control Action word set failed\n");
        goto END;
    }

#if PRINT_DEBUG
    pr_err("Flow-ctrl word = %8x\n",  _flow_ctrl.dec_enc.raw_fca_macsec);
#endif

    if ((sam_fca->flow_type == SAM_FCA_FLOW_BYPASS) && (sam_fca->flow_crypt_auth == 1)) {
        _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
        _module_offset = EIPI_CRYPT_AUTH_CTRL;
        _absreg_addr = _module_base + _module_offset;

        _flow_ctrl.cabp_len.crypt_auth_bypass_len = sam_fca->crypt_auth_bypass_len;

        /* Set CRYPT_AUTH_CTRL data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, _flow_ctrl.cabp_len.raw_fca_cac);

        if (ret < 0) {
            LOG_CRIT("ERROR: CRYPT_AUTH_CTRL word set failed\n");
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sam_flow_ctrl);

int eip160_get_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                                 struct sam_flow_ctrl *sam_fca)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    struct _sam_flow_ctrl _flow_ctrl;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sam_fca == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sam_fca->flow_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (flow_index) - SA rule num\n");
        return -EINVAL;
    }

    _flow_ctrl.dec_enc.raw_fca_macsec = _flow_ctrl.cabp_len.raw_fca_cac = 0;
    _module_base = ING_FLOW_CONTROL_PAGE0_MODULE_BASE +
                   ((sam_fca->flow_index > 127) ? 0x10000 : 0);
    _module_offset = (sam_fca->flow_index) * IE_MAX_SAM_FCA_BYTES;
    _absreg_addr = _module_base + _module_offset;

    /* Get Flow Ctrl Action data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: Flow Control Action word get failed\n");
        goto END;
    }

    _flow_ctrl.dec_enc.raw_fca_macsec = _data;
#if PRINT_DEBUG
    pr_err("Flow-ctrl word = %8x\n",  _flow_ctrl.dec_enc.raw_fca_macsec);
#endif
    sam_fca->flow_type = _flow_ctrl.dec_enc.flow_type;
    sam_fca->dest_port = _flow_ctrl.dec_enc.dest_port;
    sam_fca->drop_non_reserved = _flow_ctrl.dec_enc.drop_non_reserved;
    sam_fca->flow_crypt_auth = _flow_ctrl.dec_enc.flow_crypt_auth;
    sam_fca->drop_action = _flow_ctrl.dec_enc.drop_action;
    sam_fca->sa_index = _flow_ctrl.dec_enc.sa_index;
    sam_fca->sa_in_use = _flow_ctrl.dec_enc.sa_in_use;

    if ((sam_fca->flow_type == SAM_FCA_FLOW_BYPASS) && (sam_fca->flow_crypt_auth == 1)) {
        sam_fca->iv = _flow_ctrl.crypt_auth.iv;
        sam_fca->conf_offset_7 = _flow_ctrl.crypt_auth.conf_offset_7;
        sam_fca->icv_verify = _flow_ctrl.crypt_auth.icv_verify;
        sam_fca->conf_offset = _flow_ctrl.crypt_auth.conf_offset;
        sam_fca->encrypt_frames = _flow_ctrl.crypt_auth.encrypt_frames;
    } else if (sam_fca->flow_type == SAM_FCA_FLOW_INGRESS) {
        sam_fca->replay_protect = _flow_ctrl.dec_enc.rep_prot_fr;
        sam_fca->validate_frames = _flow_ctrl.dec_enc.vlevel_es_scb;
        sam_fca->conf_offset = _flow_ctrl.dec_enc.conf_offset;
        sam_fca->encrypt_frames = _flow_ctrl.dec_enc.encrypt_frames;
    }

    if ((_flow_ctrl.dec_enc.flow_type == SAM_FCA_FLOW_BYPASS) &&
            (_flow_ctrl.dec_enc.flow_crypt_auth == 1)) {
        _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
        _module_offset = EIPI_CRYPT_AUTH_CTRL;
        _absreg_addr = _module_base + _module_offset;

        /* Get CRYPT_AUTH_CTRL data */
        ret = IG_REG_READ(pdev, _absreg_addr, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: CRYPT_AUTH_CTRL word get failed\n");
            goto END;
        }

        _flow_ctrl.cabp_len.raw_fca_cac = _data;
        sam_fca->crypt_auth_bypass_len = _flow_ctrl.cabp_len.crypt_auth_bypass_len;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sam_flow_ctrl);

int eip160_clr_ing_sam_flow_ctrl(struct eip160_pdev *pdev,
                             const struct clr_sam_flow_ctrl *sam_fca)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sam_fca == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sam_fca->flow_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (flow_index) - SA rule num\n");
        return -EINVAL;
    }

    ret = eip160_ck_ing_sam_in_flight(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_ck_ing_sam_in_flight failed\n");
        goto END;
    }

    _module_base = ING_FLOW_CONTROL_PAGE0_MODULE_BASE +
                   ((sam_fca->flow_index > 127) ? 0x10000 : 0);
    _module_offset = (sam_fca->flow_index) * IE_MAX_SAM_FCA_BYTES;
    _absreg_addr = _module_base + _module_offset;

    /* Set Flow Ctrl Action data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, 0x0);

    if (ret < 0) {
        LOG_CRIT("ERROR: Flow Control Action word set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_clr_ing_sam_flow_ctrl);

int eip160_ck_ing_sam_in_flight(struct eip160_pdev *pdev)
{
    int ret = 0;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data, unsafe, busy_retry = IN_FLIGHT_BUSY_RETRY;

    _module_base = ING_SAM_ENABLE_CTRL_MODULE_BASE;    
    _module_offset = EIPI_SAM_IN_FLIGHT;
    _absreg_addr = _module_base + _module_offset;
    ret = IG_REG_WRITE(pdev, _absreg_addr, EIPI_SAM_IN_FLIGHT_LOAD_UNSAFE_MASK);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM_IN_FLIGHT unsafe write failed\n");
        goto END;
    }

    do {
        ret = IG_REG_READ(pdev, _absreg_addr, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM_IN_FLIGHT unsafe read failed\n");
            goto END;
        }

        unsafe = _data & EIPI_SAM_IN_FLIGHT_UNSAFE_MASK;
        busy_retry--;

        if (busy_retry == 0) {
            LOG_CRIT("\nERROR: EIP-160 device sync failed\n");
            ret = -1;
            break;
        }

    } while (unsafe);

END:
    return ret;
}
EXPORT_SYMBOL(eip160_ck_ing_sam_in_flight);

int eip160_cfg_ing_sam_rule(struct eip160_pdev *pdev,
                            const struct sa_match_parm *sam_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 *_sam = NULL;
    u16 _vlan_id = 0;
    struct _sam_misc_par _sam_misc;
    struct _sam_mask_par _sam_mask;
    struct _sam_extn_par _sam_extn;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sam_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

#if !defined(NO_TARGET_TEST)
    /* Validate SA rule num range */
    if (sam_par->rule_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (rule_index) - SA rule num\n");
        return -EINVAL;
    }
#endif

    _sam = os_mem_alloc(IE_MAX_SAM_SIZE_BYTES);

    if (!_sam) {
        LOG_CRIT("Memory alloc failed :- %s\n", __func__);
        ret = -ENOMEM;
        goto END;
    }

    memset(_sam, 0, IE_MAX_SAM_SIZE_BYTES);
    _sam_misc.raw_sam_misc = _sam_mask.raw_sam_mask = _sam_extn.raw_sam_extn = 0;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    _vlan_id = sam_par->vlan_par.vlan_id;
#endif

    _sam[0] = ((sam_par->sa_mac[0] << 00) | (sam_par->sa_mac[1] <<  8) |
               (sam_par->sa_mac[2] << 16) | (sam_par->sa_mac[3] << 24));
    _sam[1] = ((sam_par->sa_mac[4] << 00) | (sam_par->sa_mac[5] <<  8) |
               (BYTE_SWAP_16BIT(sam_par->eth_type) << 16));
    _sam[2] = ((sam_par->da_mac[0] << 00) | (sam_par->da_mac[1] <<  8) |
               (sam_par->da_mac[2] << 16) | (sam_par->da_mac[3] << 24));
    _sam[3] = ((sam_par->da_mac[4] << 00) | (sam_par->da_mac[5] <<  8) | (_vlan_id << 16));

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    _sam_misc.vlan_valid = sam_par->vlan_par.vlan_valid;
    _sam_misc.qinq_found = sam_par->vlan_par.qinq_found;
    _sam_misc.stag_valid = sam_par->vlan_par.stag_valid;
    _sam_misc.qtag_valid = sam_par->vlan_par.qtag_valid;
    _sam_misc.vlan_up = sam_par->vlan_par.vlan_up;
#endif

    _sam_misc.control_packet = sam_par->misc_par.control_packet;
    _sam_misc.untagged = sam_par->misc_par.untagged;
    _sam_misc.tagged = sam_par->misc_par.tagged;
    _sam_misc.bad_tag = sam_par->misc_par.bad_tag;
    _sam_misc.kay_tag = sam_par->misc_par.kay_tag;
    _sam_misc.source_port = sam_par->misc_par.source_port;
    _sam_misc.match_priority = sam_par->misc_par.match_priority;
    _sam_misc.macsec_tci_an = sam_par->misc_par.macsec_tci_an;
    _sam[4] = _sam_misc.raw_sam_misc;
    /* Below two words (SCI) is only applicable to IG rule */
    _sam[5] = cpu_to_le32(sam_par->sci0);
    _sam[6] = cpu_to_le32(sam_par->sci1);
    _sam_mask.mac_sa_mask = sam_par->mask_par.mac_sa_mask;
    _sam_mask.mac_da_mask = sam_par->mask_par.mac_da_mask;
    _sam_mask.mac_etype_mask = sam_par->mask_par.mac_etype_mask;
    _sam_mask.source_port_mask = sam_par->mask_par.source_port_mask;
    _sam_mask.ctrl_packet_mask = sam_par->mask_par.ctrl_packet_mask;
    _sam_mask.macsec_sci_mask = sam_par->mask_par.macsec_sci_mask;
    _sam_mask.tci_an_mask = sam_par->mask_par.tci_an_mask;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    _sam_mask.vlan_valid_mask = sam_par->mask_par.vlan_valid_mask;
    _sam_mask.qinq_found_mask = sam_par->mask_par.qinq_found_mask;
    _sam_mask.stag_valid_mask = sam_par->mask_par.stag_valid_mask;
    _sam_mask.qtag_valid_mask = sam_par->mask_par.qtag_valid_mask;
    _sam_mask.vlan_up_mask = sam_par->mask_par.vlan_up_mask;
    _sam_mask.vlan_id_mask = sam_par->mask_par.vlan_id_mask;
    _sam_mask.vlan_up_inner_mask = sam_par->mask_par.vlan_up_inner_mask;
    _sam_mask.vlan_id_inner_mask = sam_par->mask_par.vlan_id_inner_mask;
#endif

    _sam[7] = _sam_mask.raw_sam_mask;

    _sam_extn.flow_index = sam_par->flow_index;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    _sam_extn.vlan_id_inner = sam_par->vlan_par.vlan_id_inner;
    _sam_extn.vlan_up_inner = sam_par->vlan_par.vlan_up_inner;
#endif

    _sam[8] = _sam_extn.raw_sam_extn;

    _module_base = ING_SAM_RULES_PAGE0_MODULE_BASE +
                   ((sam_par->rule_index > 127) ? 0x10000 : 0);
    _module_offset = ((sam_par->rule_index) % 128) * (IE_MAX_SAM_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

#if defined(NO_TARGET_TEST) && NO_TARGET_TEST
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
        pr_info("SAM-Par[%02d] = %8x\n", i, _sam[i]);
    }
#else
    /* Set SAM's data */
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _sam[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM write fails for iteration %d\n", i);
            goto END;
        }
    }
#endif
#if PRINT_DEBUG
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
	pr_err("SAM-Par[%02d] = %8x\n", i, _sam[i]);
    }
#endif

END:
    if (_sam != NULL) {
        memzero_explicit(_sam, IE_MAX_SAM_SIZE_BYTES);
        os_mem_free(_sam);
    }

#if !defined(NO_TARGET_TEST)   
    ret = 0;
#endif

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sam_rule);

int eip160_get_ing_sam_rule(struct eip160_pdev *pdev,
                            struct sa_match_parm *sam_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    struct _raw_sam_par _sam_par;
    struct _sam_misc_par _sam_misc;
    struct _sam_mask_par _sam_mask;
    struct _sam_extn_par _sam_extn;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sam_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sam_par->rule_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (rule_index) - SA rule num\n");
        return -EINVAL;
    }

    memset(_sam_par.raw_sam_par, 0, IE_MAX_SAM_SIZE_BYTES);
    _sam_misc.raw_sam_misc = _sam_mask.raw_sam_mask = _sam_extn.raw_sam_extn = 0;
    _module_base = ING_SAM_RULES_PAGE0_MODULE_BASE +
                   ((sam_par->rule_index > 127) ? 0x10000 : 0);
    _module_offset = ((sam_par->rule_index) % 128) * (IE_MAX_SAM_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Get SAM's data */
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM read fails for iteration %d\n", i);
            goto END;
        }

        _sam_par.raw_sam_par[i] = _data;
    }

#if PRINT_DEBUG
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
        pr_err("SAM-Par[%02d] = %8x\n", i, _sam_par.raw_sam_par[i]);
    }
#endif

    memcpy(sam_par->sa_mac, (u8 *)&_sam_par.raw_sam_par[0], 6);
    memcpy(sam_par->sa_mac, (u8 *)&_sam_par.raw_sam_par[2], 6);

    sam_par->eth_type = (BYTE_SWAP_16BIT(_sam_par.raw_sam_par[1] >> 16));

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    sam_par->vlan_par.vlan_id = _sam_par.raw_sam_par[3] >> 16;
#endif

    _sam_misc.raw_sam_misc = _sam_par.raw_sam_par[4];

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    sam_par->vlan_par.vlan_valid = _sam_misc.vlan_valid;
    sam_par->vlan_par.qinq_found = _sam_misc.qinq_found;
    sam_par->vlan_par.stag_valid = _sam_misc.stag_valid;
    sam_par->vlan_par.qtag_valid = _sam_misc.qtag_valid;
    sam_par->vlan_par.vlan_up = _sam_misc.vlan_up;
#endif

    sam_par->misc_par.control_packet = _sam_misc.control_packet;
    sam_par->misc_par.untagged = _sam_misc.untagged;
    sam_par->misc_par.tagged = _sam_misc.tagged;
    sam_par->misc_par.bad_tag = _sam_misc.bad_tag;
    sam_par->misc_par.kay_tag = _sam_misc.kay_tag;
    sam_par->misc_par.source_port = _sam_misc.source_port;
    sam_par->misc_par.match_priority = _sam_misc.match_priority;
    sam_par->misc_par.macsec_tci_an = _sam_misc.macsec_tci_an;

    sam_par->sci0 = _sam_par.raw_sam_par[5];
    sam_par->sci1 = _sam_par.raw_sam_par[6];

    _sam_mask.raw_sam_mask = _sam_par.raw_sam_par[7];
    sam_par->mask_par.mac_sa_mask = _sam_mask.mac_sa_mask;
    sam_par->mask_par.mac_da_mask = _sam_mask.mac_da_mask;
    sam_par->mask_par.mac_etype_mask = _sam_mask.mac_etype_mask;
    sam_par->mask_par.source_port_mask = _sam_mask.source_port_mask;
    sam_par->mask_par.ctrl_packet_mask = _sam_mask.ctrl_packet_mask;
    sam_par->mask_par.macsec_sci_mask = _sam_mask.macsec_sci_mask;
    sam_par->mask_par.tci_an_mask = _sam_mask.tci_an_mask;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    sam_par->mask_par.vlan_valid_mask = _sam_mask.vlan_valid_mask;
    sam_par->mask_par.qinq_found_mask = _sam_mask.qinq_found_mask;
    sam_par->mask_par.stag_valid_mask = _sam_mask.stag_valid_mask;
    sam_par->mask_par.qtag_valid_mask = _sam_mask.qtag_valid_mask;
    sam_par->mask_par.vlan_up_mask = _sam_mask.vlan_up_mask;
    sam_par->mask_par.vlan_id_mask = _sam_mask.vlan_id_mask;
    sam_par->mask_par.vlan_up_inner_mask = _sam_mask.vlan_up_inner_mask;
    sam_par->mask_par.vlan_id_inner_mask = _sam_mask.vlan_id_inner_mask;
#endif

    _sam_extn.raw_sam_extn = _sam_par.raw_sam_par[8];
    sam_par->flow_index = _sam_extn.flow_index;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    sam_par->vlan_par.vlan_id_inner = _sam_extn.vlan_id_inner;
    sam_par->vlan_par.vlan_up_inner = _sam_extn.vlan_up_inner;
#endif

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sam_rule);

int eip160_clr_ing_sam_rule(struct eip160_pdev *pdev,
                             const struct clr_sam_par *clr_sam)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || clr_sam == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (clr_sam->rule_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (rule_index) - SA rule num\n");
        return -EINVAL;
    }

    ret = eip160_ck_ing_sam_in_flight(pdev);

    if (ret < 0) {
        LOG_CRIT("ERROR: api eip160_ck_ing_sam_in_flight failed\n");
        goto END;
    }

    _module_base = ING_SAM_RULES_PAGE0_MODULE_BASE +
                   ((clr_sam->rule_index > 127) ? 0x10000 : 0);
    _module_offset = ((clr_sam->rule_index) % 128) * (IE_MAX_SAM_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Set SAM's data */
    for (i = 0; i < (IE_MAX_SAM_SIZE_WORDS - 7); i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, 0x0);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM write fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_clr_ing_sam_rule);

int eip160_cfg_ing_sam_ena_ctrl(struct eip160_pdev *pdev,
                                const struct sam_enable_ctrl *ee_ctrl)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _sam_ena_ctrl _sam_eec;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ee_ctrl == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validation
    It is not allowed to write the register with the sam_index_set field equal
    to sam_index_clear while both set_enable and clear_enable are 1b. */
    if (ee_ctrl->set_enable && ee_ctrl->clear_enable) {
        if (ee_ctrl->sam_index_set == ee_ctrl->sam_index_clear)
            LOG_ERR("WARN: Set & Clear indexes can't be same at a time\n");
        return -EINVAL;
    }

    _sam_eec.raw_sam_eec = 0;
    _sam_eec.sam_index_set = ee_ctrl->sam_index_set;
    _sam_eec.set_enable = ee_ctrl->set_enable;
    _sam_eec.set_all = ee_ctrl->set_all;
    _sam_eec.sam_index_clear = ee_ctrl->sam_index_clear;
    _sam_eec.clear_enable = ee_ctrl->clear_enable;
    _sam_eec.clear_all = ee_ctrl->clear_all;

    _module_base = ING_SAM_ENABLE_CTRL_MODULE_BASE;
    _module_offset = EIPI_SAM_ENTRY_ENABLE_CTRL;
    _absreg_addr = _module_base + _module_offset;
    /* Set SAM EE control data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _sam_eec.raw_sam_eec);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM EE control write failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sam_ena_ctrl);

int eip160_get_ing_sam_ena_flags(struct eip160_pdev *pdev,
                                 struct sam_enable_flags *en_flags)
{
    int i, ret = 0;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || en_flags == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SAM Entry Enable feature range */
    if (en_flags->flags_type > SAM_ENTRY_CLEAR) {
        LOG_ERR("WARN: Invalid input - SAM Entry Enable flags type\n");
        return -EINVAL;
    }

    memset(en_flags->raw_sam_flags, 0, (IE_MAX_SAM_SIZE_BYTES / 2));

    if (en_flags->flags_type == SAM_ENTRY_ENABLE)
        _module_offset = EIPI_SAM_ENTRY_ENABLE1;
    else if (en_flags->flags_type == SAM_ENTRY_TOGGLE)
        _module_offset = EIPI_SAM_ENTRY_TOGGLE1;
    else if (en_flags->flags_type == SAM_ENTRY_SET)
        _module_offset = EIPI_SAM_ENTRY_SET1;
    else
        _module_offset = EIPI_SAM_ENTRY_CLEAR1;

    _module_base = ING_SAM_ENABLE_CTRL_MODULE_BASE;
    _absreg_addr = _module_base + _module_offset;

    /* Get SAM EE flags flags data */
    for (i = 0; i < (pdev->ig_nr_of_sas / 32); i++) {
        if (i == (MACSEC_MAX_SA_RULES / 32))
            break;

        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM EE flags read fails for iteration %d\n", i);
            goto END;
        }

        en_flags->raw_sam_flags[i] = _data;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sam_ena_flags);

int eip160_cfg_ing_cpm_rule(struct eip160_pdev *pdev,
                            const struct cp_match_parm *cpm_par)
{
    int i, num_words = 1, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 *_cpc_rule = NULL;
    struct _cpc_vlan_par _cpc_vlan;

    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cpm_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if ((cpm_par->cpc_type > CPC_ENTRY_OUTER_VLAN_ONLY) ||
            (cpm_par->cpc_index > (MAX_CTRL_PKT_CLASS_ENTRIES - 1))) {
        LOG_ERR("WARN: Invalid input (cpc_type) - CPC entry type\n");
        return -EINVAL;
    }

    /* Set Control Packet Classification rule */
    _cpc_rule = os_mem_alloc(IE_MAX_CPC_SIZE_BYTES);

    if (!_cpc_rule) {
        LOG_CRIT("Memory alloc failed :- %s\n", __func__);
        ret = -ENOMEM;
        goto END;
    }

    memset(_cpc_rule, 0, IE_MAX_CPC_SIZE_BYTES);
    _cpc_vlan.raw_cpc_vlan = 0;

    if (cpm_par->cpc_type < CPC_ENTRY_ETH_ONLY) {
        if (cpm_par->cpc_index > 9) {
            LOG_ERR("WARN: Invalid input (entry_num) - CPC ETH num\n");
            ret = -EINVAL;
            goto END;
        }

        /* Populate DA_MAC / DA_MAC_START / DA_MAC_CONST44 / DAM_MAC_CONST48 */
        _cpc_rule[0] = ((cpm_par->mac_da[0] << 00) | (cpm_par->mac_da[1] <<  8) |
                        (cpm_par->mac_da[2] << 16) | (cpm_par->mac_da[3] << 24));
        _cpc_rule[1] = ((cpm_par->mac_da[4] << 00) | (cpm_par->mac_da[5] <<  8));
        /* Update the populated 'num_words' to write */
        num_words = 2;

        if (cpm_par->cpc_type == CPC_ENTRY_DA_RANGE) {
            _module_offset =  EIPI_CP_MAC_DA_START_LO;
            /* Populate DA_MAC_END */
            _cpc_rule[2] = ((cpm_par->mac_da_end[0] << 00) | (cpm_par->mac_da_end[1] <<  8) |
                            (cpm_par->mac_da_end[2] << 16) | (cpm_par->mac_da_end[3] << 24));
            _cpc_rule[3] = ((cpm_par->mac_da_end[4] << 00) | (cpm_par->mac_da_end[5] <<  8));
            /* Update the populated 'num_words' to write */
            num_words = 4;
        } else if (cpm_par->cpc_type == CPC_ENTRY_DA_CONST48)
            /* DA_MAC_CONST44 is already populated in '_cpc_rule[0 & 1]' */
            _module_offset =  EIPI_CP_MAC_DA_48_BITS_LO;
        else if (cpm_par->cpc_type == CPC_ENTRY_DA_CONST44)
            /* DA_MAC_CONST48 is already populated in '_cpc_rule[0 & 1]' */
            _module_offset =  EIPI_CP_MAC_DA_44_BITS_LO;
        else {
            /* Populate eth-type field */
            _cpc_rule[1] = (_cpc_rule[1] | (BYTE_SWAP_16BIT(cpm_par->ether_type) << 16));
            _module_offset = (cpm_par->cpc_index) * IE_CPC_DA_ETH_SIZE_BYTES;
        }
    } else {
        if (cpm_par->cpc_index < 10) {
            LOG_ERR("WARN: Invalid input (entry_num) - CPC ETH num\n");
            ret = -EINVAL;
            goto END;
        }

        if (cpm_par->cpc_type == CPC_ENTRY_ETH_ONLY) {
            /* Populate '_cpc_rule[0]' with eth-type only */
            _cpc_rule[0] = BYTE_SWAP_16BIT(cpm_par->ether_type) << 16;
        } else {
            _cpc_vlan.vlan_id = cpm_par->vlan_id;
            _cpc_vlan.vlan_up = cpm_par->vlan_up;
            _cpc_vlan.disable_up = cpm_par->disable_up;
            /* Populate '_cpc_rule[0]' with VLAN info only */
            _cpc_rule[0] = BYTE_SWAP_16BIT(_cpc_vlan.raw_cpc_vlan >> 16) << 16;
        }

        _module_offset = ((cpm_par->cpc_index - 10) * 4) + EIPI_CP_MAC_ET_MATCH_10;
    }

    _module_base = ING_CLASSIFIER_CONTROL_PACKET_MODULE_BASE;
    _absreg_addr = _module_base + _module_offset;

    for (i = 0; i < num_words; i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _cpc_rule[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: CPC write fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    if (_cpc_rule != NULL) {
        memzero_explicit(_cpc_rule, IE_MAX_CPC_SIZE_BYTES);
        os_mem_free(_cpc_rule);
    }

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_cpm_rule);

int eip160_get_ing_cpm_rule(struct eip160_pdev *pdev,
                            struct cp_match_parm *cpm_par)
{
    int i, num_words = 1, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 *_cpc_rule = NULL;
    struct _cpc_vlan_par _cpc_vlan;

    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cpm_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if ((cpm_par->cpc_type > CPC_ENTRY_OUTER_VLAN_ONLY) ||
            (cpm_par->cpc_index > (MAX_CTRL_PKT_CLASS_ENTRIES - 1))) {
        LOG_ERR("WARN: Invalid input (cpc_type) - CPC entry type\n");
        return -EINVAL;
    }

    _cpc_rule = os_mem_alloc(IE_MAX_CPC_SIZE_BYTES);

    if (!_cpc_rule) {
        LOG_CRIT("Memory alloc failed :- %s\n", __func__);
        ret = -ENOMEM;
        goto END;
    }

    memset(_cpc_rule, 0, IE_MAX_CPC_SIZE_BYTES);
    _cpc_vlan.raw_cpc_vlan = 0;

    if (cpm_par->cpc_type < CPC_ENTRY_ETH_ONLY) {
        if (cpm_par->cpc_index > 9) {
            LOG_ERR("WARN: Invalid input (entry_num) - CPC ETH num\n");
            ret = -EINVAL;
            goto END;
        }

        /* Update 'num_words' to read */
        num_words = 2;

        if (cpm_par->cpc_type == CPC_ENTRY_DA_RANGE) {
            _module_offset =  EIPI_CP_MAC_DA_START_LO;
            /* Update 'num_words' to read */
            num_words = 4;
        } else if (cpm_par->cpc_type == CPC_ENTRY_DA_CONST48)
            _module_offset =  EIPI_CP_MAC_DA_48_BITS_LO;
        else if (cpm_par->cpc_type == CPC_ENTRY_DA_CONST44)
            _module_offset =  EIPI_CP_MAC_DA_44_BITS_LO;
        else {
            _module_offset = (cpm_par->cpc_index) * IE_CPC_DA_ETH_SIZE_BYTES;
        }
    } else {
        if (cpm_par->cpc_index < 10) {
            LOG_ERR("WARN: Invalid input (entry_num) - CPC ETH num\n");
            ret = -EINVAL;
            goto END;
        }

        _module_offset = ((cpm_par->cpc_index - 10) * 4) + EIPI_CP_MAC_ET_MATCH_10;
    }

    _module_base = ING_CLASSIFIER_CONTROL_PACKET_MODULE_BASE;
    _absreg_addr = _module_base + _module_offset;

    for (i = 0; i < num_words; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_cpc_rule[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: CPC write fails for iteration %d\n", i);
            goto END;
        }
    }

    if (cpm_par->cpc_type < CPC_ENTRY_ETH_ONLY) {
        /* Copy DA_MAC / DA_MAC_START / DA_MAC_CONST44 / DAM_MAC_CONST48 */
        memcpy(cpm_par->mac_da, (u8 *)&_cpc_rule[0], 6);

        if (cpm_par->cpc_type == CPC_ENTRY_DA_RANGE) {
            /* Copy DA_MAC_END */
            memcpy(cpm_par->mac_da_end, (u8 *)&_cpc_rule[2], 6);
        } else if (cpm_par->cpc_type < CPC_ENTRY_DA_CONST44) {
            /* Copy eth-type field */
            cpm_par->ether_type = BYTE_SWAP_16BIT(_cpc_rule[1] >> 16);
        }
    } else {
        if (cpm_par->cpc_type == CPC_ENTRY_ETH_ONLY) {
            /* Copy eth-type field */
            cpm_par->ether_type = BYTE_SWAP_16BIT(_cpc_rule[0] >> 16);
        } else {
            /* Copy VLAN info */
            _cpc_vlan.raw_cpc_vlan = (BYTE_SWAP_16BIT(_cpc_rule[0] >> 16)) << 16;
            cpm_par->vlan_id = _cpc_vlan.vlan_id;
            cpm_par->vlan_up = _cpc_vlan.vlan_up;
            cpm_par->disable_up = _cpc_vlan.disable_up;
        }
    }

END:
    if (_cpc_rule != NULL) {
        memzero_explicit(_cpc_rule, IE_MAX_CPC_SIZE_BYTES);
        os_mem_free(_cpc_rule);
    }

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_cpm_rule);

int eip160_cfg_ing_cpm_ena_ctrl(struct eip160_pdev *pdev,
                                const struct cpm_mode_ena *cpc_mm)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _cp_match_ena _cpm_en;
    struct _cp_match_mode _cpm_mode;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cpc_mm == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _cpm_en.raw_cpm_enable = _cpm_mode.raw_cpm_mode = 0;
    _cpm_mode.vlan_sel_0_7 = cpc_mm->vlan_sel_0_7;
    _cpm_mode.e_type_sel_0_9 = cpc_mm->e_type_sel_0_17 & 0x3FF;
    _cpm_mode.e_type_sel_10_17 = (cpc_mm->e_type_sel_0_17 >> 10) & 0xFF;

    _module_base = ING_CLASSIFIER_CONTROL_PACKET_MODULE_BASE;
    _module_offset = EIPI_CP_MATCH_MODE;
    _absreg_addr = _module_base + _module_offset;
    /* Set Control Packet Classification match mode */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _cpm_mode.raw_cpm_mode);

    if (ret < 0) {
        LOG_CRIT("ERROR: CP match mode set failed\n");
        goto END;
    }

    _cpm_en.mac_da_0_7 = cpc_mm->mac_da_0_7;
    _cpm_en.e_type_0_7 = cpc_mm->e_type_0_7;
    _cpm_en.combined_8_9 = cpc_mm->combined_8_9;
    _cpm_en.dmac_range = cpc_mm->dmac_range;
    _cpm_en.dmac_const_44 = cpc_mm->dmac_const_44;
    _cpm_en.dmac_const_48 = cpc_mm->dmac_const_48;
    _cpm_en.e_type_10_17 = cpc_mm->e_type_10_17;

    _module_offset = EIPI_CP_MATCH_ENABLE;
    _absreg_addr = _module_base + _module_offset;
    /* Set Control Packet Classification match enable */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _cpm_en.raw_cpm_enable);

    if (ret < 0) {
        LOG_CRIT("ERROR: CP match enable set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_cpm_ena_ctrl);

int eip160_get_ing_cpm_ena_flags(struct eip160_pdev *pdev,
                                 struct cpm_mode_ena *cpc_mm)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _cp_match_ena _cpm_en;
    struct _cp_match_mode _cpm_mode;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cpc_mm == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _cpm_en.raw_cpm_enable = _cpm_mode.raw_cpm_mode = 0;
    _module_base = ING_CLASSIFIER_CONTROL_PACKET_MODULE_BASE;
    _module_offset = EIPI_CP_MATCH_MODE;
    _absreg_addr = _module_base + _module_offset;
    /* Set Control Packet Classification match mode */
    ret = IG_REG_READ(pdev, _absreg_addr, &_cpm_mode.raw_cpm_mode);

    if (ret < 0) {
        LOG_CRIT("ERROR: CP match mode set failed\n");
        goto END;
    }

    cpc_mm->vlan_sel_0_7 = _cpm_mode.vlan_sel_0_7;
    cpc_mm->e_type_sel_0_17 = _cpm_mode.e_type_sel_0_9;
    cpc_mm->e_type_sel_0_17 |= (_cpm_mode.e_type_sel_10_17 << 10);

    _module_offset = EIPI_CP_MATCH_ENABLE;
    _absreg_addr = _module_base + _module_offset;
    /* Set Control Packet Classification match enable */
    ret = IG_REG_READ(pdev, _absreg_addr, &_cpm_en.raw_cpm_enable);

    if (ret < 0) {
        LOG_CRIT("ERROR: CP match enable set failed\n");
        goto END;
    }

    cpc_mm->mac_da_0_7 = _cpm_en.mac_da_0_7;
    cpc_mm->e_type_0_7 = _cpm_en.e_type_0_7;
    cpc_mm->combined_8_9 = _cpm_en.combined_8_9;
    cpc_mm->dmac_range = _cpm_en.dmac_range;
    cpc_mm->dmac_const_44 = _cpm_en.dmac_const_44;
    cpc_mm->dmac_const_48 = _cpm_en.dmac_const_48;
    cpc_mm->e_type_10_17 = _cpm_en.e_type_10_17;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_cpm_ena_flags);

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
int eip160_cfg_ing_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _vlan_info[4];
    struct _sam_cp_tag _cp_tag;
    struct sam_cp_tag2 _cp_tag2;
    struct _sam_pp_tags _pp_tags;
    struct _sam_pp_tags2 _pp_tags2;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || vlan_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _cp_tag.raw_ocp_tag = _cp_tag2.raw_icp_tag = 0;
    _pp_tags.raw_pp_tags = _pp_tags2.raw_pp_tags2 = 0;
    _cp_tag.map_tbl_pcp_0 = vlan_par->map_tbl_opcp_x[0] & 0x7;
    _cp_tag.map_tbl_pcp_1 = vlan_par->map_tbl_opcp_x[1] & 0x7;
    _cp_tag.map_tbl_pcp_2 = vlan_par->map_tbl_opcp_x[2] & 0x7;
    _cp_tag.map_tbl_pcp_3 = vlan_par->map_tbl_opcp_x[3] & 0x7;
    _cp_tag.map_tbl_pcp_4 = vlan_par->map_tbl_opcp_x[4] & 0x7;
    _cp_tag.map_tbl_pcp_5 = vlan_par->map_tbl_opcp_x[5] & 0x7;
    _cp_tag.map_tbl_pcp_6 = vlan_par->map_tbl_opcp_x[6] & 0x7;
    _cp_tag.map_tbl_pcp_7 = vlan_par->map_tbl_opcp_x[7] & 0x7;
    _cp_tag.def_up = vlan_par->def_up;
    _cp_tag.stag_up_en = vlan_par->stag_up_en;
    _cp_tag.qtag_up_en = vlan_par->qtag_up_en;
    _cp_tag.parse_qinq = vlan_par->parse_qinq;
    _cp_tag.parse_stag = vlan_par->parse_stag;
    _cp_tag.parse_qtag = vlan_par->parse_qtag;
    _vlan_info[0] = _cp_tag.raw_ocp_tag;

    _pp_tags.qtag_value = vlan_par->qtag_value;
    _pp_tags.stag_value = vlan_par->stag_value;
    _vlan_info[1] = _pp_tags.raw_pp_tags;

    _pp_tags2.stag_value2 = vlan_par->stag_value2;
    _pp_tags2.stag_value3 = vlan_par->stag_value3;
    _vlan_info[2] = _pp_tags2.raw_pp_tags2;

    _cp_tag2.map_tbl2_pcp_0 = vlan_par->map_tbl_ipcp_x[0] & 0x7;
    _cp_tag2.map_tbl2_pcp_1 = vlan_par->map_tbl_ipcp_x[1] & 0x7;
    _cp_tag2.map_tbl2_pcp_2 = vlan_par->map_tbl_ipcp_x[2] & 0x7;
    _cp_tag2.map_tbl2_pcp_3 = vlan_par->map_tbl_ipcp_x[3] & 0x7;
    _cp_tag2.map_tbl2_pcp_4 = vlan_par->map_tbl_ipcp_x[4] & 0x7;
    _cp_tag2.map_tbl2_pcp_5 = vlan_par->map_tbl_ipcp_x[5] & 0x7;
    _cp_tag2.map_tbl2_pcp_6 = vlan_par->map_tbl_ipcp_x[6] & 0x7;
    _cp_tag2.map_tbl2_pcp_7 = vlan_par->map_tbl_ipcp_x[7] & 0x7;
    _vlan_info[4] = _cp_tag2.raw_icp_tag;

    _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
    _module_offset = EIPI_SAM_CP_TAG;
    _absreg_addr = _module_base + _module_offset;

    /* Set VLAN parsing data */
    for (i = 0; i < 4; i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _vlan_info[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM VLAN control write fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_vlan_parser);

int eip160_get_ing_vlan_parser(struct eip160_pdev *pdev,
                               const struct vlan_parse_parm *vlan_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _vlan_info[4];
    struct _sam_cp_tag _cp_tag;
    struct _sam_cp_tag2 _cp_tag2;
    struct _sam_pp_tags _pp_tags;
    struct _sam_pp_tags2 _pp_tags2;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || vlan_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
    _module_offset = EIPI_SAM_CP_TAG;
    _absreg_addr = _module_base + _module_offset;

    /* Set VLAN parsing data */
    for (i = 0; i < 4; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_vlan_info[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: SAM VLAN control read fails for iteration %d\n", i);
            goto END;
        }
    }

    _cp_tag.raw_ocp_tag = _cp_tag2.raw_icp_tag = 0;
    _pp_tags.raw_pp_tags = _pp_tags2.raw_pp_tags2 = 0;
    _cp_tag.raw_ocp_tag = _vlan_info[0];
    vlan_par->map_tbl_opcp_x[0] = _cp_tag.map_tbl_pcp_0 & 0x7;
    vlan_par->map_tbl_opcp_x[1] = _cp_tag.map_tbl_pcp_1 & 0x7;
    vlan_par->map_tbl_opcp_x[2] = _cp_tag.map_tbl_pcp_2 & 0x7;
    vlan_par->map_tbl_opcp_x[3] = _cp_tag.map_tbl_pcp_3 & 0x7;
    vlan_par->map_tbl_opcp_x[4] = _cp_tag.map_tbl_pcp_4 & 0x7;
    vlan_par->map_tbl_opcp_x[5] = _cp_tag.map_tbl_pcp_5 & 0x7;
    vlan_par->map_tbl_opcp_x[6] = _cp_tag.map_tbl_pcp_6 & 0x7;
    vlan_par->map_tbl_opcp_x[7] = _cp_tag.map_tbl_pcp_7 & 0x7;
    vlan_par->def_up = _cp_tag.def_up;
    vlan_par->stag_up_en = _cp_tag.stag_up_en;
    vlan_par->qtag_up_en = _cp_tag.qtag_up_en;
    vlan_par->parse_qinq = _cp_tag.parse_qinq;
    vlan_par->parse_stag = _cp_tag.parse_stag;
    vlan_par->parse_qtag = _cp_tag.parse_qtag;

    _pp_tags.raw_pp_tags = _vlan_info[1];
    vlan_par->qtag_value = _pp_tags.qtag_value;
    vlan_par->stag_value = _pp_tags.stag_value;

    _pp_tags2.raw_pp_tags2 = _vlan_info[2];
    vlan_par->stag_value2 = _pp_tags2.stag_value2;
    vlan_par->stag_value3 = _pp_tags2.stag_value3;

    _cp_tag2.raw_icp_tag = _vlan_info[4];
    vlan_par->map_tbl_ipcp_x[0] = _cp_tag2.map_tbl2_pcp_0 & 0x7;
    vlan_par->map_tbl_ipcp_x[1] = _cp_tag2.map_tbl2_pcp_1 & 0x7;
    vlan_par->map_tbl_ipcp_x[2] = _cp_tag2.map_tbl2_pcp_2 & 0x7;
    vlan_par->map_tbl_ipcp_x[3] = _cp_tag2.map_tbl2_pcp_3 & 0x7;
    vlan_par->map_tbl_ipcp_x[4] = _cp_tag2.map_tbl2_pcp_4 & 0x7;
    vlan_par->map_tbl_ipcp_x[5] = _cp_tag2.map_tbl2_pcp_5 & 0x7;
    vlan_par->map_tbl_ipcp_x[6] = _cp_tag2.map_tbl2_pcp_6 & 0x7;
    vlan_par->map_tbl_ipcp_x[7] = _cp_tag2.map_tbl2_pcp_7 & 0x7;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_vlan_parser);
#endif

int eip160_cfg_ing_sanm_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct sanm_flow_ctrl *sanm_fca)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _nm_ncp = 0, _nm_cp = 0;
    struct _sanm_fca_pft _nm_ncp_fc, _nm_cp_fc;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sanm_fca == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA non-match NCP params */
    if ((sanm_fca->pkt_type > SA_NM_SECY_ALL_TAGGED) ||
            (sanm_fca->nctrl_pkt.flow_type > SA_NM_FCA_FLOW_DROP) ||
            (sanm_fca->nctrl_pkt.dest_port > SA_NM_FCA_DPT_UNCONTROL) ||
            (sanm_fca->nctrl_pkt.drop_action > SA_NM_FCA_NO_DROP)) {
        LOG_ERR("WARN: Invalid input SA non-match NCP parameter\n");
        return -EINVAL;
    }

    /* Validate SA non-match CP params */
    if ((sanm_fca->ctrl_pkt.flow_type > SA_NM_FCA_FLOW_DROP) ||
            (sanm_fca->ctrl_pkt.dest_port > SA_NM_FCA_DPT_UNCONTROL) ||
            (sanm_fca->ctrl_pkt.drop_action > SA_NM_FCA_NO_DROP)) {
        LOG_ERR("WARN: Invalid input SA non-match CP parameter\n");
        return -EINVAL;
    }

    _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
    _module_offset = EIPI_SAM_NM_FLOW_NCP;
    _absreg_addr = _module_base + _module_offset;
    /* Get SAM_NM_FLOW_NCP */
    ret = IG_REG_READ(pdev, _absreg_addr, &_nm_ncp);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM_NM_FLOW_NCP get failed\n");
        goto END;
    }

    _nm_ncp_fc.raw_sanm_fc = _nm_ncp;

    if ((sanm_fca->pkt_type == SA_NM_SECY_UNTAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_ncp_fc.utag_flow_type = sanm_fca->nctrl_pkt.flow_type;
        _nm_ncp_fc.utag_dest_port = sanm_fca->nctrl_pkt.dest_port;
        _nm_ncp_fc.utag_drop_non_res = sanm_fca->nctrl_pkt.drop_non_reserved;
        _nm_ncp_fc.utag_drop_action = sanm_fca->nctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_ncp_fc.tag_flow_type = sanm_fca->nctrl_pkt.flow_type;
        _nm_ncp_fc.tag_dest_port = sanm_fca->nctrl_pkt.dest_port;
        _nm_ncp_fc.tag_drop_non_res = sanm_fca->nctrl_pkt.drop_non_reserved;
        _nm_ncp_fc.tag_drop_action = sanm_fca->nctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_BAD_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_ncp_fc.btag_flow_type = sanm_fca->nctrl_pkt.flow_type;
        _nm_ncp_fc.btag_dest_port = sanm_fca->nctrl_pkt.dest_port;
        _nm_ncp_fc.btag_drop_non_res = sanm_fca->nctrl_pkt.drop_non_reserved;
        _nm_ncp_fc.btag_drop_action = sanm_fca->nctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_KAY_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_ncp_fc.ktag_flow_type = sanm_fca->nctrl_pkt.flow_type;
        _nm_ncp_fc.ktag_dest_port = sanm_fca->nctrl_pkt.dest_port;
        _nm_ncp_fc.ktag_drop_non_res = sanm_fca->nctrl_pkt.drop_non_reserved;
        _nm_ncp_fc.ktag_drop_action = sanm_fca->nctrl_pkt.drop_action;
    }

    /* Set SAM_NM_FLOW_NCP data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _nm_ncp_fc.raw_sanm_fc);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM_NM_FLOW_NCP set failed\n");
        goto END;
    }

    _module_offset = EIPI_SAM_NM_FLOW_CP;
    _absreg_addr = _module_base + _module_offset;
    /* Get SAM_NM_FLOW_CP */
    ret = IG_REG_READ(pdev, _absreg_addr, &_nm_cp);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM_NM_FLOW_CP get failed\n");
        goto END;
    }

    _nm_cp_fc.raw_sanm_fc = _nm_cp;

    if ((sanm_fca->pkt_type == SA_NM_SECY_UNTAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_cp_fc.utag_flow_type = sanm_fca->ctrl_pkt.flow_type;
        _nm_cp_fc.utag_dest_port = sanm_fca->ctrl_pkt.dest_port;
        _nm_cp_fc.utag_drop_non_res = sanm_fca->ctrl_pkt.drop_non_reserved;
        _nm_cp_fc.utag_drop_action = sanm_fca->ctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_cp_fc.tag_flow_type = sanm_fca->ctrl_pkt.flow_type;
        _nm_cp_fc.tag_dest_port = sanm_fca->ctrl_pkt.dest_port;
        _nm_cp_fc.tag_drop_non_res = sanm_fca->ctrl_pkt.drop_non_reserved;
        _nm_cp_fc.tag_drop_action = sanm_fca->ctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_BAD_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_cp_fc.btag_flow_type = sanm_fca->ctrl_pkt.flow_type;
        _nm_cp_fc.btag_dest_port = sanm_fca->ctrl_pkt.dest_port;
        _nm_cp_fc.btag_drop_non_res = sanm_fca->ctrl_pkt.drop_non_reserved;
        _nm_cp_fc.btag_drop_action = sanm_fca->ctrl_pkt.drop_action;
    }

    if ((sanm_fca->pkt_type == SA_NM_SECY_KAY_TAGGED) ||
            (sanm_fca->pkt_type == SA_NM_SECY_ALL_TAGGED)) {
        _nm_cp_fc.ktag_flow_type = sanm_fca->ctrl_pkt.flow_type;
        _nm_cp_fc.ktag_dest_port = sanm_fca->ctrl_pkt.dest_port;
        _nm_cp_fc.ktag_drop_non_res = sanm_fca->ctrl_pkt.drop_non_reserved;
        _nm_cp_fc.ktag_drop_action = sanm_fca->ctrl_pkt.drop_action;
    }

    /* Set SAM_NM_FLOW_CP data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _nm_cp_fc.raw_sanm_fc);

    if (ret < 0) {
        LOG_CRIT("ERROR: SAM_NM_FLOW_CP set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sanm_flow_ctrl);

int eip160_cfg_ing_misc_flow_ctrl(struct eip160_pdev *pdev,
                                  const struct misc_ctrl *misc_ctrl)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _misc_ctrl _misc_ctrl;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || misc_ctrl == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate Counter Control params */
    if (misc_ctrl->validate_frames > SAM_FCA_VALIDATE_STRICT) {
        LOG_ERR("WARN: Invalid input Misc Control parameter\n");
        return -EINVAL;
    }

    _misc_ctrl.raw_misc_ctrl = 0;
    _misc_ctrl.mc_latency_fix = misc_ctrl->mc_latency_fix & 0xFF;
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
    _misc_ctrl.static_bypass = misc_ctrl->static_bypass;
#endif
    _misc_ctrl.nm_macsec_en = misc_ctrl->nm_macsec_en;
    _misc_ctrl.validate_frames = misc_ctrl->validate_frames;
    _misc_ctrl.sectag_after_vlan = misc_ctrl->sectag_after_vlan;
    _misc_ctrl.xform_rec_size = 1;
    _misc_ctrl.mc_latency_fix_m = (misc_ctrl->mc_latency_fix >> 8) & 0x3F;

    _module_base = ING_CLASSIFIER_VARIOUS_CONTROL_REGS_MODULE_BASE;
    _module_offset = EIPI_MISC_CONTROL;
    _absreg_addr = _module_base + _module_offset;
    /* Set Counter Ctrl flags data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _misc_ctrl.raw_misc_ctrl);

    if (ret < 0) {
        LOG_CRIT("ERROR: Misc Control word set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_misc_flow_ctrl);

int eip160_get_ing_sa_stats(struct eip160_pdev *pdev,
                            struct ing_sa_stats *sa_stats)
{
    int i, ret, rule_num;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u64 _sa_stats[MAX_ING_SA_COUNTERS];
    u32 _data, _data2;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sa_stats == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA rule num range */
    if (sa_stats->rule_index >= pdev->ig_nr_of_sas) {
        LOG_ERR("WARN: Invalid input (sa_index) - SA rule num\n");
        return -EINVAL;
    }

    rule_num = sa_stats->rule_index;
    memset(sa_stats, 0, sizeof(struct ing_sa_stats));
    sa_stats->rule_index = rule_num;
    _module_base = ING_STAT_SA_PAGE0_MODULE_BASE +
                   ((sa_stats->rule_index > 127) ? 0x10000 : 0);
    _module_offset = ((sa_stats->rule_index) % 128) * (MAX_SA_STATS_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Get given SA stats */
    for (i = 0; i < MAX_ING_SA_COUNTERS; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8), &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: SA stats low read fails for iteration %d\n", i);
            goto END;
        }

        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8) + 4, &_data2);

        if (ret < 0) {
            LOG_CRIT("ERROR: SA stats high read fails for iteration %d\n", i);
            goto END;
        }

        _sa_stats[i] = ((u64)_data2 << 32) | _data;
    }

    sa_stats->InOctetsDecrypted = _sa_stats[0];
    sa_stats->InOctetsDecrypted2 = _sa_stats[1];
    sa_stats->InPktsUnchecked = _sa_stats[2];
    sa_stats->InPktsDelayed = _sa_stats[3];
    sa_stats->InPktsLate = _sa_stats[4];
    sa_stats->InPktsOK = _sa_stats[5];
    sa_stats->InPktsInvalid = _sa_stats[6];
    sa_stats->InPktsNotValid = _sa_stats[7];
    sa_stats->InPktsNotUsingSA = _sa_stats[8];
    sa_stats->InPktsUnusedSA = _sa_stats[9];
    sa_stats->InPktsUntaggedHit = _sa_stats[10];

#if PRINT_DEBUG
    pr_err("\nSA Counters:\n----------------\n");
    pr_err("InOctetsDecrypted = %llu\n", sa_stats->InOctetsDecrypted);
    pr_err("InOctetsDecrypted2 = %llu\n", sa_stats->InOctetsDecrypted2);
    pr_err("InPktsUnchecked = %llu\n", sa_stats->InPktsUnchecked);
    pr_err("InPktsDelayed = %llu\n", sa_stats->InPktsDelayed);
    pr_err("InPktsLate = %llu\n", sa_stats->InPktsLate);
    pr_err("InPktsOK = %llu\n", sa_stats->InPktsOK);
    pr_err("InPktsInvalid = %llu\n", sa_stats->InPktsInvalid);
    pr_err("InPktsNotValid = %llu\n", sa_stats->InPktsNotValid);
    pr_err("InPktsNotUsingSA = %llu\n", sa_stats->InPktsNotUsingSA);
    pr_err("InPktsUnusedSA = %llu\n", sa_stats->InPktsUnusedSA);
    pr_err("InPktsUntaggedHit = %llu\n", sa_stats->InPktsUntaggedHit);
#endif

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_sa_stats);

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
int eip160_get_ing_vlan_stats(struct eip160_pdev *pdev,
                              struct ing_vlan_stats *vlan_stats)
{
    int i, ret, vlan_up;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u64 _vlan_stats[MAX_VLAN_COUNTERS];
    u32 _data, _data2;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || vlan_stats == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate VLAN User Priority range */
    if (vlan_stats->vlan_up >= MAX_VLAN_USER_PRIORITIES) {
        LOG_ERR("WARN: Invalid input (vlan_up) - VLAN User Priority\n");
        return -EINVAL;
    }

    vlan_up = vlan_stats->vlan_up;
    memset(vlan_stats, 0, sizeof(struct ing_vlan_stats));
    vlan_stats->vlan_up = vlan_up;
    _module_base = ING_STAT_VLAN_MODULE_BASE;
    _module_offset = (vlan_stats->vlan_up) * (MAX_SA_STATS_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Get VLAN stats */
    for (i = 0; i < MAX_VLAN_COUNTERS; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8), &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: VLAN stats low read fails for iteration %d\n", i);
            goto END;
        }

        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8) + 4, &_data2);

        if (ret < 0) {
            LOG_CRIT("ERROR: VLAN stats high read fails for iteration %d\n", i);
            goto END;
        }

        _vlan_stats[i] = ((u64)_data2 << 32) | _data;
    }

    vlan_stats->InOctetsVL = _vlan_stats[0];
    vlan_stats->InOctetsVL2 = _vlan_stats[1];
    vlan_stats->InPktsVL = _vlan_stats[2];
    vlan_stats->InDroppedPktsVL = _vlan_stats[3];
    vlan_stats->InOverSizePktsVL = _vlan_stats[4];

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_vlan_stats);
#endif

int eip160_get_ing_global_stats(struct eip160_pdev *pdev,
                                struct ing_global_stats *glob_stats)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u64 _global_stats[MAX_ING_GLOBAL_COUNTERS];
    u32 _data, _data2;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || glob_stats == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    memset(glob_stats, 0, sizeof(struct ing_global_stats));
    _module_base = ING_STAT_GLOBAL_MODULE_BASE;
    _module_offset =  EIPI_TRANSFORMERRORPKTS_LO;
    _absreg_addr = _module_base + _module_offset;

    /* Get GLOBAL stats */
    for (i = 0; i < MAX_ING_GLOBAL_COUNTERS; i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8), &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: GLOBAL stats low read fails for iteration %d\n", i);
            goto END;
        }

        ret = IG_REG_READ(pdev, _absreg_addr + (i * 8) + 4, &_data2);

        if (ret < 0) {
            LOG_CRIT("ERROR: GLOBAL stats high read fails for iteration %d\n", i);
            goto END;
        }

        _global_stats[i] = ((u64)_data2 << 32) | _data;
    }

    glob_stats->TransformErrorPkts = _global_stats[0];
    glob_stats->InPktsCtrl = _global_stats[1];
    glob_stats->InPktsNoTag = _global_stats[2];
    glob_stats->InPktsUntagged = _global_stats[3];
    glob_stats->InPktsTagged = _global_stats[4];
    glob_stats->InPktsBadTag = _global_stats[5];
    glob_stats->InPktsUntaggedMiss = _global_stats[6];
    glob_stats->InPktsNoSCI = _global_stats[7];
    glob_stats->InPktsUnknownSCI = _global_stats[8];
    glob_stats->InConsistCheckControlledNotPass = _global_stats[9];
    glob_stats->InConsistCheckUncontrolledNotPass = _global_stats[10];
    glob_stats->InConsistCheckControlledPass = _global_stats[11];
    glob_stats->InConsistCheckUncontrolledPass = _global_stats[12];
    glob_stats->InOverSizePkts = _global_stats[13];

#if PRINT_DEBUG
    pr_err("\nGLOBAL Counters:\n----------------\n");
    pr_err("TransformErrorPkts = %llu\n", glob_stats->TransformErrorPkts);
    pr_err("InPktsCtrl = %llu\n", glob_stats->InPktsCtrl);
    pr_err("InPktsNoTag = %llu\n", glob_stats->InPktsNoTag);
    pr_err("InPktsUntagged = %llu\n", glob_stats->InPktsUntagged);
    pr_err("InPktsTagged = %llu\n", glob_stats->InPktsTagged);
    pr_err("InPktsBadTag = %llu\n", glob_stats->InPktsBadTag);
    pr_err("InPktsUntaggedMiss = %llu\n", glob_stats->InPktsUntaggedMiss);
    pr_err("InPktsNoSCI = %llu\n", glob_stats->InPktsNoSCI);
    pr_err("InPktsUnknownSCI = %llu\n", glob_stats->InPktsUnknownSCI);
    pr_err("InConsistCheckControlledNotPass = %llu\n", glob_stats->InConsistCheckControlledNotPass);
    pr_err("InConsistCheckUncontrolledNotPass = %llu\n", glob_stats->InConsistCheckUncontrolledNotPass);
    pr_err("InConsistCheckControlledPass = %llu\n", glob_stats->InConsistCheckControlledPass);
    pr_err("InConsistCheckUncontrolledPass = %llu\n", glob_stats->InConsistCheckUncontrolledPass);
    pr_err("InOverSizePkts = %llu\n", glob_stats->InOverSizePkts);
#endif

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_global_stats);

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
int eip160_get_ing_debug_stats(struct eip160_pdev *pdev,
                               struct debug_stats *dbg_stats)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _debug_count1 _dbg_cnt1;
    struct _debug_count2 _dbg_cnt2;
    struct _debug_count4 _dbg_cnt4;
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || dbg_stats == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _dbg_cnt1.raw_dbg_cnt1 = _dbg_cnt2.raw_dbg_cnt2 = _dbg_cnt4.raw_dbg_cnt4 = 0;
    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset =  EIPI_COUNT_DEBUG1;
    _absreg_addr = _module_base + _module_offset;
    /* Get COUNT_DEBUG1 data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_DEBUG1 stats read fails for iteration\n");
        goto END;
    }

    _dbg_cnt1.raw_dbg_cnt1 = _data;
    dbg_stats->sa_inc_mask = _dbg_cnt1.sa_inc_mask;
    dbg_stats->sa_octet_inc = _dbg_cnt1.sa_octet_inc;
    _module_offset =  EIPI_COUNT_DEBUG2;
    _absreg_addr = _module_base + _module_offset;
    /* Get COUNT_DEBUG2 data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_DEBUG2 stats read fails for iteration\n");
        goto END;
    }

    _dbg_cnt2.raw_dbg_cnt2 = _data;
    dbg_stats->vlan_inc_mask = _dbg_cnt2.vlan_inc_mask;
    dbg_stats->vlan_octet_inc = _dbg_cnt2.vlan_octet_inc;
    _module_offset =  EIPI_COUNT_DEBUG3;
    _absreg_addr = _module_base + _module_offset;
    /* Get COUNT_DEBUG3 data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_DEBUG3 stats read fails for iteration\n");
        goto END;
    }

    dbg_stats->global_inc_mask = _data;
    _module_offset =  EIPI_COUNT_DEBUG4;
    _absreg_addr = _module_base + _module_offset;
    /* Get COUNT_DEBUG4 data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_DEBUG4 stats read fails for iteration\n");
        goto END;
    }

    _dbg_cnt4.raw_dbg_cnt4 = _data;
    dbg_stats->sa_test = _dbg_cnt4.sa_test;
    dbg_stats->vlan_test = _dbg_cnt4.vlan_test;
    dbg_stats->global_test = _dbg_cnt4.global_test;
    dbg_stats->sa_nr = _dbg_cnt4.sa_nr;
    dbg_stats->vlan_up = _dbg_cnt4.vlan_up;
    dbg_stats->global_octet_inc = _dbg_cnt4.global_octet_inc;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_debug_stats);
#endif

int eip160_cfg_ing_cntrs_ctrl(struct eip160_pdev *pdev,
                              const struct cntrs_ctrl *cnt_ctrl)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _cntrs_ctrl _cntr_ctrl;
    struct _cntr_inc_en _inc_en;
    u32 _cntr_info[3];
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cnt_ctrl == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate Counter Control params */
    if (((cnt_ctrl->psa_inc_en & 0x7FFu) > 0x7FFu) ||
            ((cnt_ctrl->vlan_inc_en & 0x1F) > 0x1F) ||
            ((cnt_ctrl->global_inc_en & 0x3FFF) > 0x3FFF)) {
        LOG_ERR("WARN: Invalid input Counter Increment parameter\n");
        return -EINVAL;
    }

    _cntr_ctrl.raw_cnt_ctrl = _inc_en.raw_cnt_inc_en = 0;
    _cntr_ctrl.reset_all = cnt_ctrl->reset_all;
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
    _cntr_ctrl.debug_access = cnt_ctrl->debug_access;
#endif
    _cntr_ctrl.saturate_cntrs = cnt_ctrl->saturate_cntrs;
    _cntr_ctrl.auto_cntr_reset = cnt_ctrl->auto_cntr_reset;
    _cntr_ctrl.reset_summary = cnt_ctrl->reset_summary;
    _cntr_info[0] = _cntr_ctrl.raw_cnt_ctrl;

    _inc_en.psa_inc_en = cnt_ctrl->psa_inc_en & 0x7FFu;
    _inc_en.vlan_inc_en = cnt_ctrl->vlan_inc_en & 0x1F;
    _cntr_info[1] = _inc_en.raw_cnt_inc_en;

    _cntr_info[2] = cnt_ctrl->global_inc_en & 0x3FFF;

    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset = EIPI_COUNT_CONTROL;
    _absreg_addr = _module_base + _module_offset;

    for (i = 0; i < 3; i++) {
        /* Set COUNT_CONTROL, COUNT_INCEN1, COUNT_INCEN2 data */
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _cntr_info[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: Counter Control word set fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_cntrs_ctrl);

int eip160_cfg_ing_cntrs_thold(struct eip160_pdev *pdev,
                               const struct frm_oct_thold *cnt_thr)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _low_word, _high_word;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cnt_thr == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _low_word = (cnt_thr->frame_threshold) & 0xFFFFFFFFu;
    _high_word = cnt_thr->frame_threshold >> 32;
    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset = EIPI_COUNT_FRAME_THR1;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_FRAME_THR_1 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _low_word);

    if (ret < 0) {
        LOG_CRIT("ERROR: Frame Treshold lower word set failed\n");
        goto END;
    }

    _module_offset = EIPI_COUNT_FRAME_THR2;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_FRAME_THR_1 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _high_word);

    if (ret < 0) {
        LOG_CRIT("ERROR: Frame Treshold higher word set failed\n");
        goto END;
    }

    _low_word = (cnt_thr->octet_threshold) & 0xFFFFFFFFu;
    _high_word = cnt_thr->octet_threshold >> 32;
    _module_offset = EIPI_COUNT_OCTET_THR1;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_FRAME_THR_1 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _low_word);

    if (ret < 0) {
        LOG_CRIT("ERROR: Octet Treshold lower word set failed\n");
        goto END;
    }

    _module_offset = EIPI_COUNT_OCTET_THR2;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_FRAME_THR_1 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _high_word);

    if (ret < 0) {
        LOG_CRIT("ERROR: Octet Treshold higher word set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_cntrs_thold);

int eip160_clr_ing_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   const struct cntrs_thold_sum *thold_sum)
{
    int i, j, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || thold_sum == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset = EIPI_COUNT_SUMMARY_GL;
    _absreg_addr = _module_base + _module_offset;
    /* Clear COUNT_SUMMARY_GL data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, thold_sum->gl_cntr_sum);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_SUMMARY_GL set failed\n");
        goto END;
    }

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    for (i = 0; i < MAX_VLAN_COUNT_SUMM_WORDS; i++) {
        _module_offset = i * 4 + EIPI_COUNT_SUMMARY_VL1;
        _absreg_addr = _module_base + _module_offset;
        /* Clear COUNT_SUMMARY_VLx data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, thold_sum->vlan_cntr_sum[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: COUNT_SUMMARY_VLx set failed\n");
            goto END;
        }
    }
#endif

    for (i = 0; i < ((pdev->ig_nr_of_sas) / 32); i++) {
        if (i == (MACSEC_MAX_SA_RULES / 32))
            break;

        _module_offset = i * 4 + EIPI_COUNT_SUMMARY_PSA1;
        _absreg_addr = _module_base + _module_offset;
        /* Clear COUNT_SUMMARY_PSAx data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, thold_sum->psa_cntr_vec[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: COUNT_SUMMARY_PSAx set failed\n");
            goto END;
        }

        _data = thold_sum->psa_cntr_vec[i];

        for (j = 0; j < 32; j++) {
            if (0 == ((_data >> j) & 1))
                continue;

            _module_offset =  j * 4 + EIPI_COUNT_SUMMARY_SA0;
            _absreg_addr = _module_base + _module_offset;
            /* Clear 'COUNT_SUMMARY_SAx' for SA counters statistics summary */
            ret = IG_REG_WRITE(pdev, _absreg_addr, thold_sum->psa_cntr_sum[j]);

            if (ret < 0) {
                LOG_CRIT("ERROR: COUNT_SUMMARY_SAx set failed\n");
                goto END;
            }
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_clr_ing_cntrs_thold_sum);

int eip160_get_ing_cntrs_thold_sum(struct eip160_pdev *pdev,
                                   struct cntrs_thold_sum *thold_sum)
{
    int i, j, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || thold_sum == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_STAT_CONTROL_MODULE_BASE;
    _module_offset = EIPI_COUNT_SUMMARY_GL;
    _absreg_addr = _module_base + _module_offset;
    /* Get COUNT_SUMMARY_GL data */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: COUNT_SUMMARY_GL get failed\n");
        goto END;
    }

    thold_sum->gl_cntr_sum = _data;

#if defined(SUPPORT_MACSEC_VLAN) && SUPPORT_MACSEC_VLAN
    for (i = 0; i < MAX_VLAN_COUNT_SUMM_WORDS; i++) {
        _module_offset = i * 4 + EIPI_COUNT_SUMMARY_VL1;
        _absreg_addr = _module_base + _module_offset;
        /* Get COUNT_SUMMARY_VLx data */
        ret = IG_REG_READ(pdev, _absreg_addr, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: COUNT_SUMMARY_VLx get failed\n");
            goto END;
        }

        thold_sum->vlan_cntr_sum[i] = _data;
    }
#endif

    for (i = 0; i < ((pdev->ig_nr_of_sas) / 32); i++) {
        if (i == (MACSEC_MAX_SA_RULES / 32))
            break;

        _module_offset = i * 4 + EIPI_COUNT_SUMMARY_PSA1;
        _absreg_addr = _module_base + _module_offset;
        /* Get COUNT_SUMMARY_PSAx data */
        ret = IG_REG_READ(pdev, _absreg_addr, &_data);

        if (ret < 0) {
            LOG_CRIT("ERROR: COUNT_SUMMARY_PSAx get failed\n");
            goto END;
        }

        thold_sum->psa_cntr_vec[i] = _data;

        for (j = 0; j < 32; j++) {
            if (0 == ((_data >> j) & 1))
                continue;

            _module_offset =  j * 4 + EIPI_COUNT_SUMMARY_SA0;
            _absreg_addr = _module_base + _module_offset;
            /* Get 'COUNT_SUMMARY_SAx' for SA counters statistics summary */
            ret = IG_REG_READ(pdev, _absreg_addr, &_data);

            if (ret < 0) {
                LOG_CRIT("ERROR: COUNT_SUMMARY_SAx get failed\n");
                goto END;
            }

            thold_sum->psa_cntr_sum[j] = _data;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_cntrs_thold_sum);

int eip160_cfg_ing_ccm_rule(struct eip160_pdev *pdev,
                            const struct icc_match_parm *ccm_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _byte_swapped = 0;
    u32 _icc_match[ICC_MAX_MATCH_SIZE_WORDS - 1];
    struct _iccm_ovlan_etht _ovl_etht;
    struct _iccm_misc_par _misc_par;
    struct _iccm_inr_vlan _inr_vlan;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ccm_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _ovl_etht.raw_iccm_ovl_eth = _misc_par.raw_iccm_misc = 0;
    _inr_vlan.raw_iccm_ivl = 0;
    _byte_swapped = BYTE_SWAP_16BIT(ccm_par->payload_e_type);

    _ovl_etht.vlan_id = ccm_par->otr_vlan_id;
    _ovl_etht.payload_e_type = _byte_swapped;
    _icc_match[0] = _ovl_etht.raw_iccm_ovl_eth;

    _misc_par.raw_iccm_misc = ccm_par->iccm_misc_flags;
    _icc_match[1] = _misc_par.raw_iccm_misc;

    _inr_vlan.inr_vlan_id = ccm_par->inr_vlan_id;
    _inr_vlan.inr_vlan_up = ccm_par->inr_vlan_up;
    _icc_match[2] = _inr_vlan.raw_iccm_ivl;

    _module_base = ING_CC_RULES_PAGE0_MODULE_BASE +
                   ((ccm_par->icc_index > 127) ? 0x10000 : 0);
    _module_offset = ((ccm_par->icc_index) % 128) * (ICC_MAX_MATCH_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Set ICC's rule data */
    for (i = 0; i < (ICC_MAX_MATCH_SIZE_WORDS - 1); i++) {
        ret = IG_REG_WRITE(pdev, _absreg_addr + i * 4, _icc_match[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: ICC write fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    

    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_ccm_rule);

int eip160_get_ing_ccm_rule(struct eip160_pdev *pdev,
                            struct icc_match_parm *ccm_par)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _byte_swapped = 0;
    u32 _icc_match[ICC_MAX_MATCH_SIZE_WORDS - 1];
    struct _iccm_ovlan_etht _ovl_etht;
    struct _iccm_misc_par _misc_par;
    struct _iccm_inr_vlan _inr_vlan;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ccm_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_CC_RULES_PAGE0_MODULE_BASE +
                   ((ccm_par->icc_index > 127) ? 0x10000 : 0);
    _module_offset = ((ccm_par->icc_index) % 128) * (ICC_MAX_MATCH_SIZE_BYTES);
    _absreg_addr = _module_base + _module_offset;

    /* Get ICC's match rule data */
    for (i = 0; i < (ICC_MAX_MATCH_SIZE_WORDS - 1); i++) {
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_icc_match[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: ICC read fails for iteration %d\n", i);
            goto END;
        }
    }

    _byte_swapped = ((((_icc_match[0] >> 24) & 0xFF) << 8) |
                     (((_icc_match[0] >> 16) & 0xFF) << 0));

    _ovl_etht.raw_iccm_ovl_eth = _misc_par.raw_iccm_misc = 0;
    _inr_vlan.raw_iccm_ivl = 0;
    _ovl_etht.raw_iccm_ovl_eth = _icc_match[0];
    _byte_swapped = BYTE_SWAP_16BIT(_ovl_etht.payload_e_type);
    ccm_par->payload_e_type = _byte_swapped;
    ccm_par->otr_vlan_id = _ovl_etht.vlan_id;

    _misc_par.raw_iccm_misc = _icc_match[1];
    ccm_par->iccm_misc_flags = _misc_par.raw_iccm_misc;

    _inr_vlan.raw_iccm_ivl = _icc_match[2];
    ccm_par->inr_vlan_id = _inr_vlan.inr_vlan_id;
    ccm_par->inr_vlan_up = _inr_vlan.inr_vlan_up;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_ccm_rule);

int eip160_cfg_ing_ccm_ena_ctrl(struct eip160_pdev *pdev,
                                const struct ccm_ena_ctrl *ee_ctrl)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _ccm_ena_ctrl _ccm_eec;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ee_ctrl == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _ccm_eec.raw_icc_eec = 0;
    _ccm_eec.cc_index_set = ee_ctrl->cc_index_set;
    _ccm_eec.set_enable = ee_ctrl->set_enable;
    _ccm_eec.set_all = ee_ctrl->set_all;
    _ccm_eec.cc_index_clear = ee_ctrl->cc_index_clear;
    _ccm_eec.clear_enable = ee_ctrl->clear_enable;
    _ccm_eec.clear_all = ee_ctrl->clear_all;

    _module_base = ING_CC_CONTROL_MODULE_BASE;
    _module_offset = EIPI_CC_ENTRY_ENABLE_CTRL;
    _absreg_addr = _module_base + _module_offset;
    /* Set CCM Entry Enable control data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _ccm_eec.raw_icc_eec);

    if (ret < 0) {
        LOG_CRIT("ERROR: ICC EE control write failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_ccm_ena_ctrl);

int eip160_get_ing_ccm_ena_flags(struct eip160_pdev *pdev,
                                 struct ccm_ena_flags *ccm_par)
{
    int i, ret = 0;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ccm_par == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }
    
    _module_base = ING_CC_CONTROL_MODULE_BASE;
    _module_offset = EIPI_IG_CC_ENABLE1;
    _absreg_addr = _module_base + _module_offset;

    /* Set SAM EE flags data */
    for (i = 0; i < (pdev->ig_nr_of_sas / 32); i++) {
        if (i == (MACSEC_MAX_SA_RULES / 32))
            break;

        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &ccm_par->raw_ccm_flags[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: ICC EE Flags get failed for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_ccm_ena_flags);

int eip160_cfg_ing_ccknm_flow_ctrl(struct eip160_pdev *pdev,
                                 const struct ccknm_flow_ctrl *ccm_parc)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _icc_nm_fc _cck_ctrl;
    struct _icc_etype_mlen _etype_mlen;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || ccm_parc == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate NM action range */
    if ((ccm_parc->nm_act_ncp > 1) || (ccm_parc->nm_act_cp > 1)) {
        LOG_ERR("WARN: Invalid input (nm_act_ncp / nm_ctrl_ac) - NM action\n");
        return -EINVAL;
    }

    _cck_ctrl.raw_icc_nm_fc = _etype_mlen.raw_icc_eth_len = 0;
    _cck_ctrl.nm_act_cp = ccm_parc->nm_act_cp;
    _cck_ctrl.nm_act_ncp = ccm_parc->nm_act_ncp;
    _module_base = ING_CC_CONTROL_MODULE_BASE;
    _module_offset = EIPI_IG_CC_CONTROL;
    _absreg_addr = _module_base + _module_offset;
    /* Set IG CC Ctrl flags data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _cck_ctrl.raw_icc_nm_fc);

    if (ret < 0) {
        LOG_CRIT("ERROR: IG CC Control word set failed\n");
        goto END;
    }

    _etype_mlen.etype_max_len = ccm_parc->cp_etype_max_len;
    _module_offset = EIPI_IG_CC_TAGS;
    _absreg_addr = _module_base + _module_offset;
    /* Set IG CC max eth-type length compare value */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _etype_mlen.raw_icc_eth_len);

    if (ret < 0) {
        LOG_CRIT("ERROR: IG CC Control word set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_ccknm_flow_ctrl);

int eip160_cfg_ing_sec_fail_evnt(struct eip160_pdev *pdev,
                                 const struct sec_fail_evnt *sec_fail)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    struct _sec_fail_evnt _fail_evnt;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sec_fail == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _fail_evnt.raw_sec_fail = 0;
    _fail_evnt.sa_secfail_mask = sec_fail->sa_secfail_mask;
    _fail_evnt.vlan_cntr_update = sec_fail->vlan_cntr_update;
    _fail_evnt.gate_cons_check = sec_fail->gate_cons_check;

    _module_base = ING_OPPE_MODULE_BASE;
    _module_offset = EIPI_COUNT_SECFAIL1;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_SECFAIL1 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _fail_evnt.raw_sec_fail);

    if (ret < 0) {
        LOG_CRIT("ERROR: Counter Secfail word-0 set failed\n");
        goto END;
    }

    _module_offset = EIPI_COUNT_SECFAIL2;
    _absreg_addr = _module_base + _module_offset;
    /* Set COUNT_SECFAIL2 data */
    ret = IG_REG_WRITE(pdev, _absreg_addr, sec_fail->gbl_secfail_mask);

    if (ret < 0) {
        LOG_CRIT("ERROR: Counter Secfail word-1 set failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sec_fail_evnt);

int eip160_cfg_ing_crypto_intrp_ctrl(struct eip160_pdev *pdev,
                                   const struct crypto_intrp_csr *cr_intc)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _int_csr = 0;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cr_intc == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
    _module_offset = EIPI_EIP62_CONTEXT_STAT;
    _absreg_addr = _module_base + _module_offset;
    /* Clear Context status */
    ret = IG_REG_WRITE(pdev, _absreg_addr, cr_intc->ctx_stat);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62_CONTEXT_STAT clear failed\n");
        goto END;
    }
#endif

    _int_csr = (cr_intc->intrp_stat | cr_intc->intrp_ctrl << 16);
    _module_offset = EIPI_EIP62_INT_CTRL_STAT;
    _absreg_addr = _module_base + _module_offset;
    /* Clear Interrupt status */
    ret = IG_REG_WRITE(pdev, _absreg_addr, _int_csr);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62_INT_CTRL_STAT clear failed\n");
        goto END;
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_crypto_intrp_ctrl);

int eip160_get_ing_crypto_intrp_stat(struct eip160_pdev *pdev,
                                   struct crypto_intrp_csr *cr_ints)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _data;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || cr_ints == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
    _module_offset = EIPI_EIP62_CONTEXT_STAT;
    _absreg_addr = _module_base + _module_offset;
    /* Get Context status */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62_CONTEXT_STAT clear failed\n");
        goto END;
    }

    cr_ints->ctx_stat = _data;
#endif

    _module_offset = EIPI_EIP62_INT_CTRL_STAT;
    _absreg_addr = _module_base + _module_offset;
    /* Get Interrupt status */
    ret = IG_REG_READ(pdev, _absreg_addr, &_data);

    if (ret < 0) {
        LOG_CRIT("ERROR: EIP62_INT_CTRL_STAT clear failed\n");
        goto END;
    }

    cr_ints->intrp_ctrl = _data >> 16;
    cr_ints->intrp_stat = _data & 0xFFFF;

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_crypto_intrp_stat);

int eip160_cfg_ing_sn_thold(struct eip160_pdev *pdev,
                            const struct sn_thold *sn_thr)
{
    int ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _low_word, _high_word;
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || sn_thr == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    /* Validate SA non-match EXPL params */
    if (sn_thr->sn_type != SN_32_BIT && sn_thr->sn_type != SN_64_BIT) {
        LOG_ERR("WARN: Invalid input SN type parameter\n");
        return -EINVAL;
    }

    _module_base = ING_EIP62_REGISTERS_MODULE_BASE;
    _low_word = (sn_thr->sn_threshold) & 0xFFFFFFFFu;

    if (sn_thr->sn_type == SN_32_BIT) {
        _module_offset = EIPI_EIP62_SEQ_NUM_THRESHOLD;
        _absreg_addr = _module_base + _module_offset;
        /* Set EIP62_SEQ_NUM_THRESHOLD data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, _low_word);

        if (ret < 0) {
            LOG_CRIT("ERROR: SN Treshold lower word set failed\n");
            goto END;
        }
    } else {
        _high_word = sn_thr->sn_threshold >> 32;
        _module_offset = EIPI_EIP62_SEQ_NUM_THRESHOLD64_LO;
        _absreg_addr = _module_base + _module_offset;
        /* Set EIP62_SEQ_NUM_THRESHOLD64_LO data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, _low_word);

        if (ret < 0) {
            LOG_CRIT("ERROR: SN Treshold lower word set failed\n");
            goto END;
        }

        _module_offset = EIPI_EIP62_SEQ_NUM_THRESHOLD64_HI;
        _absreg_addr = _module_base + _module_offset;
        /* Set EIP62_SEQ_NUM_THRESHOLD data */
        ret = IG_REG_WRITE(pdev, _absreg_addr, _high_word);

        if (ret < 0) {
            LOG_CRIT("ERROR: SN Treshold lower word set failed\n");
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_sn_thold);

int eip160_cfg_ing_adv_intrp_ctrl(struct eip160_pdev *pdev,
                                const struct adv_intrp_ctrl *intrp_ctrl)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _intrp_ctrl[4];
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || intrp_ctrl == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _intrp_ctrl[0] = intrp_ctrl->enable_ctrl;
    _intrp_ctrl[1] = intrp_ctrl->enable_set;
    _intrp_ctrl[2] = intrp_ctrl->ack_intrpt;
    _intrp_ctrl[3] = intrp_ctrl->enable_clr;
    _module_base = ING_EIP201_REGISTERS_MODULE_BASE;
    _module_offset = EIPI_AIC_ENABLE_CTRL;
    _absreg_addr = _module_base + _module_offset;

    for (i = 0; i < 4; i++) {
        /* Set AIC_ENABLE_CTRL, AIC_ENABLE_SET, AIC_ACK, AIC_ENABLE_CLR data */
        ret = IG_REG_WRITE(pdev,  _absreg_addr + i * 4, _intrp_ctrl[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: AIC info set fails for iteration %d\n", i);
            goto END;
        }
    }

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_cfg_ing_adv_intrp_ctrl);

int eip160_get_ing_adv_intrp_stat(struct eip160_pdev *pdev,
                                struct adv_intrp_stat *intrp_stat)
{
    int i, ret;
    u32 _module_base, _module_offset, _absreg_addr;
    /* Internal register mapping specific variables */
    u32 _intrp_stat[5];
    LOG_INFO("Entering func :- %s\n", __func__);

    if (pdev == NULL || intrp_stat == NULL) {
        LOG_ERR("WARN: NULL input pointer(s)\n");
        return -EINVAL;
    }

    _module_base = ING_EIP201_REGISTERS_MODULE_BASE;
    _module_offset = EIPI_AIC_POL_CTRL;
    _absreg_addr = _module_base + _module_offset;

    for (i = 0; i < 5; i++) {
        /* Get AIC_POL_CTRL, AIC_TYPE_CTRL, AIC_ENABLE_CTRL,
        AIC_RAW_STAT_ENABLE_SET, AIC_ENABLED_STAT_ACK registers data */
        ret = IG_REG_READ(pdev, _absreg_addr + i * 4, &_intrp_stat[i]);

        if (ret < 0) {
            LOG_CRIT("ERROR: AIC info get failed for iteration %d\n", i);
            goto END;
        }
    }

    intrp_stat->inpol_ctrl = _intrp_stat[0];
    intrp_stat->type_ctrl = _intrp_stat[1];
    intrp_stat->enable_ctrl = _intrp_stat[2];
    intrp_stat->raw_stat = _intrp_stat[3];
    intrp_stat->masked_stat = _intrp_stat[4];

END:
    LOG_INFO("Exiting func :- %s\n", __func__);
    return ret;
}
EXPORT_SYMBOL(eip160_get_ing_adv_intrp_stat);

MODULE_AUTHOR("Govindaiah Mudepalli");
MODULE_DESCRIPTION("EIP160S MACsec APIs");
MODULE_LICENSE("GPL v2");
