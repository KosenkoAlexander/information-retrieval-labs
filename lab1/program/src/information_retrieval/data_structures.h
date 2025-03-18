#include "../data_structures/string.h"



struct record{
    struct string_fixed term; //char* better?
    struct uint_arr_list postings;
};

struct inverted_index{
    unsigned int len, len_assigned;
    char **terms;
    struct uint_arr_list *postings;
};

struct term_query{
    char* term;
    char positive;
};

struct disjunction{
    unsigned int len, len_assigned;
    struct term_query *components;
};

struct CNF_query{
    unsigned int len, len_assigned;
    struct disjunction *components;
};

struct marked_id_list{
    unsigned int len, *elements;
    char* mark;
};

struct inverted_index init_inverted_index();
void inverted_index_add_term(struct inverted_index *index, char *term);
void inverted_index_crop(struct inverted_index *index);
void inverted_index_init_postings(struct inverted_index *index);
void free_inverted_index(struct inverted_index *index);
void inverted_index_heap_sort_terms(struct inverted_index index);
int inverted_index_search(struct inverted_index index, char* key);
struct uint_arr_list* inverted_index_get_postings(struct inverted_index index, char* key);
void index_document_iterator(struct inverted_index index, char* (*next_document)(void *args), void *args);
struct CNF_query query_from_str(char *str);
struct uint_arr_list inverted_index_retrieve(struct inverted_index index, struct CNF_query query, unsigned int doc_num);
void free_CNF_query(struct CNF_query query);
