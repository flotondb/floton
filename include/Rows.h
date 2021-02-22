#ifndef FLOTON_ROWS_H
#define FLOTON_ROWS_H

#include "ObjectTypes.h"

/**
 * @file Defines the row object used in Floton tables
 */

class FlotonRow {
public:
	FlotonRow(size_t width);
	~FlotonRow();
private:
	size_t _width;
	FlotonShared* _objects;
};

typedef AtomicShared<FlotonRow> FlotonRowPtr;

#endif // FLOTON_ROWS_H