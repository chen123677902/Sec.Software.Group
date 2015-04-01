#include "config.h"

void RMStar(char* ch, char* ch1){
	int i = 2;	
	for(i; i<strlen(ch); i++){
        	ch1[i-2] = ch[i];
        }
}

/*
find char from String
*/
int IndexOf(char Ch, char* String){
   int i;
   for(i=0;i<strlen(String);i++){
	if(String[i] == Ch){return i;}
   }
   return -1;
}

/*
check if ch1 == ch2.
*/
int StringMatch(char* ch1, char* ch2){
	int i;	
	for(i=0;i<strlen(ch1);i++){
		if(ch1[i] != ch2[i]){return -1;}
	}
        return 0;
}

int checkConfigFile()
{	
  char* file_path = "name.txt";
//buf uses for store input from file
//p1 uses for split command from buf
  char buf[BUFSIZE],WIPESATR[BUFSIZE],SHA1[BUFSIZE];
  //int n = 0;
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
    printf("%s is not a ordinary file@\n",file_path);
    printf("silent exit\n");
    return -1;
  }
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
				
			//if(!SHA1_Match){return -1;}
			}
			//all check, coping commands
			SHA1_Break = IndexOf(' ',WIPESATR);
			for(count = 0; count < SHA1_Break; count++){
				COMMAND[CommandLineIndex][count] = WIPESATR[count];
			}
			CommandLineIndex++;
			}
			else{strcpy(ENVIRONMENT,buf);}
	}		
		if(CommandLineIndex==0 || StringMatch(ENVIRONMENT," ") == 0){
		return -1;}
}

/*
	restrict Str_Input < mybursize:4096
*/
int CommandMatch(char* Str_Input){
	if(strlen(Str_Input)>4096){return -1;}
	//Serach is fullname inside of commandlist
	for(count = 0; count < CommandLineIndex; count++){
		if(StringMatch(Str_Input,COMMAND[count])){printf("123");return 0;}
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

/*
	find last binary
*/
int Lastbinary(char* FullName, char* LastName){
	int i = 0;	
	char temp[BUFSIZE];
	count = 0;	
	if(IndexOf('/',FullName)){
		while(FullName[count] != '\0'){count ++;}
		count--;	
		while(FullName[count] != '/')
		{
			temp[i] = FullName[count]; 
			//printf("%c/n",FullName[count]);
			i++;
			count--;
		}
		strcpy(LastName,temp);
	}
}

/*
searching if there is a command match the last binary from command list
*/
int MatchLastbinary(char* CommandList, char* InputCommand){
	char temp1[BUFSIZE], temp2[BUFSIZE];
	Lastbinary(CommandList, temp1);
	printf("%s\n",temp1);
	Lastbinary(InputCommand, temp2);
	printf("%s\n",temp2);
	return StringMatch(temp1,temp2);
}
