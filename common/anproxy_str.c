#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int str_token_count(char delimiter, char* str)
{
	int len = strlen(str);
	int count = 0;
	char tmp = *str;
	int i = 0;
	for (i = 1; i < len; i++) {
		if (tmp == delimiter) {
			count++;	
		}
		tmp = *(str + i);
	}
	return count;
}

void str_copy_by_len(char* dest, const char* src, int len)
{
	int i=0;
	while((*dest++ = *src++) && (++i) && (i<len));
	*dest= '\0';
}

void str_copy_by_start_end(char* dest, const char* src, int start, int end)
{
	int i = end - start;
	str_copy_by_len(dest, src+start, i);
}

int find_delimiter_by_pos(char* src, char delimiter, int pos)
{
	int i = pos;
	src += i;
	while((*src != '\0') && (*src != delimiter))
	{
		src++;
		i++;
	}
	return i;	
}

void print_string_array(char** string_array, int len)
{
	int i=0;
	for(i=0; i<len; i++){
		printf("the index is %d, the string is %s\n", i, *string_array++);
	}
}

void free_string_array(char** string_array, int len)
{
	int i = 0;
	for(i=0; i<len; i++) {
	     free(*(string_array+i));
	}
	free(string_array);
}


void str_split(char delimiter, char* str, char*** output, int* count)
{
		int i;
		*count = str_token_count(delimiter, str);
		int start_pos = 0;
		int char_size;
		char** tmp;
		char** tmp_result;
		int end_pos = find_delimiter_by_pos(str, delimiter, 0);
		if (*count == 0) {
			return;
		}
		(*count)++;
   		tmp_result = (char**)malloc(sizeof(char*) * (*count));
		tmp = tmp_result;
		for (i = 0; i < *count; i++) {
			char_size = end_pos - start_pos + 1;	
			*tmp = (char *)malloc(sizeof(char) * char_size);
			printf("the char size at loop:%d, the char_size :%d, the output is %x \n", i, char_size, *tmp);
		    str_copy_by_start_end(*tmp, str, start_pos, end_pos);							
			*tmp++;
			start_pos = end_pos+1;
			end_pos = find_delimiter_by_pos(str, delimiter, start_pos);
		}
		*output = tmp_result;
}

