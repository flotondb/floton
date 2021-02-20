#ifndef FLOTON_OBJECT_TYPES_H
#define FLOTON_OBJECT_TYPES_H

#include <cstdint>
#include "AtomicShared.h"

class FlotonObject {
public:
	enum class Type {
		eBool
	};

	virtual Type getType() const = 0;
};

typedef AtomicShared<FlotonObject> FlotonShared;


class FlotonBool : public FlotonObject {
public:
	FlotonBool(bool val);
	FlotonBool(int val);

	Type getType() const { return Type::eBool; }

	void setTrue() { _val.store(1); }
	void setFalse() { _val.store(0); }
	void flip()  { _val.fetch_xor(1); }
	void opAnd(unsigned char b) { _val.fetch_and(b); }
	void opOr(unsigned char b) { _val.fecth_or(b); }
private:
	std::atomic<unsigned char> _val;
};

#endif // FLOTON_OBJECT_TYPES_H