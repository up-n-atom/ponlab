/*******************************************************************************
 **
 ** FILE NAME	: ppa_api_tools.c
 ** PROJECT	: PPA
 ** MODULES	: PPA API (Routing/Bridging Acceleration APIs)
 **
 ** DATE	: 18 March 2010
 ** AUTHOR	: Shao Guohua
 ** DESCRIPTION	: PPA Protocol Stack Tools API Implementation
 ** COPYRIGHT	: Copyright (c) 2020-2024 MaxLinear, Inc.
 **     	Copyright (c) 2009
 **	        Lantiq Deutschland GmbH
 **             Am Campeon 3; 85579 Neubiberg, Germany
 **
 **	 For licensing information, see the file 'LICENSE' in the root folder of
 **	 this software module.
 **
 ** HISTORY
 ** $Date		$Author		 $Comment
 ** 18 March 2010	Shao Guohua		Initiate Version
 *******************************************************************************/
/*
 *	Common Head File
 */
#include <linux/version.h>
#include <generated/autoconf.h>

/*
 *	PPA Specific Head File
 */
#include <net/ppa/ppa_api_common.h>
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hook.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/ppa/ppa_hal_wrapper.h>

#include "ppa_api_misc.h"
#include "ppa_api_netif.h"
#include "ppa_api_session.h"
#include "ppa_api_core.h"

#if IS_ENABLED(CONFIG_LTQ_CPU_FREQ) || IS_ENABLED(CONFIG_LTQ_PMCU)
#include "ppa_api_pwm.h"
#include "ppa_api_pwm_logic.h"
#endif

#include "ppa_api_mib.h"

#define PPA_WRPFN_DEFINITION
#include "ppa_api_tools.h"
/*
 * Definition
 */
#define QOS_FUNC_EXPORT_SUPPORT 0
#define MFE_FUNC_EXPORT_SUPPORT 0
/* PPA init flag */
static uint32_t g_init = 0;
PPA_HOOK_INFO g_expfn_table[PPA_HOOK_FN_MAX];

/* PPA init flag get/set functions */
void ppa_set_init_status(PPA_INIT_STATUS_t state)
{
	g_init = state;
}

uint32_t ppa_is_init(void)
{
	return g_init;
}

/*
 * No lock, so should be called only on module initialization
 */
void ppa_reg_export_fn(PPA_EXPORT_FN_NO fn_no, void* in_fn_addr, uint8_t* name, void** out_fn_addr, void* mid_fn_addr )
{
	if(fn_no >= PPA_HOOK_FN_MAX){
		err("Error: function reg no is bigger than max Number !!!\n");
		return;
	}

	if( g_expfn_table[fn_no].used_flag ){
		err("Warning: function has been registered, NO: %d, address: %px\n",
				fn_no, (void *)g_expfn_table[fn_no].hook_addr);
		return;
	}

	ppa_rcu_read_lock();

	g_expfn_table[fn_no].hook_addr = in_fn_addr;
	ppa_strncpy(g_expfn_table[fn_no].hookname,name,sizeof(g_expfn_table[fn_no].hookname));
	g_expfn_table[fn_no].hook_flag =1;
	g_expfn_table[fn_no].used_flag =1;

	*out_fn_addr = mid_fn_addr;

	ppa_rcu_read_unlock();
}

int32_t ppa_unreg_export_fn(PPA_EXPORT_FN_NO fn_no, void** out_fn_addr)
{
	if(fn_no >= PPA_HOOK_FN_MAX){
		err("Error: function reg no is bigger than max Number !!!\n");
		return PPA_FAILURE;
	}

	if( g_expfn_table[fn_no].used_flag ) {

		ppa_rcu_read_lock();

		g_expfn_table[fn_no].hook_flag = 0;
		g_expfn_table[fn_no].used_flag = 0;
		g_expfn_table[fn_no].hook_addr = 0;
		*out_fn_addr = NULL;

		ppa_rcu_read_unlock();
	}
	return PPA_SUCCESS;
}


/*
 * must be called at the module unload
 */
void ppa_export_fn_manager_exit(void)
{
	int i;

	for(i=0; i<PPA_HOOK_FN_MAX; i++) {
		g_expfn_table[i].used_flag = 0;
		g_expfn_table[i].hook_flag = 0;
		g_expfn_table[i].hook_addr = 0;
	}
}


int32_t ppa_enable_hook(int8_t *name, uint32_t enable, uint32_t flag)
{
	int i;
	int32_t res = PPA_FAILURE;

	for(i=0; i<PPA_HOOK_FN_MAX; i++) {
		if( !g_expfn_table[i].used_flag ) continue;
		if( strcmp( g_expfn_table[i].hookname, name ) == 0 ) {
			if( enable ) {
				g_expfn_table[i].hook_flag = 1;
			} else {
				g_expfn_table[i].hook_flag = 0;
			}

			res = PPA_SUCCESS;
			break;
		}
	}

	return res;
}

EXPORT_SYMBOL(ppa_is_init);
EXPORT_SYMBOL(ppa_set_init_status);
EXPORT_SYMBOL(ppa_reg_export_fn);
EXPORT_SYMBOL(ppa_unreg_export_fn);
EXPORT_SYMBOL(ppa_export_fn_manager_exit);
EXPORT_SYMBOL(ppa_enable_hook);
EXPORT_SYMBOL(g_expfn_table);
