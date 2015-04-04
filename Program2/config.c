#include "config.h"

/* remove '*' from command line */
void RMStar(char* ch, char* ch1){
	int i;
	for(i = 2; i < strlen(ch); i++){
        	ch1[i-2] = ch[i];
        }
}

/* find char from String */
int IndexOf(char Ch, char* String){
   int i;
   for(i=0;i<strlen(String);i++){
	if(String[i] == Ch){return i;}
   }
   return -1;
}

/* check if ch1 == ch2 */
int StringMatch(char* ch1, char* ch2){
	int i;
	for(i=0;i<strlen(ch1);i++){
		if(ch1[i] != ch2[i]){return -1;}
	}
        return 0;
}

/* get first address of ENVIRONMENT Array */
char* GetEnvironment() {
	return	&ENVIRONMENT[0][0];
}

int checkConfigFile() {

  char buf[BUFSIZE] = {0};
  char WIPESATR[BUFSIZE] = {0};
  int SHA1_Break = 0;
  int EnvironmentLineIndex = 0;
  struct stat CheckFileMode;
  const char* file_path = CONFIG;
  int count = 0;
  CommandLineIndex = 0;
  envPs = 0;

  if(open(file_path, O_CREAT | O_EXCL) != -1)
  {
    printf("file not found!\n");
    return -1;
  }

  if(lstat(file_path,&CheckFileMode) < 0)
  {
    printf("lstat error!\n");
    printf("silent exit\n");
    return -1;
  }

  if(!S_ISREG(CheckFileMode.st_mode))
  {
    return -1;
  }

  //file is not writable by group or world
  if(S_IWGRP&CheckFileMode.st_mode){ printf("%x",S_IWGRP);return -1; }
  if(S_IWOTH&CheckFileMode.st_mode){ printf("%x",S_IWOTH);return -1; }

  //setting up configuration file
  FILE *Cfile = fopen(file_path,"r");
	while(!feof(Cfile))
	{
		fgets(buf,BUFSIZE-1,Cfile);
		if(feof(Cfile)) { break; }

		/*  ***** If line empty fail? ***** */
		if(buf[0] == '\n') { return -1; }

		/* ***** If line contains "Empty" return good? ***** */
		else if( StringMatch(buf,"EMPTY") == 0 ) { return 0; }

		else if( buf[0] == '*' ) {
			RMStar(buf,WIPESATR);
			if((SHA1_Break = IndexOf(' ',WIPESATR))>-1){
				count = 0;
				for(SHA1_Break = SHA1_Break + 1; SHA1_Break < strlen(WIPESATR); SHA1_Break++){
					SHA1[CommandLineIndex][count] = WIPESATR[SHA1_Break];
					count++;
				}
			}
			//all check, coping commands
			SHA1_Break = IndexOf(' ',WIPESATR);
			for(count = 0; count < SHA1_Break; count++){
				COMMAND[CommandLineIndex][count] = WIPESATR[count];
			}

			CommandLineIndex++;
		}
		else if( CommandLineIndex==0 ) {
			strncpy(ENVIRONMENT[EnvironmentLineIndex], buf, BUFSIZE);

			int i;
			for (i = 0; i < BUFSIZE; i++) {
				if(ENVIRONMENT[EnvironmentLineIndex][i] == '\n') {
					ENVIRONMENT[EnvironmentLineIndex][i] = '\0';
					break;
				}
			}

			envPs++;
			EnvironmentLineIndex++;
		}
		//if contain doesn't belong any of situation above. return -1
		else{return -1;}
	}
		if(CommandLineIndex == 0 || EnvironmentLineIndex == 0) { return -1; }

	return 0;
}

/* restrict Str_Input < mybursize:4096 */
int GetCommand(char* Str_Input){

	int count = 0;
	if(strlen(Str_Input)>4096) { return -1; }

	//Serach is fullname inside of commandlist
	for(count = 0; count < CommandLineIndex; count++) {
		if(strcmp(Str_Input,COMMAND[count]) == 0) { return count; }
	}
	//Serach is lastbinary inside of commandlist
	for(count = 0; count < CommandLineIndex; count++) {
		if( MatchLastbinary(COMMAND[count], Str_Input) == 0) {
		strcpy(Str_Input, COMMAND[count]);
		return count;
		}
	}
	return -1;
}

/* find last binary */
int Lastbinary(char* FullName, char* LastName) {

	int i = 0;
	char temp[BUFSIZE] = {0};
	int j = 0;

	while(FullName[j] != '\0') { j++; }
	//***********************************
	if(j == 0) { return -1; }
	//**********************************
	j--;
	if(IndexOf('/',FullName) != -1) {
		while(FullName[j] != '/')
		{
			temp[i] = FullName[j];
			i++;
			j--;
		}
	}
	else{

		while(j>-1){
			temp[i] = FullName[j];
			i++;
			j--;
		}
	}
	strcpy(LastName,temp);
	return 0;
}

/* searching if there is a command match the last binary from command list */
int MatchLastbinary(char* CommandList, char* InputCommand) {
	char temp1[BUFSIZE] = {0}, temp2[BUFSIZE] = {0};
	Lastbinary(CommandList, temp1);
	Lastbinary(InputCommand, temp2);
	return StringMatch(temp1,temp2);
}

int getSHA(char * fileName, char * sha) {
   EVP_MD_CTX    mdctx;
   unsigned char md_value[EVP_MAX_MD_SIZE];
   int           md_len;
   struct stat   fileStat;
   int           fd;
   char          *buf;

        fd=open(fileName,O_RDONLY);

        fstat(fd,&fileStat);
        buf=malloc(fileStat.st_size);
        read(fd,buf,fileStat.st_size);

        EVP_MD_CTX_init(&mdctx);

        EVP_DigestInit_ex(&mdctx, EVP_sha1(), NULL);

        /* Demonstrate that we can do this in pieces */
        if (fileStat.st_size > 100){
             EVP_DigestUpdate(&mdctx, buf, 100);
             EVP_DigestUpdate(&mdctx, buf + 100, fileStat.st_size-100);
       	 } else {
             EVP_DigestUpdate(&mdctx, buf, fileStat.st_size);
	 }

        EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
        EVP_MD_CTX_cleanup(&mdctx);

		strcpy(sha, md_value);
        return 0;
}
