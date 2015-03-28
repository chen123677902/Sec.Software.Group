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

#define MAXLENGTH 512

int main {
  int isParent = 0;
  int pipe = 0;
  int pipePos = 0;
  int unipipe[2];
  int argc1 = 0;
  int argc2 = 0;
  int argNum = 0;
  char c = 'z';	/* scratch char variable */
  
/* Empty SHASH */

/* Run Setup Config
 * Check return code
*/

/* Log */

/* While input */
  while (fgets(cmdString, MAXLENGTH, stdin) != NULL) {

	  /* Log requested operation and IDs */
	  
	  /* Check for pipe */
	  int i = 0;
	  while (i < MAXLENGTH && cmdString[i] != '\0' && pipe == 0) {
	   if (cmdString[i] ==  '|') {
	     pipe = 1;
	     pipePos = i;
	     cmdString[i] = '\0'
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
	  if (pipe == 1) {
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

	  /* Check for one or both binaries in config ( In parallel arrays )
	  * Else fail
	  */
	  
	  /* Check SHA sum
	  * Else fail
	  */
	  
	  /* Get ENV
	   * Else fail
	   */

	  /* Construct argv arrays */
	  char * argv1[argc1];
	  char * argv2[argc2];
	  
	  i = 1;
	  while (i < MAXLENGTH && argNum < argc1) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
		argv1[argNum] = &cmdString[i]
		argcNum += 1;
	      }
	    i += 1;
	  }
	  if (pipe == 1) {
	    argNum = 0;
	    i = pipePos + 2;
	    while (i < MAXLENGTH && argNum < argc2) {
	      if ( cmdString[i] != '\0' && cmdString[i-1] == '\0' )
	      {
		argv2[argNum] = &cmdString[i]
		argcNum += 1;
	      }
	      i += 1;
	    }
	  }

	  /* If no pipe exev op1 */
	  if (pipe == 0) {
	    /* execve command */
	  }
	  else {
	    /* Create pipe */
	    if(pipe(uniPipe) == -1) {
	      printf("Error: Create pipe failed");
	      return -1;
	    }
	    
	    if (!isParent){	      
	      /* ** Child ** */
	      close(unipipe[1]);
	      close(0);
	      dup(unipipe[0]);
	      close(unipipe[0]);
	      /* exeve op2 */
	      exit(0);
	    }
	    else {
	      /* ** Parent ** */
	      close(unipipe[0]);
	      close(1);
	      dup(unipipe[1]);
	      close(unipipe[1]);
	      /* execve op 1 */
	      /* wait for child */	      
	    }
	  }
  }
}