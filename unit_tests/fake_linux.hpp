#ifndef FAKE_LINUX_HPP
#define FAKE_LINUX_HPP

#include <mutex>
#include <memory>

#define __packed __attribute__((packed))

#define container_of(address, type, field) \
	((type *)( \
		(char *)(address) - \
		(size_t)(&((type *)0)->field)))

#define noop do { (void)1; } while (0)

#define jiffies (mock_jiffies())

#define BIT(nr) (1UL << (nr))

#define time_after(a, b)		\
	 ((long)((b) - (a)) < 0)
#define time_before(a, b)	time_after(b, a)

#define time_is_before_jiffies(a) time_after(jiffies, a)

/* time_is_after_jiffies(a) return true if a is after jiffies */
#define time_is_after_jiffies(a) time_before(jiffies, a)

#define GFP_KERNEL 0

#define kmalloc(size, flags) malloc(size)

#define kzalloc(size, flags) calloc(1, size)

#define kfree(ptr) free(ptr)

#define mutex_init(type) (type)->mutex = std::make_unique<std::mutex>()

#define mutex_destroy(type) (type)->mutex = nullptr

#define flush_work(x) noop

#define dev_dbg(foo, ...) (void)(foo)
#define dev_err(foo, ...) (void)(foo)
#define pr_err(foo, ...) (void)(foo)

#define atomic_read(ptr) (*(ptr))
#define atomic_set(ptr, val) ((*(ptr)) = (val))

#define schedule_work(x) ((void) (x))

#define atomic_t unsigned int

#define u8 __u8
#define u16 __u16
#define u32 __u32
#define u64 __u64

#define msecs_to_jiffies(x) (x)

#define timer_setup(timer, fun, flags) \
	do { (void) (timer); (void)(fun); (void)(flags); } while (0)

#define del_timer_sync(x) noop

#define INIT_WORK(work, fun) do { (void)work; (void)fun; } while (0)

#define EXPORT_SYMBOL(name)
typedef unsigned int uid_t;
typedef struct {
        uid_t val;
} kuid_t;

struct completion {
	int unused;
};

#define __iomem

struct mutex {
	std::unique_ptr<std::mutex> mutex;
};

#define irqreturn_t unsigned int

struct wait_queue_head_t {
	int unused;
};

struct timer_list {
	int unused;
};

struct work_struct {
	int unused;
};

#define mod_timer(timer, time) ((void)(time))

#endif
