#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int file_handling(Dl **head, Dl **tail, char *argv[])
{
    int i = 1;

    while (argv[i] != NULL)
    {
        if (strlen(argv[i]) > 4 &&
            strncmp(argv[i] + strlen(argv[i]) - 4, ".txt", 4) == 0)
        {
            FILE *fptr = fopen(argv[i], "r");

            if (!fptr)
            {
                printf("Error: Could not open file '%s'\n", argv[i]);
                i++;
                continue;
            }

            fseek(fptr, 0, SEEK_END);
            int pos = ftell(fptr);

            if (!pos)
            {
                fclose(fptr);
                 printf("Warning: File '%s' is empty, skipping...\n", argv[i]);
                i++;
                continue;
            }

            fclose(fptr);

            if (*head == NULL && *tail == NULL)
            {
                insert_last(head, tail, argv[i]);
            }
            else
            {
                int duplicate = 0;
                Dl *temp = *head;

                while (temp != NULL)
                {
                    if (strcmp(temp->file_name, argv[i]) == 0)
                    {
                        duplicate = 1;
                        printf("Error: this file is already present\n");
                        break;
                    }

                    temp = temp->next;
                }

                if (duplicate == 0)
                {
                    insert_last(head, tail, argv[i]);
                }
            }
        }

        i++;
    }

    return SUCCESS;
}


int create_db(hash_t *arr, int size, Dl *head, Dl *tail)
{
    (void)size;
    (void)tail;

    Dl *temph = head;
    int index;

    while (temph != NULL)
    {
        FILE *fptr = fopen(temph->file_name, "r");

        if (fptr == NULL)
            return FAILURE;

        char word[20];

        while (fscanf(fptr, "%19s", word) != EOF)
        {
            to_lower(word);

            //Find hash index 

            if (islower(word[0]))
            {
                index = word[0] - 'a';
            }
            else
            {
                index = 26;
            }


            // Hash index has no main node 

            if (arr[index].main_link == NULL)
            {
                mnode_t *newm = malloc(sizeof(mnode_t));
                snode_t *news = malloc(sizeof(snode_t));

                if (newm == NULL || news == NULL)
                {
                    free(newm);
                    free(news);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(newm->word, word);
                newm->f_count = 1;
                newm->main_nodelink = NULL;
                newm->sub_nodelink = news;

                strcpy(news->f_name, temph->file_name);
                news->w_count = 1;
                news->sub_nodelink = NULL;

                arr[index].main_link = newm;
            }


            // Hash index already has main node 

            else
            {
                mnode_t *prevm = NULL;
                mnode_t *tempm = arr[index].main_link;

                int w_found = 0;

                // Search for word 

                while (tempm != NULL)
                {
                    if (strcmp(word, tempm->word) == 0)
                    {
                        w_found = 1;
                        break;
                    }

                    prevm = tempm;
                    tempm = tempm->main_nodelink;
                }


                // Word already exists 

                if (w_found == 1)
                {
                    snode_t *temps = tempm->sub_nodelink;
                    int f_found = 0;

                    /* Search for file */

                    while (temps != NULL)
                    {
                        if (strcmp(temph->file_name, temps->f_name) == 0)
                        {
                            f_found = 1;
                            temps->w_count++;
                            break;
                        }

                        temps = temps->sub_nodelink;
                    }


                    // Same word but new file 

                    if (f_found == 0)
                    {
                        snode_t *news = malloc(sizeof(snode_t));

                        if (news == NULL)
                        {
                            fclose(fptr);
                            return FAILURE;
                        }

                        strcpy(news->f_name, temph->file_name);
                        news->w_count = 1;
                        news->sub_nodelink = NULL;

                        temps = tempm->sub_nodelink;

                        while (temps != NULL && temps->sub_nodelink != NULL)
                        {
                            temps = temps->sub_nodelink;
                        }

                        temps->sub_nodelink = news;

                        tempm->f_count++;
                    }
                }


                // Word does not exist 

                else
                {
                    mnode_t *newm = malloc(sizeof(mnode_t));
                    snode_t *news = malloc(sizeof(snode_t));

                    if (newm == NULL || news == NULL)
                    {
                        free(newm);
                        free(news);
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(newm->word, word);
                    newm->f_count = 1;
                    newm->main_nodelink = NULL;
                    newm->sub_nodelink = news;

                    strcpy(news->f_name, temph->file_name);
                    news->w_count = 1;
                    news->sub_nodelink = NULL;

                     //Connecting new main node 

                    prevm->main_nodelink = newm;
                }
            }
        }

        fclose(fptr);
        temph = temph->next;
    }

    return SUCCESS;
}

int search_db(hash_t *arr, char *word){
    int index;

    to_lower(word);

    if(islower(word[0])){
        index = word[0] - 'a';
    }
    else
    {
        index = 26;
    }
    if(arr[index].main_link!=NULL){
        mnode_t *temp=arr[index].main_link;
        while(temp!=NULL)
        {
            if(strcmp(temp->word,word)==0){
                printf("The word is: %s\n",temp->word);
                printf("File count is:%d\n",temp->f_count);
                snode_t *stemp = temp->sub_nodelink;

                while (stemp != NULL)
                {
                    printf("File: %s  Wordcount: %d\n",
                           stemp->f_name, stemp->w_count);

                    stemp = stemp->sub_nodelink;
                }

                return SUCCESS;
            }
            temp=temp->main_nodelink;
        }
        return NODATAFOUND;

    }
    return NODATAFOUND;
}

int display_db(hash_t *arr, int size)
{
    int i;
    printf("DISPLAYING DATA BASE: \n");

    printf("[Index]\t[word]\t\tfile count\tFile: File_name word_count\n");

    for (i = 0; i < size; i++)
    {
        mnode_t *mtemp = arr[i].main_link;

        while (mtemp != NULL)
        {
            printf("[%d]\t[%s]\t\t%d file/s:\t",
                   i, mtemp->word, mtemp->f_count);

            snode_t *stemp = mtemp->sub_nodelink;

            while (stemp != NULL)
            {
                printf("File: %s %d\t",
                       stemp->f_name, stemp->w_count);

                stemp = stemp->sub_nodelink;
            }

            printf("\n");

            mtemp = mtemp->main_nodelink;
        }
    }

    return SUCCESS;
}

int update_db(hash_t *arr, int size, Dl **head, Dl **tail)
{
    (void)size;

    char svfname[20];
    char word[20];

    printf("Enter the new file name: ");
    scanf("%19s", svfname);

    // Check .txt extension 
    int len = strlen(svfname);

    if (len <= 4 || strcmp(svfname + len - 4, ".txt") != 0)
    {
        printf("Invalid file extension\n");
        return FAILURE;
    }

    // Open new file 
    FILE *fptr = fopen(svfname, "r");

    if (fptr == NULL)
    {
        printf("File does not exist\n");
        return FAILURE;
    }

    // Check empty file 
    fseek(fptr, 0, SEEK_END);

    if (ftell(fptr) == 0)
    {
        printf("File is empty\n");
        fclose(fptr);
        return FAILURE;
    }

    rewind(fptr);

    // Check duplicate file in DLL 
    Dl *temp = *head;

    while (temp != NULL)
    {
        if (strcmp(temp->file_name, svfname) == 0)
        {
            printf("File is already present\n");
            fclose(fptr);
            return FAILURE;
        }

        temp = temp->next;
    }

    // Add new file to DLL 
    if (insert_last(head, tail, svfname) != SUCCESS)
    {
        fclose(fptr);
        return FAILURE;
    }

    /*
      Read ONLY the new file
      and update existing hash table
     */
    while (fscanf(fptr, "%19s", word) != EOF)
    {
        int index;

        to_lower(word);

        if (islower(word[0]))
            index = word[0] - 'a';
        else
            index = 26;

        mnode_t *tempm = arr[index].main_link;
        mnode_t *prev = NULL;

        // Search word in main node 
        while (tempm != NULL)
        {
            if (strcmp(tempm->word, word) == 0)
                break;

            prev = tempm;
            tempm = tempm->main_nodelink;
        }

        // Word already exists 
        if (tempm != NULL)
        {
            snode_t *temps = tempm->sub_nodelink;

            // Search file under this word 
            while (temps != NULL)
            {
                if (strcmp(temps->f_name, svfname) == 0)
                    break;

                temps = temps->sub_nodelink;
            }

            // Same file already exists 
            if (temps != NULL)
            {
                temps->w_count++;
            }
            else
            {
                // New file for existing word 
                snode_t *news = malloc(sizeof(snode_t));

                if (news == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(news->f_name, svfname);
                news->w_count = 1;
                news->sub_nodelink = NULL;

                //* Add new sub node 
                temps = tempm->sub_nodelink;

                while (temps->sub_nodelink != NULL)
                    temps = temps->sub_nodelink;

                temps->sub_nodelink = news;

                //* Increase file count 
                tempm->f_count++;
            }
        }
        else
        {
            // New word 
            mnode_t *newm = malloc(sizeof(mnode_t));
            snode_t *news = malloc(sizeof(snode_t));

            if (newm == NULL || news == NULL)
            {
                free(newm);
                free(news);
                fclose(fptr);
                return FAILURE;
            }

            strcpy(newm->word, word);
            newm->f_count = 1;

            strcpy(news->f_name, svfname);
            news->w_count = 1;
            news->sub_nodelink = NULL;

            newm->sub_nodelink = news;
            newm->main_nodelink = NULL;

            // Link new main node 
            if (arr[index].main_link == NULL)
            {
                arr[index].main_link = newm;
            }
            else
            {
                prev->main_nodelink = newm;
            }
        }
    }

    fclose(fptr);

    printf("Database updated successfully\n");

    return SUCCESS;
}

int save_db(hash_t *arr, int size)
{
    FILE *fp;
    char svf[30];
    int opt;
    while (1)
    {
        printf("Enter the file to save database: ");
        scanf("%29s", svf);

        /* Check .txt extension */
        char *ext = strrchr(svf, '.');

        if (ext == NULL || strcmp(ext, ".txt") != 0)
        {
            printf("Invalid file extension. Enter a .txt file.\n");
            continue;
        }

        /* Check whether file exists */
        fp = fopen(svf, "r");

        if (fp == NULL)
        {
            /* File doesn't exist -> create it */
            fp = fopen(svf, "w");

            if (fp == NULL)
            {
                printf("Error creating file.\n");
                continue;
            }

            printf("New database file created.\n");
            break;
        }

        /* File exists */
        int ch = fgetc(fp);
        fclose(fp);

        if (ch == EOF)
        {
            /* Empty file */
            fp = fopen(svf, "w");

            if (fp == NULL)
                continue;

            break;
        }

        if (ch == '#')
        {
            printf("Database file already contains data.\n");
            printf("1. Overwrite\n");
            printf("2. Append\n");
            printf("Enter option: ");
            scanf("%d", &opt);

            if (opt == 1)
            {
                fp = fopen(svf, "w");
                break;
            }
            else if (opt == 2)
            {
                fp = fopen(svf, "a");
                break;
            }
            else
            {
                printf("Invalid option. Try again.\n");
                continue;
            }
        }
        else
        {
            printf("Invalid database file.\n");
            printf("Please enter the correct file.\n");
            continue;
        }
    }
    for (int i = 0; i < 27; i++){

        mnode_t *mtemp=arr[i].main_link;

        while(mtemp!=NULL){

            fprintf(fp,"#%d : %s : %d ",i,mtemp->word,mtemp->f_count);

            snode_t *stemp=mtemp->sub_nodelink;

            while(stemp!=NULL){

                fprintf(fp," %s : %d ",stemp->f_name,stemp->w_count);

                stemp=stemp->sub_nodelink;

            }
            fprintf(fp,"#\n");

            mtemp=mtemp->main_nodelink;

        }
    }
    
    fclose(fp);

    printf("Database saved successfully.\n");

    return SUCCESS;
}   

