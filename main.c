#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>

#define READ_END 0
#define WRITE_END 1

void crible(int *in) {
	int p;
	if (read(in[READ_END], &p, sizeof(int)) == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("%d\t", p);

	int out[2];
	if (pipe(out) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
 
	pid_t pid = fork();
 	if (pid == -1) {
 		perror("fork");
 		exit(EXIT_FAILURE);
 	}
 	if (pid) {
 		close(out[READ_END]);
		int i;
		while (read(in[READ_END], &i, sizeof(int)) == sizeof(int)) {
			if (i%p) {
				if (write(out[WRITE_END], &i, sizeof(int)) == -1) {
					perror("write");
					exit(EXIT_FAILURE);
				}
			}
		}
		close(in[READ_END]);
		close(out[WRITE_END]);
		wait(NULL);
 	}
 	else {
 		close(out[WRITE_END]);
		crible(out);
 	}
}

int main(int argc, char *argv[]) {
	printf("Enter a integer\n");
	int limit;
	scanf("%d", &limit);

	int out[2];
	if (pipe(out) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid) {
		close(out[READ_END]);
		for (int i = 2; i <= limit; i += 1) {
			if (write(out[WRITE_END], &i, sizeof(int)) == -1) {
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
		close(out[WRITE_END]);
		wait(NULL);
	}
	else {
		close(out[WRITE_END]);
		crible(out);
	}

	exit(EXIT_SUCCESS);
}
