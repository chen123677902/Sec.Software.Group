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


/* Empty SHASH */

/* check configureartion file
 * Make sure it is not writable
 * fail silently if it is
*/

/* Log */

/* While input */

	/* Log requested operation and IDs */
	
	/* Check for pipe */

	/* Check for one or both binaries in config
	 * Else fail
	 */
	 
	 /* Check for Env for command
	 * Else fail
	 */
	 
	 /* Check SHA sum
	 * Else fail
	 */


	/* If no pipe exev op1 */
	
	/* Else pipe create pipe */

		/* fork */

			/* ** Parent ** */
			 /* dupe write end of pipe to stdout */
			 /* execve op 1 w/ out stdout */
			 /* wait for child */

			/* ** Child ** */
			 /* dupe read end of pipe to stdin */
			 /* exeve op2 w/ in from stdin */
			 /* exit

/*
	

	
