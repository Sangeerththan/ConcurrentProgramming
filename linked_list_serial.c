#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define min 0
#define max 65536

/* This program implements linkedlist adn it's operations as a serial program */

/* this Node struct used to represent data and pointer in linked list. these nodes will create chained linked 
list */
struct Node
{
	int data;
	struct Node* pointerToNext;
};

/* function deculartions */
int Insert(int value, struct Node** head);
int Member(int value, struct Node* head);
int Delete(int value, struct Node** head);
long getCurrentTime(void);

int main(int argc, char *argv[])
{
	int i, j;
	int noOfMember;
	int noOfInsert;
	int noOfDelete;

	//Convert given fractions of operations of each type into floating-point number 
	noOfMember = strtod(argv[1], NULL) * 10000;
	noOfInsert = strtod(argv[2], NULL) * 10000;
	noOfDelete = strtod(argv[3], NULL) * 10000;



	int data[1000]; //data set to be inserted on linkedlist initially
	int testData[10000]; //data set to be used for perform m operations on linkedlist 
	struct Node*  head;
	int n2 = sizeof(data);
	int n3;
	int n4 = 0;


	
//Generate random number within 0 and 2 16 – 1 without repetition for data set
    for (i = 0; i < 1000; i++) {
        while (1) {
            int temp = rand() % 65536;
            int found = 0;
            for (j = 0; j < i; j++) {
                if (data[j] == temp) { found = 1;printf("SAME VALUE FOUND"); n3 = n3 + 1; break; 	printf("%ld\n", i); printf("INSIDE J AND J IS");  printf("%ld\n", j); printf(" AND THE TEMP VAL IS");	 printf("%ld\n", temp);}

		 //printf(temp);
            }
            if (found == 0) { data[i] = temp; 	n4 = n4 + 1;   printf("%ld\n", i); printf("not INSIDE J AND "); printf(" AND THE TEMP VAL IS");	 printf("%ld\n", temp); break;}
        }
    }
	int n1 = sizeof(data);
        printf("the value of n2 is : ");printf("%ld\n", n2);
	printf("the value of n1 is : ");printf("%ld\n", n1);
	printf("the value of 3999th element in data array is : ");printf("%ld\n", data[1000 - n3]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - n3 -1]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - n3 -2]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - 1]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - 2]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - 3]);
	printf("the value of 1999th element in data array is : ");printf("%ld\n", data[1000 - n3 +1 ]);
	printf("the value of 1th element in data array is : ");printf("%ld\n", data[1]);
	printf("the value of n3 is : ");printf("%ld\n", n3);
	printf("the value of n4 count is : ");printf("%ld\n", n4);

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

	for (i = 0; i < 1000; i++){
		Insert(data[i], &head);
		printf("the value of i is : ");printf("%ld\n", i);
		printf("the value of data is : ");printf("%ld\n", data[i]);
		printf("the value of head is : ");printf("%ld\n", &head);
	}
		//printf(data[i]);



	//Insert,Delete,Member operations in linked list
	long startTime, endTime, timeSpent;

	//get the time before perform m operations
	startTime = getCurrentTime();
	
	for (i = 0; i < 10000; i++) {
		if (i < noOfInsert) {
			Insert(testData[i], &head);
		} else if (i < noOfInsert + noOfDelete) {
			Delete(testData[i], &head);
		} else {
			Member(testData[i], head);
		}
	}

	//get the time after all m operations
	endTime = getCurrentTime();

	//Calculate time taken for m operations
	timeSpent = endTime - startTime;
	printf("TIME SPENT IS:");
	printf("%ld\n", timeSpent);

	//printf("%ld\n", testData[0]);
	return 0;
}

long getCurrentTime() {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // caculate milliseconds
	return milliseconds;
}

int Insert(int value, struct Node**head)
{
	struct Node*curr_p = *head;
	struct Node*pred_p = NULL;
	struct Node*temp_p;

	while (curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->pointerToNext;
	}

	if (curr_p == NULL || curr_p->data > value)
	{
		temp_p = malloc(sizeof(struct Node));
		temp_p->data = value;
		temp_p->pointerToNext = curr_p;

		if (pred_p == NULL) 
		{
			*head = temp_p;
		}
		else
		{
			pred_p->pointerToNext = temp_p;
		}
		return 1;
	}
	else 
	{
		return 0;
	}
}

int Member(int value, struct Node*head)
{
	struct Node*curr_p = head;

	while (curr_p != NULL && curr_p->data < value)
	{
		curr_p = curr_p->pointerToNext;
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

int Delete(int value, struct Node**head)
{
	struct Node*curr_p = *head;
	struct Node*pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->pointerToNext;
	}
	if (curr_p != NULL && curr_p->data == value)
	{
		if (pred_p == NULL)
		{
			*head = curr_p->pointerToNext;
			free(curr_p);
		}
		else
		{
			pred_p->pointerToNext = curr_p->pointerToNext;
			free(curr_p);
		}
		return 1;
	}
	else 
	{
		return 0;
	}
}
