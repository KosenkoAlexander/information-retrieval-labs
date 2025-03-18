#include "data_structures.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../data_structures/string.h"
#include <stdio.h>

struct inverted_index init_inverted_index(){
    struct inverted_index result;
    result.len = 0;
    result.len_assigned = 4;
    result.terms = malloc(result.len_assigned*sizeof(char*));
    return result;
}

void inverted_index_add_term(struct inverted_index *index, char *term){ //assuming that postings are not initialized yet
    if(index->len_assigned<=index->len){
        index->len_assigned*=2;
        index->terms = realloc(index->terms, index->len_assigned*sizeof(char*));
    }
    index->terms[index->len] = term;
    index->len++;
}

void inverted_index_crop(struct inverted_index *index){ //assuming that postings are not initialized yet
    index->len_assigned=index->len;
    index->terms = realloc(index->terms, index->len_assigned*sizeof(char*));
}

void inverted_index_init_postings(struct inverted_index *index){
    index->postings = malloc(index->len_assigned*sizeof(struct uint_arr_list));
    for(int i=0; i<index->len; i++){
        index->postings[i] = init_uint_arr_list();
    }
}

void free_inverted_index(struct inverted_index *index){
    for(int i=0; i<index->len; i++){
        free(index->terms[i]);
        free(index->postings[i].elements);
    }
    free(index->terms);
    free(index->postings);
}

void inverted_index_heap_sort_terms(struct inverted_index index){
    unsigned int start = index.len/2, end = index.len;
    while(end>1){
        if(start){
            start--;
        }else{
            end--;
            char *temp=index.terms[0];
            index.terms[0]=index.terms[end];
            index.terms[end]=temp;
        }
        unsigned int root=start, child = root*2+1;
        while(child<end){
            if(child+1<end && strcmp(index.terms[child], index.terms[child+1])<0){
                child++;
            }
            if(strcmp(index.terms[root], index.terms[child])<0){
                char *temp=index.terms[root];
                index.terms[root]=index.terms[child];
                index.terms[child]=temp;
                root = child;
                child = root*2+1;
            }else{
                break;
            }
        }
    }
}

int strcmp_voidp(const void * a, const void* b){
    return strcmp(*(const char**)a, *(const char**)b);
}

int inverted_index_search(struct inverted_index index, char* key){
    char **found = bsearch(&key, index.terms, index.len, sizeof(char*), strcmp_voidp);
    if(found){
        return found - index.terms;
    }
    return -1;
}

struct uint_arr_list* inverted_index_get_postings(struct inverted_index index, char* key){
    char **found = bsearch(&key, index.terms, index.len, sizeof(char*), strcmp_voidp);
    if(found){
        return index.postings+(found - index.terms);
    }
    return NULL;
}

void disjunction_add_term(struct disjunction *d, struct term_query term){
    if(d->len>=d->len_assigned){
        d->len_assigned*=2;
        d->components = realloc(d->components, d->len_assigned*sizeof(struct term_query));
    }
    d->components[d->len]=term;
    d->len++;
}

void CNF_add_disjunction(struct CNF_query *CNF, struct disjunction d){
    if(CNF->len>=CNF->len_assigned){
        CNF->len_assigned*=2;
        CNF->components = realloc(CNF->components, CNF->len_assigned*sizeof(struct disjunction));
    }
    CNF->components[CNF->len]=d;
    CNF->len++;
}

struct disjunction init_disjunction(){
    struct disjunction result;
    result.len=0;
    result.len_assigned=1;
    result.components = malloc(result.len_assigned*sizeof(struct term_query));
    return result;
}

struct CNF_query init_CNF_query(){
    struct CNF_query result;
    result.len=0;
    result.len_assigned=1;
    result.components = malloc(result.len_assigned*sizeof(struct disjunction));
    return result;
}

struct CNF_query query_from_str(char *str){
    struct CNF_query query = init_CNF_query();
    struct disjunction current_d;
    char in_d=0;
    char positive=1;
    while(*str){
        if(*str=='!') positive=0;
        else if(*str=='('){
            in_d=1;
            current_d = init_disjunction();
        }else if(*str==')'){
            in_d=0;
            CNF_add_disjunction(&query, current_d);
        }else if(isalnum(*str)){
            char *ptr=str;
            while(isalnum(*str))str++;
            char *t = malloc(str-ptr+1);
            memcpy(t, ptr, str-ptr);
            t[str-ptr]='\0';
            struct term_query term;
            term.term = t;
            term.positive=positive;
            positive=1;
            if(in_d){
                disjunction_add_term(&current_d, term);
            }else{
                current_d = init_disjunction();
                disjunction_add_term(&current_d, term);
                CNF_add_disjunction(&query, current_d);
            }
            str--;
        }
        str++;
    }
    return query;
}

void index_document_iterator(struct inverted_index index, char* (*next_document)(void *args), void *args){
    char *document =next_document(args);
    unsigned int id=0;
    while(document){
        char *p2=document;
        while(*p2&&isspace(*p2)){
            p2++;
        }
        char *p1=p2;
        while(*p1){
            while(*p2&&!isspace(*p2)){
                p2++;
            }
            *p2++='\0';
            struct uint_arr_list* postings = inverted_index_get_postings(index, p1);
            if(postings){
                if(postings->len==0||postings->elements[postings->len-1]!=id){
                    uint_arr_list_append(postings, id);
                }
            }
            while(*p2&&isspace(*p2)){
                p2++;
            }
            p1=p2;
        }
        id++;
        free(document);
        document = next_document(args);
    }
}

struct uint_arr_list uint_arr_list_union(struct uint_arr_list a, struct uint_arr_list b){
    struct uint_arr_list result = init_uint_arr_list();
    unsigned int *a_end = a.elements+a.len, *b_end=b.elements+b.len;
    while(a.elements<a_end&&b.elements<b_end){
        if(*a.elements>*b.elements){
            uint_arr_list_append(&result, *b.elements);
            b.elements++;
        }else{
            uint_arr_list_append(&result, *a.elements);
            if(*a.elements==*b.elements)b.elements++;
            a.elements++;
        }
    }
    while(a.elements<a_end){
        uint_arr_list_append(&result, *a.elements);
        a.elements++;
    }
    while(b.elements<b_end){
        uint_arr_list_append(&result, *b.elements);
        b.elements++;
    }
    return result;
}

struct uint_arr_list uint_arr_list_complement(struct uint_arr_list a, unsigned int doc_num){
    struct uint_arr_list result = init_uint_arr_list();
    for(unsigned int i=0; i<doc_num; i++){
        if(*a.elements==i){
            a.elements++;
        }else{
            uint_arr_list_append(&result, i);
        }
    }
    return result;
}

struct disjunction_postings{
    unsigned int len;
    struct uint_arr_list** postings;
    char *positives;
    unsigned int estimated_size;
};

int compare_disjunction_postings(const void* a, const void* b){
    unsigned int ai = ((const struct disjunction_postings*)a)->estimated_size, bi = ((const struct disjunction_postings*)b)->estimated_size;
    if(ai<bi) return -1;
    if(ai>bi) return 1;
    return 0;
}

struct uint_arr_list inverted_index_retrieve(struct inverted_index index, struct CNF_query query, unsigned int doc_num){
    struct disjunction_postings *dps = malloc(query.len*sizeof(struct disjunction_postings));
    for(int i=0; i<query.len; i++){
        struct disjunction d = query.components[i];
        struct disjunction_postings dp;
        dp.len = d.len;
        dp.postings = malloc(d.len*sizeof(struct uint_arr_list*));
        dp.positives = malloc(d.len);
        unsigned int estimated_size=0;
        for(int j=0; j<d.len; j++){
            struct term_query t = d.components[j];
            struct uint_arr_list *p = inverted_index_get_postings(index, t.term);
            dp.postings[j]=p;
            dp.positives[j]=t.positive;
            if(p){
                estimated_size+=t.positive?p->len:doc_num-p->len;
            }else if(!t.positive){
                estimated_size+=doc_num;
            }
        }
        dp.estimated_size = estimated_size;
        dps[i]=dp;
    }
    qsort(dps, query.len, sizeof(struct disjunction_postings), compare_disjunction_postings);
    struct disjunction_postings dp0 = *dps;
    struct uint_arr_list *full_list=NULL;
    for(int i=0; i<dp0.len; i++){
        if(!dp0.positives[i]){
            struct uint_arr_list *p = dp0.postings[i];
            if(p){
                *p = uint_arr_list_complement(*p, doc_num);
            }else{
                if(!full_list){
                    full_list = malloc(sizeof(struct uint_arr_list));
                    full_list->len = doc_num;
                    full_list->len_assigned=doc_num;
                    full_list->elements = malloc(doc_num*sizeof(unsigned int));
                    for(unsigned int j=0; j<doc_num; j++){
                        full_list->elements[j] = j;
                    }
                }
                dp0.postings[i]=full_list;
            }
        }
    }
    struct uint_arr_list r;//, *result = *dp0.postings;
    char r_null=1, r_free_needed=0;
    if(*dp0.postings) {
        r_null=0;
        r=**dp0.postings;
    }
    for(int i=1; i<dp0.len; i++){
        if(!r_null){
            if(dp0.postings[i]){
                unsigned int *old_elements = r.elements;
                r = uint_arr_list_union(r, *dp0.postings[i]);
                if(r_free_needed)free(old_elements);
                r_free_needed=1;
            }
        }else{
            if(dp0.postings[i]){
                r = *dp0.postings[i];
                r_null=0;
            }else r_null=1;
        }
    }
    if(r_null){
        return init_uint_arr_list();
    }
    if(!r_free_needed){
        unsigned int *old_elements = r.elements;
        r.elements = malloc(r.len*sizeof(unsigned int));
        memcpy(r.elements, old_elements, r.len*sizeof(unsigned int));
    }
    for(int i=1; i<query.len; i++){
        char *keep = malloc(r.len);
        for(int j=0; j<r.len; j++){
            keep[j] = 0;
        }
        struct disjunction_postings dp = dps[i];
        for(int j=0; j<dp.len; j++){
            if(dp.postings[j]){
                struct uint_arr_list dpj = *dp.postings[j];
                unsigned int *r_ptr = r.elements, *r_end = r_ptr+r.len, *dpj_ptr = dpj.elements ,*dpj_end=dpj_ptr+dpj.len;
                while(r_ptr<r_end&&dpj_ptr<dpj_end){
                    if(*r_ptr>*dpj_ptr){
                        dpj_ptr++;
                    }else{
                        if(*r_ptr==*dpj_ptr){
                            if(dp.positives[j]) keep[r_ptr-r.elements]=1;
                            dpj_ptr++;
                        }else if(!dp.positives[j]) keep[r_ptr-r.elements]=1;
                        r_ptr++;
                    }
                }
                if(!dp.positives[j]){
                    char *k_end = keep+(r_end-r.elements);
                    for(char *k = keep+(r_ptr-r.elements); k<k_end; k++){
                        *k=1;
                    }
                }
            }else if(!dp.positives[j]) goto continuation;
        }
        int cumulative = 0;
        for(int j=0; j<r.len; j++){
            if(keep[j]){
                r.elements[j-cumulative]=r.elements[j];
            }else cumulative++;
        }
        r.len-=cumulative;
        continuation:free(keep);
    }
    for(int i=0; i<query.len; i++){
        struct disjunction_postings dp = dps[i];
        free(dp.postings);
        free(dp.positives);
    }
    free(dps);
    if(full_list) free(full_list);
    return r;
}

void free_CNF_query(struct CNF_query query){
    for(int i=0; i<query.len; i++){
        struct disjunction d=query.components[i];
        for(int j=0; j<d.len; j++){
            free(d.components[j].term);
        }
        free(d.components);
    }
    free(query.components);
}
