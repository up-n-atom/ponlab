#ifndef __BACKPORT_LINUX_ETHTOOL_H
#define __BACKPORT_LINUX_ETHTOOL_H
#include_next <linux/ethtool.h>
#include <linux/version.h>

#ifndef SPEED_UNKNOWN
#define SPEED_UNKNOWN  -1
#endif /* SPEED_UNKNOWN */

#ifndef DUPLEX_UNKNOWN
#define DUPLEX_UNKNOWN 0xff
#endif /* DUPLEX_UNKNOWN */

#ifndef ETHTOOL_FWVERS_LEN
#define ETHTOOL_FWVERS_LEN 32
#endif
#if LINUX_VERSION_IS_LESS(6,0,0)
/**
 * struct kernel_ethtool_ringparam - RX/TX ring configuration
 * @rx_buf_len: Current length of buffers on the rx ring.
 * @tcp_data_split: Scatter packet headers and data to separate buffers
 * @tx_push: The flag of tx push mode
 * @rx_push: The flag of rx push mode
 * @cqe_size: Size of TX/RX completion queue event
 * @tx_push_buf_len: Size of TX push buffer
 * @tx_push_buf_max_len: Maximum allowed size of TX push buffer
 */
struct kernel_ethtool_ringparam {
        u32     rx_buf_len;
        u8      tcp_data_split;
        u8      tx_push;
        u8      rx_push;
        u32     cqe_size;
        u32     tx_push_buf_len;
        u32     tx_push_buf_max_len;
};
#endif

#endif /* __BACKPORT_LINUX_ETHTOOL_H */
