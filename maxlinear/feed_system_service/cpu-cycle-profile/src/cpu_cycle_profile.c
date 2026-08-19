/********************************************************************************
**
** FILE NAME    : 	cpu_cycles_profile.c
** DESCRIPTION  : 	Profiling driver for XRX500 series
** COPYRIGHT    :       Copyright (c) 2015 Intel
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** 	HISTORY
**
** 	$Date                	$Author                 $Comment
	Nov 2015			Hanamantha VJ			Init version	
	May 2017			Moinak Debnath			overflow bug fixes added
	Oct 2020			Moinak Debnath			Code cleanup, indentation
											added for maintainability
********************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/mipsmtregs.h>
#include <linux/version.h>
#include <linux/percpu.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/timer.h>
#include <linux/math64.h>

/* cycle count */
#define get_current_vpe()   \
        ((read_c0_tcbind() >> TCBIND_CURVPE_SHIFT) & TCBIND_CURVPE)

#undef CycleCounter_Start
#undef CycleCounter_End

#define		M_PERFCTL_EXL			(1 << 0)
#define 	M_PERFCTL_KERNEL		(1 << 1)
#define 	M_PERFCTL_SUPERVISOR		(1 << 2)
#define 	M_PERFCTL_USER			(1 << 3)
#define 	M_PERFCTL_INTERRUPT_ENABLE	(1 << 4)
#define		M_TC_EN_ALL			 0
#define 	M_PERFCTL_EVENT(event) 		(((event) & 0x3ff) << 5)
#define 	M_PERFCTL_EVENT_MASK 		0xfe0
#define 	M_PERFCTL_CONFIG_MASK 		0x3fff801f
#define 	M_PERFCTL_MT_EN(filter) 	((filter) << 20)
#define    	M_TC_EN_VPE M_PERFCTL_MT_EN	(1)
#define 	M_PERFCTL_VPEID(vpe) 		((vpe) << 16)
#define 	M_PERFCTL_CONFIG_MASK 		0x3fff801f

typedef struct {
	uint32_t start;
	uint32_t end;
	uint32_t cnt;
	uint64_t total; 
} profile_counter_t;

typedef struct {
	char name[128];
	profile_counter_t __percpu* pCounter;
	struct list_head list;
} profile_t;

static struct proc_dir_entry* s_profile_proc;
static LIST_HEAD(CycleCounterList);
static LIST_HEAD(InstCounterList);
static DEFINE_SPINLOCK(profile_lock);

#define DEFINE_STATIC_PROFILE(p_name) \
	DEFINE_PER_CPU(profile_counter_t, c_##p_name); \
	static profile_t s_##p_name =  { \
		.name     = #p_name, \
		.pCounter = &c_##p_name,   \
	};  \
	void* p_name = &s_##p_name; \
EXPORT_SYMBOL(p_name); 

static uint32_t s_StartCycles = 0;
static uint64_t s_TotalCycles = 0;
struct timer_list s_profOverflow_timer;
inline static void AddProfileToList(profile_t *pProf,struct list_head* head);

static void* CreateProfile(char *pcName, struct list_head* head)
{
	/* Allocate a new Counter */
	profile_t *pProf;

	if ((pProf = (profile_t*)kmalloc(sizeof(profile_t), GFP_ATOMIC))) {
	/* Initialize */
	memset(pProf, 0, sizeof(profile_t));
	strcpy(pProf->name,pcName);

		if (NULL == (pProf->pCounter = alloc_percpu(profile_counter_t))) {
			kfree(pProf);
			return NULL;
		}
		/* Add to list */
		AddProfileToList(pProf,head);
	}

	return pProf;
}

static void DestroyProfile(void *pvHandle)
{
	unsigned long flags;
	profile_t *pProf = (profile_t *)pvHandle;
	
	free_percpu(pProf->pCounter);

	/* Remove the node from list */
	spin_lock_irqsave(&profile_lock, flags);
	list_del(&pProf->list);
	spin_unlock_irqrestore(&profile_lock, flags);

	/* Free memory */
	kfree(pProf);
}

static void ProfileCounterStart(profile_counter_t *pCnt, uint32_t startCount)
{
	pCnt = this_cpu_ptr(pCnt);
	pCnt->start = startCount;
}

static void ProfileCounterEnd(profile_counter_t *pCnt, uint32_t EndCount)
{
	uint32_t tmp = 0;
	profile_counter_t* pCpuCnt;
	pCpuCnt = this_cpu_ptr(pCnt);
  	pCpuCnt->end = EndCount;
	
  	if (pCpuCnt->start) {
		if (pCpuCnt->start > pCpuCnt->end)
			tmp = (0xFFFFFFFF - pCpuCnt->start + pCpuCnt->end);
    		else
			tmp = pCpuCnt->end - pCpuCnt->start;

		/* Each hit of function, cumalative addtion will happen. at the end average will be calculated */
    		pCpuCnt->total += tmp;
	    	pCpuCnt->cnt++;
    		pCpuCnt->start = 0;
	}
}

inline static void AddProfileToList(profile_t *pProf,struct list_head* head)
{
	unsigned long flags;
	
	spin_lock_irqsave(&profile_lock, flags);
	list_add(&pProf->list, head);
	spin_unlock_irqrestore(&profile_lock, flags);
}

inline static void ResetProfileCounter(profile_counter_t* pCnt)
{
	int32_t cpu;
	profile_counter_t* pCpuCnt;

	for_each_possible_cpu(cpu) {
		pCpuCnt = per_cpu_ptr(pCnt, cpu);
		pCpuCnt->cnt    = 0;
		pCpuCnt->total  = 0;
		pCpuCnt->start  = 0;
		pCpuCnt->end    = 0;
	}
}

inline static void GetProfileCounterStats(profile_counter_t* pCnt, 
                                   int32_t cpu,
                                   uint32_t* hit, 
                                   uint64_t* total)
{
	profile_counter_t* pCpuCnt;

	pCpuCnt = (profile_counter_t*) per_cpu_ptr(pCnt, cpu);
	*hit	= pCpuCnt->cnt;
	*total  = pCpuCnt->total;
}

static void ResetAllProfileCounters(void)
{
	profile_t *pProf = NULL;

	/* Reset all counter to 1 */
	list_for_each_entry(pProf , &CycleCounterList, list)
		ResetProfileCounter(pProf->pCounter);
	
	list_for_each_entry(pProf , &InstCounterList, list)
		ResetProfileCounter(pProf->pCounter);

	s_StartCycles = read_c0_count();
	s_TotalCycles = 0;
}

void* CycleCounter_Create(char *pcName)
{
	return CreateProfile(pcName, &CycleCounterList);
}
EXPORT_SYMBOL(CycleCounter_Create);

void CycleCounter_Destroy(void *pvHandle)
{
	DestroyProfile(pvHandle);
}
EXPORT_SYMBOL(CycleCounter_Destroy);

void CycleCounter_Start(void *pvHandle)
{
	ProfileCounterStart(((profile_t *)pvHandle)->pCounter,read_c0_count());
}
EXPORT_SYMBOL(CycleCounter_Start);

void CycleCounter_End(void *pvHandle)
{
	ProfileCounterEnd(((profile_t*)pvHandle)->pCounter, read_c0_count());
}
EXPORT_SYMBOL(CycleCounter_End);

void* InstCounter_Create(char *piName)
{
	return CreateProfile(piName, &InstCounterList);
}
EXPORT_SYMBOL(InstCounter_Create);

void InstCounter_Destroy(void *pvHandle)
{
	DestroyProfile(pvHandle);
}
EXPORT_SYMBOL(InstCounter_Destroy);

void InstCounter_Start(void *pvHandle)
{

	uint32_t configctrl,eventctrl;
	int cpu;

	cpu = smp_processor_id();

	configctrl = (M_PERFCTL_KERNEL | M_PERFCTL_EXL | M_PERFCTL_SUPERVISOR | 
		M_PERFCTL_VPEID(cpu) | M_TC_EN_VPE) & M_PERFCTL_CONFIG_MASK;

	eventctrl = M_PERFCTL_EVENT(0x1) | configctrl;
	write_c0_perfcntr0((unsigned int)0);
	write_c0_perfctrl0(eventctrl);
	ProfileCounterStart(((profile_t *) pvHandle)->pCounter, read_c0_perfcntr0());
}
EXPORT_SYMBOL(InstCounter_Start);

void InstCounter_End(void *pvHandle)
{
	ProfileCounterEnd(((profile_t*)pvHandle)->pCounter, read_c0_perfcntr0());
}
EXPORT_SYMBOL(InstCounter_End);

static int proc_read_profile(struct seq_file *seq, void *v)
{
	profile_t *pProf = NULL ;
	uint32_t  percpu_hit;
	uint64_t  percpu_total;
	uint32_t  hit;
	uint64_t  total;
	uint64_t  avg;
	int32_t   cpu, i;

	/*The info created in proc and displayed */
	for_each_possible_cpu(cpu)
		seq_printf(seq ,"\t\t\t  CPU:%d", cpu);

	seq_printf(seq ,"\t\t\t  Total");
	seq_printf(seq ,"\nCycle Counter Name:");

	for_each_possible_cpu(cpu)
		seq_printf(seq ,"\t  Hit\t  Cycles\t  avg");

	seq_printf(seq ,"\t  Hit\t  Cycles\t  avg\n");
	list_for_each_entry(pProf , &CycleCounterList, list) {
		total = 0;
		hit   = 0;
		seq_printf(seq, "%-24s:",pProf->name);

		for_each_possible_cpu(cpu) {
			GetProfileCounterStats(pProf->pCounter, cpu, &percpu_hit, &percpu_total);
			avg = (percpu_hit)?div_u64(percpu_total,percpu_hit):0; 
			seq_printf(seq,"%u\t%llu\t%llu\txx\t",
		  	percpu_hit, percpu_total, avg);
			hit += percpu_hit;
			total += percpu_total;
		}

		avg = (hit) ? div_u64(total, hit) : 0; 
		seq_printf(seq, "%u\t%llu\t%llu\n", hit, total, avg);
	}

	seq_printf(seq, "\nInstruction Counter Name:\t\tHit\tInstructions\tavg\n");
	list_for_each_entry(pProf , &InstCounterList, list) { 
		total = 0;
		hit   = 0;
		for_each_possible_cpu(cpu) {
			GetProfileCounterStats(pProf->pCounter, cpu, &percpu_hit, &percpu_total);
			avg = (percpu_hit) ? div_u64(percpu_total, percpu_hit) : 0; 
			seq_printf(seq, "%s-CPU-%d:\t\t%u\t%llu\t%llu\n",
				pProf->name, cpu, percpu_hit, percpu_total, avg);
			hit += percpu_hit;
			total += percpu_total;
		}
		avg = (hit) ? div_u64(total, hit) : 0; 
		seq_printf(seq,"%s-Total:\t\t%u\t%llu\t%llu\n\n",
			pProf->name, hit, total, avg);
	}
	seq_printf(seq,"Total Cycles Since Last Clear  = %llu\n", 
		s_TotalCycles);

	return 0;
}

static ssize_t proc_write_profile(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
	ResetAllProfileCounters();

	return count;
}

static int proc_open_profile(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_profile, NULL);
}

static struct file_operations s_proc_profile_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_open_profile,
	.read       = seq_read,
	.write	    = proc_write_profile,
	.llseek     = seq_lseek,
	.release    = seq_release,
};

static void profile_proc_create(void)
{
	s_profile_proc = proc_mkdir("driver/cycle", NULL);

	if (s_profile_proc) {
		proc_create("profile",
                S_IRUGO,
                s_profile_proc,
                &s_proc_profile_fops);
	}
}

static void profile_proc_destory(void)
{
	if (s_profile_proc) {
		remove_proc_entry("profile", s_profile_proc);
		proc_remove(s_profile_proc);
		s_profile_proc = NULL;
  	}
}

static void profile_timer(unsigned long data)
{
	uint32_t t_cycles;
	uint32_t CyclesSpent;

	t_cycles = read_c0_count();

	if (t_cycles < s_StartCycles)
		CyclesSpent = 0xFFFFFFFF - s_StartCycles + t_cycles; 
	else
		CyclesSpent = t_cycles - s_StartCycles;

	s_TotalCycles += CyclesSpent;
	s_StartCycles = t_cycles;

	s_profOverflow_timer.expires = jiffies + 1 * HZ - 1;
	add_timer(&s_profOverflow_timer);
}

static int __init CycleCounter_Init(void)
{
	profile_proc_create();
	ResetAllProfileCounters();
	setup_timer(&s_profOverflow_timer,profile_timer,0);
	s_profOverflow_timer.expires = jiffies + 1 * HZ - 1;
	add_timer(&s_profOverflow_timer);

	return 0;
}

static void __exit CycleCounter_Cleanup(void)
{
	profile_proc_destory();
}

module_init(CycleCounter_Init);
module_exit(CycleCounter_Cleanup);

MODULE_AUTHOR("INTEL");
MODULE_DESCRIPTION("CPU Cycles Profiling driver");
MODULE_LICENSE("GPL");
