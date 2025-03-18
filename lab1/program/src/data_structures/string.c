#include "./string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct uint_arr_list init_uint_arr_list(){
    struct uint_arr_list result;
    result.len=0;
    result.len_assigned=4;
    result.elements = malloc(result.len_assigned*sizeof(unsigned int));
    return result;
}

void uint_arr_list_append(struct uint_arr_list* list, unsigned int i){
    if(list->len>=list->len_assigned){
        list->len_assigned*=2;
        list->elements=realloc(list->elements, list->len_assigned*sizeof(unsigned int));
    }
    list->elements[list->len]=i;
    list->len++;
}

struct str_arr_list init_str_arr_list(){
    struct str_arr_list result;
    result.len=0;
    result.len_assigned=4;
    result.strs = malloc(result.len_assigned*sizeof(char*));
    return result;
}

void str_arr_list_append(struct str_arr_list* list, char *str){
    if(list->len>=list->len_assigned){
        list->len_assigned*=2;
        list->strs=realloc(list->strs, list->len_assigned*sizeof(char*));
    }
    list->strs[list->len]=str;
    list->len++;
}

struct string input_string(){
    int i=0, j = 5;
    char *str = (char*)calloc(j,sizeof(char));
    while((*(str+i)=getchar())!='\n'){
        i++;
        if (i >= j){
            j *= 2;
            str = (char*)realloc(str,j*sizeof(char));
        }
    }
    *(str+i)='\0';
    struct string result = {i,j,str};
    return result;
}

struct string concat_str_charp(struct string str, char* to_add){
    unsigned int a_len = strlen(to_add), len = str.len+a_len+1;
    char *c = malloc(len);
    memcpy(c, str.chars, str.len);
    memcpy(c+str.len, to_add, a_len);
    *(c+len-1)='\0';
    struct string result = {len,len,c};
    return result;
}
