#ifndef IRLAB_STRING_H
#define IRLAB_STRING_H
struct string{
    unsigned int len, len_assigned; //strings are ended with '\0' which is not counted in len but is counted in len_assigned
    char* chars;
};

struct string_fixed{
    unsigned int len; //strings are ended with '\0' which is not counted in len
    char *chars;
};

struct str_arr_list{
    unsigned int len, len_assigned;
    char **strs;
};

struct str_arr_fixed{
    unsigned int len;
    char **strs;
};

struct uint_arr_list{
    unsigned int len, len_assigned;
    unsigned int *elements;
};
struct uint_arr_list init_uint_arr_list();
void uint_arr_list_append(struct uint_arr_list* list, unsigned int i);
struct str_arr_list init_str_arr_list();
void str_arr_list_append(struct str_arr_list* list, char* str);
struct string input_string();
struct string concat_str_charp(struct string str, char* to_add);
#endif
