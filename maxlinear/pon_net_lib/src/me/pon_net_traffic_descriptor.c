/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_netlink.h"
#include "../pon_net_uni.h"
#include "pon_net_gem_port_network_ctp.h"
#include "pon_net_traffic_descriptor.h"
#include "pon_net_tcont.h"
#include "pon_net_onu_g.h"
#include <netlink/route/act/colmark.h>

#include <omci/me/pon_adapter_onu_g.h>
#include <omci/me/pon_adapter_traffic_descriptor.h>

/* Compares both netlink_police_data strcutes and returns true if they
 * are the same. The active attribute is not checked.
 */
static bool pon_net_police_same(const struct netlink_police_data *a,
			       const struct netlink_police_data *b)
{
	return	a->cir == b->cir &&
		a->pir == b->pir &&
		a->cbs == b->cbs &&
		a->pbs == b->pbs &&
		a->colmark.mode == b->colmark.mode &&
		a->colmark.drop_precedence == b->colmark.drop_precedence &&
		a->colmark.meter_type == b->colmark.meter_type;
}

static enum pon_adapter_errno
pon_net_police_update(struct pon_net_context *ctx, uint16_t class_id,
		      uint16_t me_id, enum netlink_filter_dir dir,
		      struct netlink_police_data *police_old,
		      uint16_t td_me_id)
{
	struct pa_traffic_descriptor_update_data upd_data;
	struct netlink_police_data police;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u, %u, %p, %u", ctx, class_id, me_id, dir,
		    police_old, td_me_id);

	if (pon_net_traffic_management_option_get(ctx) != PA_TMO_PRIO_RATE) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_TRAFFIC_DESCRIPTOR,
				   td_me_id,
				   &upd_data,
				   sizeof(upd_data));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		if (police_old->active) {
			ret = pon_net_police_unset(ctx, class_id, me_id, dir);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, pon_net_police_unset, ret);
				return ret;
			}
			police_old->active = false;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	} else if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	netlink_colmark_defaults(&police.colmark);
	police.cir = upd_data.cir;
	police.pir = upd_data.pir;
	police.cbs = upd_data.cbs;
	police.pbs = upd_data.pbs;
	police.colmark.mode = upd_data.color_mode;
	switch (upd_data.meter_type) {
	case PON_ADAPTER_TD_NOTSPECIFIED_METERTYPE:
	case PON_ADAPTER_TD_RFC2698_METERTYPE:
	case PON_ADAPTER_TD_RFC4115_METERTYPE:
		/* trTCM is defined in RFC2698 and RFC4115 is similar,
		 * also use trTCM when RFC4115 is requested for now.
		 * To fully support this, extra work is needed.
		 */
		police.colmark.meter_type = COLMARK_TRTCM;
		break;
	};

	if (dir == NETLINK_FILTER_DIR_INGRESS)
		police.colmark.drop_precedence = upd_data.ingress_color_marking;
	else if  (dir == NETLINK_FILTER_DIR_EGRESS)
		police.colmark.drop_precedence = upd_data.egress_color_marking;

	/* If we configure "No marking" just ignmore this setting */
	if (police.colmark.drop_precedence == PON_ADAPTER_TD_NO_MARKING)
		police.colmark.drop_precedence = NETLINK_FILTER_UNUSED;

	/* Fixup for strange OLT configurations. If the OLT configures CIR to 0
	 * and PIR to some value, use the PIR value as CIR instead.
	 * The Calix OLT does this.
	 */
	if (police.cir == 0 && police.pir != 0) {
		police.cir = police.pir;
		police.pir = 0;
	}

	/* If these are set to 0 (OMCI default value) by the OLT,
	 * use the ONU default values of 64K.
	 */
	if (police.cbs == 0)
		police.cbs = 64 * 1024;
	if (police.pbs == 0)
		police.pbs = 64 * 1024;

	if (!pon_net_police_same(police_old, &police) || !police_old->active) {
		ret = pon_net_police_set(ctx, class_id, me_id, dir, &police);
		if (ret != PON_ADAPTER_SUCCESS) {
			/* Restore old */
			enum pon_adapter_errno err;

			dbg_err_fn_ret(pon_net_police_set, ret);

			err = pon_net_police_set(ctx, class_id, me_id, dir,
						 police_old);
			if (err != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_police_set, err);

			dbg_out_ret("%d", ret);
			return ret;
		}
		*police_old = police;
		police_old->active = true;
	}
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_police_update_bp(struct pon_net_context *ctx,
						uint16_t me_id,
						enum netlink_filter_dir dir)
{
	struct netlink_police_data *police_old;
	uint16_t td_me_id;
	enum pon_adapter_errno ret;
	struct pon_net_bridge_port_config *bp;

	dbg_in_args("%p, %u, %u", ctx, me_id, dir);

	bp = pon_net_me_list_get_data(&ctx->me_list,
				      PON_CLASS_ID_BRIDGE_PORT_CONFIG, me_id);
	if (!bp) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_RET(ret, pon_net_me_list_get_data, ret);
		return ret;
	}

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		td_me_id = bp->inbound_td_ptr;
		police_old = &bp->us_police;
	} else if  (dir == NETLINK_FILTER_DIR_EGRESS) {
		td_me_id = bp->outbound_td_ptr;
		police_old = &bp->ds_police;
	} else {
		ret = PON_ADAPTER_ERROR;
		dbg_out_ret("%d", ret);
		return ret;
	}

	return pon_net_police_update(ctx, PON_CLASS_ID_BRIDGE_PORT_CONFIG,
				     me_id, dir, police_old, td_me_id);
}

enum pon_adapter_errno pon_net_police_update_gem(struct pon_net_context *ctx,
						 uint16_t me_id,
						 enum netlink_filter_dir dir)
{
	struct netlink_police_data *police_old;
	uint16_t td_me_id;
	enum pon_adapter_errno ret;
	struct pon_net_gem_port_net_ctp *gem_ctp;

	dbg_in_args("%p, %u, %u", ctx, me_id, dir);

	gem_ctp = pon_net_me_list_get_data(&ctx->me_list,
					   PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
					   me_id);
	if (!gem_ctp) {
		ret = PON_ADAPTER_ERROR;
		FN_ERR_RET(ret, pon_net_me_list_get_data, ret);
		return ret;
	}

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		td_me_id = gem_ctp->ds_td_me_id;
		police_old = &gem_ctp->ds_police;
	} else if  (dir == NETLINK_FILTER_DIR_EGRESS) {
		td_me_id = gem_ctp->us_td_me_id;
		police_old = &gem_ctp->us_police;
	} else {
		ret = PON_ADAPTER_ERROR;
		dbg_out_ret("%d", ret);
		return ret;
	}

	return pon_net_police_update(ctx, PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				     me_id, dir, police_old, td_me_id);
}

static void update_td_prio_rate(struct pon_net_context *ctx, uint16_t me_id)
{
	struct pon_net_me_list_item *item;

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				      item) {
		struct pon_net_gem_port_net_ctp *gem_ctp  = item->data;

		if (gem_ctp->ds_td_me_id == me_id)
			pon_net_police_update_gem(ctx, item->me_id,
						 NETLINK_FILTER_DIR_INGRESS);
		if (gem_ctp->us_td_me_id == me_id)
			pon_net_police_update_gem(ctx, item->me_id,
						  NETLINK_FILTER_DIR_EGRESS);
	};

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_BRIDGE_PORT_CONFIG, item) {
		struct pon_net_bridge_port_config *bp  = item->data;

		if (bp->inbound_td_ptr == me_id)
			pon_net_police_update_bp(ctx, item->me_id,
						 NETLINK_FILTER_DIR_INGRESS);
		if (bp->outbound_td_ptr == me_id)
			pon_net_police_update_bp(ctx, item->me_id,
						 NETLINK_FILTER_DIR_EGRESS);
	};
}

static enum pon_adapter_errno
update_td_rate(struct pon_net_context *ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret;
	uint16_t tcont_me_ids[TCONT_MAX];
	unsigned int num_tconts = 0;
	uint16_t uni_me_ids[LAN_PORT_MAX];
	unsigned int num_unis = 0;
	struct pon_net_me_list_item *item;
	struct queue_assignment *as;

	dbg_in_args("%p, %u", ctx, me_id);

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				      item) {
		struct pon_net_gem_port_net_ctp *gem_ctp  = item->data;

		if (gem_ctp->us_td_me_id != me_id)
			continue;

		if (pon_net_me_id_in_array(gem_ctp->tcont_me_id, tcont_me_ids,
					   num_tconts))
			continue;

		if (num_tconts == ARRAY_SIZE(tcont_me_ids)) {
			dbg_err("Too many T-CONTs\n");
			return PON_ADAPTER_ERROR;
		}
		tcont_me_ids[num_tconts++] = gem_ctp->tcont_me_id;
	};

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				      item) {
		struct pon_net_gem_port_net_ctp *gem_ctp = item->data;
		unsigned int i = 0;

		if (gem_ctp->ds_td_me_id != me_id)
			continue;

		for (i = 0; i < gem_ctp->ds.num_assignments; ++i) {
			as = &gem_ctp->ds.assignments[i];
			if (pon_net_me_id_in_array(as->me_id, uni_me_ids,
						   num_unis))
				continue;

			if (num_unis == ARRAY_SIZE(uni_me_ids)) {
				dbg_err("Too many UNIs\n");
				return PON_ADAPTER_ERROR;
			}
			uni_me_ids[num_unis++] = as->me_id;
		}
	};

	if (num_unis >= ARRAY_SIZE(uni_me_ids)) {
		dbg_err("Too many UNIs\n");
		return PON_ADAPTER_ERROR;
	}
	ret = pon_net_uni_qdiscs_update(ctx, PON_CLASS_ID_VEIP, uni_me_ids,
					num_unis);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_qdiscs_update, ret);
		return ret;
	}
	ret = pon_net_uni_qdiscs_update(ctx, PON_CLASS_ID_PPTP_ETHERNET_UNI,
					uni_me_ids, num_unis);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_qdiscs_update, ret);
		return ret;
	}

	if (num_tconts) {
		ret =
		    pon_net_tcont_qdiscs_update(ctx, tcont_me_ids, num_tconts);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tcont_qdiscs_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno update(void *ll_handle, uint16_t me_id,
		const struct pa_traffic_descriptor_update_data *upd_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	uint8_t traffic_management_option = 0;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, upd_data);

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_TRAFFIC_DESCRIPTOR,
				    me_id, upd_data, sizeof(*upd_data));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	traffic_management_option = pon_net_traffic_management_option_get(ctx);
	if (traffic_management_option == PA_TMO_PRIO_RATE) {
		update_td_prio_rate(ctx, me_id);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (traffic_management_option == PA_TMO_RATE) {
		ret = update_td_rate(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, update_td_rate, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	uint8_t traffic_management_option = 0;

	dbg_in_args("%p, %u", ll_handle, me_id);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_TRAFFIC_DESCRIPTOR,
			       me_id);

	traffic_management_option = pon_net_traffic_management_option_get(ctx);
	if (traffic_management_option == PA_TMO_PRIO_RATE) {
		update_td_prio_rate(ctx, me_id);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (traffic_management_option == PA_TMO_RATE) {
		update_td_rate(ctx, me_id);
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_traffic_descriptor_ops traffic_descriptor_ops = {
	.update = update,
	.destroy = destroy,
};
