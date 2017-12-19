/*
	@author: Brandon Glajchen
	@date: 10/20/15	
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#include <sys/types.h>

#define MEM_BUFF 65536
#define ASCII 128
#define THREAD_COUNT 8

// struct to hold start and stop values for threads
struct infoStruct
{
	pthread_t threads[THREAD_COUNT];
	int start[THREAD_COUNT];
	int end[THREAD_COUNT];
};

// globabl variables
int asciiArray[ASCII][2];
char buffer[MEM_BUFF];
struct infoStruct positions;
pthread_mutex_t count_mutex;

// count the number of ASCII characters in a given space
void *asciiCounter(void *threadID)
{
	int start, end, thread, count;
	long tid;
	tid = (long) threadID;
	start = positions.start[tid];
	end = positions.end[tid];
	thread = positions.threads[tid];
	for (start; start < end; start++)
	{
		count = 0;
		for (count; count < ASCII; count++)
		{
			if ( ((char) count) == (buffer[start]) )
			{
				pthread_mutex_lock(&count_mutex);
					asciiArray[count][1]++;
				pthread_mutex_unlock(&count_mutex);
			}
		}
	}

	pthread_exit(NULL);
}

// main method
int main (int argc, char* argv[])
{
	// variable declaration
	FILE *file;
	char tempChar;
	int countInt, response;
	long countLong;

	// check for only one file used as argument
	if (argc != 2)
	{
		printf("INPUT_ERROR: Only one file may be read at a time.\n");
		exit(EXIT_FAILURE);
	}

	// check whether selected file exists
	if ( !(file = fopen(argv[1], "rt") ) ) 
	{
		printf("FILE_ERROR: File %s could not be found.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	// read file lines into buffer
	countInt = 0;
	while ( (tempChar = fgetc(file) ) != EOF )
	{
		buffer[countInt] = tempChar;
		countInt++;
	}

	// fill buffer with ASCII characters
	for (countInt = 0; countInt < ASCII; countInt++)
	{
		asciiArray[countInt][0] = (char) countInt;
		asciiArray[countInt][1] = 0;
	}
	
	// assign start and end values for struct array
	for (countInt = 0; countInt < THREAD_COUNT; countInt++)
	{
		positions.start[countInt] = ( (ASCII / THREAD_COUNT) * countInt);
		positions.end[countInt] = ( (ASCII / THREAD_COUNT) * (countInt + 1) );
		countLong = countInt;
		response = pthread_create(&positions.threads[countInt], NULL, asciiCounter, (void *) countLong);
	}

	// print results
	for (countInt = 0; countInt < ASCII; countInt++)
	{
			printf("0x%02x appeared...\t%d times\n", countInt, asciiArray[countInt][1]);
	}

	pthread_exit(NULL);
}
