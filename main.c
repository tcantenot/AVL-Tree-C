#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "avl_tree.h"

#define ARRAY_FACTORS_SIZE 64


typedef struct prime_data
{
    uint64_t number;
    uint64_t factors[ARRAY_FACTORS_SIZE];
} prime_data;

int random_number(int min, int max)
{
    return (int) rand() % (max - min + 1) + min;
}

void * key_int(void * data)
{
    return (void *) data;
}

int less_int(void * lhs, void * rhs)
{
    /*printf("lhs : %d < rhs : %d ? \n", *(int *) lhs, *(int *) rhs);*/
    return (*(int *) lhs) < (*(int *) rhs);
}


void insert_int(void * value, void ** node)
{
    ((AvlNode *)(*node))->value = value;
}


void print_int(void * v)
{
    printf("%d ", *(int *) v);
}


void free_int(void * v)
{
    free((int *) v);
}


void * key_prime(void * data)
{
    return (void *) &(((prime_data *) data)->number);
}

int less_prime(void * lhs, void * rhs)
{
    return *((uint64_t *) lhs) < *((uint64_t *) rhs);
}

void insert_prime(void * value, void ** node)
{
    ((AvlNode *)(*node))->value = value;
}


void print_prime(void * v)
{
    printf("%" PRIu64 " ", ((prime_data *) v)->number);
}

void free_prime(void * v)
{
    free((prime_data *) v);
}



int main(int argc, const char *argv[])
{
    if(argc > 2)
    {
        AvlTree tree = { NULL, key_prime, less_prime, insert_prime, print_prime, free_prime };

        srand(time(0));

        printf("Size of AvlNode : %lu\n", sizeof(AvlNode));

        int i;
        for(i = 0; i < atoi(argv[1]); i++)
        {
            prime_data * data = (prime_data *) malloc(sizeof(prime_data));
            data->number = random_number(0, 10000000);
            insert(data, &tree);
        }

        printf("Height : %d\n", height(&tree));


        int found = 0;
        int h;
        for(h = 0; h < atoi(argv[2]); h++)
        {
            uint64_t nb = (uint64_t) random_number(0, 10000000);
            if(search(&nb, &tree) != NULL)
                found++;
        }

        printf("Found : %d/%d\n", found, atoi(argv[2]));

        empty(&tree);
    }
    else if(argc == 2)
    {
        AvlTree tree = { NULL, key_int, less_int, insert_int, print_int, free_int };

        int i;
        for(i = 0; i < atoi(argv[1]); i++)
        {
            int * nb = (int *) malloc(sizeof(int));
            *nb = i;
            insert(nb, &tree);
        }

        printf("Pre-order\n");
        pre_order(&tree);
        printf("\n");

        printf("In order\n");
        in_order(&tree);
        printf("\n");

        printf("Post-order\n");
        post_order(&tree);
        printf("\n");

        empty(&tree);
    }
    else
    {
        printf("How to use it: \n\n");

        printf("./avl <max> \n-> Print the pre-order, in order and post-order traversal "
            "of an avl tree filled with the value from 0 to <max>\n\n"),

        printf("./avl <nb_insertion> <nb_search> \n-> Do <nb_insertion> "
            "insertions of random numbers in an avl tree and then <nb_search> searches "
            "of random numbers in this avl.\n");
    }

    return 0;
}
