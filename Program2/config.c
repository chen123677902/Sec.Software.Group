#include "config.h"

char COMMAND[COMMANDSIZE][BUFSIZE];
int CommandLineIndex = 0;
int EnvironmentLineIndex = 0;

/* remove '*' from command line */
void RMStar(char* ch, char* ch1){
	int i = 2;	
	for(i; i<strlen(ch); i++){
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
char* GetEnvironment()
{
	return	&ENVIRONMENT[0][0];
}


int checkConfigFile()
{	
 
//buf uses for store input from file
//p1 uses for split command from buf
  char buf[BUFSIZE]={0};
  char WIPESATR[BUFSIZE]={0};
  char SHA1[BUFSIZE]={0};
  int count = 0;
  int SHA1_Break = 0;
  struct stat CheckFileMode;
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
  if(S_IWGRP&CheckFileMode.st_mode){printf("%x",S_IWGRP);return -1;}
  if(S_IWOTH&CheckFileMode.st_mode){printf("%x",S_IWOTH);return -1;}

  //setting up configuration file
  FILE *Cfile =fopen(file_path,"r");
	while(!feof(Cfile))
	{	
		fgets(buf,BUFSIZE-1,Cfile);
		if(feof(Cfile)){break;}	

		if(buf[0] == '\n'){return -1;}
		else if(StringMatch(buf,"EMPTY") == 0){return 0;}
		else if(buf[0] == '*'){
			RMStar(buf,WIPESATR);			
			if((SHA1_Break = IndexOf(' ',WIPESATR))>-1){
			SHA1_Break = SHA1_Break + 1;		
			count = 0;
			for(SHA1_Break;SHA1_Break<strlen(WIPESATR);SHA1_Break++){
					SHA1[count] = WIPESATR[SHA1_Break];
					count++;
				}
			}
			//all check, coping commands
			SHA1_Break = IndexOf(' ',WIPESATR);
			for(count = 0; count < SHA1_Break; count++){
				COMMAND[CommandLineIndex][count] = WIPESATR[count];
			}
			/*
			///SHA1 check. 
			if(!strcmp(SHA1,EVP(COMMAND[CommandLineIndex]))){
			return -1;
			}
			*/
			CommandLineIndex++;
			}
		else if(CommandLineIndex==0){
		strcpy(ENVIRONMENT[EnvironmentLineIndex],buf);
		EnvironmentLineIndex++;		
		}
		//if contain doesn't belong any of situation above. return -1
		else{return -1;}
	}		
		if(CommandLineIndex==0 || EnvironmentLineIndex==0){
		return -1;}
	return 0;
}

/* restrict Str_Input < mybursize:4096 */
int GetCommand(char* Str_Input){
	int count = 0;	
	if(strlen(Str_Input)>4096){return -1;}
	//Serach is fullname inside of commandlist
	for(count = 0; count < CommandLineIndex; count++){
		if(strcmp(Str_Input,COMMAND[count])==0){return 0;}
	}	
	//Serach is lastbinary inside of commandlist
	for(count = 0; count < CommandLineIndex; count++){		
		if(MatchLastbinary(COMMAND[count],Str_Input)==0){	
		strcpy(Str_Input,COMMAND[count]);
		return 0;
		}	
	}
	return -1;
}

/* find last binary */
int Lastbinary(char* FullName, char* LastName){
	int i = 0;	
	char temp[BUFSIZE] = {0};
	int j = 0;
	while(FullName[j] != '\0'){j ++;}
	//***********************************		
	if(j == 0){return -1;}
	//**********************************
	j--;	
	if(IndexOf('/',FullName)!=-1){				
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
int MatchLastbinary(char* CommandList, char* InputCommand){
	char temp1[BUFSIZE]={0}, temp2[BUFSIZE]={0};	
	Lastbinary(CommandList, temp1);
	Lastbinary(InputCommand, temp2);
	return StringMatch(temp1,temp2);
}

//~ int main(int argc,char *argv[])
//~ {
	//~ char input[50];
	//~ int i=0;	
	//~ printf("entry command:");	
	//~ scanf("%s",input);
	//~ printf("Environment return status : %d\n",checkConfigFile());
	//~ printf("lines: %d\n",CommandLineIndex);
	//~ for(i;i<CommandLineIndex;i++){
		//~ printf("%s\n",COMMAND[i]);
	//~ }
//~ 
	//~ printf("return CommandMatch status :%d, return input :%s\n",GetCommand(input),input);
	//~ for(i=0;i<EnvironmentLineIndex;i++){
		//~ printf("Environment[%d]:%s", i, ENVIRONMENT[i]);
	//~ }
	//~ printf("%s",EVP("/bin/cp"));	
//~ }
