#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct list_node_s
{
    int data;
    struct list_node_s *next;
};
void *thread_function(void *);
int Member(int value);
int Insert(int value);
int Delete(int value);

int n;               //number of values
int m;               //number of operations in each execution
float m_member_frac; //member fraction
float m_insert_frac; //insert fraction
float m_delete_frac; //delete fraction


pthread_mutex_t mutex, mutex1;

int count, count_m, count_i, count_d = 0;
int m_member, m_insert, m_delete;
int thread_count; //number of threads
struct list_node_s *head_p = NULL;
int *opr_values;

int main(int argc, char *argv[])
{ //argc : no. of arguments and argv : vector of arguments
    long thread;
    pthread_t *thread_handles;
    clock_t start, end;
    double cpu_time_used;
    srand(time(NULL));
    int i, ins_value;

    if (argc != 7)
    {
        //error in input values
        fprintf(stderr, "error in input format. correct format: <thread_count> <n> <m> <m_member_frac> <m_insert_frac> <m_delete_frac>\n");
        exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0)
    {
        fprintf(stderr, "error. thread count should be greater than 0\n");

        exit(0);
    }
    n = (int)strtol(argv[2], NULL, 10);
    m = (int)strtol(argv[3], NULL, 10);
    m_member_frac = (float)atof(argv[4]);
    m_insert_frac = (float)atof(argv[5]);
    m_delete_frac = (float)atof(argv[6]);
    if (n < 0 || m < 0 || m_member_frac < 0 || m_insert_frac < 0 || m_delete_frac < 0 || m_member_frac + m_insert_frac + m_delete_frac != 1.0)
    {
        //error
        fprintf(stderr, "error in input values.\n");
        fprintf(stderr, "n: no of initial values in the linkedlist.\n");
        fprintf(stderr, "m: no. of values for carrying out operations\n");
        fprintf(stderr, "m_member_frac: fraction of operations for member function\n");
        fprintf(stderr, "m_insert_frac: fraction of operations for insert function\n");
        fprintf(stderr, "m_delete_frac: fraction of operations for delete function\n");
        exit(0);
    }

    opr_values = malloc(m * sizeof(int));
    m_member = m * m_member_frac;
    m_insert = m * m_insert_frac;
    m_delete = m - (m_member + m_insert);

    for (i = 0; i < n; i++)
    {
        ins_value = rand() % 65535; //value should be between 2^16 - 1
        Insert(ins_value);
    }
    for (i = 0; i < m; i++)
    {
        opr_values[i] = rand() % 65535; //value should be between 2^16 - 1
    }

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    start = clock();
    pthread_mutex_init(&mutex, NULL);
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, thread_function, (void *)thread);
    }
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    FILE *pFile;
    pFile = fopen("data-mutex.txt", "a");
    fprintf(pFile, "%f\n", cpu_time_used);
    printf("execution time is : %f\n", cpu_time_used);
    free(thread_handles);
    return 0;
}
void *thread_function(void *rank)
{

    //int *opr_values = (int *) arg;
    //long my_rank = (long)rank;

    pthread_mutex_lock(&mutex1);
    int i = count++;
    pthread_mutex_unlock(&mutex1);

    while (i < m)
    {
        //only one thread can get access to the linked-list at a time
        if (count_m < sizeof(m_member))
        {
            pthread_mutex_lock(&mutex);
            Member(opr_values[i]);
            pthread_mutex_unlock(&mutex);
        }

        else if (count_i < sizeof(m_insert) + sizeof(m_member))
        {
            pthread_mutex_lock(&mutex);
            Insert(opr_values[i]);
            pthread_mutex_unlock(&mutex);
        }

        else
        {
            pthread_mutex_lock(&mutex);
            Delete(opr_values[i]);
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex1);
        i = count++;
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}
//Member function
int Member(int value)
{
    count_m++;
    struct list_node_s *curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
    {
        curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//insert function
int Insert(int value)
{
    count_i++;
    struct list_node_s **head_pp = &head_p;
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;
    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL) //new first node
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else
    {
        return 0; //value already in the list
    }
}

//delete function
int Delete(int value)
{
    count_d++;
    struct list_node_s **head_pp = &head_p;
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if (curr_p != NULL && curr_p->data == value)
    {
        if (pred_p == NULL)
        {
            *head_pp = curr_p->next; //deleting the first node in the list
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
    {
        return 0; //value not in the list
    }
}