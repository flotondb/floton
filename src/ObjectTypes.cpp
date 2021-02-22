#include "ObjectTypes.h"

FlotonObject::~FlotonObject(){}

FlotonBool::FlotonBool(bool val): _val(val ? 1 : 0)
{}
	
FlotonBool::FlotonBool(int val): _val(val ? 1 : 0)
{}
