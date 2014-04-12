#pragma once

// Safely deletes a pointer.
#define SAFE_DELETE(ptr)	\
	if (ptr != NULL)		\
	{						\
		delete ptr;			\
		ptr = NULL;			\
	}