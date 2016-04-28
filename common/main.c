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
	char* test = "adfadsfas,addsafa,asdfaf";
	printf("the output adrr is %x \n", &output);	
	str_split(',', test, &output, &count); 	
	print_string_array(output, count);
	free_string_array(output, count);
	return 0;
}
