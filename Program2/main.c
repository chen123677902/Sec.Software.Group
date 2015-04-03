#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "config.h"

#define MAXLENGTH 512

int main(int argc, char *argv[]) {
  int isParent = 0;
  int piping = 0;
  int pipePos = 0;
  int uniPipe[2];
  int argc1 = 0;
  int argc2 = 0;
  int argNum = 0;
  char cmdString[MAXLENGTH];
  char c = 'z';	/* scratch char variable */

/* Empty SHASH */

/* Check and Parse config file */
if (!checkConfigFile()) {
	printf("config check failed");
	return 1;
}

/* Log */

/* While input */
  while (fgets(cmdString, MAXLENGTH, stdin) != NULL) {

	  /* Check for pipe */
	  int i = 0;
	  while (i < MAXLENGTH && cmdString[i] != '\0' && piping == 0) {
	   if (cmdString[i] ==  '|') {
	     piping = 1;
	     pipePos = i;
	     cmdString[i] = '\0';
	   }
	   i += 1;
	  }

	  /* get argcs */
	  i = 0;
	  while (i < MAXLENGTH && c != '\0') {
	    c = cmdString[i];
	    if ( isspace(c) ) {
	      if ( cmdString[i-1] != '\0' )
	      {
		argc1 += 1;
	      }
	      cmdString[i] = '\0';
	    }
	    i += 1;
	  }
	  if (piping == 1) {
	    c = 'z';
	    i = pipePos + 1;
	    while (i < MAXLENGTH && c != '\0') {
	      c = cmdString[i];
	      if ( isspace(c) ) {
		if ( cmdString[i-1] != '\0' )
		{
		  argc2 += 1;
		}
		cmdString[i] = '\0';
	      }
	      i += 1;
	    }
	  }

	  /* Check for one or both binaries in config ( In parallel arrays ) */
	  if (GetCommand(&cmdString[0])) {
		  printf("Command 1 not found\n");
		  return 1;
	  }

	  if (GetCommand(&cmdString[pipePos+2])) {
		  printf("Command 2 not found\n");
		  return 1;
	  }

	  /* Check SHA sum
	  * Else fail
	  */

	  /* Get ENV */
	  char * env[envPs];
	  for (i = 0; i < envPs; i++) {
		  env[i] = &ENVIRONMENT[i][0];
	  }
	  env[i] = NULL;


	  /* Construct argv arrays */
	  char * argv1[argc1];
	  char * argv2[argc2];

	  i = 1;
	  while (i < MAXLENGTH && argNum < argc1) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
			argv1[argNum] = &cmdString[i];
			argNum += 1;
	      }
	    i += 1;
	  }
	  if (piping == 1) {
	    argNum = 0;
	    i = pipePos + 2;
	    while (i < MAXLENGTH && argNum < argc2) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
			argv2[argNum] = &cmdString[i];
			argNum += 1;
	      }
	      i += 1;
	    }
	  }

	  /* If no pipe exev op1 */
	  if (piping == 0) {
	    execve(argv1[0], & argv1[1], env);
	  }
	  else {
	    /* Create pipe */
	    if(pipe(uniPipe) == -1) {
	      printf("Error: Create pipe failed");
	      return -1;
	    }

	    if (!isParent){
	      /* ** Child ** */
	      close(uniPipe[1]);
	      close(0);
	      dup(uniPipe[0]);
	      close(uniPipe[0]);
	      /* exeve op2 */
	      exit(0);
	    }
	    else {
	      /* ** Parent ** */
	      close(uniPipe[0]);
	      close(1);
	      dup(uniPipe[1]);
	      close(uniPipe[1]);
	      /* execve op 1 */
	      /* wait for child */
	    }
	  }
  }
  return 0;
}
