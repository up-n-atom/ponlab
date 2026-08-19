#include <cstdint>
#include <cstddef>
#include <mutex>

#include "mocks.hpp"
#include "fake_linux.hpp"

/* Helper macro to save some lines of code */
#define MOCK(type, func, args, pass) \
	type func args { return mocks::state. func pass; }

/* Helper macro to save some lines of code,
   void functions do not return value */
#define MOCK_VOID(func, args, pass) \
	void func args { mocks::state. func pass; }

namespace mocks
{
State state;

void reset()
{
	state = State{};

	state.mutex_lock.set_function_handler(
	    [](void *l) {
			static_cast<struct mutex*>(l)->mutex->lock();
		});

	state.mutex_unlock.set_function_handler(
	    [](void *l) {
			static_cast<struct mutex*>(l)->mutex->unlock();
		});
}
} // namespace mocks

extern "C" {
	MOCK_VOID(mutex_lock, (void *lock), (lock))
	MOCK_VOID(mutex_unlock, (void *lock), (lock))
	MOCK(ssize_t, mock_pon_mbox_send,
	    (unsigned int cmd_id, unsigned int rw,
	     void *input, size_t input_size, void *output,
	     size_t output_size),
	    (cmd_id, rw, input, input_size, output, output_size))
	MOCK(unsigned long, mock_jiffies, (void), ())
}
