#ifndef FLOTON_ATOMIC_SHARED_H
#define FLOTON_ATOMIC_SHARED_H

#include <atomic>

/**
 * @file A shared pointer that is always lock free and supports concurrent
 *       read and write operations through CAS and exch
 */

template<class T>
class AtomicShared {
public:
	AtomicShared(): _ptr(nullptr), _counter(nullptr) 
	                 {}
	AtomicShared(T* ptr): _ptr(ptr),
	                       _counter(new std::atomic<size_t>(1))
	                 {}
	bool valid() const { return _ptr.load() == nullptr; }

	AtomicShared(const AtomicShared<T>& other) {
		_counter.store(other._counter.load());
		_counter.load()->fetch_add(1);
		_ptr.store(other._ptr.load());
	}

	AtomicShared<T>& operator=(const AtomicShared<T>& other) noexcept
	{
		if (this != &other) {
			if (_counter.load() != nullptr && _counter.load()->fetch_sub(1) == 1) {
				delete _ptr.load();
				delete _counter.load();			
			}
			_counter.store(other._counter.load());
			_counter.load()->fetch_add(1);
			_ptr.store(other._ptr.load());
		}
		return *this;
	}

	T* operator->() const { return _ptr.load(); }

	inline T* get() const { return _ptr.load(); }

	void reset(T* ptr = nullptr) {
		if ( _counter.load()->fetch_sub(1) == 1) {
			delete _ptr.load();
			delete _counter.load();
		}
		_ptr.store(ptr);
		_counter.store(new std::atomic<size_t>(1));
	}

	bool cmp_exchange(T* expected, T* desired = nullptr) {
		if (_ptr.compare_exchange_strong(expected, desired)) {
			if (_counter.load()->fetch_sub(1) == 1) {
				delete expected;
				delete _counter.load();
			}
			_counter.store(new std::atomic<size_t>(1));
			return true;
		}
		return false;
	}

	~AtomicShared() {
		if (_counter.load()->fetch_sub(1) == 1) {
			delete _ptr.load();
			_ptr.store(nullptr);
			delete _counter.load();
		}
	}
private:
	std::atomic<T*> _ptr;
	std::atomic<std::atomic<size_t>*> _counter;
};

#endif // FLOTON_ATOMIC_SHARED_H
