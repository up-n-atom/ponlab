/****************************************************************************** 

  Copyright © 2020-2021 MaxLinear, Inc.
  Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
******************************************************************************/

/*
 * This file contains the logic of AAL5 Reassembly component of IMA+ Bonding
 * Driver. Within the IMA+ Bonding Driver, it occupies the layer shown below:
 *
 *    +-------+-------------------------------------+---------------------------+
 *    |       |  Rx Bonding Driver code components  |        Source code        |
 *    |       |        (sorted from UE to LE)       |                           |
 *    +-------+-------------------------------------+---------------------------+
 *    |       | T C  D R I V E R  (upper layer)     |                           |
 *    | - - - | - - - - - - - - - - - - - - - - - - | - - - - - - - - - - - - - |
 *    |   1   | Reassembly FSM Library              | ima_rx_reassembler.c(.h)  |<=
 *    |   2   | Bonding Library                     | ima_rx_bonding.c(.h)      |
 *    |   3   | ATM Cell Decode Library             | ima_rx_atm_cell.h         |
 *    |   4   | Line Queue Abstraction Library      | ima_rx_queue.c(.h)        |
 *    |   5   | H/W RXOUT Ring Interface            | ima_atm_rx.c(.h)          |
 *    | - - - | - - - - - - - - - - - - - - - - - - | - - - - - - - - - - - - - |
 *    |       | H/W RXOUT RINGS   (lower layer)     |                           |
 *    +-------+-------------------------------------+---------------------------+
 *
 *    Unit Tests: 1. src/test/ut_rx_reassembly_testgroup_1.c
 *                2. src/test/ut_rx_bonding_and_reassembly_testgroup_1.c
 */

#ifdef __KERNEL__
#include <linux/skbuff.h>
#include <linux/byteorder/generic.h>
#include <linux/spinlock.h>
#include <linux/crc32.h>
#else
#include <stdint.h>
#include <stddef.h> // NULL
#include <arpa/inet.h> // ntohs
#endif
#ifdef CONFIG_LTQ_CBM
#include <net/lantiq_cbm_api.h>
#endif

#include "common/ima_rx_queue.h"
#include "atm/ima_atm_rx.h"
#include "atm/ima_rx_atm_cell.h"

#ifdef __KERNEL__
#ifndef PRIV
#define PRIV (&g_privdata)
#endif

#define RX_SKB_POOL_SIZE 256
static inline int init_rxbuff_list(u32 maxcellcount);
static inline void enqueue_rxbuff_list(struct sk_buff *skb);
static inline struct sk_buff *dequeue_rxbuff_list(void);
static inline void cleanup_rxbuff_list(void);

u8 *AAL5FrameAlloc(uint32_t cellcount)
{
	struct sk_buff *frame;

	frame = dequeue_rxbuff_list();
	if (likely(frame))
		skb_reserve(frame, NET_SKB_PAD);

	return (u8 *)frame;
}

uint32_t AAL5FrameCopyCell(uint8_t *frame, uint8_t *cell)
{
	memcpy(skb_put((struct sk_buff *)frame, 48), cell, 48);
	return 0;
}

void AAL5FrameDiscard(uint8_t *frame)
{
	enqueue_rxbuff_list((struct sk_buff *)frame);
}

u8 *AAL5FrameGetData(uint8_t *frame)
{
	return ((struct sk_buff *)frame)->data;
}

void AAL5FrameAccept(int destid, uint8_t *aal5frame, uint32_t aal5framelen)
{
	int ret;
	struct atm_aal5_t atm_rx_pkt = {0};
	struct sk_buff *skb = (struct sk_buff *)aal5frame;

	atm_rx_pkt.skb = skb;
	atm_rx_pkt.conn = (destid >> 3) & 0xf;

	ret = tc_decap_rx_pkt(&atm_rx_pkt);
	if (unlikely(ret)) {
		UPDATE_STATS( discardedframecount[UNRECOGNIZED], 1);
	} else
		ret = enqueue_rx_hostq(destid, skb->data, skb->len, skb);

	if (unlikely(ret))
		enqueue_rxbuff_list(skb);
}

#ifndef AAL5FRAME_ALLOC
#define AAL5FRAME_ALLOC(frame, cellcount) ((frame) = AAL5FrameAlloc((cellcount)))
#endif

#ifndef AAL5FRAME_COPY_CELL
#define AAL5FRAME_COPY_CELL(frame, data, len) AAL5FrameCopyCell((frame), (data))
#endif

#ifndef AAL5FRAME_DISCARD
#define AAL5FRAME_DISCARD(frame) AAL5FrameDiscard((frame))
#endif

#ifndef AAL5FRAME_DATA
#define AAL5FRAME_DATA(frame) AAL5FrameGetData((frame))
#endif

#define PRINT(format, ...)

#ifndef AAL5FRAME_ACCEPT
#define AAL5FRAME_ACCEPT(destid, aal5frame, aal5framelen) AAL5FrameAccept((destid), (aal5frame), (aal5framelen))
#endif

#endif

int32_t
init_rx_reassembler( void *cfg, unsigned int maxcellcount )
{
	int32_t ret;

	PRIV->reassemble.rejectionmode = 0;
	PRIV->reassemble.cellcount = 0;
	PRIV->reassemble.frame = NULL;
	PRIV->reassemble.maxcellcount = maxcellcount; 
	/* Others...    *
	 * PRIV->reassemble.lastsid
	 */

	ret = init_rx_hostq(cfg);
	if (ret)
		goto out;

	ret = init_rxbuff_list(maxcellcount);
	if (ret) {
		cleanup_rx_hostq();
		goto out;
	}

out:
	return ret;
}

void
cleanup_rx_reassembler(void)
{
	if( PRIV->reassemble.frame != NULL ) {
		PRIV->reassemble.cellcount = 0;
		AAL5FRAME_DISCARD( PRIV->reassemble.frame );
		PRIV->reassemble.frame = NULL;
	}

	cleanup_rxbuff_list();
	cleanup_rx_hostq();
}

void
reassemble_aal5(ima_rx_atm_header_t *cell, u32 sid, u32 sid_range, u32 linkid)
{
	int goodmood = 1;
	uint32_t crc;
	uint16_t trailer_length;

#if 0
	if( cell->pt2 ) // Congestion. Reset current AAL5 frame
	{
		//print_hex_dump(KERN_CRIT, "Bonding Driver: DUMPING CONGESTION ATM Cell: ", DUMP_PREFIX_OFFSET, 16, 1, cell, 56, false);
		// Update cell-stats by incrementing dropped cell count
		UPDATE_STATS( droppedcells[linkid], 1);
		if( PRIV->reassemble.cellcount ) {

			PRIV->reassemble.cellcount = 0;
			AAL5FRAME_DISCARD( PRIV->reassemble.frame );
			PRIV->reassemble.frame = NULL;
			UPDATE_STATS( discardedframecount[INVALIDPT2ORCLP], 1);
		}

		return;
	}
#endif

	if (PRIV->reassemble.rejectionmode) {
		// In bad mood...

		if (cell->pt1)
		{
			// Sunshine

			PRIV->reassemble.rejectionmode = 0;
			PRIV->reassemble.cellcount = 0;
			PRIV->reassemble.frame = NULL;
		}

		// And thats it
		return;
	}

	// In good mood

	if ( PRIV->reassemble.cellcount && ( PRIV->reassemble.lastsid != ( sid ? (sid - 1) : (sid_range - 1) ) ) )
	{
		UPDATE_STATS( discardedframecount[SIDJUMP], 1); //++(PRIV->stats.discardedframecount[SIDJUMP]);
		PRINT("Frame discarded. SID jump count goes up to %u\n", PRIV->stats.discardedframecount[SIDJUMP]);
		goodmood = 0;

	} else if ( cell->pt1 ) // Last cell. Frame to depart.
	{
		int validframe = 1;

		if( !PRIV->reassemble.frame ) { // also the first cell

			AAL5FRAME_ALLOC( PRIV->reassemble.frame, PRIV->reassemble.maxcellcount );
			PRIV->reassemble.cellcount = 0;

			if( unlikely( !PRIV->reassemble.frame ) ) { // Allocation failed for this unicellular AAL5 frame

				PRINT("Frame discarded. Allocation failure\n" );
				UPDATE_STATS( discardedframecount[ALLOCFAILURE], 1);

				// Reset counters
				PRIV->reassemble.frame = NULL;
				PRIV->reassemble.cellcount = 0;
				return;
			}
		}

		++(PRIV->reassemble.cellcount);
#ifdef BADFRAME_THEN_DUMPITSCELLS
		memcpy( PRIV->celldumpbuffer + (PRIV->reassemble.cellcount-1)*64, (u8 *)cell, 56);
#endif /* BADFRAME_THEN_DUMPITSCELLS */
		AAL5FRAME_COPY_CELL( PRIV->reassemble.frame, cell->payload, 48 );

		// Validate frame
		trailer_length = /*ntohs*/( *((uint16_t *)((AAL5FRAME_DATA(PRIV->reassemble.frame)) + (PRIV->reassemble.cellcount)*48 - 6)) );
		if( ( ( trailer_length + 8 - 1 ) / 48 ) != ( PRIV->reassemble.cellcount - 1) )
		{
			/* Invalid Length */
			pr_info("Bonding Driver: Invalid AAL5 Trailer length (%d) given number of cells in the AAL5 frame is %d\n", trailer_length, PRIV->reassemble.cellcount);
#ifdef BADFRAME_THEN_DUMPITSCELLS
			print_hex_dump(KERN_CRIT, "Bonding Driver: AAL5 Cells: ", DUMP_PREFIX_OFFSET, 16, 1, PRIV->celldumpbuffer, PRIV->reassemble.cellcount*64, false);
#endif /* BADFRAME_THEN_DUMPITSCELLS */
			UPDATE_STATS( discardedframecount[INVALIDLENGTH], 1); // ++(PRIV->stats.discardedframecount[INVALIDLENGTH]);
			PRINT("Frame discarded. Invalid length count goes up to %u\n", PRIV->stats.discardedframecount[INVALIDLENGTH]);
			validframe = 0;

		} else {

			crc = crc32_be(0xFFFFFFFF, AAL5FRAME_DATA(PRIV->reassemble.frame), (PRIV->reassemble.cellcount*48) - 4 ) ^ 0xFFFFFFFF;
			if( crc != ntohl( *((uint32_t *)((AAL5FRAME_DATA(PRIV->reassemble.frame)) + (PRIV->reassemble.cellcount)*48 - 4))))
			{
				/* Invalid CRC */
				/*pr_info("Invalid CRC: calculated crc = 0x%08X trailer crc = 0x%08X\n", (ntohl(crc)), \
					*((uint32_t *)((AAL5FRAME_DATA(PRIV->reassemble.frame)) + (PRIV->reassemble.cellcount)*48 - 4)) ); */
				UPDATE_STATS( discardedframecount[INVALIDCRC32], 1); // ++(PRIV->stats.discardedframecount[INVALIDCRC32]);
				PRINT("Frame discarded. Invalid CRC32 count goes up to %u\n", PRIV->stats.discardedframecount[INVALIDCRC32]);
				validframe = 0;
			}
		}

		if( validframe )
		{
			// Goodbye and good luck
			REPORT( "Frame accepted", PRIV->reassemble.cellcount, 0 );
			UPDATE_STATS( acceptedframecount, 1); //++(PRIV->stats.acceptedframecount);
#ifdef DISABLE_RX_ATMSTACK
			AAL5FRAME_DISCARD( PRIV->reassemble.frame );
#else
			AAL5FRAME_ACCEPT( *(u16 *)(cell->padding), PRIV->reassemble.frame, 48* (PRIV->reassemble.cellcount ));
#endif
			PRINT( "AAL5 frame with trailer length %u received\n", trailer_length );

		} else {

			// Else discard the frame
			REPORT( "Frame discarded", 1, PRIV->reassemble.cellcount );
			PRINT("Frame discarded\n");
			AAL5FRAME_DISCARD( PRIV->reassemble.frame );
		}

		// Reset counters
		PRIV->reassemble.frame = NULL;
		PRIV->reassemble.cellcount = 0;
		return;

	} else if( PRIV->reassemble.maxcellcount <= (PRIV->reassemble.cellcount + 1) ) // Houseful -- cannot accomodate next cell
	{
		PRINT("Frame discarded. Buffer overflow\n" );
		UPDATE_STATS( discardedframecount[SIZEOVERFLOW], 1);
		goodmood = 0;
	}

	if( !goodmood )
	{

		PRIV->reassemble.rejectionmode = 1;
		if( PRIV->reassemble.frame )
		{
			REPORT( "Frame discarded", 2, PRIV->reassemble.cellcount );
			AAL5FRAME_DISCARD( PRIV->reassemble.frame );
			PRIV->reassemble.frame = NULL;
		}

		return;
	}

	// Good mood

	if( PRIV->reassemble.frame ) // Not first cell. Growing frame
	{
		PRIV->reassemble.lastsid = sid;
		++(PRIV->reassemble.cellcount);
#ifdef BADFRAME_THEN_DUMPITSCELLS
		memcpy( PRIV->celldumpbuffer + (PRIV->reassemble.cellcount-1)*64, (u8 *)cell, 56);
#endif /* BADFRAME_THEN_DUMPITSCELLS */
		AAL5FRAME_COPY_CELL( PRIV->reassemble.frame, cell->payload, 48 );

	} else { // First cell. New Frame will be born thus

		AAL5FRAME_ALLOC( PRIV->reassemble.frame, PRIV->reassemble.maxcellcount );
		if( unlikely( !PRIV->reassemble.frame ) ) { // Allocation failure

			// Keep rejecting all cells till the end of this AAL5 frame
			PRIV->reassemble.rejectionmode = 1;
			UPDATE_STATS( discardedframecount[ALLOCFAILURE], 1);
			PRINT("Frame discarded. Allocation failure\n" );
			return;
		}

		PRIV->reassemble.lastsid = sid;
		PRIV->reassemble.cellcount = 1;
#ifdef BADFRAME_THEN_DUMPITSCELLS
		memcpy( PRIV->celldumpbuffer + (PRIV->reassemble.cellcount-1)*64, (u8 *)cell, 56);
#endif /* BADFRAME_THEN_DUMPITSCELLS */
		AAL5FRAME_COPY_CELL( PRIV->reassemble.frame, cell->payload, 48 );
	}

}

static inline int
init_rxbuff_list(u32 maxcellcount)
{
	int ret = 0;
	struct sk_buff *skb;
	u32 size;
	int i;

	size = NET_SKB_PAD + (maxcellcount * 48);
	__skb_queue_head_init(&PRIV->reassemble.skb_list);
	for (i = 0; i < RX_SKB_POOL_SIZE; i++) {
		skb = alloc_skb(size, GFP_KERNEL);
		if (!skb) {
			pr_err("[%s:%d] Rx skb of size:%d allocation failed!\n",
				__func__, __LINE__, size);
			cleanup_rxbuff_list();
			ret = -ENOMEM;
			break;
		}
		__skb_queue_tail(&PRIV->reassemble.skb_list, skb);
	}

	return ret;
}

static inline struct sk_buff *
dequeue_rxbuff_list(void)
{
	struct sk_buff *skb;

	/* Re-cycle buffers from host rx ring, if any */
	skb = dequeue_rx_hostq(false);
	while (skb) {
		enqueue_rxbuff_list(skb);
		skb = dequeue_rx_hostq(false);
	}

	return __skb_dequeue(&PRIV->reassemble.skb_list);
}

static inline void
enqueue_rxbuff_list(struct sk_buff *skb)
{
	/* Reset skb parameters before re-cycle */
	skb->len = 0;
	skb->data = skb->head;
	skb_reset_tail_pointer(skb);
	__skb_queue_tail(&PRIV->reassemble.skb_list, skb);
}

static inline void
cleanup_rxbuff_list(void)
{
	struct sk_buff *skb;

	skb = dequeue_rx_hostq(true);
	while (skb) {
		enqueue_rxbuff_list(skb);
		skb = dequeue_rx_hostq(true);
	}

	if (skb_queue_len(&PRIV->reassemble.skb_list) != RX_SKB_POOL_SIZE)
		pr_warn("[%s] queue len:%d is not equal to pool size:%d, "
			"any skb leak?\n", __func__,
			skb_queue_len(&PRIV->reassemble.skb_list), RX_SKB_POOL_SIZE);

	__skb_queue_purge(&PRIV->reassemble.skb_list);
}
