#ifndef FLOTON_ATOMIC_CACHE_H
#define FLOTON_ATOMIC_CACHE_H

#include <atomic>

// A cache that allows fast access to unordered wait free objects

template<class T>
class AtomicCache {
public:
	struct node {
		size_t size;
		std::atomic<T*>* items;
	};
private:
	std::atomic<node*> _head;
};


#endif // FLOTON_ATOMIC_CACHE_H
