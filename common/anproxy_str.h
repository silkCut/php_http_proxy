#ifndef __ANPROXY_STR_H
#define __ANPROXY_STR_H
#endif

int str_token_count(char delimiter, char* str);
void str_copy_by_len(char* dest, const char* src, int len);
void str_copy_by_start_end(char* dest, const char* src, int start, int end);
int find_delimiter_by_pos(char* src, char delimiter, int pos);
void print_string_array(char** string_array, int len);
void free_string_array(char** string_array, int len);
void str_split(char delimiter, char* str, char*** output, int* count);
