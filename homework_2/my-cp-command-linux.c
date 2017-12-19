/* My imitation of a 'cp' command. Must display bytes copied, and not damage
 * any other files that may have the same name.*/
/* Author: Brandon Glajchen*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

int main (int argc, char* argv[]) {

	int result;
	int resultWrite;
	int resultRead;
	ssize_t byteRead = 0;
	ssize_t byteWrite = 0;
	ssize_t totalByteWritten = 0;
	char buffer[BUFFER_SIZE];

	// check to see if correct amount of argument
	if (argc < 3) {
		printf("To use the program, type: './mycp <filename> <filename>'\n");
		exit(EXIT_FAILURE);
	}

	// check whether the first file exists
	result = access(argv[1], F_OK);
	if (result == -1) {
		printf("File %s could not be found!.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	// check whether the second file exists
	result = access(argv[2], F_OK);
	if (result == 0) {
		printf("File %s already exists! Please choose a different name.\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	// check if the first file can open
	resultRead = open(argv[1], O_RDONLY);
	if (resultRead == -1) {
		printf("File %s was unable to be created!\n", argv[1]);
	}

	// check if the second file can be created
	resultWrite = open(argv[2], O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
	if (resultWrite == -1) {
		printf("File %s was unable to be created!\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	// copying content from the first file to the second
	while (true) {
		byteRead = read(resultRead, buffer, BUFFER_SIZE);
		if (byteRead <= 0) {
			if (byteRead < 0) {
				printf("File %s was unable to be read!\n", argv[1]);
				exit(EXIT_FAILURE);
			}
			break;
		}

		byteWrite = write(resultWrite, buffer, byteRead);
		if (byteWrite < 0) {
			printf("File %s was unable to be written!\n", argv[2]);
			exit(EXIT_FAILURE);
		}

		totalByteWritten += byteWrite;
	}

	close(resultRead);
	close(resultWrite);
	
	printf("copied %zd bytes\n", totalByteWritten);

	return EXIT_SUCCESS;
}
