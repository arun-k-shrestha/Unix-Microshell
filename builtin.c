#include <assert.h>
#include <bits/time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include "builtin.h"
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <linux/stat.h>
#include <linux/limits.h>

//Prototypes
static void exitProgram(char** args, int argcp);
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void my_stat(char** args, int argcp);
static void tail(char** args, int argcp);
static void touch(char** args, int argcp);


/* builtIn
 ** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
  */
int builtIn(char** args, int argcp)
{

  if (strcmp(args[0], "exitProgram") == 0) {
      exitProgram(args, argcp);
      return 1;
  } else if (strcmp(args[0], "pwd") == 0) {
      pwd(args, argcp);
      return 1;
  } else if (strcmp(args[0], "cd") == 0) {
      cd(args, argcp);
      return 1;
  } else if (strcmp(args[0], "my_stat") == 0) {
      my_stat(args, argcp);
      return 1;
  } else if (strcmp(args[0], "tail") == 0) {
      tail(args, argcp);
      return 1;
  } else if (strcmp(args[0], "touch") == 0) {
      touch(args, argcp);
      return 1;
  }
  return 0;
}

// exit the shell with the value. If value is not given, exit with value 0
static void exitProgram(char** args, int argcp)
{
  int value =0;
 if(argcp>1){
  //converts string into int -> "2" into 2
  value = atoi(args[1]);
  exit(value);
  }
  exit(value);
}

static void pwd(char** args, int argpc)
{ 
  if(strcmp(args[0],"pwd")==0){
  // Book Page 49  table 2.15 -> PATH_MAX => maximum-sized pathname
    char buf[PATH_MAX];

  // Book Page 136 -> getcwd -> buf is ok, NULL on error
    if(getcwd(buf,sizeof(buf))!=NULL){
      printf("%s\n",buf);
    }else{
      perror("error");
    }
  }
}


static void cd(char** args, int argcp)
{
 //book page 135 -> int chdir(const char *pathname)

// if input is just "cd" or "cd ~"
// do need to worry about "cd /"?
  if(argcp <=2){
    if(argcp==1 || strcmp(args[1], "~") == 0){
      // Book page 210, section 7.9. Environment Varibles. char *getenv(const char *name)
      const char *home = getenv("HOME");
      if (home != NULL){
        if(chdir(home) ==-1){
          perror("Error on changing directory");
        };
      }else
        perror("error: home directory");
    }
    else{
      if(chdir(args[1]) == -1){
        perror("Error: no directory");
      }
    }
  }
}


// my_stat <file1 or directory1...fileN or directoryN>: prints the information about the files. 

static void my_stat(char** args, int argcp){

  struct stat buf;
  for(int i =1; i<argcp;i++){
    const char *stat_file_name = args[i];
    if(stat(stat_file_name,&buf)==-1){
      perror("stat error:");
      continue;
    }
    else{

      printf("File: %s\n",args[i]);

      printf("Size: %d            Blocks: %d          IO Block: %d  ", 
      buf.st_size, buf.st_blocks, buf.st_blksize);


      // from man 2 stat
      switch (buf.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
        }

      printf("Device: %d    Inode: %d    Links: %d\n", 
      buf.st_rdev, buf.st_ino, buf.st_nlink);

      printf("Access: (%d)  Uid: (%d)    Gid: (%d)\n", 
      buf.st_mode, buf.st_uid, buf.st_gid);

      printf("Access: %s\n",ctime(&buf.st_atime));

      printf("Modify: %s\n", ctime(&buf.st_mtime));

      printf("Change: %s\n", ctime(&buf.st_ctime));

      printf("Birth: - \n");

    }
  }
}


static void tail(char** args, int argcp){

  for(int i =1;i<argcp;i++){
    // From book page 148 Section 5.5 -> fopen opens a specified file.
    FILE *file_open = fopen(args[i],"r");

    // returns NULL on error
    if(file_open==NULL){
      // to output the words with filename
      fprintf(stderr, "tail: cannot open '%s' for reading: ", args[i]);
      //to print the the error. The goal is to print/output the exact error message
      perror("");
      continue;
    }else{
      // Book Page 152 Section 5.7 -> fgets
      printf("==> %s <==\n",args[i]);
      int max_line = BUFSIZ;
      char multiple_lines[10][BUFSIZ]; // row and column
      char one_line[BUFSIZ];
      int j =0;
      // fgets -> (store, max character to store, file stream)
      while(fgets(one_line,max_line,file_open)!= NULL){
        strcpy(multiple_lines[j%10],one_line);
        j++;
      }

      // index to print lines. star_line is set up to print the last 10 lines
      int start_line = 0; 
      // prints the lines 10 step back from the end point
      if (j>10){
        start_line = j-10;
      } 

      for(start_line;start_line<j;start_line++){
        printf("%s",multiple_lines[start_line%10]);
      }
    }

    // close the file
    fclose(file_open);
  }
}


static void touch(char** args, int argcp){
  
  // should be two arguments. For example touch fileName.
  if (argcp != 2) {
    fprintf(stderr, "Error: touch <fileName>\n");
    return;
  }

  // args -> {touch, fileName}
  // args[0] - touch
  // args[1] - fileName

  //file_name a pointer to the actual file name passed through arguments
  const char *file_name  = args[1];

  // touch_file is variable to store file info.
  struct stat touch_file;

  // -1 on error -> book page 93,section 4.2 
  if (stat(file_name,&touch_file)== -1){

    // create a file if not found
    int fd = open(file_name,O_CREAT|O_WRONLY, 0644);

    // -1 on error
    if(fd==-1){
      perror("Couldn't create a file");
      return;
    }
    close(fd);
  }
  else{
    
    // opening the file if found
    int fd = open(file_name,O_RDWR | O_TRUNC);
    if(fd==-1){
      perror("can't open the file");
      return;
    }

    struct timespec times[2]; 
    clock_gettime(CLOCK_REALTIME, &times[0]);
    clock_gettime(CLOCK_REALTIME, &times[1]);

    // From book Page 128;
    if(futimens(fd,times)<0){
      perror("futimens error");
      close(fd);
      return;
    }
    close(fd);
    return;
  }
}

