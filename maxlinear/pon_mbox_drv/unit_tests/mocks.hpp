#ifndef __mocks_hpp__
#define __mocks_hpp__

#include <iostream>
#include <thread>
#include <mutex>

#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>

#ifdef _WIN32
extern "C" {
	typedef signed long int ssize_t;
};
#endif

namespace mocks {

template<class RetType, class... FuncArgs>
struct Mock {
	std::function<RetType(FuncArgs...)> m_handler;
	RetType m_ret_reset;

	public:
	RetType ret_default;
	int ret_calls;

	void reset() {
		ret_calls = 0;
		ret_default = m_ret_reset;
		m_handler = NULL;
	}

	Mock(RetType default_ret_val):
		m_ret_reset(default_ret_val),
		ret_default(default_ret_val),
		ret_calls(0) { reset(); }

	void set_function_handler(std::function<RetType(FuncArgs...)> && fun) {
		m_handler = fun;
	}

	RetType operator()(FuncArgs... args) {
		++ret_calls;

		if(m_handler)
			return m_handler(args...);

		return ret_default;
	}
};

/** void needs special handling */
template <class... FuncArgs>
struct Mock<void, FuncArgs...> {
	std::function<void(FuncArgs...)> m_handler;

	public:
	int ret_calls;

	void reset() {
		ret_calls = 0;
		m_handler = NULL;
	}

	Mock(): ret_calls(0) { reset(); }

	void set_function_handler(std::function<void(FuncArgs...)> && fun) {
		m_handler = fun;
	}

	void operator()(FuncArgs... args) {
		++ret_calls;

		if(m_handler)
			return m_handler(args...);
	}
};

#define DEFINE_MUTEX(x) struct mutex x = { std::make_unique<std::mutex>() };

struct State {
	Mock<void, void *> mutex_lock;
	Mock<void, void *> mutex_unlock;
	Mock<ssize_t, unsigned int, unsigned int, void *, size_t, void *, size_t> mock_pon_mbox_send {0};
	Mock<unsigned long> mock_jiffies { 0 };
};

extern State state;

void reset();

}

extern "C" {
	extern void mutex_lock(void *);
	extern void mutex_unlock(void *);

	extern ssize_t mock_pon_mbox_send(unsigned int cmd_id, unsigned int rw,
					  void *input, size_t input_size,
					  void *output, size_t output_size);

	extern unsigned long mock_jiffies(void);
};

#endif

