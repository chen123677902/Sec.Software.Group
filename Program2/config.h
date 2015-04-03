#ifndef CONFIG_H
#define CONFIG_H

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

#define BUFSIZE 4096
#define COMMANDSIZE 100
#define ENVIRONMENTSIZE 128
#define CONFIG ".shash.config"

char COMMAND[COMMANDSIZE][BUFSIZE];
char ENVIRONMENT[ENVIRONMENTSIZE][BUFSIZE];
int CommandLineIndex;
int envPs;

/* remove '*' from command line */
void RMStar(char* ch, char* ch1);

/* find char from String */
int IndexOf(char Ch, char* String);

/* check if ch1 == ch2 */
int StringMatch(char* ch1, char* ch2);

int checkConfigFile();

/* restrict Str_Input < mybursize:4096 */
int GetCommand(char* Str_Input);

/* find last binary */
int Lastbinary(char* FullName, char* LastName);

/* searching if thHre is a command match the last binary from command list */
int MatchLastbinary(char* CommandList, char* InputCommand);

#endif
