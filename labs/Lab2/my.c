#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_SIZE 10000
#define clear() printf("\033[H\033[J")

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
    if (tokenIndex != 0){
        tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
        strcpy(tokens[tokenNo++], token);
        tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}
// cat shell.c | grep and
// cmd1 cat 

/*
I TRIED SO HARD AND GOT SO FAR
IN THE END IT DOESN'T EVEN MATTER
*/

void pipe_implementation(char * cmd0,char * cmd1,char * cmd2,char * cmd3){
    if(!strcmp(cmd0,"cat") && !strcmp(cmd2,"grep")){
        char arr[MAX_INPUT_SIZE];
        FILE * fpr;
        fpr = fopen(cmd1,"r");
        char c;
        c = fgetc(fpr);
        int prt=0;
        while(c!=EOF){
            arr[prt++] = c;
            // printf("%c",c);
            c = fgetc(fpr);
        }
        arr[prt] = '\0';
        if(strstr(arr,cmd3)!=NULL){
            printf("found \n");
        }
    }
}
// void pipe_implementation(char * cmd0,char * cmd1,char * cmd2,char * cmd3){
//     char fd[2];
    
//     if(pipe(fd)==-1){
//         printf("error");
//     }
    
//     int id = fork();
//     if (id == 0) {
//         close(fd[0]);
//         char* x=(char*)malloc(MAX_INPUT_SIZE);
//         if (!strcmp(cmd0,"cat")){
//             char filecontent[MAX_INPUT_SIZE];
//             FILE * fpr;
//             fpr = fopen(cmd1,"r");
//             char c;
//             c = fgetc(fpr);
//             int prt=0;
//             while(c!=EOF){
//                 filecontent[prt++] = c;
//                 printf("%c",c);
//                 c = fgetc(fpr);

//             }
//             filecontent[prt] = '\0';
//             fclose(fpr);
//             x=filecontent;
//         }

//         write(fd[1],x,MAX_INPUT_SIZE);
//         close(fd[1]);
//     } else {
//         close(fd[1]);
//         char* y=(char*)malloc(MAX_INPUT_SIZE);
//         read(fd[0],y,MAX_INPUT_SIZE);
//         if(!strcmp(cmd2,"grep")){
//             printf("%s \n",y);
//         }
//         close(fd[0]);

//     }
// }

int main(int argc , char * argv[],char *  envparameters[]){
    char * line;
    char * history[MAX_SIZE];
    char **tokens;
    char fileline[MAX_INPUT_SIZE];
    int i = 0;

    FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

    while(1){
        
            
        bzero(fileline, sizeof(fileline));
		if(argc == 2) { // batch mode
			// line = malloc(10000);
			if(fgets(fileline, sizeof(fileline), fp) == NULL) { // file reading finished
				break;	
			}
			
			fileline[strlen(fileline) - 1] = '\0';
            line = malloc(MAX_INPUT_SIZE);

            strcpy(line,fileline);
        }else{
            printf("CSE-114-053~$ ");
            line=malloc(200);

        
            scanf("%[^\n]", line);
            getchar();
        }

        // printf("the line is : %s \n ",line);
        history[i] = malloc(MAX_INPUT_SIZE);
        history[i++] = line;


        if(!strcmp(line,"history")){
            for(int t=0;t<i;t++){
                printf("%s \n",history[t]);
            }
        }

        else if(!strcmp(line,"clear")){
            clear();
        }
        else if (!strcmp(line,"env")){
            for (int tt = 0; envparameters[tt] != NULL; tt++){    
                printf("%s \n", envparameters[tt]);
            }
        }
        else if(!strcmp(line,"ls")){
            struct dirent *de;
            DIR *directory = opendir(".");
            if (directory == NULL)
            {
                printf("Could not open current directory" );
                continue;
            }
            while ((de = readdir(directory)) != NULL)
                    printf("%s\n", de->d_name);
        
            closedir(directory);
        }else if(!strcmp(line,"pwd")){
            char cwd[500];
            if(getcwd(cwd,sizeof(cwd))!=NULL){
                printf("%s \n",cwd);
            }
        }
        else if(!strcmp(line,"whoami")){
            char * username;
            username = getenv("USER");
            printf("%s \n",username);
        }
        else if(!strcmp(line,"exit")){
            break;
        }else{
            line[strlen(line)] = '\n'; //terminate with new line
		    tokens = tokenize(line);
            // printf("Command %s does not exist currently in our shell \n",line);
            // for(int ter=0;tokens[ter]!=NULL;ter++){
            //     printf("%s \n",tokens[ter]);
            // }

            if(!strcmp(tokens[0],"mkdir")){
                struct stat st = {0};

                if (stat(tokens[1], &st) == -1) {
                    mkdir(tokens[1]);
                }else{
                    printf("directory already created\n");
                }
            }else if(!strcmp(tokens[0],"rmdir")){
                if(rmdir(tokens[1])!=0){
                    printf("no such directory exits\n");
                }else{
                    rmdir(tokens[1]);
                }

            }else if(!strcmp(tokens[0],"cd")){
                chdir(tokens[1]);
            }else if(!strcmp(tokens[0],"ls")){
                ;
            }else if(!strcmp(tokens[0],"touch")){
                FILE * fptr;
                fptr = fopen(tokens[1],"w");
                if(fptr == NULL){
                    printf("file cannot be created \n");
                    continue;
                }
                fclose(fptr);

            }else if(!strcmp(tokens[0],"rm")){
                if(!strcmp(tokens[1],"my-primitive-shell.c")){
                    printf("cannote delete code source file \n");
                    continue;
                }
                int rmindex = remove(tokens[1]);
                if(rmindex != 0){
                    printf("File cannot be deleted \n");
                }
            }else if(!strcmp(tokens[0],"cat")){
                int len=0;
                for(int tkn = 0;tokens[tkn]!=NULL;tkn++){
                    len++;
                }
                if(len > 2){
                    printf("1");
                    int flag=0;
                    for(int ter=0;tokens[ter]!=NULL;ter++){
                        if(!strcmp(tokens[ter],"|")){
                            printf("2");
                            flag=1;
                            pipe_implementation(tokens[0],tokens[1],tokens[3],tokens[4]);
                        }
                    }
                    if(!flag){
                        printf("Command %s does not exist currently in our shell \n",tokens[0]);

                    }
                    continue;
                }
                FILE * fpr;
                fpr = fopen(tokens[1],"r");
                char c;
                c = fgetc(fpr);
                while(c!=EOF){
                    printf("%c",c);
                    c = fgetc(fpr);
                }

                fclose(fpr);
            }else if(!strcmp(tokens[0],"echo")){
                for(int ter=1;tokens[ter]!=NULL;ter++){
                    printf("%s ",tokens[ter]);
                }
                printf("\n");
            }
            else if(!strcmp(tokens[0],"sleep")){
                sleep(atoi(tokens[1]));
                continue;

            }
            else{
                printf("Command %s does not exist currently in our shell \n",tokens[0]);

                
            }
        }




        // free(line);
        
    }

    
}