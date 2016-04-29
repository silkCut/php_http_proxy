#include <stdio.h>
#include "anproxy_str.h"

#ifdef MEMCHECK
#include <mcheck.h>
#endif
int main()
{
#ifdef MEMCHECK
	mtrace();
#endif
	char** output;
	int count;
	char* test = "11";
	char* match="11";
	printf("the adrr is match %d \n", check_str_array_match(match, ',', test));	

	return 0;
}
