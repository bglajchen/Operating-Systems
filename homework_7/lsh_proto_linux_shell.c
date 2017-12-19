#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 1024
#define TOKEN_BUFFERSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/* Function Declarations for builtin shell commands */
int cd(char  **args);
int help(char **args);
int exit(char **args);

/* List of builtin commands, followed by their corresponding functions */
char *command_list[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin[]) (char **) =
{
	&cd,
	&help,
	&exit
};

int builtin_num()
{
	return sizeof(command_list) / sizeof(char *);
}

int cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	} else
	{
		if (chdir(args[1]) != 0)
		{
			perror("lsh");
		}
	}

	return 1;
}

int help(char **args)
{
	printf("Type program names and arguments, and hit enter.\n");
	printf("THe following commands are built in:\n");

	for (int i = 0; i < num_builtins(); i++)
	{
		printf("\t%s\b", command_list[i]);
	}

	printf("Use the 'man' command for additional information.\n");
	return 1;
}

int exit(char **args)
{
	return 0;
}

int launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0)
	{
		perror("lsh");
	} else
	{
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

char read()
{
	int position = 0;
	char *buffer = malloc(sizeof(char) * BUFFERSIZE);
	int tmp;

	if (!buffer)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		tmp = getchar();

		if ( tmp == EOF | tmp == '\n')
		{
			buffer[position] = '\0';
			return buffer;
		} else
		{
			buffer[position] = tmp;
		}
		position++;

		if (position >= BUFFERSIZE)
		{
			fprintf(stderr, "lsh: allocation error\n");
			exit(EXIT_FAILURE);
		}
	}
}

char **split(char *line)
{
	int position = 0;
	char **tokens = malloc(sizeof(char*) * TOKEN_BUFFERSIZE);
	char *token;

	if(!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= TOKEN_BUFFERSIZE)
		{
			fprintf(stderr, "lsh: allocation error\n");
			exit(EXIT_FAILURE);
		}
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int execute(char **args)
{
	if (args[0] == NULL)
	{
		return 1;
	}

	int i;
	for (i = 0; i < builtin_num(); i++)
	{
		if (strcmp(args[0], command_list[i]) == 0)
		{
			return (*builtin[i])(args);
		}
	}

	return launch(args);
}

int main (int argc, char **argv)
{
	char *line;
	char **argc;
	int keepGoing;

	do
	{
		printf("bgshell > ");
		line = read();
		args = split(line);
		status = execute(args);

		free(line));
		free(args);
	} while (keepGoing);
}
