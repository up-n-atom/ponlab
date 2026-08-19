/*
 * ebtable priority target can be used in any chain
 */

#include <linux/proc_fs.h>
#include <net/dsfield.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_prio.h>

#define XT_DSCP_MASK    0xfc	/* 11111100 */
#define XT_DSCP_SHIFT   2
#define XT_DSCP_MAX     0x3f	/* 00111111 */

#define OPT_SET_PRIO    0x01
#define OPT_DSCP_PRIO   0x02
#define OPT_PCP_PRIO    0x04
#define OPT_PRIO_PCP    0x08

#define MAX_DSCP	64
#define MAX_PRIORITY    8
#define MAX_STR	 512

struct ebt_prio_tables {
	unsigned char dscp2prio[MAX_DSCP];
	unsigned char pcp2prio[MAX_PRIORITY];
	unsigned char prio2pcp[MAX_PRIORITY];
};

struct ebt_prio_tables tables;
const unsigned char def_dscp_prio[MAX_DSCP] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 2, 3, 2, 3, 2, 3, 2, 4, 3, 4, 3, 4, 3, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 5, 6, 5, 7, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7 };
const unsigned char def_pcp_prio[MAX_PRIORITY] = {0, 1, 2, 3, 4, 5, 6, 7};
const unsigned char def_prio_pcp[MAX_PRIORITY] = {0, 1, 2, 3, 4, 5, 6, 7};

/**********************************************************************************************/
/*! \fn ssize_t read_proc_dscp_prio(struct file *filp, char __user *buffer,
 *  size_t length,loff_t *offset)
 **********************************************************************************************
 *  \brief  dscp-prio-table procfs read handler.
 *  \return count of chars, if error <0
 *********************************************************************************************/
static ssize_t read_proc_dscp_prio(struct file *filp, char __user *buffer,
				   size_t length, loff_t *offset)
{
	int i;
	char str_buffer[MAX_STR];

	memset(str_buffer, 0, sizeof(str_buffer));
	for (i = 0; i < sizeof(tables.dscp2prio); i++)
		sprintf(str_buffer + strlen(str_buffer), "%d ", tables.dscp2prio[i]);

	strcat(str_buffer, "\n");
	return simple_read_from_buffer(buffer, length, offset, str_buffer, strlen(str_buffer));
}

/**********************************************************************************************/
/*! \fn ssize_t read_proc_pcp_prio(struct file *filp, char __user *buffer,
 * size_t length,loff_t *offset)
 **********************************************************************************************
 *  \brief  pcp-prio-table procfs read handler.
 *  \return count of chars, if error <0
 **********************************************************************************************/
static ssize_t read_proc_pcp_prio(struct file *filp, char __user *buffer,
				  size_t length, loff_t *offset)
{
	int i;
	char str_buffer[MAX_STR];

	memset(str_buffer, 0, sizeof(str_buffer));
	for (i = 0; i < sizeof(tables.pcp2prio); i++)
		sprintf(str_buffer + strlen(str_buffer), "%d ", tables.pcp2prio[i]);

	strcat(str_buffer, "\n");
	return simple_read_from_buffer(buffer, length, offset, str_buffer, strlen(str_buffer));
}

/**********************************************************************************************/
/*! \fn ssize_t read_proc_prio_pcp(struct file *filp, char __user *buffer,
 * size_t length,loff_t *offset)
 **********************************************************************************************
 *  \brief  prio-pcp-table procfs read handler.
 *  \return count of chars, if error <0
 **********************************************************************************************/
static ssize_t read_proc_prio_pcp(struct file *filp, char __user *buffer,
				  size_t length, loff_t *offset)
{
	int i;
	char str_buffer[MAX_STR];

	memset(str_buffer, 0, sizeof(str_buffer));
	for (i = 0; i < sizeof(tables.prio2pcp); i++)
		sprintf(str_buffer + strlen(str_buffer), "%d ", tables.prio2pcp[i]);

	strcat(str_buffer, "\n");
	return simple_read_from_buffer(buffer, length, offset, str_buffer, strlen(str_buffer));
}

/**********************************************************************************************/
/*! \fn int parse(char *buf, unsigned char max, unsigned char *arr, char *custom,
 * const unsigned char *def_arr)
 **********************************************************************************************
 *  \brief  parse command.
 *  \param [in] buf - string with command
 *  \param [in] max - max index in table
 *  \param [in, out] arr - work table
 *  \param [in] custom - custom command: dscp, prio or pcp
 *  \param [in] def_arr - table with default values
 *  \return 0, if error -EINVAL
 **********************************************************************************************/
int parse(char *buf, unsigned char max, unsigned char *arr, char *custom,
	  const unsigned char *def_arr)
{
	unsigned char i, value;
	char *token, *options;

	if (strncmp(buf, "table ", strlen("table ")) == 0) {
		/*
		 * parse "table" command, update whole table with a new priority values
		 *
		 * format "table N_1 .. N_max"
		 * e.g. "table  0 1 2 3 ...."
		 * N_i in 0 .. MAX_PRIORITY range, return -EINVAL on error
		 */
		i = 0;
		options = buf + 6;
		while ((token = strsep(&options, " ")) != NULL) {
			if (kstrtou8(token, 0, &value))
				return -EINVAL;
			if (value >= 0 && value < MAX_PRIORITY && i < max)
				arr[i] = value;
			else
				return -EINVAL;
			i++;
		}
		if (i != max)
			return -EINVAL;
	} else if (strncmp(buf, custom, strlen(custom)) == 0) {
		/*
		 * parse "dscp"; "pcp"; "prio" commands, update priority at specified index in
		 * the mapping table
		 * e.g. echo "dscp  2 3" > /proc/dscp-prio-table
		 * e.g. echo "pcp  4 0" > /proc/pcp-prio-table
		 * e.g. echo "prio  6 1" > /proc/prio-pcp-table
		 * format "< command > index priority value" value in 0 .. MAX_PRIORITY range
		 * return -EINVAL on error
		 */
		options = buf + strlen(custom);
		if ((token = strsep(&options, " ")) == NULL)
			return -EINVAL;
		if (kstrtou8(token, 0, &i))
			return -EINVAL;
		if (token && i >= 0 && i < max) {
			if ((token = strsep(&options, " ")) == NULL)
				return -EINVAL;
			if (kstrtou8(token, 0, &value))
				return -EINVAL;
			if (value >= 0 && value < MAX_PRIORITY)
				arr[i] = value;
			else
				return -EINVAL;
		} else
			return -EINVAL;
	} else if (strncmp(buf, "default", strlen("default")) == 0) {
		/* "default" commands, restore default mapping */
		for (i = 0; i < max; i++)
			arr[i] = def_arr[i];
	} else
		return -EINVAL;
	return 0;
}

/**********************************************************************************************/
/*! \fn ssize_t copy_from_user_to_buf(char *out_buf, const char __user *in_buf,
 * size_t out_buf_size, size_t bytes_to_read)
 **********************************************************************************************
 *  \brief copy command from user space.
 *  \param [in, out] out_buf - pointer to local buffer
 *  \param [in] in_buf - pointer to incoming buffer
 *  \param [in] out_buf_size - maximum size of output buffer
 *  \param [in] bytes_to_read - size of incoming buffer
 *  \return 0, if error -EINVAL
 **********************************************************************************************/
static ssize_t copy_from_user_to_buf(char *out_buf, const char __user *in_buf,
				     size_t out_buf_size, size_t bytes_to_read)
{
	unsigned long bytes_not_copied = 0;

	/* +1 for terminating null byte */
	if ((bytes_to_read + 1) > out_buf_size) {
		printk(KERN_ERR "Buffer overflow. Size of the buffer: %zu (1 byte for terminating null byte), length of the data: %zu\n",
		       out_buf_size, bytes_to_read);
		return -EFAULT;
	}

	bytes_not_copied = (unsigned long)copy_from_user(out_buf, in_buf, bytes_to_read);

	if (bytes_not_copied) {
		printk(KERN_ERR "copy_from_user failed; returned value indicating number of bytes not copied: %lu, length of the data: %zu\n",
		       bytes_not_copied, bytes_to_read);
		return -EFAULT;
	}

	out_buf[bytes_to_read] = '\0';

	return 0;
}

/************************************************************************************************/
/*! \fn ssize_t write_proc_dscp_prio(struct file *filp, const char *buff, size_t len, loff_t *off)
 ************************************************************************************************
 *  \brief  dscp-prio-table procfs write handler.
 *  \return count of chars, if error <0
 ************************************************************************************************/
static ssize_t write_proc_dscp_prio(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	unsigned char options[MAX_DSCP];
	char str_buffer[MAX_STR];

	if (copy_from_user_to_buf(str_buffer, buff, sizeof(str_buffer), len))
		return -EFAULT;
	else {
		/* Copy current DSCP to prio mapping to temporary table  */
		memcpy(options, tables.dscp2prio, sizeof(def_dscp_prio));
		if (parse(str_buffer, sizeof(def_dscp_prio), options, "dscp ", def_dscp_prio))
			return -EINVAL;
		else {
			/* Update DSCP to prio mapping table */
			memcpy(tables.dscp2prio, options, sizeof(def_dscp_prio));
		}
	}
	return len;
}

/***********************************************************************************************/
/*! \fn ssize_t write_proc_pcp_prio(struct file *filp, const char *buff, size_t len, loff_t *off)
 ***********************************************************************************************
 *  \brief  pcp-prio-table procfs write handler.
 *  \return count of chars, if error <0
 ***********************************************************************************************/
static ssize_t write_proc_pcp_prio(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	unsigned char options[MAX_PRIORITY];
	char str_buffer[MAX_STR];

	if (copy_from_user_to_buf(str_buffer, buff, sizeof(str_buffer), len))
		return -EFAULT;
	else {
		/* Copy current PCP to prio mapping to temporary table  */
		memcpy(options, tables.pcp2prio, sizeof(def_pcp_prio));
		if (parse(str_buffer, sizeof(def_pcp_prio), options, "pcp ", def_pcp_prio))
			return -EINVAL;
		else {
			/* Update PCP to prio mapping table */
			memcpy(tables.pcp2prio, options, sizeof(def_pcp_prio));
		}
	}
	return len;
}

/***********************************************************************************************/
/*! \fn ssize_t write_proc_prio_pcp(struct file *filp, const char *buff, size_t len, loff_t *off)
 ************************************************************************************************
 *  \brief  prio-pcp-table procfs write handler.
 *  \return count of chars, if error <0
 ***********************************************************************************************/
static ssize_t write_proc_prio_pcp(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	unsigned char options[MAX_PRIORITY];
	char str_buffer[MAX_STR];

	if (copy_from_user_to_buf(str_buffer, buff, sizeof(str_buffer), len))
		return -EFAULT;
	else {
		/* Copy current prio to PCP mapping to temporary table  */
		memcpy(options, tables.prio2pcp, sizeof(def_prio_pcp));
		if (parse(str_buffer, sizeof(def_prio_pcp), options, "prio ", def_prio_pcp))
			return -EINVAL;
		else {
			/* Update prio to PCP mapping table */
			memcpy(tables.prio2pcp, options, sizeof(def_prio_pcp));
		}
	}
	return len;
}

static struct proc_ops proc_fops_dscp_prio = {
	.proc_read = read_proc_dscp_prio,
	.proc_write = write_proc_dscp_prio
};

static struct proc_ops proc_fops_pcp_prio = {
	.proc_read = read_proc_pcp_prio,
	.proc_write = write_proc_pcp_prio
};

static struct proc_ops proc_fops_prio_pcp = {
	.proc_read = read_proc_prio_pcp,
	.proc_write = write_proc_prio_pcp
};

/**************************************************************************************/
/*! \fn unsigned int ebt_prio_tg(struct sk_buff *skb, const struct xt_action_param *par)
 **************************************************************************************
 *  \brief  Main function apply rule to skb structure.
 *  \return EBT_ACCEPT, EBT_DROP, EBT_CONTINUE, EBT_RETURN
 **************************************************************************************/
static unsigned int ebt_prio_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct ebt_prio_info *info = par->targinfo;
	unsigned short TCI;
	unsigned char prio, need_prio;

	if (skb_ensure_writable(skb, 0))
		return EBT_DROP;
	switch (xt_hooknum(par)) {
	case NF_BR_PRE_ROUTING:
			if (info->bitmask & OPT_SET_PRIO) {
				skb->priority &= ~7;
				skb->priority |= info->default_prio;
				return EBT_CONTINUE;
			}
			if (info->bitmask & OPT_DSCP_PRIO) {
				const struct ethhdr *h = eth_hdr(skb);
				unsigned char dscp;

				if (h) {
					/* Check protocol IPv4 or IPv6 */
					if (h->h_proto == htons(ETH_P_IP)) {
						const struct iphdr *ih;
						struct iphdr _iph;

						ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
						if (ih) {
							/* Convert from field TOS to DSCP value */
							dscp = (ih->tos >> XT_DSCP_SHIFT) & 63;
							/* Set priority from dscp2prio table */
							skb->priority &= ~7;
							skb->priority |= tables.dscp2prio[dscp];
						}
					} else if (h->h_proto == htons(ETH_P_IPV6)) {
						const struct ipv6hdr *ih;
						struct ipv6hdr _iph;

						ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
						if (ih) {
							/* Convert from field priority to DSCP value  */
							dscp = (ih->priority << 2) | ((ih->flow_lbl[0] & 0xc0) >> 6);
							/* Set priority from dscp2prio table */
							skb->priority &= ~7;
							skb->priority |= tables.dscp2prio[dscp];
						}
					}
				}
			}
			if (info->bitmask & OPT_PCP_PRIO) {
				/* Check vlan tag in skb structure */
				if (skb_vlan_tag_present(skb)) {
					/* Get field TCI from decoded vlan tag in skb structure */
					TCI = skb_vlan_tag_get(skb);
				} else {
					/* Get field TCI from undecoded vlan tag in skb structure */
					struct vlan_hdr *fp;
					struct vlan_hdr _frame;

					fp = skb_header_pointer(skb, 0, sizeof(_frame), &_frame);
					if (fp == NULL)
						return info->target;
					TCI = ntohs(fp->h_vlan_TCI);
				}
				/* Extract vlan priority from field TCI */
				prio = (TCI >> 13) & 0x7;
				/* Set priority from pcp2prio table */
				skb->priority &= ~7;
				skb->priority |= tables.pcp2prio[prio];
			}
			break;
	case NF_BR_POST_ROUTING:
			if (info->bitmask & OPT_PRIO_PCP) {
				/* Convert skb->priority via prio2pcp table */
				need_prio = tables.prio2pcp[skb->priority & 7];
				/* Check vlan tag in skb structure */
				if (skb_vlan_tag_present(skb)) {
					/* Get field TCI from decoded vlan tag in skb structure */
					TCI = skb_vlan_tag_get(skb);
					/* Extract vlan priority from field TCI */
					prio = (TCI >> 13) & 0x7;
					/* Compare current priority with need from table */
					if (prio != (need_prio & 7)) {
						/* Set vlan priority (pcp) */
						skb->vlan_tci = (skb->vlan_tci & 0x1fff) | ((need_prio & 7) << 13) ;
					}
				} else {
					/* Get field TCI from undecoded vlan tag in skb structure */
					struct vlan_hdr *fp;
					struct vlan_hdr _frame;

					fp = skb_header_pointer(skb, 0, sizeof(_frame), &_frame);
					if (fp == NULL)
						return info->target;
					TCI = ntohs(fp->h_vlan_TCI);
					/* Extract vlan priority from field TCI */
					prio = (TCI >> 13) & 0x7;
					/* Compare current priority with need from table */
					if (prio != (need_prio & 7)) {
						/* Set vlan priority (pcp) */
						TCI &= 0x1fff;
						TCI |= ((need_prio & 7) << 13);
						fp->h_vlan_TCI = ntohs(TCI);
					}
				}
			}
			break;
	}
	return info->target;
}

/*************************************************************/
/*! \fn int ebt_prio_tg_check(const struct xt_tgchk_param *par)
 *************************************************************
 *  \brief  Check table and chains supported in extension.
 *  \return 0 if ok -EINVAL if error
 *************************************************************/
static int ebt_prio_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_prio_info *info = par->targinfo;
	unsigned int hook_mask;

	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;

	hook_mask = par->hook_mask & ~(1 << NF_BR_NUMHOOKS);
	if ((strncmp(par->table, "nat", sizeof("nat")) != 0 ||
		(hook_mask & ~((1 << NF_BR_PRE_ROUTING) | (1 << NF_BR_POST_ROUTING)))))
		return -EINVAL;
	if (ebt_invalid_target(info->target))
		return -EINVAL;
	return 0;
}

static struct xt_target ebt_prio_tg_reg __read_mostly = {
	.name		= "prio",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_PRE_ROUTING) | (1 << NF_BR_POST_ROUTING),
	.target		= ebt_prio_tg,
	.checkentry	= ebt_prio_tg_check,
	.targetsize	= sizeof(struct ebt_prio_info),
	.me		= THIS_MODULE,
};

/****************************************/
/*! \fn int ebt_prio_init(void)
 ****************************************
 *  \brief  ebtable prio initialization.
 *
 *  \return 0 if ok -1 if error
 ****************************************/
static int __init ebt_prio_init(void)
{
	/* Preset tables from default values */
	memcpy(tables.dscp2prio, def_dscp_prio, sizeof(def_dscp_prio));
	memcpy(tables.pcp2prio,  def_pcp_prio,  sizeof(def_pcp_prio));
	memcpy(tables.prio2pcp,  def_prio_pcp,  sizeof(def_prio_pcp));
	/* Register target */
	if (!xt_register_target(&ebt_prio_tg_reg)) {
		/* Creating Proc entry under "/proc/" */
		if (!proc_create("dscp-prio-table", 0644, NULL, &proc_fops_dscp_prio)) {
			xt_unregister_target(&ebt_prio_tg_reg);
			return (-1);
		}
		if (!proc_create("prio-pcp-table",  0644, NULL, &proc_fops_prio_pcp)) {
			remove_proc_entry("dscp-prio-table", NULL);
			xt_unregister_target(&ebt_prio_tg_reg);
			return (-1);
		}
		if (!proc_create("pcp-prio-table",  0644, NULL, &proc_fops_pcp_prio)) {
			remove_proc_entry("prio-pcp-table", NULL);
			remove_proc_entry("dscp-prio-table", NULL);
			xt_unregister_target(&ebt_prio_tg_reg);
			return (-1);
		}

		pr_info("Device Prio Driver Insert...Done!!!\n");
	}
	return 0;
}

/**********************************/
/*! \fn void ebt_prio_exit(void)
 **********************************
 *  \brief  ebtable prio exit.
 *
 *  \return VOID
 **********************************/
static void __exit ebt_prio_exit(void)
{
	/* Remove Procfs "/proc/" */
	remove_proc_entry("pcp-prio-table", NULL);
	remove_proc_entry("prio-pcp-table", NULL);
	remove_proc_entry("dscp-prio-table", NULL);
	/* Unregister target */
	xt_unregister_target(&ebt_prio_tg_reg);
}

module_init(ebt_prio_init);
module_exit(ebt_prio_exit);
MODULE_DESCRIPTION("Ebtables: Priority translation");
MODULE_LICENSE("GPL");
