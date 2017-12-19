#include <stdio.h>
#include <string.h>
#include <windows.h>

//set buffer size
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char input[BUFFER_SIZE];
    char copy[BUFFER_SIZE];
    char *my_argv [64];
    int my_argc;
    char *buffer;
    int childProcess = 0;
    char applicationName[] = "C:\\windows\\system32\\cmd.exe";
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    HANDLE ReadHandle;
    HANDLE WriteHandle;
	
start:
    si.cb = sizeof(si);
    GetStartupInfo(&si);
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags = STARTF_USESTDHANDLES;

	
    // user input
    printf("> ");
    fgets(input, BUFFER_SIZE, stdin);
	strncpy(copy, input,BUFFER_SIZE);

	
    //add null terminator
    if(input[strlen(input) - 1] == '\n') {
	input[strlen(input) - 1] = '\0';
    }


    //separate input into a collection of substring
    buffer = strtok(input, " \n\0");
    int i = 0;
    for(; buffer != NULL; i++) {
        my_argv[i] = buffer;
        buffer = strtok(NULL, " \n\0");        
    }
    my_argc = i;
    childProcess = 1;


    //if input is exit, then terminate from shell
    if(strcmp(my_argv[0], "exit") == 0) return 0;

	if (!CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
		fprintf(stderr, "ERROR");
		return 0;
	}

	if (childProcess) {
		char cmdline[1024];
		sprintf(cmdline, "%s /c %s", my_argv[0], copy);
		CreateProcess(applicationName, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		childProcess = 0;
	}

	goto start; 
}