#include"main.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
    Dl *head = NULL;
    Dl *tail = NULL;
    int created = 0;

    if(argc < 2){
        printf("Usage: %s file1.txt file2.txt ...\n", argv[0]);
        return FAILURE;
    }

    hash_t *arr = malloc(27 * sizeof(hash_t));

    if (arr == NULL)
        return FAILURE;

    for (int i = 0; i < 27; i++)
    {
        arr[i].index = i;
        arr[i].main_link = NULL;
    }

    /* argv contains any number of files */
    if (file_handling(&head, &tail, argv) != SUCCESS)
    {
        free(arr);
        return FAILURE;
    }

    int choice;

    while (1)
    {
        printf("\n1. CREATE DATABASE\n");
        printf("2. DISPLAY DATABASE\n");
        printf("3. UPDATE DATABASE\n");
        printf("4. SEARCH\n");
        printf("5. SAVE DATABASE\n");
        printf("6. EXIT\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            /* non-number typed: stop if input ended, else clear it */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            if (ch == EOF)
            {
                free_all(arr, 27, head);
                free(arr);
                return FAILURE;
            }

            printf("Invalid choice\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                if (created == 1)
                {
                    printf("Database already created\n");
                    break;
                }

                if(create_db(arr, 27, head, tail)==SUCCESS)
                {
                    created = 1;
                    printf("Database created successfully\n");
                }
                else
                    printf("Database creation failed\n");
                break;

            case 2:
                display_db(arr, 27);
                break;

            case 3:
                if (created == 0)
                {
                    printf("Create the database first\n");
                    break;
                }

                update_db(arr, 27, &head, &tail);
                break;

            case 4:
            {
                char word[30];

                printf("Enter the word to search in database:\n");
                scanf("%29s", word);

                if(search_db(arr, word) == SUCCESS){
                    printf("\n");
                    printf("Your DATA  is found \n");
                    printf("\n");
                }
                else{
                    printf("no data found");
                    printf("\n");
                }
                break;
            }

            case 5:
                if(save_db(arr, 27) != SUCCESS){
                    printf("Database save failed\n");
                }
                break;

            case 6:
                free_all(arr, 27, head);
                free(arr);
                return SUCCESS;

            default:
                printf("Invalid choice\n");
        }
    }
}