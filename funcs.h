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

char** parse_args(char* line, char * pat);
char* strip(char * str);
int pipe_it(char ** pipe_args);
int redirects(char * line, char * in, char * out);
