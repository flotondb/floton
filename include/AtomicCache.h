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

		~node()
		{
			delete[] _enqBarrier;
			delete[] _deqBarrier;
			delete[] _data;
		}

		bool inline recycle() {
			bool claimedByte = false;
			if (_recycling.compare_exchange_strong(claimedByte, true)) {
				// Only one thread succeeds here.
				_enqState.store(0);
				_deqState.store(0);
				for (size_t i = 0; i < ATOMIC_CACHE_BARR_SIZE(_size); ++i)
				{
					_enqBarrier[i] = 0;
					_deqBarrier[i] = 0;
				}
				_recycling.store(false);
				return true;
			} else {
				return false;
			}
		}

		bool inline isFull() const {
			for (size_t i = 0; i < ATOMIC_CACHE_BARR_SIZE(_size); ++i)
			{
				if (_deqBarrier[i].load() != 0xFFFFFFFFFFFFFFFF)
					return false;
			}
			return true;
		}

		void inline setEnqBarrier(uint64_t index) {
			_enqBarrier[index / 64].fetch_xor(1 << (index % 64));
		}

		bool inline checkEnqBarrier(uint64_t index) {
			return _enqBarrier[index / 64] & (1 << (index % 64));
		}

		void inline setDeqBarrier(uint64_t index) {
			_deqBarrier[index / 64].fetch_xor(1 << (index % 64));
		}

		bool inline checkDeqBarrier(uint64_t index) {
			return _deqBarrier[index / 64] & (1 << (index % 64));
		}
	};

	AtomicCache(size_t startingNodeSize): _marker(new node(startingNodeSize)),
	                                      _nextNodeSize(startingNodeSize)
	                                      {}

	void give(const T& object)
	{
		node* markNode = _marker.load();
		if (markNode->isFull()) {
			if (markNode->recycle()) {

			} else {

			}
		}
		uint64_t markEnq = markNode->_enqState.fetch_add(1);
		while (markEnq >= markNode->_size) {

		}
		markNode->_data[markEnq].store(object);
		markNode->setEnqBarrier(markEnq);
	}

	bool take(T& object)
	{
		return true;
	}

	void inline increaseNodeSize(size_t factor)
	{
		size_t currNodeSize = _nextNodeSize.load();
		_nextNodeSize.compare_exchange_strong(currNodeSize, currNodeSize * factor);
	}
private:
	std::atomic<node*> _marker;
	std::atomic<size_t> _nextNodeSize;
};


#endif // FLOTON_ATOMIC_CACHE_H
