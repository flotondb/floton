#include "Rows.h"

FlotonRow::FlotonRow(size_t width): _width(width),
                                    _objects(new FlotonShared[_width])
{}
	
FlotonRow::~FlotonRow()
{
	delete[] _objects;
}
