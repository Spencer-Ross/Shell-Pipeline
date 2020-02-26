#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char* buffer[argc];
	int i, j=0;

	for(i=1; i<argc; i++) {
		printf("%s\n", argv[i]);
	}

	//printf("next part\n");

	for(i=1; i<argc; i++) {
		buffer[j++] = argv[i];
	}

	buffer[j] = NULL;

	/*for(i=0; buffer[i] != NULL; i++) {
		printf("%s\n", buffer[i]);
	}*/

	/*i=0;
	printf("%s\n", buffer[i++]);
	printf("%s\n", buffer[i++]);
	printf("%s\n", buffer[i]);*/

	if(buffer[0] == NULL) return 0;
	if(execvp(buffer[0], buffer) == -1)
		printf("%s\n",strerror(errno));

	return 0;
}