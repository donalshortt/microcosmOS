#include "../includes/front.h"

#ifndef _TEST_H_
#define _TEST_H_

void fail(char* test)
{

}

int assert_eq(long lhs, long rhs)
{
	if (lhs != rhs)
		return -1;

	return 0;
}

#endif
