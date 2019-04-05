#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct Node {
	int data;
	struct Node* next;
};

stcruct Node*head;
pthread_rwlock_t rwlock;


//variables for no of member insert and delete opertions and no of threads. 
//these values are taken from given commands
int noOfMember;
int noOfInsert;
int noOfDelete;
int noOfThreads;

int i, j;
int data[1000]; //data set to be inserted on linkedlist initially
int testData[10000];//data set to be used for perform m operations on linkedlist 

void *ProcessOperation(void* rank);
int Member(int val);
int Insert(int val);
int Delete(int val);
long getCurrentTime(void);

int main(int argc, char* argv[]) {

	if (argc != 5) {
        printf("Please follow the command instruction");
        exit(0);
    }

    //Convert given fractions of operations of each type into floating-point number 
	noOfMember = strtod(argv[1], NULL) * 10000;
	noOfInsert = strtod(argv[2], NULL) * 10000;
	noOfDelete = strtod(argv[3], NULL) * 10000;

    //Convert given no of threads into floating-point number
	noOfThreads = strtod(argv[4], NULL);

	pthread_rwlock_init(&rwlock, NULL);

	//Generate random number within 0 and 2 16 – 1 without repetition for data set
	for (i = 0; i < 1000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (data[j] == temp) { found = 1; break;}
			}
			if (found == 0) { data[i] = temp; break;}
		}
	}

	//Generate random number within 0 and 2 16 – 1 without repetition for testData set
	for (i = 0; i < 10000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (testData[j] == temp) { found = 1; break;}
			}
			if (found == 0) { testData[i] = temp; break;}
		}
	}

	//populate linked list with 1000 random values
    	for (i = 0; i < 1000; i++)
        	Insert(data[i]);

	long thread;
	pthread_t* thread_handles;
	thread_handles = malloc (noOfThreads * sizeof(pthread_t));

	//Insert,Delete,Member operations in linked list starts here
    	long startTime, endTime, timeSpent;

	//get the time before perform m operations
	startTime = getCurrentTime();
	for (thread = 0; thread < noOfThreads; thread++)
		pthread_create(&thread_handles[thread], NULL, ProcessOperation, (void*) thread);

	for (thread = 0; thread < noOfThreads; thread++)
		pthread_join(thread_handles[thread], NULL);


	//--------
	endTime = getCurrentTime();
	timeSpent = endTime - startTime;
	free(thread_handles);
	printf("%ld", timeSpent);
	return 0;
}

long getCurrentTime() {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // caculate milliseconds
	return milliseconds;
}

void *ProcessOperation(void* rank) {

	long my_rank = (long) rank;
	int i, offset = (10000 * my_rank) / noOfThreads;
	int my_last_i = offset + (10000 / noOfThreads);
	long insertOperation = noOfInsert / noOfThreads;
	long deleteOperation = noOfDelete / noOfThreads;
	long memberOperation = noOfMember / noOfThreads;

	for (i = offset; i < my_last_i; i++) {
		if (i < offset + insertOperation) {	//insert operation
			pthread_rwlock_wrlock(&rwlock);
			Insert(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
		else if (i < offset + insertOperation + deleteOperation) {	//delete operation
			pthread_rwlock_wrlock(&rwlock);
			Delete(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
		else {
			pthread_rwlock_rdlock(&rwlock);
			Member(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
	}
	return NULL;
}

int Insert(int value)
{
	struct Node*curr_p = head;
	struct Node*pred_p = NULL;
	struct Node*temp_p;

	while (curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p == NULL || curr_p->data > value)
	{
		temp_p = malloc(sizeof(struct Node));
		temp_p->data = value;
		temp_p->next = curr_p;

		if (pred_p == NULL) /** New first node */
		{
			head = temp_p;
		}
		else
		{
			pred_p->next = temp_p;
		}
		return 1;
	}
	else /* value already in list*/
	{
		return 0;
	}
}

int Member(int value)
{
	struct Node*curr_p = head;

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

int Delete(int value)
{
	struct Node*curr_p = head;
	struct Node*pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->next;
	}
	if (curr_p != NULL && curr_p->data == value)
	{
		if (pred_p == NULL) /** deleting first node in list */
		{
			head = curr_p->next;
			free(curr_p);
		}
		else
		{
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	}
	else /* Value isn't in list */
	{
		return 0;
	}
}