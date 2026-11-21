#include "common/itoa.h"

char* itoa(int i)
{
	char * res = (char *)malloc(8*sizeof(int));
	sprintf(res, "%d", i);
	return res;
}
