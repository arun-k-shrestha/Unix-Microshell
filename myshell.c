/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "argparse.h"
#include "builtin.h"


/* PROTOTYPES */

void processline(char *line);
ssize_t getinput(char** line, size_t* size);

/* main
 * This function is the main entry point to the program.  This is essentially
 * the primary read-eval-print loop of the command interpreter.
 */

int main () {
//  man 3 getline

  // to store the size of an allocated memory
  size_t size = 0;
  // to store a length of an input 
  ssize_t len;
  // to store input from the user. Null -> not pointing to any valid memory address
  char *line = NULL;

  // run the program forever until input command is exit or encounter any errors.
  while (1) {
  // prints -> %
    printf("%% "); 
    // get user input. line -> address to a pointer array. size -> stores buffer size
    len = getinput(&line, &size); 
    // getinput returns the number of characters read on suceess and -1 on failure
    if (len < 0){ 
        perror("Error");
        continue; 
    }
    processline(line); 
    //return 0;
  }
  return EXIT_SUCCESS;
}


/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.
* returns  The length of the string stored in *line.
*
* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* Hint: There is a standard i/o function that can make getinput easier than it sounds.
*/
ssize_t getinput(char** line, size_t* size) {
  size_t len = 0;
  if ((len= getline(line, size,stdin)==-1)){
    perror("Error");
    exit(EXIT_FAILURE);
  }

  return len;
}


/* processline
 * The parameter line is interpreted as a command name.  This function creates a
 * new process that executes that command.
 * Note the three cases of the switch: fork failed, fork succeeded and this is
 * the child, fork succeeded and this is the parent (see fork(2)).
 * processline only forks when the line is not empty, and the line is not trying to run a built in command
 */
void processline (char *line)
{
 /*check whether line is empty*/
  while(*line !='\0'){
    if(!isspace(*line)){
      break;
    }
    line++;
    if(*line=='\0'){
      perror("Empty Space");
      return;
    }
  }
  pid_t cpid;
  int   status;
  int argCount;
  char** arguments = argparse(line, &argCount);
  
  /*check whether arguments are builtin commands
   *if not builtin, fork to execute the command.
   */
  if(!builtIn(arguments,argCount)){
    cpid = fork();
    if(cpid==0){
      if(execvp(arguments[0],arguments)==-1){
        perror("Error");
        exit(EXIT_FAILURE);
      };

    }else if(cpid>0){
      pid_t id = wait(NULL);
    }
    else{
      perror("fork error");
      exit(EXIT_FAILURE);
    }
  }
  // free the allocated space
  for (int i = 0; i < argCount; i++) {
    free(arguments[i]);
  }
  free(arguments);

}

