#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG
#ifdef DEBUG
#define TRACE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define TRACE(fmt, ...) do {} while (0)
#endif

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
#include <sys/wait.h>
#include <openssl/evp.h>

#define BUFSIZE 4096
#define COMMANDSIZE 100
#define ENVIRONMENTSIZE 128
#define CONFIG ".shash.config"

char COMMAND[COMMANDSIZE][BUFSIZE];
char ENVIRONMENT[ENVIRONMENTSIZE][BUFSIZE];
char SHA1[BUFSIZE][BUFSIZE];
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

/* Generate and return SHA1 sum for given file name */
int getSHA(char * fileName, char * sha);

#endif