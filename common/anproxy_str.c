#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "anproxy_str.h"

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

int check_str_array_match(char* match, char delimiter, char* str)
{
	char** input;
	char** tmp;
	char* str_tmp;
	int match_count;
	unsigned short int is_match=0;
	int i;
	str_split(delimiter, str, &input, &match_count);
	if (match_count == 0) {
		if(strcmp(match, str) == 0) {
			return 1;
		} else {
			return 0;
		}
	}
	tmp=input;
	for(i=0; i < match_count; i++){
		str_tmp = *tmp;	
#ifdef DEBUG_PRINT
		printf("the match is %s, the tmp is %s\n", match, str_tmp);
#else
		if(strcmp(match,str_tmp) == 0) {
			is_match = 1;
			break;
		}
#endif
		tmp++;
	}
	free_string_array(input, match_count);
	return is_match;
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
#ifdef DEBUG_PRINT
			printf("the char size at loop:%d, the char_size :%d, the output is %x \n", i, char_size, *tmp);
#endif
		    str_copy_by_start_end(*tmp, str, start_pos, end_pos);							
			*tmp++;
			start_pos = end_pos+1;
			end_pos = find_delimiter_by_pos(str, delimiter, start_pos);
		}
		*output = tmp_result;
}

