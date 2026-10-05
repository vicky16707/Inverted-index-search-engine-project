#ifndef MAIN_H
#define MAIN_H


#define SUCCESS 0
#define FAILURE -1
#define FILE_EMPTY 2
#define NODATAFOUND 3
#define HEADER "# Word : File Count : Word Count : File Name #"


typedef struct sub_node{

    int w_count;
    char f_name[20];   
    struct sub_node *sub_nodelink;

}snode_t;

typedef struct main_node{

    int f_count;
    char word[20];
    struct main_node *main_nodelink;
    snode_t *sub_nodelink;

}mnode_t;


typedef struct hash{//hash table
    int index;
    mnode_t * main_link;
}hash_t;

typedef struct doubl{
    struct doubl *prev;
    char file_name[20];
    struct doubl *next;

}Dl;

//function prototype

int file_handling(Dl **head,Dl **tail,char *argv[]);

int create_db(hash_t *arr,int size,Dl *head,Dl *tail);
int display_db(hash_t *arr, int size);
int search_db(hash_t *arr, char *word);
int update_db(hash_t *arr, int size, Dl **head, Dl **tail);
int save_db(hash_t *arr, int size);

void free_all(hash_t *arr, int size, Dl *head);
int insert_last(Dl **head, Dl **tail, char *filename);
void to_lower(char *s);
#endif