#pragma once

#include "Exception.h"

class ListEmptyException : public Exception {
public:
	ListEmptyException() : Exception("Список пуст") {}
};
