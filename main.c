#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include "funcs.h"

int main() {
	printf("\n================================================\nHello, and welcome to our shell!\n");
	printf("By Max Walden and Winston Peng\n");
	printf("This shell can: - pipe - redirect - redirect both input and output - ");
	printf("multi redirection - cd - exit\n");
	printf("This shell can NOT: - do multi piping - do multi redirection while redirecting both input and output\n");
	printf("================================================\n\n");
	char term_in[256];
	char * dir = malloc(256 * sizeof(char*));
  while(1) {
    printf(">%s$ ", getcwd(dir, 256));
    fgets(term_in, 256, stdin);
		int i;
		int n = 1;
    term_in[strlen(term_in) - 1] = '\0';
		for (i = 0; i < strlen(term_in); i++) {
			if (term_in[i] == ';') {
				n++;
			}
		}
		char ** lines = parse_args(term_in, ";");
    for(i = 0; i < n; i++) {
			char * line = lines[i];

			char * redirect_out = strchr(line, '>');
			char * redirect_in = strchr(line, '<');
			if (redirects(line, redirect_in, redirect_out)) continue;

      char ** to_exec = parse_args(line, " ");

			if (strcmp(to_exec[0], "cd") == 0) {
				int f = chdir(to_exec[1]);
				if (f != 0) {
					printf("file not found.\n");
				}
				continue;
			}

			if (strcmp(to_exec[0], "exit")==0) {
				printf("\nThanks for using our shell!\nHave a great day :)\n\n");
				exit(0);
			}

      int f = fork();
      int status;
      int child = wait(&status);

      if (f == 0) {

				int j;
				int pipes = 0;
				for (j = 0; j < strlen(line); j++) {
					if (line[j] == '|') pipes++;
				}

				if (pipes > 0) {
					char ** pipe_args = parse_args(line, "|");
					pipe_it(pipe_args);
					return 0;
				}

        execvp(to_exec[0], to_exec);
        free(to_exec);
        return 0;
      }
    }
  }
}
