/***********************************************************************
name: Spencer Ross
	assignment4 -- acts as a pipe using ":" to seperate programs.
description:	
	See CS 360 Processes and Exec/Pipes and Signals lecture for helpful tips.
***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/
int main(int argc, char *argv[]){
	int status, fd[2], isColon = 0;
	int i, j = 0, k = 0;
	char *left[argc+1], *right[argc+1];

	if(pipe(fd) < 0)
		printf("%s\n", strerror(errno));

	/*fit the args int buffers for left
	of ':' and to the right of the ':'*/
	for(int i=1; i < argc; i++) {
		if(strcmp(argv[i], ":") == 0) {
			isColon = 1;
			i++; //skip this char
		}
		if(isColon) 				//if seen ":"
			right[k++] = argv[i];	//put into right buffer
		else
			left[j++] = argv[i]; //else put into left buffer
	}

	//set NULL pointers for exec to end
	left[j] = NULL;
	right[k] = NULL;

	if(right[0] == NULL) {
		if(execvp(left[0], left) == -1) 	//execute only left of pipe
			printf("%s\n", strerror(errno));
		exit(0);
	} else if(left[0] == NULL) {
		if(execvp(right[0], right) == -1) 	//execute only left of pipe
			printf("%s\n", strerror(errno));
		exit(0);
	}

	if(!fork()) {	//child: exec left of ':' to write to pipe
		if(close(fd[0]) < 0)				//read end is unused
			printf("%s\n", strerror(errno));
		if(fd[1] != 1) {		//Defensive check -- from textbook
			if((dup2(fd[1], 1) < 0) || (close(fd[1]) < 0))
				printf("%s\n", strerror(errno));
		}

		if(execvp(left[0], left) == -1) 	//Writes to pipe
			printf("%s\n", strerror(errno));
	} else {				//parent: exec right of ':'
		wait(&status); 		//wait for child to end
		if(close(fd[1]) < 0)			//write end is unused
			printf("%s\n", strerror(errno));	
		if(fd[0] != 0) {
			if((dup2(fd[0], 0) < 0) || (close(fd[0]) < 0))
				printf("%s\n", strerror(errno));
		}	
		if(execvp(right[0], right) == -1)
			printf("%s\n", strerror(errno));
	}

	if(
		(close(fd[0]) < 0) ||
		(close(fd[0]) < 0) ||
		(wait(NULL) < 0)
	)
		printf("%s\n", strerror(errno));

	exit(0);
}