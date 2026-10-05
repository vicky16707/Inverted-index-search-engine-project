#include "main.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

int insert_last(Dl **head, Dl **tail, char *filename)
{
    Dl *new=malloc(sizeof(Dl));
    
    if(new==NULL){
        return FAILURE;
    }  
    strcpy(new->file_name, filename);   
    new->next=NULL;
    
    if(*head==NULL&&*tail==NULL)
    {
        new->prev=NULL;
        *head=new;
        *tail=new;       
    }
    else{
        new->prev=*tail;
        (*tail)->next=new;
        *tail=new;       
    }
    return SUCCESS;

}
void free_all(hash_t *arr, int size, Dl *head)
{
    int i;

    for (i = 0; i < size; i++)
    {
        mnode_t *mtemp = arr[i].main_link;

        while (mtemp != NULL)
        {
            snode_t *stemp = mtemp->sub_nodelink;

            while (stemp != NULL)
            {
                snode_t *snext = stemp->sub_nodelink;
                free(stemp);
                stemp = snext;
            }

            mnode_t *mnext = mtemp->main_nodelink;
            free(mtemp);
            mtemp = mnext;
        }

        arr[i].main_link = NULL;
    }

    while (head != NULL)
    {
        Dl *next = head->next;
        free(head);
        head = next;
    }
}
void to_lower(char *s)
{
    while (*s)
    {
        *s = tolower((unsigned char)*s);
        s++;
    }
}