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
  int cmd1Index;
  int cmd2Index;
  int argc1 = 0;
  int argc2 = 0;
  int argNum = 0;
  int configCheck = 0;
  unsigned char sha1[MAXLENGTH];
  unsigned char sha2[MAXLENGTH];
  char cmd1[MAXLENGTH];
  char cmd2[MAXLENGTH];
  char cmdString[MAXLENGTH];
  char c = 'z';	/* scratch char variable */

/* Empty SHASH */

/* Check and Parse config file */
configCheck = checkConfigFile();
if (configCheck ) {
	TRACE("config check failed: %i", configCheck);
	printf("Silent Exit\n");
	return 1;
}

/* Log */

/* While input */
printf("SHASH awaits-> ");
  while (fgets(cmdString, MAXLENGTH, stdin) != NULL) {

	  TRACE("Got the Followoing cmdString: %s\n", cmdString);

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

	  TRACE("ARGC1: %i\n", argc1);

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
		TRACE("ARGC2: %i\n", argc2);
	  }

	  /* Get base cmd names */
	  strncpy(cmd1, cmdString, MAXLENGTH);
	  if (piping == 1) {
		  strncpy(cmd2, &cmdString[pipePos + 2], MAXLENGTH);
	  }

	  /* Check for one or both binaries in config ( In parallel arrays ) */
	  if (cmd1Index = GetCommand(cmd1)) {
		  TRACE("Command 1 not found: %s\n", cmd1);
		  printf("Silent Exit\n");
		  return 1;
	  }
	  TRACE("FOUND: %s\n", cmd1);

	  if (piping == 1) {
		  if (cmd2Index = GetCommand(cmd2)) {
			TRACE("Command 2 not found: %s\n", cmd2);
			printf("Silent Exit\n");
			return 1;
		  }
		  TRACE("FOUND: %s\n", cmd2);
	  }

	  /* Get ENV */
	  char * env[envPs];
	  for (i = 0; i < envPs; i++) {
		  env[i] = &ENVIRONMENT[i][0];
	  }
	  env[i] = NULL;

	  #ifdef DEBUG
	  for (i = 0; i < envPs; i++) {
		  TRACE("ENV %i: %s\n", i, env[i]);
	  }
	  #endif

	  /* Gen SHA1 for file 1 */
	  getSHA(cmd1, sha1);

	  /* Gen SHA1 for file 2 */
	  if (piping == 1) {
		  getSHA(cmd2, sha2);
	  }

	  /* Check SHA1 digest */
	  if (sha1 != strtol(SHA1[cmd1Index], NULL, 16)) {
		  printf("Silent Exit\n");
		  return 1;
		}

	  /* Check SHA2 */
	  if (piping == 1) {
	  /* Gen SHA1 for file 2 */
		  getSHA(cmd2, sha2);
		  //printf("cmd1 SHA1: %s\n", sha1);

		  /* Check SHA1 digest */
		  if (sha2 != strtol(SHA1[cmd2Index], NULL, 16)) {
			  printf("Silent Exit\n");
			  return 1;
		  }
	  }

	  /* Construct argv arrays */
	  char * argv1[argc1 + 1];
	  char * argv2[argc2 + 1];

	  argv1[0] = cmd1;
	  argv2[0] = cmd2;

	  i = 1;
	  while (i < MAXLENGTH && argNum < argc1) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
			argv1[argNum + 1] = &cmdString[i];
			argNum += 1;
	      }
	    i += 1;
	  }
	  TRACE("argv1[ %i ] = NULL\n", argc1);
	  argv1[argc1] = NULL;

	  #ifdef DEBUG
	  for (i = 0; i < argc1; i++) {
		  if (argv1[i] == NULL ) {
			  TRACE("Done: %p\n", argv1[i]);
		  }
		  else {
			  TRACE("ARGV1[ %i ]: %s\n", i, argv1[i]);
		  }
	  }
	  #endif

	  if (piping == 1) {
	    argNum = 0;
	    i = pipePos + strlen(&cmdString[ pipePos + 2 ]);
	    while (i < MAXLENGTH && argNum < argc2) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
			argv2[argNum + 1] = &cmdString[i];
			argNum += 1;
	      }
	      i += 1;
	    }
		argv2[argNum+1] = NULL;
		TRACE("argv2[ %i ] = NULL\n", argNum + 1);

		#ifdef DEBUG
		for (i = 0; i < argc2; i++) {
			if (argv2[i] == NULL ) {
			  TRACE("Done: %p\n", argv2[i]);
			}
			else {
			  TRACE("ARGV2[ %i ]: %s\n", i, argv2[i]);
			}
		}
		#endif
	  }

	  /* If no pipe exev op1 */
	  if (piping == 0) {
		#ifdef DEBUG
			TRACE("EXEC: %s, ", cmd1);
			for(i = 0; i < argc1 - 1; i++) {
				TRACE("%s ", argv1[i]);
			}
			TRACE("%s", ", ");
			for(i = 0; i < envPs; i++) {
				TRACE("%s ", env[i]);
			}
		#endif

	    execve(cmd1, & argv1[0], env);
	  }
	  else {
	    /* Create pipe */
	    if(pipe(uniPipe) == -1) {
	      TRACE("Error: Create pipe failed: %i", -1);
	      return -1;
	    }

		/* Fork child */
		if (( isParent = fork()) == - 1 ) {
			TRACE( "Error: Fork failed: %i", isParent );
			return - 1 ;
		}

	    if (!isParent){
	      /* ** Child ** */
	      close(uniPipe[1]);
	      close(0);
	      dup(uniPipe[0]);
	      close(uniPipe[0]);
	      execve(cmd2, & argv2[0], env);
	      return 0;
	    }
	    else {
	      /* ** Parent ** */
	      close(uniPipe[0]);
	      close(1);
	      dup(uniPipe[1]);
	      close(uniPipe[1]);
	      execve(cmd1, & argv1[0], env);
	      /* Wait for Child to terminate */
		  int childStatus;
		  waitpid( isParent, & childStatus, 0 );
	    }
	  }
  }

  return 0;
}
