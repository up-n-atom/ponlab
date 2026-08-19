// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2023, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include "hal/datapath_spl_conn.h"
#endif
#include "hal/datapath_ppv4.h"
#include "hal/datapath_misc.h"

#define PROC_MAX_BOX_LVL (DP_MAX_SCH_LVL + 1) /* Sched/Port both map to a box */
/* max direct child per scheduler/port */
#define PROC_DP_MAX_CHILD_PER_SCH_PORT DP_MAX_CHILD_PER_NODE
#define PROC_MAX_Q_PER_PORT MAX_Q_PER_PORT /* max queues per port */
/* max schedulers per port */
#define PROC_DP_MAX_SCH_PER_PORT ((PROC_MAX_Q_PER_PORT) * (DP_MAX_SCH_LVL))
#define PROC_DP_MAX_LEAF 8 /* max leaf per scheduler */

#define DEBUGFS_QOS DP_DEBUGFS_PATH "/qos"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

struct location {
	int x1, y1; /* start axis */
	int x2, y2; /* end axis */
};

struct box_info;

struct child_node {
	int filled;
	enum dp_node_type type;
	union {
		int qid;
		struct box_info *box;
	} box_qid;
	struct location l;
};

struct box_info {
	int filled;
	int id;  /* physical id if available, otherwise -1 */
	int node; /* node id if available, otherwise -1 */
	int pir, cir, pbs, cbs;
	int prn_leaf;  /* since PPV4 not support leaf,
			* here generate leaf for layout printing
			*/

	int box_x, box_y, box_height; /* axis (x,y) for box */
	int size;
	int n_q, n_sch; /* direct child queue/scheduler */
	struct box_info *p_box; /* parent box */
	struct child_node child[PROC_DP_MAX_CHILD_PER_SCH_PORT];
	struct location l; /* location */
};

struct q_print_info {
	int q_num;
	int q_id[PROC_MAX_Q_PER_PORT]; /* physical queue id if available,
					*   otherwise -1
					*/
	int q_node_id[PROC_MAX_Q_PER_PORT]; /* queue node id if available,
					     * otherwise -1
					     */
	int q_prn_leaf[PROC_MAX_Q_PER_PORT];  /* since PPV4 not support leaf,
					       * here generate leaf for layout
					       * printing
					       */
	int sch_lvl[PROC_MAX_Q_PER_PORT];

	/* point to one of box entry */
	struct box_info *sch_box[PROC_MAX_Q_PER_PORT][DP_MAX_SCH_LVL];
	struct box_info port_box;
	int box_num;
	struct box_info box[PROC_DP_MAX_SCH_PER_PORT]; /* need kmalloc/kfree */
};

static const char *get_node_stat(int node_id, int type);
static char *get_node_pri(int node_id, int type);
char *dp_port_flag_str(int cqm_deq_port, int flag);
char *dp_port_dma_tx_str(int cqm_deq_port, int flag);
static void conv_limit_to_str(u32 shaper_limit, char *buf, int size);

struct box_info *find_box_via_nodeid(struct box_info *box,
				     int box_num, int sch_node_id)
{
	int i;

	for (i = 0; i < box_num; i++) {
		if (!box[i].filled)
			continue;
		if (box[i].node != sch_node_id)
			continue;
		return &box[i];
	}
	return NULL;
}

struct box_info *find_child_box(
	struct child_node child_list[PROC_DP_MAX_CHILD_PER_SCH_PORT],
	struct box_info *curr_box)
{
	int i = 0;

	while (child_list[i].filled) {
		if (child_list[i].type != DP_NODE_SCH) {
			i++;
			continue;
		}
		if (child_list[i].box_qid.box == curr_box)
			return curr_box;
		i++;
	}
	return NULL;
}

void set_child_per_box(struct q_print_info *q_info)
{
	int i, j, idx;
	struct box_info *p_box, *c_box;

	/* Get child number and info */
	for (i = 0; i < q_info->q_num; i++) { /* queue */
		if (!q_info->sch_box[i][0]) {/* queue to port case */
			p_box = &q_info->port_box;
			idx = p_box->n_q + p_box->n_sch;

			if (idx >= PROC_DP_MAX_CHILD_PER_SCH_PORT) {
				pr_err("DPM: %s: %d, PortnodeId: %d has too many child: %d, maxSupportedChild: %d\n",
				       __func__, __LINE__, p_box->node, idx+1,
				       PROC_DP_MAX_CHILD_PER_SCH_PORT);
				return;
			}
			p_box->child[idx].filled = 1;
			p_box->child[idx].box_qid.qid = q_info->q_id[i];
			p_box->child[idx].type = DP_NODE_QUEUE;
			q_info->q_prn_leaf[i] = idx;
			p_box->n_q++;
			continue;
		}
		/* queue to 1st scheduler */
		p_box = q_info->sch_box[i][0];
		idx = p_box->n_q + p_box->n_sch;

		if (idx >= PROC_DP_MAX_CHILD_PER_SCH_PORT) {
			pr_err("DPM: %s: %d, SchednodeId: %d, has too many child: %d, maxSupportedChild: %d\n",
			       __func__, __LINE__, p_box->node, idx+1,
			       PROC_DP_MAX_CHILD_PER_SCH_PORT);
			return;
		}
		if (idx < 0) {
			pr_err("DPM: %s: %d, wrong: idx(%d) should >= 0\n",
					__func__, __LINE__, idx);
			return;
		}
		p_box->child[idx].filled = 1;
		p_box->child[idx].box_qid.qid = q_info->q_id[i];
		p_box->child[idx].type = DP_NODE_QUEUE;
		q_info->q_prn_leaf[i] = idx;
		p_box->n_q++;

		/* scheduler to schduer/port */
		for (j = 0; j < q_info->sch_lvl[i]; j++) {
			if (j < (q_info->sch_lvl[i] - 1))
				p_box = q_info->sch_box[i][j + 1];
			else
				p_box = &q_info->port_box;
			c_box = q_info->sch_box[i][j];
			idx = p_box->n_q + p_box->n_sch;
			if (idx < 0) {
				pr_err("DPM: %s: %d, nodeId: %d, wrong: idx(%d) should >= 0\n",
						__func__, __LINE__,
						c_box->node, idx);
				return;
			}
			c_box->p_box = p_box;
			if (find_child_box(p_box->child, c_box))
				continue;
			if (idx >= PROC_DP_MAX_CHILD_PER_SCH_PORT) {
				pr_err("DPM: %s: %d, %snodeId: %d, has too many child: %d, maxSupportedChild: %d\n",
				       __func__, __LINE__,
				       (q_info->sch_lvl[i] == (j+1)) ? "Port" : "Sched",
				       p_box->node,
				       idx+1, PROC_DP_MAX_CHILD_PER_SCH_PORT);
				return;
			}
			p_box->child[idx].filled = 1;
			p_box->child[idx].box_qid.box = c_box;
			p_box->child[idx].type = DP_NODE_SCH;
			c_box->prn_leaf = idx;
			p_box->n_sch++;
		}
	}
}

#define PREFIX_SIZE_PER_BOX 1 /* for opening ----- */
#define INFO_SIZE_PER_BOX   2 /* for box info, like node id and others */
#define SUFIX_SIZE_PER_BOX  1 /* for closing ---- */
#define EXTRA_SIZE_PER_BOX (PREFIX_SIZE_PER_BOX + INFO_SIZE_PER_BOX + \
			    SUFIX_SIZE_PER_BOX)
#define PADDING_BETWEEN_BOX_X 2 /* axis x */
#define PADDING_BETWEEN_BOX_Y 1 /* axis y */
#define SIZE_PER_QUEUE      3
#define BOX_WIDTH           20
#define Q_WIDTH             18
#define PORT_OTHER_INFO     20
#define PORT_BOX_SUFFIX     22

int set_location_size(struct box_info *box, int y)
{
	int i, y2 = 0, size = 0;

	box->l.x1 = Q_WIDTH + box->box_x * (BOX_WIDTH + PADDING_BETWEEN_BOX_X);
	box->l.x2 = box->l.x1 + BOX_WIDTH;
	box->l.y1 = y;
	y2 = box->l.y1 + PREFIX_SIZE_PER_BOX + INFO_SIZE_PER_BOX;
	for (i = 0; i < box->n_q + box->n_sch && box->child[i].filled; i++) {
		if (box->child[i].type == DP_NODE_QUEUE) {
			box->child[i].l.x2 = box->l.x1;
			box->child[i].l.y2 = y2;
			size += SIZE_PER_QUEUE;
			y2 += SIZE_PER_QUEUE;
		} else if (box->child[i].type == DP_NODE_SCH) {
			set_location_size(box->child[i].box_qid.box, y2);
			box->child[i].l.x2 = box->l.x1;
			box->child[i].l.y2 = y2;
			size += box->child[i].box_qid.box->size;
			y2 += box->child[i].box_qid.box->size;
		}
	}
	y2 += SUFIX_SIZE_PER_BOX;
	size += EXTRA_SIZE_PER_BOX;
	box->l.y2 = y2;
	box->size = size;
	return 0;
}

int check_location(struct q_print_info *q_info)
{
	int i;

	for (i = 0; i < q_info->box_num; i++) {
		if ((q_info->box[i].l.x2 - q_info->box[i].l.x1) != BOX_WIDTH) {
			pr_err("DPM: sched[%d] x1/x2: %d - %d should equal%d\n",
			       q_info->box[i].node,
			       q_info->box[i].l.x2,
			       q_info->box[i].l.x1,
			       q_info->box[i].l.x2 - q_info->box[i].l.x1);
			return -1;
		}
		if (!q_info->box[i].p_box)
			continue;
		if ((q_info->box[i].p_box->l.x1 - q_info->box[i].l.x2) !=
		    PADDING_BETWEEN_BOX_X) {
			pr_err("DPM: sched[%d]<->sched[%d]: %d - %d %s%d\n",
			       q_info->box[i].node,
			       q_info->box[i].p_box->node,
			       q_info->box[i].p_box->l.x2,
			       q_info->box[i].l.x1,
			       "should equal",
			       q_info->box[i].p_box->l.x2 -
			       q_info->box[i].l.x1);
			return -1;
		}
	}
	return 0;
}

void virtual_print_box(struct box_info *box,
		       struct box_info *p_box,
		       char *buf, int rows, int cols)
{
	char *p;
	int i, len;
	const char *stat = NULL;
	char *info = NULL, *p_flag = NULL, *p_dma_tx = NULL;
	struct dp_shaper_conf *shaper;
	char buf_cir[6] = {0};
	char buf_pir[6] = {0};

	/* The format like below
	 *         -----------------------
	 *        |sched[%03d]            |
	 *        |                       |
	 *        |leaf[%2d]:kbps         |
	 *        | cir/pir:%5d/%5d       |
	 *        | cbs/pbs:%5d/%5d       |
	 *        | ....                  |
	 *        |                       |
	 *         -----------------------
	 */
	shaper = dp_kzalloc(sizeof(*shaper), GFP_ATOMIC);
	if (!shaper)
		return;
	p = &buf[cols * box->l.y1];
	for (i = box->l.x1; i < box->l.x2; i++)
		p[i] = '-';
	p = &buf[cols * (box->l.y2 - 1)];
	for (i = box->l.x1; i < box->l.x2; i++)
		p[i] = '-';

	for (i = 0; i < INFO_SIZE_PER_BOX; i++) {
		p = &buf[cols * (box->l.y1 + 1 + i)];
		p += box->l.x1 + 1;
		if (i == 0) { /* print 1st info of box */
			if (!p_box) { /* port box */
				len = snprintf(p, BOX_WIDTH - 3,
					       "port[%d/%d]",
					       box->id, box->node);
				p[len] = ' ';
				stat = get_node_stat(box->id, DP_NODE_PORT);
				shaper->id.cqm_deq_port = box->id;
				shaper->type = DP_NODE_PORT;
				dp_shaper_conf_get(shaper, 0);
				if (shaper->cir == DP_MAX_SHAPER_LIMIT ||
				    shaper->cir == DP_NO_SHAPER_LIMIT) {
					conv_limit_to_str(shaper->cir, buf_cir,
							  sizeof(buf_cir));
					DP_DEBUG(DP_DBG_FLAG_QOS,
						 "port[%d] shaper=%d(%s)\n",
						 shaper->id.cqm_deq_port,
						 shaper->cir,
						 buf_cir);
				} else {
					DP_DEBUG(DP_DBG_FLAG_QOS,
						 "port[%d] shaper=%d\n",
						 shaper->id.cqm_deq_port,
						 shaper->cir);
				}
				p_flag = dp_port_flag_str(box->id, 0);
				p_dma_tx = dp_port_dma_tx_str(box->id, 0);
				DP_DEBUG(DP_DBG_FLAG_QOS,
					 "port[%d] p_flag=%s\n",
					 box->id, p_flag);
			} else { /* sched box */
				len = snprintf(p, BOX_WIDTH - 3,
					       "sched[/%d]",  box->node);
				stat = get_node_stat(box->node, DP_NODE_SCH);
				info = get_node_pri(box->node, DP_NODE_SCH);
				shaper->id.sch_id = box->node;
				shaper->type = DP_NODE_SCH;
				dp_shaper_conf_get(shaper, 0);
				if (shaper->cir == DP_MAX_SHAPER_LIMIT ||
				    shaper->cir == DP_NO_SHAPER_LIMIT) {
					conv_limit_to_str(shaper->cir, buf_cir,
							  sizeof(buf_cir));
					DP_DEBUG(DP_DBG_FLAG_QOS,
						 "sched[%d] shaper=%d(%s)\n",
						 shaper->id.sch_id, shaper->cir,
						 buf_cir);
				} else {
					DP_DEBUG(DP_DBG_FLAG_QOS,
						 "sched[%d] shaper=%d\n",
						 shaper->id.sch_id, shaper->cir);
				}
				p[len] = ' ';
			}
		} else if (i == 1) {
			len = snprintf(p, BOX_WIDTH - 3, " stat:%s",
				       stat ? stat : "NULL");
			p[len] = ' ';
		}
	}

	for (i = box->l.y1 + 1; i < box->l.y2 - 1; i++) {
		p = &buf[cols * i];
		p[box->l.x1] = '|';
	}
	for (i = box->l.y1 + 1; i < box->l.y2 - 1; i++) {
		p = &buf[cols * i];
		p[box->l.x2 - 1] = '|';
	}
	if (!p_box) { /* port information */
		p = &buf[cols * ((box->l.y1 + box->l.y2) / 2 - 1)];
		p += box->l.x2;
		len = snprintf(p, cols - box->l.x2 - 1,
			       "--%s", p_flag ? p_flag : "");
		if (len >= 0)
			p[len] = ' ';

		p += cols;
		len = snprintf(p, cols - box->l.x2 - 1,
			       "  %s", p_dma_tx ? p_dma_tx : "");
		if (len >= 0)
			p[len] = ' ';

		p += cols;
		conv_limit_to_str(shaper->cir, buf_cir, sizeof(buf_cir));
		conv_limit_to_str(shaper->pir, buf_pir, sizeof(buf_pir));
		len = snprintf(p, cols - box->l.x2 - 1,
			       "  C/P:%5s/%5s", buf_cir, buf_pir);
		p[len] = ' '; /* remove \0' added by snprintf */
		p += cols;
		len = snprintf(p, cols - box->l.x2 - 1,
			       "  c/p:%5d/%5d", shaper->cbs, shaper->pbs);
		p[len] = ' ';
		kfree(shaper);
		return;
	}

	/* print link to the parent box */
	p = &buf[cols * ((box->l.y1 + box->l.y2) / 2)];
	for (i = box->l.x2; i < p_box->l.x1; i++)
		p[i] = '-';

	/* print leaf info in the parent box:sched/port */
	p += p_box->l.x1 + 1; /* skip '|' */
	for (i = 0; i < SIZE_PER_QUEUE; i++) {
		if (i == 0) {
			len = snprintf(p, BOX_WIDTH - 3,/* skip: | & | & null */
				       "child[%d] %s", box->prn_leaf,
				       info ? info : "");
			p[len] = ' ';
		} else if (i == 1) {
			conv_limit_to_str(shaper->cir, buf_cir, sizeof(buf_cir));
			conv_limit_to_str(shaper->pir, buf_pir, sizeof(buf_pir));
			len = snprintf(p, BOX_WIDTH - 3,
				       " C/P:%5s/%5s", buf_cir, buf_pir);
			p[len] = ' ';
		} else if (i == 2) {
			len = snprintf(p, BOX_WIDTH - 3,
				       " c/p:%5d/%5d", shaper->cbs, shaper->pbs);
			p[len] = ' ';
		}
		/* move to next row */
		p += cols;
	}
	kfree(shaper);
}

#define DROP_MODE(x) \
	x.codel ? "Codel" : x.drop == DP_QUEUE_DROP_WRED ? "Wred" : "Tail"
void virtual_print_queues(struct q_print_info *q_info,
			  char *buf, int rows, int cols)
{
	int i, j;
	struct box_info *box;
	int len, idx;
	char *p;
	const char *stat = NULL;
	char *info = NULL;
	struct dp_shaper_conf shaper = {0};
	struct dp_queue_conf q_conf = {0};
	char buf_cir[6] = {0};
	char buf_pir[6] = {0};

	for (i = 0; i < q_info->q_num; i++) {
		if (q_info->sch_box[i][0])
			box = q_info->sch_box[i][0];
		else
			box = &q_info->port_box;
		idx = q_info->q_prn_leaf[i];
		DP_DEBUG(DP_DBG_FLAG_QOS, "get_node_stat:queue=%d\n",
			 q_info->q_id[i]);
		stat = get_node_stat(q_info->q_id[i], DP_NODE_QUEUE);
		info = get_node_pri(q_info->q_id[i], DP_NODE_QUEUE);
		shaper.id.q_id = q_info->q_id[i];
		shaper.type = DP_NODE_QUEUE;
		dp_shaper_conf_get(&shaper, 0);

		if (shaper.cir == DP_MAX_SHAPER_LIMIT ||
		    shaper.cir == DP_NO_SHAPER_LIMIT) {
			conv_limit_to_str(shaper.cir, buf_cir, sizeof(buf_cir));
			DP_DEBUG(DP_DBG_FLAG_QOS, "q[%d] shaper=%d(%s)\n",
				 shaper.id.q_id, shaper.cir, buf_cir);
		} else {
			DP_DEBUG(DP_DBG_FLAG_QOS, "q[%d] shaper=%d\n",
				 shaper.id.q_id, shaper.cir);
		}
		p = &buf[cols * box->child[idx].l.y2];
		len = snprintf(p, Q_WIDTH - 1, "q[%4d/%4d]",
			       q_info->q_id[i], q_info->q_node_id[i]);
		for (j = len; j < box->l.x1; j++)
			p[j] = '-';

		p = &buf[cols * (box->child[idx].l.y2 + 1)];
		len = snprintf(p, Q_WIDTH - 1, "  stat:%s",
			       stat ? stat : "");
		p[len] = ' ';

		q_conf.q_id = q_info->q_id[i];
		if (!dp_queue_conf_get(&q_conf, 0)) {
			p = &buf[cols * (box->child[idx].l.y2 + 2)];
			len = snprintf(p, Q_WIDTH - 1, "  max=%d:%s",
				       q_conf.wred_max_allowed,
				       DROP_MODE(q_conf));
			p[len] = ' ';
		}

		/* print leaf info in the parent box:sched/port */
		p = &buf[cols * box->child[idx].l.y2];
		p += box->l.x1 + 1; /* skip '|' */
		for (j = 0; j < SIZE_PER_QUEUE; j++) {
			if (j == 0) {
				len = snprintf(p, BOX_WIDTH - 3, /* skip:| and |
								  * and null
								  */
					       "child[%d]:%s",
					       q_info->q_prn_leaf[i],
					       info ? info : "");
				p[len] = ' ';
			} else if (j == 1) {
				conv_limit_to_str(shaper.cir, buf_cir,
						  sizeof(buf_cir));
				conv_limit_to_str(shaper.pir, buf_pir,
						  sizeof(buf_pir));
				len = snprintf(p, BOX_WIDTH - 3,
					       " C/P:%5s/%5s",
					       buf_cir, buf_pir);
				p[len] = ' ';
			} else if (j == 2) {
				len = snprintf(p, BOX_WIDTH - 3,
					       " c/p:%5d/%5d",
					       shaper.cbs, shaper.pbs);
				p[len] = ' ';
			}
			/* move to next row */
			p += cols;
		}
	}
}

#define PRINT_QOS_DETAIL  0
void print_all(struct seq_file *s, struct q_print_info *q_info)
{
	int cols = q_info->port_box.l.x2 + PORT_BOX_SUFFIX;
	int rows = q_info->port_box.l.y2 + 1;
	int i;
	char *buf;
	char *p;

	if (cols <= 0)
		return;
	buf = dp_kzalloc(cols * rows + 1, GFP_ATOMIC);
	if (!buf)
		return;
	dp_memset(buf, ' ', cols * rows);
	buf[cols * rows] = 0;
#if PRINT_QOS_DETAIL
	dp_sprintf(s, "allocate buffer: %d bytes(%d * %d)\n",
		   cols * rows, cols, rows);
#endif

	p = buf;
	for (i = 0; i < rows; i++)
		buf[((i + 1) * cols) - 1] = 0;

	/* print port box */
	virtual_print_box(&q_info->port_box, NULL, buf, rows, cols);
	for (i = 0; i < q_info->box_num; i++)
		virtual_print_box(&q_info->box[i], q_info->box[i].p_box, buf,
				  rows, cols);
	/* print queue */
	virtual_print_queues(q_info, buf, rows, cols);
	p = buf;
	for (i = 0; i < rows; i++) {
		dp_sprintf(s, "%s\n", p);
		p += cols;
	}
	kfree(buf);
}

/* print_box_lvl must bigger 1 than sch_lvl */
struct q_print_info *collect_info(struct seq_file *s,
				  struct dp_dequeue_res *res,
				  int print_box_lvl)
{
	int i, j, curr_box_y = 0, curr_box_x;
	struct q_print_info *q_info = NULL;
	struct box_info *box;
	char f_new_box;

	if (!res || !res->num_q || res->num_deq_ports < 1)
		goto ERR_EXIT;
	q_info = dp_kzalloc(sizeof(*q_info), GFP_ATOMIC);
	if (!q_info)
		return NULL;

	q_info->port_box.filled = 1;
	q_info->port_box.id = res->q_res[0].cqm_deq_port;
	q_info->port_box.node = res->q_res[0].qos_deq_port;
	q_info->port_box.box_x = print_box_lvl - 1;
	q_info->port_box.box_y = 0;

	for (i = 0; i < res->num_q; i++) { /* q loop */
		q_info->q_id[i] = res->q_res[i].q_id;
		q_info->q_node_id[i] = res->q_res[i].q_node;
		if (res->q_res[i].sch_lvl <= 0)
			continue;
		if (res->q_res[i].sch_lvl > DP_MAX_SCH_LVL) {
			pr_err("DPM: Too many sched lvl(%d): expect<=%d\n",
			       res->q_res[i].sch_lvl, DP_MAX_SCH_LVL);
			goto ERR_EXIT;
		}
		f_new_box = 0;
		curr_box_x = print_box_lvl - res->q_res[i].sch_lvl - 1;
		for (j = 0; j < res->q_res[i].sch_lvl; j++) { /* each sched */
			box = find_box_via_nodeid(q_info->box,
						  q_info->box_num,
						  res->q_res[i].sch_id[j]);
			if (box) {
				q_info->sch_box[i][j] = box;
				continue;
			}
			/* create a new box */
			dp_memset(&q_info->box[q_info->box_num], 0,
				  sizeof(q_info->box[q_info->box_num]));
			q_info->box[q_info->box_num].filled = 1;
			q_info->box[q_info->box_num].node =
				res->q_res[i].sch_id[j];
			q_info->box[q_info->box_num].id = -1; /* not valid */
			q_info->box[q_info->box_num].box_x = curr_box_x + j;
			q_info->box[q_info->box_num].box_y = curr_box_y;
			q_info->sch_box[i][j] = &q_info->box[q_info->box_num];
			q_info->box_num++;
			f_new_box = 1;
		}
		q_info->sch_lvl[i] = res->q_res[i].sch_lvl;
		if (f_new_box)
			curr_box_y++;
	}
	q_info->q_num = res->num_q;

	/* sanity check */
	for (i = 0; i < res->num_q; i++) {
		if (!q_info->sch_lvl[i])
			continue;
		for (j = 0; j < q_info->sch_lvl[i]; j++) {
			if (!q_info->sch_box[i][j]->filled) {
				pr_err("DPM: sch_box[%d][%d].fill should 1:%d\n",
				       i, j, q_info->sch_box[i][j]->filled);
				goto ERR_EXIT;
			}
			if (q_info->sch_box[i][j]->n_q < 0) {
				pr_err("DPM: sch_box[%d][%d].n_q should >=0:%d\n",
				       i, j, q_info->sch_box[i][j]->n_q);
				goto ERR_EXIT;
			}
			if (q_info->sch_box[i][j]->n_sch < 0) {
				pr_err("DPM: sch_box[%d][%d].n_sch should >=0:%d\n",
				       i, j, q_info->sch_box[i][j]->n_sch);
				goto ERR_EXIT;
			}
		}
	}
	return q_info;
ERR_EXIT:
	kfree(q_info);
	return NULL;
}

//static struct dp_dequeue_res res;
static struct dp_queue_res q_res[PROC_MAX_Q_PER_PORT * 4];
static int qos_layout_inst;
static int qos_layout_max_lvl = PROC_MAX_BOX_LVL; /* sched/port box */
struct dp_dequeue_res tmp_res;
struct dp_queue_res tmp_q_res[PROC_MAX_Q_PER_PORT] = {0};
#define DP_PROC_BUF_LEN 50
static char dma_flag[DP_PROC_BUF_LEN];
static char port_flag[DP_PROC_BUF_LEN];

static char *port_flag_str(int inst, int dp_port)
{
	int i;
	struct pmac_port_info *port_info;

	port_info = get_dp_port_info(inst, dp_port);
	if (!port_info)
		return "";
	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (port_info->alloc_flags & dp_port_flag[i])
			return dp_port_type_str[i];
	}
	return "";
}

bool is_in_deq_port_range(struct pmac_port_info *pi, int cqm_deq_port)
{
	int i;

	/* CQM dequeue port maybe not continuous for one device.
	 *So have to manually check one by one
	 */
	for (i = 0; i < pi->deq_port_num; i++) {
		if (cqm_deq_port == pi->deq_ports[i])
			return true;
	}
	return false;
}

char *dp_port_flag_str(int cqm_deq_port, int flag)
{
	int i;
	int inst = qos_layout_inst;
	struct inst_info *dp_info = get_dp_prop_info(inst);
	struct cqm_port_info *deq =
		get_dp_deqport_info(inst, cqm_deq_port);
	int egp_flag;
	int len, seek = 0;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	const char *name = NULL;
#endif

	port_flag[0] = 0;
	if (cqm_deq_port == reinsert_deq_port[inst]) {
		snprintf(port_flag, sizeof(port_flag), "Reinsert:");
		return port_flag;
	}

	for (i = 0; i < ARRAY_SIZE(deq->dp_port); i++) {
		if(!deq->dp_port[i])
			continue;

		if (i == CPU_PORT) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
			name = dp_spl_conn_get_name_from_egp(inst,
							     cqm_deq_port);
			if (name)
				snprintf(port_flag, sizeof(port_flag),
					 "SPL:%s", name);
			else
#endif
				snprintf(port_flag, sizeof(port_flag),
					"CPU:%d", i);

			return port_flag;
		} else {
			len = sizeof(port_flag) - seek;
			if (seek == 0)
				seek += snprintf(port_flag + seek, len,
						"%s:%d",
						port_flag_str(inst, i),
						i);
			else
				seek += snprintf(port_flag + seek, len,
						",%d", i);
		}
	}
	egp_flag = dp_info->dp_get_deq_port_flag(inst, cqm_deq_port);
	if (egp_flag == CQM_PORT_INFO_DC_RX_Q) {
		snprintf(port_flag, sizeof(port_flag), "DC_RX_Q:");
		return port_flag;
	}

	if (seek == 0)
		snprintf(port_flag, sizeof(port_flag), "?:%d", cqm_deq_port);
	return port_flag;
}

char *dp_port_dma_tx_str(int cqm_deq_port, int flag)
{
	int i;
	int inst = qos_layout_inst;
	struct cqm_port_info *deq_pinfo =
		get_dp_deqport_info(inst, cqm_deq_port);
	struct inst_info *dp_info = get_dp_prop_info(inst);
	int egp_flag;

	dma_flag[0] = 0;
	if (cqm_deq_port == reinsert_deq_port[inst]) {
		deq_pinfo = get_dp_deqport_info(0, reinsert_deq_port[inst]);
		if (deq_pinfo->f_dma_ch)
			snprintf(dma_flag, sizeof(dma_flag), "CH%x",
				 deq_pinfo->dma_chan);
		return dma_flag;
	}
	for (i = 0; i < dp_info->cap.max_num_dp_ports; i++) {
		struct pmac_port_info *pi = get_dp_port_info(inst, i);

		if (is_in_deq_port_range(pi, cqm_deq_port)) {
			if (i == 0) {
				snprintf(dma_flag, sizeof(dma_flag), "-");
				return dma_flag;
			}
			if (!pi->alloc_flags)
				continue;
			if (deq_pinfo->f_dma_ch)
				snprintf(dma_flag, sizeof(dma_flag), "CH%x",
					 deq_pinfo->dma_chan);
			return dma_flag;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		} else if (i == 0 &&
			   dp_spl_conn_get_name_from_egp(inst, cqm_deq_port)) {
			if (deq_pinfo->f_dma_ch)
				snprintf(dma_flag, sizeof(dma_flag), "CH%x",
					 deq_pinfo->dma_chan);
			return dma_flag;
#endif
		}
	}

	egp_flag = dp_info->dp_get_deq_port_flag(inst, cqm_deq_port);
	if (egp_flag == CQM_PORT_INFO_DC_RX_Q) /* not show any CH info */
		return dma_flag;

	return dma_flag;
}

static const char *get_node_stat(int node_id, int type)
{
	struct dp_node_link_enable node_en = {0};

	node_en.inst = qos_layout_inst;
	node_en.id.q_id = node_id;
	node_en.type = (enum dp_node_type)type;
	if (dp_node_link_en_get(&node_en, 0)) {
		return "?";
	} else {
		if (node_en.en & DP_NODE_DIS)
			return "Blk ";
		if (node_en.en & DP_NODE_SUSPEND)
			return "Susp";
		if (node_en.en & DP_NODE_EN)
			return "Normal";
	}

	return "??";
}

static char *get_node_pri(int node_id, int type)
{
	struct dp_node_prio node_prio = {0};
	static char stat[20];
	int len;

	stat[0] = 0;
	node_prio.inst = qos_layout_inst;
	node_prio.id.q_id = node_id;
	node_prio.type = (enum dp_node_type)type;
	if (dp_qos_link_prio_get(&node_prio, 0)) {
		dp_strlcpy(stat, "?", sizeof(stat));
	} else {
		if (node_prio.arbi == ARBITRATION_WRR)
			dp_strlcpy(stat, "WRR", sizeof(stat));
		else if (node_prio.arbi == ARBITRATION_SP)
			dp_strlcpy(stat, "SP", sizeof(stat));
		else if (node_prio.arbi == ARBITRATION_WSP)
			dp_strlcpy(stat, "WSP", sizeof(stat));
		else if (node_prio.arbi == ARBITRATION_WFQ)
			dp_strlcpy(stat, "WFQ", sizeof(stat));
		else
			snprintf(stat, sizeof(stat), "?%d:", node_prio.arbi);

		len = strlen(stat);
		snprintf(stat + len, sizeof(stat) - len - 1, ":%d",
			 node_prio.prio_wfq);
	}
	if (node_prio.type == DP_NODE_QUEUE)
		DP_DEBUG(DP_DBG_FLAG_QOS, "q[%d] arbi=%d prio=%d\n",
			 node_prio.id.q_id, node_prio.arbi, node_prio.prio_wfq);
	else if (node_prio.type == DP_NODE_SCH)
		DP_DEBUG(DP_DBG_FLAG_QOS, "sch[%d] arbi=%d prio=%d\n",
			 node_prio.id.sch_id, node_prio.arbi,
			 node_prio.prio_wfq);
	else if (node_prio.type == DP_NODE_PORT)
		DP_DEBUG(DP_DBG_FLAG_QOS, "port[%d] arbi=%d prio=%d\n",
			 node_prio.id.cqm_deq_port, node_prio.arbi,
			 node_prio.prio_wfq);
	else
		DP_DEBUG(DP_DBG_FLAG_QOS, "unknown type\n");
	return stat;
}

void conv_limit_to_str(u32 shaper_limit, char *buf, int size)
{
	if (shaper_limit == DP_NO_SHAPER_LIMIT)
		snprintf(buf, size, "NoLim");
	else if (shaper_limit == DP_MAX_SHAPER_LIMIT)
		snprintf(buf, size, "MaxLm");
	else
		snprintf(buf, size, "%d", shaper_limit);
}

#define BUF_SIZE 1048
static int deq_port_res_get(int inst, int dp_port, int deq_idx)
{
	struct local {
		struct dp_dequeue_res dp_res;
		struct dp_queue_res queue_res;
		char p[BUF_SIZE];
	};
	struct local *l;
	int i, j, num_q, num;
	size_t len;

	/* retrieve the num_q for DP port*/
	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return -1;
	l->dp_res.inst = inst;
	l->dp_res.dp_port = dp_port;
	l->dp_res.cqm_deq_idx = deq_idx;
	if (dp_deq_port_res_get(&l->dp_res, 0)) {
		kfree(l);
		return -1;
	}
	num_q = l->dp_res.num_q;
	DP_INFO("num of q:%d\n", l->dp_res.num_q);
	/* retrieve the Q info */
	len = sizeof(struct dp_queue_res) * num_q;
	l->dp_res.q_res = &l->queue_res;
	l->dp_res.q_res_size = num_q;
	l->dp_res.cqm_deq_idx = deq_idx;
	if (dp_deq_port_res_get(&l->dp_res, 0)) {
		pr_err("DPM: dp_deq_port_res_get fail: inst=%d dp_port=%d idx:%d\n",
		       inst, dp_port, deq_idx);
		kfree(l);
		return -1;
	}
	DP_INFO("deq_ports:%d\n", l->dp_res.num_deq_ports);

	for (i = 0; i < num_q; i++) {
		num = 0;
		DP_DUMP("sch_level:%d\n", l->dp_res.q_res[i].sch_lvl);
		num = snprintf(l->p, BUF_SIZE - 1,
			       "qid[%d]/qnode:%d/%d->", i,
			       l->dp_res.q_res[i].q_id, l->dp_res.q_res[i].q_node);
		for (j = 0; j < l->dp_res.q_res[i].sch_lvl; j++) {
			num += snprintf(l->p + num, BUF_SIZE - num - 1,
					"sched[%d]:%d->",
					j, l->dp_res.q_res[i].sch_id[j]);
		}
		num += snprintf(l->p + num, BUF_SIZE - num - 1,
			       "cqm_deq_port/logical_port:%d/%d",
			       l->dp_res.q_res[i].cqm_deq_port,
			       l->dp_res.q_res[i].qos_deq_port);
		DP_DUMP("%s\n", l->p);
	}
	kfree(l);
	return 0;
}

int get_res(struct dp_dequeue_res *t, int inst, int dp_port, int tconf_idx)
{
	dp_memset(t, 0, sizeof(*t));
	t->inst = inst;
	t->dp_port = dp_port;
	t->cqm_deq_idx = tconf_idx;
	t->q_res = q_res;
	t->q_res_size = ARRAY_SIZE(q_res);
	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "%s: dp_port=%d tconf_idx=%d\n",
		 __func__, t->dp_port, t->cqm_deq_idx);
	if (dp_deq_port_res_get(t, 0)) {
		pr_err("DPM: dp_deq_port_res_get fail: inst=%d dp_port=%d\n",
		       qos_layout_inst, t->dp_port);
		return -1;
	}
	return 0;
}

int dump_q_info_dbg(struct seq_file *s, struct dp_dequeue_res *res)
{
#if PRINT_QOS_DETAIL
	struct dp_queue_res *q_res = res->q_res;
	int i, j;

	for (i = 0; i < res->num_q; i++) {
		dp_sprintf(s, "q[%d]-", q_res[i].q_id);
		for (j = 0; j < q_res[i].sch_lvl; j++)
			dp_sprintf(s, "sched[%d]-", q_res[i].sch_id[j]);
		dp_sprintf(s, "p[%d/%d]\n", q_res[i].cqm_deq_port,
			   q_res[i].qos_deq_port);
	}
#endif
	return 0;
}

/* egp: relative index
 * pos: absolute port id
 */
static bool print_qos_entry(struct seq_file *s, u8 dpid, u8 egp,
				   int pos)
{
	struct q_print_info *q_info;

	if (get_res(&tmp_res, qos_layout_inst, dpid, egp))
		return true;
	dump_q_info_dbg(s, &tmp_res);
	if (!tmp_res.num_q)
		return true;
	if (tmp_res.num_q > PROC_MAX_Q_PER_PORT) {
		dp_sprintf(s, "error: too many queues(%d) in one port: expect<=%d for tcont=%d egp=%d\n",
			   tmp_res.num_q, PROC_MAX_Q_PER_PORT,
			   egp, pos);
		return false;
	}
	q_info = collect_info(s, &tmp_res, qos_layout_max_lvl);
	if (!q_info)
		return false;
	set_child_per_box(q_info);
	set_location_size(&q_info->port_box, 0);
	if (!check_location(q_info))
		print_all(s, q_info);
	kfree(q_info);
	return true;
}

int proc_qos_dump(struct seq_file *s, int pos)
{
	int inst = qos_layout_inst, tmp;
	struct cqm_port_info *deq_pinfo;
	struct pmac_port_info *pi;
	u32 base;
	struct dp_cap *cap;
	int i;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;
	if (pos == 0) {
		dp_sputs(s, "Note:\n");
		dp_sputs(s, "  x/y :physical node/logical node\n");
		dp_sputs(s, "      :cqm dequeue port/ppv4 logical node\n");
		dp_sputs(s, "  Blk :blocked, ie new incoming packet will be dropped\n");
		dp_sputs(s, "  Susp:suspended, ie, packet in queue will not be scheduled\n");
		dp_sputs(s, "  C   :CIR in kbps\n");
		dp_sputs(s, "  P   :PIR in kbps\n");
		dp_sputs(s, "  c   :cbs in bytes\n");
		dp_sputs(s, "  p   :pbs in bytes\n");
		dp_sputs(s, "\n");
	}
	cap = &get_dp_prop_info(inst)->cap;
	deq_pinfo = get_dp_deqport_info(inst, pos);
	if (!deq_pinfo->ref_cnt)
		goto exit;
	tmp = dp_deq_find_a_dpport(deq_pinfo->dp_port);
	if (tmp < 0) {
		dp_sprintf(s, "DPM: Could not find valid DP port for "
				"deq port(ref): %d(%d)\n",
				pos, deq_pinfo->ref_cnt);
		goto exit;
	}
	pi = get_dp_port_info(qos_layout_inst, tmp);

	if (pi->port_id == CPU_PORT &&
			(dp_is_spl_conn(qos_layout_inst, pos) ||
			 dp_is_reinsert_deq_port(inst, pos))) {
		/* SPL connections like toe, vpn, applite, voice etc.
		 * These spl devices deq ports are not listed in pi->deq_ports[]
		 * And also "Reinsert" port also handled here.
		 */
		base = pi->deq_port_base;
		if (pos >= base) {
			print_qos_entry(s, tmp, pos - base, pos);
		} else {
			dp_sprintf(s, "error: why pos(%d)<base(%d)\n", pos, base);
		}
	} else {
		/* All other cases(also handles dp_port == CPU_PORT && its non spl device)
		 * that are having deq_port_num >= 1 are handled here
		 */
		for (i = 0; i < pi->deq_port_num; i++) {
			if (pi->deq_ports[i] == pos) {
				print_qos_entry(s, tmp, i, pos);
				break;
			}
		}
	}
	if (dp_seq_has_overflowed(s))
		return pos;
exit:
	pos++;
	if (pos >= DP_MAX_CQM_DEQ)
		pos = -1;

	return pos;
}
EXPORT_SYMBOL(proc_qos_dump);

int proc_qos_init(void *param)
{
	struct dp_qos_level dp = {0};

	qos_layout_inst = 0;
	/* Current using maximum value. Should check real QOS
	 * configuration and get its max hirachy layers
	 * - qos_layout_max_lvl = PROC_MAX_BOX_LVL;
	 */
	dp_qos_level_get(&dp, 0);
	qos_layout_max_lvl = dp.max_sch_lvl + 1;
	return 0;
}

void qos_create_qos_help(void)
{
	pr_info("QOS Command Help:\n");
	pr_info("\n");
	pr_info("     ADD QUEUE: echo add_q <qid> <schid>:<leaf>... <portid> <dp_port> > %s\n",
		DEBUGFS_QOS);
	pr_info("            id: qid/schid_node/cbm_port/dp_port\n");
	pr_info("\n");
	pr_info("  DELETE QUEUE: echo del_q <qid> > %s\n", DEBUGFS_QOS);
	pr_info("\n");
	pr_info("  DELETE SCHED: echo del_sch <schid> > %s\n", DEBUGFS_QOS);
	pr_info("\n");
	pr_info("      SET PRIO: echo prio <id> <type> <arbi> <prio_wfq> > %s\n",
		DEBUGFS_QOS);
	pr_info("            id: phy_queue/sched_node\n");
	pr_info("          type: queue/sched\n");
	pr_info("          arbi: null/sp/wsp/wrr/wrr_wsp/wfq\n");
	pr_info("\n");
	pr_info("    CFG SHAPER: echo shaper <cmd> <id> <type> <cir> <pir> <cbs> <pbs> > %s\n",
		DEBUGFS_QOS);
	pr_info("           cmd: add/remove/disable\n");
	pr_info("            id: qid/sched_node/cbm_port\n");
	pr_info("          type: queue/sched/port\n");
	pr_info("           cir: no_limit/max/value\n");
	pr_info("\n");
	pr_info("      SET NODE: echo set_node <id> <type> <cmd>... > %s\n",
		DEBUGFS_QOS);
	pr_info("            id: phy_queue/sched_node/cbm_port\n");
	pr_info("          type: queue/sched/port\n");
	pr_info("           cmd: enable(unblock)/disable(block)/resume/suspend\n");
	pr_info("                enable/disable: only for queue/port\n");
	pr_info("                resume/suspend: for all nodes\n");
	pr_info("\n");
	pr_info("   CLEAR STATS: echo clear > %s\n", DEBUGFS_QOS);
	pr_info("\n");
	pr_info("   PRINT QUEUE CFG: echo q_conf <id> > %s\n", DEBUGFS_QOS);
	pr_info("   SET QUEUE CFG: echo set_q_conf <id> [[options] [value]] > %s\n",
		DEBUGFS_QOS);
	pr_info("       options : action [0x1 : disable, 0x2 : enable, 0x4 suspend]\n");
	pr_info("                        [0x8 : resume]\n");
	pr_info("               : drop [0 : tail drop mode, 1 : wred mode]\n");
	pr_info("               : green_min_size/green_max_size/\n");
	pr_info("               : green_slope/yellow_min_size/yellow_max_size/\n");
	pr_info("               : yellow_slope/min_guaranteed/max_allowed\n");
	pr_info("               : codel [0 : disable, 1 : enable]\n");
	pr_info("\n");
	pr_info("   DEQ Q RES GET: echo queue_res_get <dp_port> <dq_idx> > %s\n",
		DEBUGFS_QOS);
	pr_info("            dp_port: datapath port id\n");
	pr_info("            deq_idx: CQM Dequeue port index\n");
	pr_info("		     -1 to get all Queues under all DQ port\n");
}

static int get_arbi(struct dp_node_child *c, enum dp_arbitate *p)
{
	struct dp_node_prio prio = {0};
	int i;

	if (!c)
		return DP_FAILURE;
	if (c->id.sch_id == DP_NODE_AUTO_ID)
		return DP_SUCCESS;
	if (dp_children_get(c, 0)) {
		pr_err("DPM: %s: fail to get children of node %d\n",
		       __func__, c->id.sch_id);
		return DP_FAILURE;
	}
	if (!c->num)
		return DP_SUCCESS;
	for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
		if (c->child[i].type)
			goto found;
	}
	pr_err("DPM: %s: no valid children found for node %d\n",
	       __func__, c->id.sch_id);
	return DP_FAILURE;
found:
	prio.inst = c->inst;
	prio.type = c->child[i].type;
	prio.id = c->child[i].id;
	if (dp_qos_link_prio_get(&prio, 0)) {
		pr_err("DPM: %s: fail to get arbi of node %d via child %d\n",
		       __func__, c->id.sch_id, prio.id.sch_id);
		return DP_FAILURE;
	}
	if (p)
		*p = prio.arbi;
	return DP_SUCCESS;
}

static int get_prio(int inst, int id, enum dp_node_type type, int *p)
{
	struct dp_node_prio prio = {0};

	if (id == DP_NODE_AUTO_ID)
		return DP_SUCCESS;
	prio.inst = inst;
	prio.type = type;
	prio.id.sch_id = id;
	if (dp_qos_link_prio_get(&prio, 0)) {
		pr_err("DPM: %s: fail to get priority of node %d\n", __func__, id);
		return DP_FAILURE;
	}
	if (p)
		*p = prio.prio_wfq;
	return DP_SUCCESS;
}
static int add_qos_link(struct dp_qos_link *cfg, int flag)
{
	struct dp_node_child *node;
	struct dp_sch_lvl *s;
	int i, res;

	node = dp_kzalloc(sizeof(*node), GFP_ATOMIC);
	if (!node)
		return DP_FAILURE;
	node->inst = cfg->inst;
	node->type = DP_NODE_PORT;
	node->id.cqm_deq_port = cfg->cqm_deq_port;

	for (i = cfg->n_sch_lvl - 1; i >= 0; i--) {
		s = &cfg->sch[i];
		if (get_arbi(node, &s->arbi)) {
			kfree(node);
			return DP_FAILURE;
		}
		if (get_prio(cfg->inst, s->id, DP_NODE_SCH, &s->prio_wfq)) {
			kfree(node);
			return DP_FAILURE;
		}
		node->type = DP_NODE_SCH;
		node->id.sch_id = s->id;
	}
	if (get_arbi(node, &cfg->q_arbi)) {
		kfree(node);
		return DP_FAILURE;
	}
	if (get_prio(cfg->inst, cfg->q_id, DP_NODE_QUEUE, &cfg->q_prio_wfq)) {
		kfree(node);
		return DP_FAILURE;
	}
	res = dp_link_add(cfg, flag);
	kfree(node);
	return res;
}

static void print_queue_conf(struct dp_queue_conf *q_conf)
{

	if (q_conf->act & DP_NODE_EN)
		pr_info("Q(%d) action is ENABLED:[%d]\n",
			q_conf->q_id, q_conf->act);
	else
		pr_info("Q(%d) action is BLOCKED:[%d]\n",
			q_conf->q_id, q_conf->act);
	if (q_conf->drop == DP_QUEUE_DROP_WRED) {
		pr_info("Q(%d) is in WRED MODE:[%d]\n",
			q_conf->q_id, q_conf->drop);
	} else {
		pr_info("Q(%d) is in DROP TAIL MODE:[%d]\n",
			q_conf->q_id, q_conf->drop);
	}
	pr_info("Q(%d) is green_min_size:[%d]\n",
		q_conf->q_id, q_conf->min_size[0]);
	pr_info("Q(%d) is green_max_size:[%d]\n",
		q_conf->q_id, q_conf->max_size[0]);
	pr_info("Q(%d) is yellow_min_size:[%d]\n",
		q_conf->q_id, q_conf->min_size[1]);
	pr_info("Q(%d) is yellow_max_size:[%d]\n",
		q_conf->q_id, q_conf->max_size[1]);
	pr_info("Q(%d) is max_allowed:[%d]\n",
		q_conf->q_id, q_conf->wred_max_allowed);
	pr_info("Q(%d) is min_gauranteed:[%d]\n",
		q_conf->q_id, q_conf->wred_min_guaranteed);
	pr_info("Q(%d) is green_wred_slope:[%d]\n",
		q_conf->q_id, q_conf->wred_slope[0]);
	pr_info("Q(%d) is yellow_wred_slope:[%d]\n",
		q_conf->q_id, q_conf->wred_slope[1]);
}

ssize_t proc_qos_write(struct file *file, const char __user *buf,
		       size_t count, loff_t *ppos)
{
	struct {
		char str[200];
		char *param_list[25];
		struct dp_qos_link link_cfg;
		char *temp_list[2];
		struct dp_node_alloc node_alloc;
		struct dp_node_link node_link;
		struct dp_shaper_conf shaper_cfg;
		struct dp_queue_map_set qmap_set;
		struct dp_node_child node_child;
		struct dp_qos_link link;
		struct dp_queue_conf q_conf;
		struct dp_node_prio node_prio;
		struct dp_node_link_enable en_node;
		struct dp_qos_level level;
	} *l;
	int num;
	size_t len;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;
	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return count;

	len = (sizeof(l->str) > count) ? count : sizeof(l->str) - 1;
	len -= dp_copy_from_user(l->str, buf, len);
	l->str[len] = 0;

	num = dp_split_buffer(l->str, l->param_list, ARRAY_SIZE(l->param_list));

	if (num <= 1 || (dp_strncmpi(l->param_list[0], "help",
				     strlen("help")) == 0)) {
		qos_create_qos_help();
	} else if (dp_strncmpi(l->param_list[0], "add_q", strlen("add_q")) == 0) {
		int i;

		if (num < 4 || num - 4 > DP_MAX_SCH_LVL) {
			pr_info("Wrong Parameter(try help):echo help > %s\n",
				DEBUGFS_QOS);
			goto exit;
		}

		l->link_cfg.q_id = dp_atoi(l->param_list[1]);
		l->link_cfg.dp_port = dp_atoi(l->param_list[num - 1]);
		l->link_cfg.cqm_deq_port = dp_atoi(l->param_list[num - 2]);
		l->link_cfg.q_leaf = 0;
		l->link_cfg.q_arbi = ARBITRATION_WSP; /* default WSP */
		l->link_cfg.q_prio_wfq = 0;
		l->link_cfg.n_sch_lvl = num - 4;

		for (i = 0; i < l->link_cfg.n_sch_lvl; i++) {
			dp_replace_ch(l->param_list[i + 2],
				      strlen(l->param_list[i + 2]), ':', ' ');
			dp_split_buffer(l->param_list[i + 2], l->temp_list,
					ARRAY_SIZE(l->temp_list));
			l->link_cfg.sch[i].id = dp_atoi(l->temp_list[0]);
			l->link_cfg.sch[i].leaf = dp_atoi(l->temp_list[1]);
			/* Later get child arbi & update accordingly */
			l->link_cfg.sch[i].arbi = ARBITRATION_WSP;
			l->link_cfg.sch[i].prio_wfq = 0;
		}
		if (add_qos_link(&l->link_cfg, 0)) {
			pr_err("DPM: fail to add qos link\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "del_q", strlen("del_q")) == 0) {

		l->node_alloc.id.q_id = dp_atoi(l->param_list[1]);
		l->node_alloc.type = DP_NODE_QUEUE;

		if (dp_node_free(&l->node_alloc, DP_NODE_SMART_FREE)) {
			pr_err("DPM: dp_node_free failed\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "del_sch",
			       strlen("del_sch")) == 0) {
		int res;

		l->node_link.node_id.sch_id = dp_atoi(l->param_list[1]);
		l->node_link.node_type = DP_NODE_SCH;
		l->node_alloc.id.q_id = dp_atoi(l->param_list[1]);
		l->node_alloc.type = DP_NODE_SCH;

		if (dp_node_link_get(&l->node_link, 0))
			pr_err("DPM: dp_node_link_get failed\n");

		if (dp_node_free(&l->node_alloc, 0)) {
			pr_err("DPM: dp_node_free failed\n");
			goto exit;
		}

		while (1) {
			l->node_link.node_id = l->node_link.p_node_id;
			l->node_link.node_type = l->node_link.p_node_type;
			l->node_alloc.id = l->node_link.p_node_id;
			l->node_alloc.type = l->node_link.p_node_type;

			res = dp_node_link_get(&l->node_link, 0);

			if (dp_node_free(&l->node_alloc, 0))
				pr_err("DPM: dp_node_free failed\n");

			if (res) {
				pr_err("DPM: dp_node_link_get failed\n");
				break;
			}
		}
		pr_info("\nSched %d deleted\n\n", l->node_link.node_id.sch_id);
	} else if (dp_strncmpi(l->param_list[0], "prio", strlen("prio")) == 0) {

		if (num < 3) {
			pr_err("DPM: id, type are required!%s%s\n",
			       "\n(try help):echo help > ", DEBUGFS_QOS);
			goto exit;
		}

		if (dp_strncmpi(l->param_list[2], "queue", strlen("queue")) == 0) {
			l->node_prio.type = DP_NODE_QUEUE;
			l->node_prio.id.q_id = dp_atoi(l->param_list[1]);
		} else if (dp_strncmpi(l->param_list[2], "sched",
				       strlen("sched")) == 0) {
			l->node_prio.type = DP_NODE_SCH;
			l->node_prio.id.sch_id = dp_atoi(l->param_list[1]);
		} else if (dp_strncmpi(l->param_list[2], "port",
				       strlen("port")) == 0) {
			l->node_prio.type = DP_NODE_PORT;
			l->node_prio.id.cqm_deq_port = dp_atoi(l->param_list[1]);
		} else {
			pr_err("DPM: unknown type %s\n", l->param_list[2]);
		}

		if (dp_qos_link_prio_get(&l->node_prio, 0))
			pr_err("DPM: dp_qos_link_prio_get failed\n");

		if (dp_strncmpi(l->param_list[3], "null", strlen("null")) == 0) {
			l->node_prio.arbi = ARBITRATION_NULL;
		} else if (dp_strncmpi(l->param_list[3], "sp",
				       strlen("sp")) == 0) {
			l->node_prio.arbi = ARBITRATION_SP;
		} else if (dp_strncmpi(l->param_list[3], "wsp",
				       strlen("wsp")) == 0) {
			l->node_prio.arbi = ARBITRATION_WSP;
		} else if (dp_strncmpi(l->param_list[3], "wrr",
				       strlen("wrr")) == 0) {
			l->node_prio.arbi = ARBITRATION_WRR;
		} else if (dp_strncmpi(l->param_list[3], "wsp_wrr",
				       strlen("wsp_wrr")) == 0) {
			l->node_prio.arbi = ARBITRATION_WSP_WRR;
		} else if (dp_strncmpi(l->param_list[3], "wfq",
				       strlen("wfq")) == 0) {
			l->node_prio.arbi = ARBITRATION_WFQ;
		} else {
			pr_err("DPM: unknown type %s\n", l->param_list[3]);
			goto exit;
		}

		l->node_prio.prio_wfq = dp_atoi(l->param_list[4]);
		if (dp_qos_link_prio_set(&l->node_prio, 0)) {
			pr_err("DPM: dp_qos_link_prio_set failed\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "shaper",
			       strlen("shaper")) == 0) {

		if (num < 4) {
			pr_err("DPM: cmd, id, type are required!%s%s\n",
			       "\n(try help):echo help > ", DEBUGFS_QOS);
			goto exit;
		}

		if (dp_strncmpi(l->param_list[1], "add", strlen("add")) == 0)
			l->shaper_cfg.cmd = DP_SHAPER_CMD_ADD;
		else if (dp_strncmpi(l->param_list[1], "remove",
				     strlen("remove")) == 0)
			l->shaper_cfg.cmd = DP_SHAPER_CMD_REMOVE;
		else if (dp_strncmpi(l->param_list[1], "disable",
				     strlen("disable")) == 0)
			l->shaper_cfg.cmd = DP_SHAPER_CMD_DISABLE;
		else
			pr_err("DPM: unknown cmd try: echo help > %s", DEBUGFS_QOS);

		if (dp_strncmpi(l->param_list[3], "queue", strlen("queue")) == 0) {
			l->shaper_cfg.type = DP_NODE_QUEUE;
			l->shaper_cfg.id.sch_id = dp_atoi(l->param_list[2]);
		} else if (dp_strncmpi(l->param_list[3], "sched",
				       strlen("sched")) == 0) {
			l->shaper_cfg.type = DP_NODE_SCH;
			l->shaper_cfg.id.sch_id = dp_atoi(l->param_list[2]);
		} else if (dp_strncmpi(l->param_list[3], "port",
				       strlen("port")) == 0) {
			l->shaper_cfg.type = DP_NODE_PORT;
			l->shaper_cfg.id.sch_id = dp_atoi(l->param_list[2]);
		} else {
			pr_err("DPM: unknown type %s\n", l->param_list[3]);
			goto exit;
		}

		if (dp_shaper_conf_get(&l->shaper_cfg, 0))
			pr_err("DPM: dp_shaper_conf_get failed\n");

		if (dp_strncmpi(l->param_list[4], "no_limit",
				strlen("no_limit")) == 0)
			l->shaper_cfg.cir = DP_NO_SHAPER_LIMIT;
		else if (dp_strncmpi(l->param_list[4], "max", strlen("max")) == 0)
			l->shaper_cfg.cir = DP_MAX_SHAPER_LIMIT;
		else
			l->shaper_cfg.cir = dp_atoi(l->param_list[4]);

		if (dp_atoi(l->param_list[5]))
			l->shaper_cfg.pir = dp_atoi(l->param_list[5]);
		if (dp_atoi(l->param_list[6]))
			l->shaper_cfg.cbs = dp_atoi(l->param_list[6]);
		if (dp_atoi(l->param_list[7]))
			l->shaper_cfg.pbs = dp_atoi(l->param_list[7]);

		if (dp_shaper_conf_set(&l->shaper_cfg, 0)) {
			pr_err("DPM: dp_shaper_conf_set failed\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "set_node",
			       strlen("set_node")) == 0) {

		if (num < 4 || num > 5) {
			pr_err("DPM: id, type, cmd are required!%s%s\n",
			       "\n(try help):echo help > ", DEBUGFS_QOS);
			goto exit;
		}

		if (dp_strncmpi(l->param_list[2], "queue", strlen("queue")) == 0) {
			l->en_node.type = DP_NODE_QUEUE;
			l->en_node.id.q_id = dp_atoi(l->param_list[1]);
		} else if (dp_strncmpi(l->param_list[2], "sched",
				       strlen("sched")) == 0) {
			l->en_node.type = DP_NODE_SCH;
			l->en_node.id.sch_id = dp_atoi(l->param_list[1]);
		} else if (dp_strncmpi(l->param_list[2], "port",
				       strlen("port")) == 0) {
			l->en_node.type = DP_NODE_PORT;
			l->en_node.id.cqm_deq_port = dp_atoi(l->param_list[1]);
		} else {
			pr_err("DPM: Incorrect parameter!%s%s%s\n", l->param_list[2],
			       "\n(try help):echo help > ", DEBUGFS_QOS);
			goto exit;
		}

		if (dp_strncmpi(l->param_list[3], "enable",
				strlen("enable")) == 0) {
			l->en_node.en |= DP_NODE_EN;
		} else if (dp_strncmpi(l->param_list[3], "disable",
				       strlen("disable")) == 0) {
			l->en_node.en |= DP_NODE_DIS;
		} else if (dp_strncmpi(l->param_list[3], "suspend",
				       strlen("suspend")) == 0) {
			l->en_node.en |= DP_NODE_SUSPEND;
		} else if (dp_strncmpi(l->param_list[3], "resume",
				       strlen("resume")) == 0) {
			l->en_node.en |= DP_NODE_RESUME;
		} else {
			pr_err("DPM: Incorrect parameter!%s%s%s\n", l->param_list[3],
			       "\n(try help):echo help > ", DEBUGFS_QOS);
			goto exit;
		}

		if (num == 5) {
			if (dp_strncmpi(l->param_list[4], "enable",
					strlen("enable")) == 0) {
				l->en_node.en |= DP_NODE_EN;
			} else if (dp_strncmpi(l->param_list[4], "disable",
					       strlen("disable")) == 0) {
				l->en_node.en |= DP_NODE_DIS;
			} else if (dp_strncmpi(l->param_list[4], "suspend",
					       strlen("suspend")) == 0) {
				l->en_node.en |= DP_NODE_SUSPEND;
			} else if (dp_strncmpi(l->param_list[4], "resume",
					       strlen("resume")) == 0) {
				l->en_node.en |= DP_NODE_RESUME;
			} else {
				pr_err("DPM: Incorrect parameter!%s%s%s\n",
				       l->param_list[4],
				       "\n(try help):echo help > ",
				       DEBUGFS_QOS);
				goto exit;
			}
		}

		if (dp_node_link_en_set(&l->en_node, 0)) {
			pr_err("DPM: dp_node_link_en_set failed\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "qmap_set",
			       strlen("qmap_set")) == 0) {

		l->qmap_set.q_id = dp_atoi(l->param_list[1]);

		if (num < 10) {
			pr_info("Wrong Parameter(try help):%s%s\n",
				"echo help > ", DEBUGFS_QOS);
			goto exit;
		}
		l->qmap_set.map.mpe1 = dp_atoi(l->param_list[2]);
		l->qmap_set.map.mpe2 = dp_atoi(l->param_list[3]);
		l->qmap_set.map.dp_port = dp_atoi(l->param_list[4]);
		l->qmap_set.map.flowid = dp_atoi(l->param_list[5]);
		l->qmap_set.map.dec = dp_atoi(l->param_list[6]);
		l->qmap_set.map.enc = dp_atoi(l->param_list[7]);
		l->qmap_set.map.class = dp_atoi(l->param_list[8]);
		l->qmap_set.map.subif = dp_atoi(l->param_list[9]);
		if (dp_queue_map_set(&l->qmap_set, 0)) {
			pr_err("DPM: dp_queue_map_set failed\n");
			goto exit;
		}
	} else if (dp_strncmpi(l->param_list[0], "get_child",
			       strlen("get_child")) == 0) {
				int idx = 0;

		if (dp_strncmpi(l->param_list[2], "sched", strlen("sched")) == 0) {
			l->node_child.type = DP_NODE_SCH;
			l->node_child.id.sch_id = dp_atoi(l->param_list[1]);
		} else if (dp_strncmpi(l->param_list[2], "port",
				       strlen("port")) == 0) {
			l->node_child.type = DP_NODE_PORT;
			l->node_child.id.cqm_deq_port = dp_atoi(l->param_list[1]);
		} else {
			pr_err("DPM: unknown type %s\n", l->param_list[2]);
		}

		if (dp_children_get(&l->node_child, 0)) {
			pr_err("DPM: dp_children_get failed\n");
			goto exit;
		}
		if (l->node_child.num)
			pr_info("Node[%d] has {%d} Children!!\n",
				l->node_child.id.q_id, l->node_child.num);
		for (idx = 0; idx < PROC_DP_MAX_LEAF; idx++) {
			if (l->node_child.child[idx].id.q_id) {
				if (l->node_child.child[idx].type == DP_NODE_SCH)
					pr_info("Child:[%d] is Sched:[%d]\n",
						idx, l->node_child.child[idx].id.q_id);
				else if (l->node_child.child[idx].type == DP_NODE_QUEUE)
					pr_info("Child:[%d] is Q:[%d]\n",
						idx, l->node_child.child[idx].id.q_id);
				else
					pr_info("Child:[%d] is FREE\n", idx);
			}
		}
	} else if (dp_strncmpi(l->param_list[0], "q_link",
			       strlen("q_link")) == 0) {
		int i = 0;

		l->link.q_id = dp_atoi(l->param_list[1]);

		if (dp_link_get(&l->link, 0)) {
			pr_err("DPM: dp_link_get failed\n");
			goto exit;
		}
		if (!l->link.n_sch_lvl) {
			pr_info("Q[%d](arbi:%d|prio:%d)->PORT[%d]\n",
				l->link.q_id, l->link.q_arbi, l->link.q_prio_wfq,
				l->link.cqm_deq_port);
		} else {
			pr_info("Q[%d](arbi:%d|prio:%d)\n",
				l->link.q_id, l->link.q_arbi,
				l->link.q_prio_wfq);
			for (i = 0; i < l->link.n_sch_lvl; i++) {
				pr_info("%s(%d):SCH[%d](arbi:%d|prio:%d)\n",
					"Parent level", i, l->link.sch[i].id,
					l->link.sch[i].arbi,
					l->link.sch[i].prio_wfq);
			}
			pr_info("Parent level(%d):PORT[%d] <Final Parent>\n",
				i, l->link.cqm_deq_port);
		}
	} else if (dp_strncmpi(l->param_list[0], "get_level",
			       strlen("get_level")) == 0) {

		l->level.inst = 0;

		if (dp_qos_level_get(&l->level, 0)) {
			pr_err("DPM: dp_qos_level_get failed\n");
			goto exit;
		}
		if (l->level.max_sch_lvl)
			pr_info("Q->SCH(%d)->PORT, level:[%d]\n",
				l->level.max_sch_lvl, l->level.max_sch_lvl);
		else
			pr_info("Q->PORT, level:[%d]\n", l->level.max_sch_lvl);
	} else if (dp_strncmpi(l->param_list[0], "l->q_conf",
			       strlen("l->q_conf")) == 0) {
		l->q_conf.inst = 0;
		l->q_conf.q_id = dp_atoi(l->param_list[1]);

		if (dp_queue_conf_get(&l->q_conf, 0)) {
			pr_err("DPM: dp_queue_conf_get failed\n");
			goto exit;
		}

		print_queue_conf(&l->q_conf);
	} else if (dp_strncmpi(l->param_list[0], "queue_res_get",
			       strlen("queue_res_get") + 1) == 0) {

		int dp_port = dp_atoi(l->param_list[1]);
		int cqm_deq_idx = dp_atoi(l->param_list[2]);

		if (deq_port_res_get(0, dp_port, cqm_deq_idx)) {
			pr_err("DPM: deq_res_get failed\n");
			goto exit;
		}

	} else if (dp_strncmpi(l->param_list[0], "set_q_conf",
			       strlen("set_q_conf")) == 0) {
		int i;

		l->q_conf.inst = 0;
		l->q_conf.q_id = dp_atoi(l->param_list[1]);
		if (dp_queue_conf_get(&l->q_conf, 0)) {
			pr_err("DPM: dp_queue_conf_get failed\n");
			goto exit;
		}

		i = 2; /* skip param 0 and param 1*/
		while (i < num) {
			if (dp_strncmpi(l->param_list[i], "action",
					strlen("action")) == 0) {
				l->q_conf.act = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "drop",
					strlen("drop")) == 0) {
				l->q_conf.drop = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "green_min_size",
					strlen("green_min_size")) == 0) {
				l->q_conf.min_size[DP_COLOR_GREEN] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "green_max_size",
					strlen("green_max_size")) == 0) {
				l->q_conf.max_size[DP_COLOR_GREEN] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "green_slope",
					strlen("green_slope")) == 0) {
				l->q_conf.wred_slope[DP_COLOR_GREEN] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "yellow_min_size",
					strlen("yellow_min_size")) == 0) {
				l->q_conf.min_size[DP_COLOR_YELLOW] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "yellow_max_size",
					strlen("yellow_max_size")) == 0) {
				l->q_conf.max_size[DP_COLOR_YELLOW] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "yellow_slope",
					strlen("yellow_slope")) == 0) {
				l->q_conf.wred_slope[DP_COLOR_YELLOW] = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "min_guaranteed",
					strlen("min_guaranteed")) == 0) {
				l->q_conf.wred_min_guaranteed = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "max_allowed",
					strlen("max_allowed")) == 0) {
				l->q_conf.wred_max_allowed = dp_atoi(l->param_list[++i]);
			} else if (dp_strncmpi(l->param_list[i], "codel",
					strlen("codel")) == 0) {
				l->q_conf.codel = dp_atoi(l->param_list[++i]);
			} else {
				pr_info("Wrong Parameter [%s]:\n", l->param_list[i]);
				qos_create_qos_help();
				goto exit;
			}
			i++;
		}

		if (dp_queue_conf_set(&l->q_conf, 0)) {
			pr_err("DPM: dp_queue_conf_get failed\n");
			goto exit;
		}
	} else {
		pr_info("Wrong Parameter:\n");
		qos_create_qos_help();
	}
exit:
	kfree(l);
	return count;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
#endif
