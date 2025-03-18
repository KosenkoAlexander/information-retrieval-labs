#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "information_retrieval/data_structures.h"
#include "data_structures/string.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

struct inverted_index index_from_terms(char *terms){
    struct inverted_index result = init_inverted_index();
    while(*terms&&isspace(*terms)){
        terms++;
    }
    char* p1=terms;
    while(*p1){
        while(*terms&&!isspace(*terms)){
            terms++;
        }
        int len = terms-p1;
        char* term = malloc((len+1));
        memcpy(term, p1, len);
        term[len]='\0';
        inverted_index_add_term(&result, term);
        while(*terms&&isspace(*terms)){
            terms++;
        }
        p1=terms;
    }
    inverted_index_crop(&result);
    inverted_index_heap_sort_terms(result);
    inverted_index_init_postings(&result);
    return result;
}

struct string input_string_from_file(FILE *f){
    int i=0, j = 5;
    char *str = (char*)calloc(j,sizeof(char));
    int c;
    while((c=fgetc(f))!=EOF){
        *(str+i)=c;
        i++;
        if (i >= j){
            j *= 2;
            str = (char*)realloc(str,j);
        }
    }
    *(str+i)='\0';
    struct string result = {i,j,str};
    return result;

}

struct iter_file_list_args{
    struct str_arr_list list;
    int i;
};

char* iter_file_list(void *args){
    struct iter_file_list_args *iter_args = args;
    if(iter_args->i<iter_args->list.len){
        FILE *f = fopen(iter_args->list.strs[iter_args->i], "r");
        struct string str = input_string_from_file(f);
        fclose(f);
        iter_args->i++;
        return str.chars;
    }
    return NULL;
}

char* concat_path(struct string str, char* to_add){
    unsigned int a_len = strlen(to_add), len = str.len+a_len+2;
    char *c = malloc(len);
    memcpy(c, str.chars, str.len);
    *(c+str.len)='/';
    memcpy(c+str.len+1, to_add, a_len);
    *(c+len-1)='\0';
    return c;
}

int main(){
    printf("Input indexing terms\n");
    struct string terms = input_string();
    struct inverted_index index = index_from_terms(terms.chars);
    free(terms.chars);
    printf("\nInput path to documents\n");
    struct string path = input_string();
    DIR* dir;
    struct dirent* in_file;
    dir=opendir(path.chars);
    if(!dir){
        printf("\nDirectory not found\n");
        return 1;
    }
    struct str_arr_list files=init_str_arr_list();
    while((in_file=readdir(dir))){
        if(strcmp(in_file->d_name, ".")&&strcmp(in_file->d_name, "..")){
            str_arr_list_append(&files, concat_path(path, in_file->d_name));
        }
    }
    closedir(dir);
    free(path.chars);
    struct iter_file_list_args args = {files, 0};
    index_document_iterator(index, iter_file_list, &args);
    printf("index content\n");
    for(int i=0; i<index.len; i++){
        printf("%s: [", index.terms[i]);
        for(int j=0; j<index.postings[i].len; j++){
            printf("%u ", index.postings[i].elements[j]);
        }
        printf("]\n");
    }
    while(1){
        printf("\nInput query (empty for exiting)\n");
        struct string q = input_string();
        if(!q.len){
            break;
        }
        struct CNF_query query = query_from_str(q.chars);
        free(q.chars);
        struct uint_arr_list retrieved = inverted_index_retrieve(index, query, files.len);
        free_CNF_query(query);
        for(int i=0; i<retrieved.len; i++){
            char *fname= files.strs[retrieved.elements[i]];
            printf("%s\n",fname);
            FILE *doc_f = fopen(fname, "r");
            struct string doc_s = input_string_from_file(doc_f);
            fclose(doc_f);
            printf("%s\n\n",doc_s.chars);
            free(doc_s.chars);
        }
        free(retrieved.elements);
    }
}
