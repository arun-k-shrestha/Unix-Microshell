#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argparse.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FALSE (0)
#define TRUE  (1)

/*
* argCount is a helper function that takes in a String and returns the number of "words" in the string assuming that whitespace is the only possible delimeter.
*/
static int argCount(char*line)  // question? can the input value be "  "-> two white spaces?
{
  // count variable to store the number of words in the string
  int count = 0;
  int word = 0;
  // loop until the end of the line is reached
  while(*line !='\0'){
    if (*line == ' '|| *line == '\n'){ 
      word = 0;
    }
    else{
      if(word==0){
        count++;
        word=1;
      }
    }
    line++;
  }
  return count;
}



/*
*
* Argparse takes in a String and returns an array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer
*/
char** argparse(char* line, int* argcp) // how to free malloc
{return 0;}

  
  // // array stores the pointers to the words in the strings
  // char **array = malloc(*argcp * sizeof(char*));

  // int i = 0;
  // //man strtok  -> string token -> char *strtok(char *restrict str, const char *restrict delim);
  
  
  // char *word = strtok(line," "); // can't use this!!!

  // // NULL tells strtok to continue from same string
  // while(word != NULL){

  //   array[i] = malloc((strlen(word)+1)*sizeof(char));
  //   strcpy(array[i],word);

  //   i++;
  //   word = strtok(NULL," ");
  // }
  //return array;







// if (args == NULL) {
//         fprintf(stderr, "Error\n");
//         return NULL;
// }