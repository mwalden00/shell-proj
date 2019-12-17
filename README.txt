Shell project, by Max Walden and Winston Peng

Features:
 - Forks, then executes commands
 - Can parse multiple commands in one line (using ";")
 - Redirects using < and >
 - Can redirect both stdin and stdout in one line
 - Can use the output of one command as the input for another (using "|")
 - Can change directory using cd
 - Can exit using "exit"
 - Can do multiple redirections in a line (aka "ls > file1.txt > file2.txt")

Attempted:
 - Cannot redirect and pipe in one line
 - Cannot do redirection and piping in the same line
 - Cannot do multiple redirection with both "<" and ">"
 - Cannot do multiple pipes in a line (aka "ls | wc | wc")
   + It will do the same thing as "ls | wc" if you do that
 - Sadly, due to a badly sprained wrist and a broken computer,
   we didn't finish on time :(


Files and Function Headers:

funcs.c; funcs.h
 Contains all helper functions, including a parser,
 redirection handler, pipe handler, and a whitespace stripper

 ======= char ** parse_args() =======
 Inputs: char * line
         char * delimiter
 Returns: A string array of the line fully parsed
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This function splits the given line on any
 character found in the delimiter string, stripping
 the line fragments of whitespace.


 ========== char * strip() ==========
 Inputs: char * str
 Returns: a copy of the string stripped of whitespace
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Simply returns a string with all whitespace stripped
 It does so by moving up the start pointer (if at all) and
 moving down the '\0' character (again, if at all)


 ========= int redirects() ==========
 Inputs: char * line
         char * in
         char * out
 Output: 1 or 0
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This function single handedly does all redirects.
 The *line argument is simply the line being executed.
 The *in argument is a pointer to a '<' character in *line.
 If a '<' character is absent, *in equates to NULL.
 The *out argument is a pointer to a '>' character in *line.
 If a '>' character is absent, *out equates to NULL.
 The *in and *out arguments only exist as a means to identify
 whether or not to redirect stdin, stdout, or both.
 If both *in and *out are not NULL, then the program parses *line on
 both '<' and '>', forks, uses dup2 to redirect stdin and stdout,
 executes, and returns 1.
 If only *in is not NULL, then the program parses *line on '<', forks,
 uses dup2 to redirect stdin, executes, and returns 1.
 If only *out is not NULL, then the program parses *line on '>', forks,
 uses dup2 to redirect stdout, executes, and returns 1.
 If both *in and *out equate to NULL, meaning no redirection must occur,
 the function returns 0.


 ========== int pipe_it() ===========
 Inputs: char ** pipe_args
 Output: 1
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This function deals with any piping that occurs.
 **pipe_args is a line of code parsed on '|'.
 The function first opens FILE pointers (pipes) using popen,
 one to be read from and one to write to.
 It then reads the entirety of the read pipe to a string,
 which is then written to the write pipe.
 After closing the pipes, the function returns 1.

main.c
 Contains the main() function
 ============ int main() ============
 Inputs: None
 Returns: None
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 The main function utilizes a while(1) loop
 to continually run the shell.
 Within the loop:
 main() deals with reading a input
 from the terminal, parsing this input over
 ';', and executing each individual
 line of code. The function then uses strchr()
 to look for pointers to '<' and '>' within the
 line parsed from shell input. These pointers are
 stored in variables called *redirect_in and
 *redirect_out. main() then calls redirects() on
 the line of commands, *redirect_in, and *redirect_out.
 If redirects() returns 1 and redirection successfully
 occurred, then main() skips to the next line of code, as
 redirects() executes the line itself.
 main() then checks if the first argument in the parsed
 line of code is "cd". If it is, then main() calls chdir() on
 the second argument in the line.
 If the first argument in the line is "exit", then main()
 calls exit(0).
 After checking for exit, main() forks. The child then
 counts the number of pipes in the line, storing this number
 in a variable named pipes. If pipes is greater than 1,
 main() parses the line on '|', and calls pipe_it() on the
 char ** that is returned.
 If no redirection, piping, cd-ing, or exiting occurs,
 main() parses the line over ' ' and call execvp() on the
 resulting char **


makefile

  make
   - compiles everything.

  make run
   - runs the executable.

  make clean
   - cleans up .o files and the executable.
