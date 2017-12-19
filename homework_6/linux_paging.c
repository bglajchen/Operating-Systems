#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

bool intCheck(char number[])
{
	int i = 0;

	// check for negative numbers
	if (number[0] == '-')
		i = 1;
	
	if (isdigit(number[i]))
		return 1;
	return 0;
}

int randNum(int num) 
{
	return ((rand() << 2) % num);
}

void toTop(int index, int length, int* input)
{
	int temp = input[index];
	int i = index;
	for (i; i > 0; i--)
	{
		input[i] = input[i - 1];
	}
	input[0] = temp;
}

void push (int toPush, int length, int* input)
{
	int i = length - 1;
	for (i; i > 0; i--)
	{
		input[i] = input[i - 1];
	}
	input[0] = toPush;
}

int screenTest(int pageSize, int logMemSize, int frameNum, int isLRU)
{
	int faultTracker = 0;
	int* frames = malloc(sizeof(int) * frameNum);

	int i = 0;
	for (i; i < frameNum; i++)
	{
		frames[i] = -1;
	}
	
	int address = 0;
	int reference = 0;
	
	i = 0;
	for (i; i < 1000000; i++)
	{
		address = randNum(logMemSize);
		reference = address / pageSize;
		int count = 0;
		int found = 0;
		int full = 0;

		while (count < frameNum && !found)
		{
			found = frames[count] == reference;
			if (found && isLRU)
			{
				toTop(count, frameNum, frames);
			}
			
			count++;
		}
		
		if (!found)
		{
			push(reference, frameNum, frames);
			faultTracker++;
		}
	}
	
	return faultTracker;
}

int main (int argc, char* argv[]) 
{
	// check for correct argument count
	if  (argc != 5)
	{
		printf("ERROR: incorrect number of arguments\n");
		exit(EXIT_FAILURE);
	}

	// check all arguments are integers
	if (!intCheck(argv[1]) || !intCheck(argv[2]) || !intCheck(argv[4]))
	{
		printf("ERROR: ensure arguments are integers\n");
		exit(EXIT_FAILURE);
	}
	
	// check for algortthm choice
	if (strcmp(argv[3], "FIFO") != 0 
		&& strcmp(argv[3], "fifo") != 0 
		&& strcmp(argv[3], "LRU") != 0 
		&& strcmp(argv[3], "lru") != 0)
	{
		printf("ERROR: ensure page type is correct (LRU or FIFO)\n");
		exit(EXIT_FAILURE);
	}
	
	// set variables
	int pageSize, logMemSize, frameNum, isLRU;
	pageSize = 1 << atoi(argv[1]);
	logMemSize = 1 << atoi(argv[2]);
	frameNum = atoi(argv[4]);
	isLRU = (strcmp(argv[3], "LRU") == 0 || strcmp(argv[3], "lru") == 0);

	// seed raandom number generator
	srand((unsigned)time(NULL));

	// run test
	int i = 1;
	for (i; i <= 50; i++)
	{
		int faults = screenTest(i, pageSize, logMemSize, isLRU);
		printf("%d faults detected using %s algorithm with %d page frames\n", faults, argv[3], i);
	}
	
	system("pause");
	return 0;
}
