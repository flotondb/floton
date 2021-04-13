#ifndef FLOTON_ATOMIC_CACHE_H
#define FLOTON_ATOMIC_CACHE_H

#include <atomic>
#include <cstdio>
#include <cstdint>

// A cache that allows fast access to unordered wait free objects

#define ATOMIC_CACHE_BARR_SIZE(size) ((size) >= 64 ? (size) / 64 : 1)

template<class T>
class AtomicCache {
public:
	struct node {
		const size_t _size;
		std::atomic<bool> _recycling;
		std::atomic<uint64_t> _enqState;
		std::atomic<uint64_t> _deqState;
		std::atomic<uint64_t>* _enqBarrier;
		std::atomic<uint64_t>* _deqBarrier;
		std::atomic<node*> _next;
		std::atomic<T>* _data;

		node(size_t size):
		       _size(size),
		       _recycling(false),
		       _enqState(0), 
		       _deqState(0), 
		       _enqBarrier(new std::atomic<uint64_t>[ATOMIC_CACHE_BARR_SIZE(_size)]()), 
		       _deqBarrier(new std::atomic<uint64_t>[ATOMIC_CACHE_BARR_SIZE(_size)]()), 
		       _next(nullptr),
		       _data(new std::atomic<T>[_size])
		{}

		void inline setEnqBarrier(uint64_t index) {
			_enqBarrier[index / 64].fetch_xor(index % 64);
		}

		void inline setDeqBarrier(uint64_t index) {
			_deqBarrier[index / 64].fetch_xor(index % 64);
		}
	};

	AtomicCache(size_t startingNodeSize): _marker(new node(startingNodeSize)),
	                                      _nextNodeSize(startingNodeSize)
	                                      {}

	void give(const T& object);

	bool take(T& object);
private:
	std::atomic<node*> _marker;
	std::atomic<size_t> _nextNodeSize;
};


#endif // FLOTON_ATOMIC_CACHE_H
