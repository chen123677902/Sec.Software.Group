#include <unistd.h>
#include <stdlib.h>

main(){
 int isParent;
 int apipe[2];
 char *cmd[2][3];

 //
 //--- Want to execute command: "ps -axu | grep root" 
 //

 cmd[0][0] = "ps"; 
 cmd[0][1] = "-axu";
 cmd[0][2] = NULL; 
 cmd[1][0] = "grep";
 cmd[1][1] = "root";
 cmd[1][2] = NULL;

 pipe(apipe);
 isParent=fork();
 if (!isParent){ //---Child is "grep root"

	 // Want stdin connected to pipe
 	close(apipe[1]); // not writing to pipe write end

	close(0); // close stdin (0 now available)
 	dup(apipe[0]); // dup read end to stdin (will use 0)
 	close(apipe[0]); // close old read end

 	execvp(cmd[1][0],cmd[1]); // stdin now the pipe read end
 	perror("Child exec::");
 	exit(1);

 } else { //--- Parent is "ps -axu" 

	// Want stdout connected to pipe
 	close(apipe[0]); // not reading from pipe read end
 	close(1); // close stdout (1 now available)
 	dup(apipe[1]); // dup write end to stdout (will use 1)
 	close(apipe[1]); // close old write end

 	execvp(cmd[0][0],cmd[0]); // stdout is pipe write end
 	perror("Parent exec::");
 	exit(1);

 }
}