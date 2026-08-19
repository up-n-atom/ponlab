/***********************************************************************
 *		Copyright (C) 2021-2022  MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 *******************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/random.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/stat.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/io.h>
#include "drv_icc.h"
#include "drv_mps.h"
#include "secure_services.h"
#include <linux/genalloc.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <linux/dma-direct.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MaxLinear Inc");
MODULE_DESCRIPTION("Secure Debug Driver");

struct mutex sec_dbg_client_mutex;
static wait_queue_head_t sec_dbg_wakeuplist;
static refcount_t sec_dbg_wakeup = REFCOUNT_INIT(0);

typedef struct sst_iccpool_info {
	struct device *icc_dev;
	struct gen_pool *icc_pool;
} iccpool_info_t;

static iccpool_info_t iccpool_inf;
static int dbgPort_unlock_st;

static int sec_dbg_send_icc_msg(icc_msg_t *secdbg_client_msg,
						icc_msg_t *secdbg_reply_msg,
						sec_dbg_config_t *secure_debug_config)
{
	dma_addr_t secdbg_dma_addr;
	int timeout;
	int ret = 0;

	secdbg_client_msg->src_client_id = SECURE_DEBUG;
	secdbg_client_msg->dst_client_id = SECURE_DEBUG;

	secdbg_dma_addr = dma_map_single_attrs(iccpool_inf.icc_dev, (void *)secure_debug_config,
				sizeof(sec_dbg_config_t), DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, secdbg_dma_addr)) {
		pr_err("<Secure Debug Client> Unable to map secure debug config\n");
		return -ENOMEM;
	}

	dma_sync_single_for_device(iccpool_inf.icc_dev, secdbg_dma_addr, sizeof(sec_dbg_config_t), DMA_TO_DEVICE);
	secdbg_client_msg->param[SS_ICC_PARAM_1] = (uint32_t)secdbg_dma_addr;
	secdbg_client_msg->param[SS_ICC_PARAM_2] = sizeof(sec_dbg_config_t);
#ifdef SDBG_DEBUG
	dump_ss_icc_msg(secdbg_client_msg);
#endif
	if (icc_write(SECURE_DEBUG, secdbg_client_msg) <= 0) {
		pr_err("Failed to write icc msg \r\n");
		ret = -ECONNREFUSED;
		goto out;
	}

	timeout = wait_event_interruptible_timeout(sec_dbg_wakeuplist,
				(refcount_read(&sec_dbg_wakeup) != 0), msecs_to_jiffies(SSS_ICC_REQ_TIMEOUT * HZ));
	if (timeout == 0) {
		pr_err("<Secure Debug Client> SECDBG request timeout\n");
		ret = -ETIMEDOUT;
		goto out;
	}

	if (icc_read(SECURE_DEBUG, secdbg_reply_msg) < 0) {
		pr_err("Failed to read icc msg \r\n");
		ret = -ETIMEDOUT;
		goto out;
	}
#ifdef SDBG_DEBUG
	dump_ss_icc_msg(secdbg_reply_msg);
#endif

out:
	refcount_set(&sec_dbg_wakeup, 0);
	dma_sync_single_for_cpu(iccpool_inf.icc_dev, secdbg_dma_addr, sizeof(sec_dbg_config_t), DMA_TO_DEVICE);
	return ret;
}

static int secure_debug_auth_start(sec_dbg_auth_start_t *sec_dbg_param,
						sec_dbg_config_t *secure_debug_config, bool isadmin)
{
	icc_msg_t *ret_msg = NULL, secdbg_client_msg;
	int ret = 0;
	void *nonce_buf = NULL;
	void *key_buf = NULL;
	dma_addr_t nonce_dma_addr;
	dma_addr_t key_dma_addr;

	memset(&secdbg_client_msg, 0, sizeof(icc_msg_t));

	if (sec_dbg_param->auth_key_len <= 0) {
		pr_err("Secure debug key length is not valid\n");
		ret = -EAGAIN;
		goto finish;
	}

	key_buf = (void *)gen_pool_alloc(iccpool_inf.icc_pool, sec_dbg_param->auth_key_len);
	if (key_buf == NULL) {
		pr_err("Memory Allocation for Auth Key failed\n");
		ret = -ENOMEM;
		goto finish;
	}

	if (copy_from_user
		(key_buf, sec_dbg_param->auth_key, sec_dbg_param->auth_key_len)) {
		pr_err("copy_from_user error\r\n");
		ret = -EAGAIN;
		goto finish;
	}

	key_dma_addr = dma_map_single_attrs(iccpool_inf.icc_dev, key_buf,
				sec_dbg_param->auth_key_len, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, key_dma_addr)) {
		pr_err("<Secure Debug Client> Unable to map Auth key\n");
		ret = -ENOMEM;
		goto finish;
	}

	dma_sync_single_for_device(iccpool_inf.icc_dev, key_dma_addr, sec_dbg_param->auth_key_len, DMA_TO_DEVICE);
	nonce_buf = (void *)gen_pool_alloc(iccpool_inf.icc_pool, sec_dbg_param->nonce_len);
	if (nonce_buf == NULL) {
		pr_err("Memory Allocation for nonce failed\n");
		ret = -ENOMEM;
		goto finish;
	}

	nonce_dma_addr = dma_map_single_attrs(iccpool_inf.icc_dev, nonce_buf,
				sec_dbg_param->nonce_len, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, nonce_dma_addr)) {
		pr_err("<Secure Debug Client> Unable to map Nonce DMA\n");
		ret = -ENOMEM;
		goto finish;
	}

	secdbg_client_msg.msg_id = SECURE_DEBUG_AUTH_START;
	secdbg_client_msg.param[SS_ICC_PARAM_0] = sec_dbg_param->port_num;
	secure_debug_config->nonce = (uint32_t)nonce_dma_addr;
	secure_debug_config->nonce_len = sec_dbg_param->nonce_len;
	secure_debug_config->u.auth_info.auth_key_addr = (uint32_t)key_dma_addr;
	secure_debug_config->u.auth_info.auth_key_len = sec_dbg_param->auth_key_len;
	secdbg_client_msg.icc_flags.priority = isadmin;

	ret_msg = kmalloc(sizeof(icc_msg_t), GFP_DMA);
	if (!ret_msg) {
		ret = -ENOMEM;
		goto finish;
	}

	ret = sec_dbg_send_icc_msg(&secdbg_client_msg, ret_msg, secure_debug_config);
	if (!ret) {
		/* TEE send the 0 in case of success while non-zero value for error
		 * cases. As error codes are generally -ve value, but TEE is currently
		 * sending the +ve value.
		 * So handled the error cases by sending(always) the -ve value to
		 * userspace irrespective of TEE (+ve or -ve) reply.
		 */
		ret = -abs(((char)ret_msg->param_attr));
		sec_dbg_param->port_num = ret_msg->param[SS_ICC_PARAM_0];
		sec_dbg_param->nonce_len = ret_msg->param[SS_ICC_PARAM_2];
		if (ret < 0)
			goto finish;
		dma_sync_single_for_cpu(iccpool_inf.icc_dev, nonce_dma_addr, sec_dbg_param->nonce_len, DMA_FROM_DEVICE);
		if (copy_to_user((void *)sec_dbg_param->nonce, nonce_buf, sec_dbg_param->nonce_len)) {
			pr_err("copy_to_user error\r\n");
			ret = -EFAULT;
			goto finish;
		}
	}

finish:
	if (ret_msg)
		kfree(ret_msg);

	if (nonce_buf) {
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)nonce_buf, sec_dbg_param->nonce_len);
	}

	if (key_buf) {
		dma_sync_single_for_device(iccpool_inf.icc_dev, key_dma_addr, sec_dbg_param->auth_key_len, DMA_TO_DEVICE);
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)key_buf, sec_dbg_param->auth_key_len);
	}

	return ret;
}

static int secure_debug_auth_verify(sec_dbg_auth_unlock_t *sec_dbg_param,
						sec_dbg_config_t *secure_debug_config, bool isadmin)
{
	icc_msg_t *ret_msg = NULL, secdbg_client_msg;
	int ret = 0;
	void *nonce_buf = NULL;
	void *sign_buf = NULL;
	dma_addr_t nonce_dma_addr;
	dma_addr_t sign_dma_addr;

	memset(&secdbg_client_msg, 0, sizeof(icc_msg_t));

	if (sec_dbg_param->sign_sz <= 0) {
		pr_err("Secure debug key signature length is not valid\n");
		ret = -EAGAIN;
		goto finish;
	}

	sign_buf = (void *)gen_pool_alloc(iccpool_inf.icc_pool, sec_dbg_param->sign_sz);
	if (sign_buf == NULL) {
		pr_err("Memory Allocation for Signature Auth failed\n");
		ret = -ENOMEM;
		goto finish;
	}

	if (copy_from_user
		(sign_buf, sec_dbg_param->p_signature, sec_dbg_param->sign_sz)) {
		pr_err("copy_from_user error\r\n");
		ret = -EAGAIN;
		goto finish;
	}

	sign_dma_addr = dma_map_single_attrs(iccpool_inf.icc_dev, sign_buf,
				sec_dbg_param->sign_sz, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, sign_dma_addr)) {
		pr_err("<Secure Debug Client> Unable to map signature\n");
		ret = -ENOMEM;
		goto finish;
	}

	dma_sync_single_for_device(iccpool_inf.icc_dev, sign_dma_addr, sec_dbg_param->sign_sz, DMA_TO_DEVICE);
	nonce_buf = (void *)gen_pool_alloc(iccpool_inf.icc_pool, sec_dbg_param->rd_nonce_len);
	if (nonce_buf == NULL) {
		pr_err("Memory Allocation for nonce failed\n");
		ret = -ENOMEM;
		goto finish;
	}

	if (copy_from_user
		(nonce_buf, sec_dbg_param->random_nonce, sec_dbg_param->rd_nonce_len)) {
		pr_err("copy_from_user error\r\n");
		ret = -EAGAIN;
		goto finish;
	}

	nonce_dma_addr = dma_map_single_attrs(iccpool_inf.icc_dev, nonce_buf,
				sec_dbg_param->rd_nonce_len, DMA_BIDIRECTIONAL, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(iccpool_inf.icc_dev, nonce_dma_addr)) {
		pr_err("<Secure Debug Client> Unable to map Nonce DMA\n");
		ret = -ENOMEM;
		goto finish;
	}

	dma_sync_single_for_device(iccpool_inf.icc_dev, nonce_dma_addr, sec_dbg_param->rd_nonce_len, DMA_TO_DEVICE);
	secdbg_client_msg.msg_id = SECURE_DEBUG_AUTH_VERIFY;
	secdbg_client_msg.param[SS_ICC_PARAM_0] = sec_dbg_param->port_num;
	secure_debug_config->nonce = (uint32_t)nonce_dma_addr;
	secure_debug_config->nonce_len = sec_dbg_param->rd_nonce_len;
	secure_debug_config->u.sig_info.p_signature_addr = (uint32_t)sign_dma_addr;
	secure_debug_config->u.sig_info.sign_sz = sec_dbg_param->sign_sz;
	secdbg_client_msg.icc_flags.priority = isadmin;

	ret_msg = kmalloc(sizeof(icc_msg_t), GFP_DMA);
	if (!ret_msg) {
		ret = -ENOMEM;
		goto finish;
	}

	ret = sec_dbg_send_icc_msg(&secdbg_client_msg, ret_msg, secure_debug_config);
	if (!ret)
		/* TEE send the 0 in case of success while non-zero value for error
		 * cases. As error codes are generally -ve value, but TEE is currently
		 * sending the +ve value.
		 * So handled the error cases by sending(always) the -ve value to
		 * userspace irrespective of TEE (+ve or -ve) reply.
		 */
		ret = -abs(((char)ret_msg->param_attr));

finish:
	if (ret_msg)
		kfree(ret_msg);

	if (nonce_buf) {
		dma_sync_single_for_cpu(iccpool_inf.icc_dev, nonce_dma_addr, sec_dbg_param->rd_nonce_len, DMA_TO_DEVICE);
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)nonce_buf, sec_dbg_param->rd_nonce_len);
	}

	if (sign_buf) {
		dma_sync_single_for_cpu(iccpool_inf.icc_dev, sign_dma_addr, sec_dbg_param->sign_sz, DMA_TO_DEVICE);
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)sign_buf, sec_dbg_param->sign_sz);
	}

	return ret;

}

long sse_sec_dbg_client_ioctl(struct file *fd,
				unsigned int cmd, unsigned long arg)
{
	sec_dbg_auth_start_t *auth_start_param = NULL;
	sec_dbg_auth_unlock_t *auth_unlock_param = NULL;
	sec_dbg_config_t *secure_debug_config = NULL;
	const struct cred *cred = NULL;
	int dev = 0;
	int ret = 0;
	bool isadmin = true;

	secure_debug_config = (void *)gen_pool_alloc(iccpool_inf.icc_pool, sizeof(sec_dbg_config_t));
	if (secure_debug_config == NULL) {
		pr_err("Memory Allocation for secure debug auth start failed. \r\n");
		ret = -ENOMEM;
		goto finish;
	}

	memset(secure_debug_config, 0, sizeof(sec_dbg_config_t));
	mutex_lock(&sec_dbg_client_mutex);
	dev = MINOR(fd->f_inode->i_rdev);

	if (dev == ICC_SEC_DBG_ADMIN_NR) {
		/* DAC Enforcement Check */
		if (!capable(CAP_SYS_ADMIN)) {
			ret = -EACCES;
			pr_err("DAC Enformcement check failed\n");
			goto finish;
		}
	} else {
		goto finish;
	}
	/* Get UID & GID */
	cred = current_cred();
#ifdef SDBG_DEBUG
	pr_info
		("secure debug client : devnode uid [%d]  - devnode gid [%d]\n",
			fd->f_inode->i_uid.val, fd->f_inode->i_gid.val);
#endif
	/* Compare the UID and GID (dynamic) against admin node which have
	 * fixed owner and group (root) UID and GID (0)as well.
	 */
	if ((S_IRUSR & fd->f_inode->i_mode) || (S_IWUSR & fd->f_inode->i_mode)) {
		if (fd->f_inode->i_uid.val != cred->uid.val) {
			ret = -EPERM;
			goto finish;
		}
	}

	if ((S_IRGRP & fd->f_inode->i_mode) || (S_IWGRP & fd->f_inode->i_mode)) {
		if (fd->f_inode->i_gid.val != cred->gid.val) {
			ret = -EPERM;
			goto finish;
		}
	}
	if ((S_IROTH & fd->f_inode->i_mode) || (S_IWOTH & fd->f_inode->i_mode)) {
		ret = -EPERM;
		goto finish;
	}

	switch (cmd) {
	case SSC_SECURE_DEBUG_AUTH_START:
		auth_start_param =
			kmalloc(sizeof(sec_dbg_auth_start_t), GFP_DMA);
		if (!auth_start_param) {
			pr_err("Allocation failed. \r\n");
			ret = -ENOMEM;
			goto finish;
		}
		if (copy_from_user
			(auth_start_param, (void *)arg, sizeof(sec_dbg_auth_start_t))) {
			pr_err(" secure debug auth start copy_from_user error\r\n");
			ret = -EFAULT;
			goto finish;
		}
		/* If Secure Debug port is already unlocked successfully then
		 * return the Error indicating "Debug port is already unlocked".
		 */
		if (dbgPort_unlock_st & (1 << auth_start_param->port_num)) {
			pr_info("Debug port :%d is already unlocked\n",auth_start_param->port_num);
			ret = -SDBG_PORT_ALREADY_UNLOCKED;
			goto finish;
		}

		ret =
			secure_debug_auth_start(auth_start_param,
							secure_debug_config, isadmin);
		if (ret < 0) {
			pr_err("secure debug Failed to start authentication\n");
			goto finish;
		}

		put_user(auth_start_param->port_num,
				(unsigned char __user *)&((sec_dbg_auth_start_t *)arg)->port_num);
		put_user(auth_start_param->nonce_len,
				(unsigned char __user *)&((sec_dbg_auth_start_t *)arg)->nonce_len);
		break;
	case SSC_SECURE_DEBUG_AUTH_VERIFY:
		auth_unlock_param =
			kmalloc(sizeof(sec_dbg_auth_unlock_t), GFP_DMA);
		if (!auth_unlock_param) {
			pr_err("Allocation failed. \r\n");
			ret = -ENOMEM;
			goto finish;
		}
		if (copy_from_user
			(auth_unlock_param, (void *)arg, sizeof(sec_dbg_auth_unlock_t))) {
			pr_err(" secure debug auth verify copy_from_user error\r\n");
			ret = -EFAULT;
			goto finish;
		}

		/* If Secure Debug Port is already unlocked successfully then
		 * return Error indicating "Debug port is already unlocked".
		 */
		if (dbgPort_unlock_st & (1 << auth_unlock_param->port_num)) {
			pr_info("Debug port :%d\n already unlocked\n",auth_unlock_param->port_num);
			ret = -SDBG_PORT_ALREADY_UNLOCKED;
			goto finish;
		}

		ret =
			secure_debug_auth_verify(auth_unlock_param,
								secure_debug_config, isadmin);
		if (ret < 0) {
			pr_err("secure debug Failed to verify authentication\n");
			goto finish;
		}
		/* Mark the Secure Debug Port status as unlocked */
		dbgPort_unlock_st |= (1 << auth_unlock_param->port_num);
		break;
	default:
		ret = -EINVAL;
		pr_err("Invalid ioctl cmd\r\n");
		break;
	}

finish:
	mutex_unlock(&sec_dbg_client_mutex);
	kfree(auth_start_param);
	kfree(auth_unlock_param);
	if (secure_debug_config)
		gen_pool_free(iccpool_inf.icc_pool, (unsigned long)secure_debug_config, sizeof(sec_dbg_config_t));

	return ret;
}

void pfn_secure_debug_callback(icc_wake_type wake_type)
{
	if ((wake_type & ICC_READ)) {
		refcount_set(&sec_dbg_wakeup, 1);
		wake_up_interruptible(&sec_dbg_wakeuplist);
	}
}

int __init secure_debug_client_init(void)
{
	int ret = 0;

	ret = icc_open((struct inode *)SECURE_DEBUG, NULL);
	if (ret < 0) {
		pr_err("Failed to open secure debug ICC client\n");
		ret = -ENODEV;
		goto out;
	}

	ret = icc_register_callback(SECURE_DEBUG, &pfn_secure_debug_callback);
	if (ret < 0) {
		pr_err("CallBack Register with ICC Failed for secure debug\n");
		ret = -EACCES;
		goto close_icc_dev;
	}

	iccpool_inf.icc_dev = icc_get_genpool_dev();
	if (iccpool_inf.icc_dev == NULL) {
		pr_err("Failed to get the genpool dev\n");
		ret = -EFAULT;
		goto finish;
	}

	iccpool_inf.icc_pool = icc_get_genpool();
	if (iccpool_inf.icc_pool == NULL) {
		pr_err("Failed to get the ICC genpool\n");
		ret = -EFAULT;
		goto finish;
	}


	init_waitqueue_head(&sec_dbg_wakeuplist);
	mutex_init(&sec_dbg_client_mutex);
	return ret;

finish:
	icc_unregister_callback(SECURE_DEBUG);
close_icc_dev:
	icc_close((struct inode *)SECURE_DEBUG, NULL);
out:
	return ret;
}

void __exit secure_debug_client_cleanup(void)
{
	icc_unregister_callback(SECURE_DEBUG);
	icc_close((struct inode *)SECURE_DEBUG, NULL);
}
