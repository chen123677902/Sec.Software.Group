#ifndef CONFIG_H
#defind CONFIG_H

#define BUFSIZE 4096
#define COMMANDSIZE 100

const char* file_path = ".shash.config";
char COMMAND[COMMANDSIZE][BUFSIZE];
char ENVIRONMENT[ENVIRONMENTSIZE][BUFSIZE];
int CommandLineIndex = 0;
int count = 0;

/*
remove '*' from command line
*/
void RMStar(char* ch, char* ch1);

/*
find char from String
*/
int IndexOf(char Ch, char* String);

/*
check if ch1 == ch2.
*/
int StringMatch(char* ch1, char* ch2);

int checkConfigFile()\

/*
	restrict Str_Input < mybursize:4096
*/
int CommandMatch(char* Str_Input);

/*
	find last binary
*/
int Lastbinary(char* FullName, char* LastName);

/*
searching if thHre is a command match the last binary from command list
*/
int MatchLastbinary(char* CommandList, char* InputCommand);

#endif
