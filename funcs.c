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

int redirects(char * line, char * in, char * out) {
	if (in && out) {
		char ** pipe_args = parse_args(line, "<>");
		int i = 0;
		int f = fork();
		int status;
		int child = wait(&status);
		if (f == 0) {
			int in_file = 0;
			int out_file = 0;
			char * file;
			in_file = open(strip(pipe_args[1]), O_RDONLY);
			out_file = open(strip(pipe_args[2]), O_WRONLY | O_CREAT, 0644);
			if (in_file == -1) printf("%s\n", strerror(errno));
			if (out_file == -1) printf("%s\n", strerror(errno));
			dup2(in_file, STDIN_FILENO);
			dup2(out_file, STDOUT_FILENO);
			char ** args = parse_args(pipe_args[0], " ");
			execvp(args[0], args);
			exit(0);
		}
		return 1;
	}
	if (in) {
		char ** pipe_args = parse_args(line, "<");
		int f = fork();
		int status;
		int child = wait(&status);
		if (f == 0) {
			int in_file;
			int i = 1;
			char * file;
			while (pipe_args[i]) {
				in_file = open(strip(pipe_args[i]), O_RDONLY);
				if (in_file == -1) printf("%s\n", strerror(errno));
				i++;
			}
			dup2(in_file, STDIN_FILENO);
			char ** args = parse_args(pipe_args[0], " ");
			execvp(args[0], args);
			exit(0);
		}
		return 1;
	}
	if (out) {
		char ** pipe_args = parse_args(line, ">");
		int f = fork();
		int status;
		int child = wait(&status);
		if (f == 0) {
			int out_file;
			int i = 1;
			char * file;
			while (pipe_args[i]) {
				out_file = open(strip(pipe_args[i]), O_WRONLY | O_CREAT, 0644);
				if (out_file == -1) printf("%s\n", strerror(errno));
				i++;
			}
			dup2(out_file, STDOUT_FILENO);
			char ** args = parse_args(pipe_args[0], " ");
			i = 0;
			execvp(args[0], args);
			exit(0);
		}
		return 1;
	}
	return 0;
}

char ** parse_args(char * l, char * pat) {
	char * line = calloc(strlen(l), sizeof(char *));
	strcpy(line, l);
	line = strip(line);
	if (line == NULL) return NULL;
	int i = 0;
	char ** args = calloc(10, sizeof(char *));
	i = 0;
	char * curr = line;
	while (curr) {
		args[i] = strsep(&curr, pat);
		i++;
	}
	args[i] = NULL;
	return args;
}

char * strip(char * line) {
  char* start = line;
  while (*start == ' ') {
    start++;
  }
  char* end = line;
  while (*end) {
  	end++;
  }
  end--;
  while (*end == ' ') {
    *end = 0;
  	end--;
	}
  return start;
}

int pipe_it(char ** pipe_args) {
	char * to_in = strip(pipe_args[0]);
	char * to_out = strip(pipe_args[1]);
	FILE * in = popen(to_in, "r");
	FILE * out = popen(to_out, "w");
	long length;
	fseek (in, 0, SEEK_END);
	length = ftell(in);
	fseek (in, 0, SEEK_SET);
	char * buf = malloc(length);
	fread(buf, 1, length, in);
	fputs(buf, out);
	pclose(in);
	pclose(out);
	return 1;
}
