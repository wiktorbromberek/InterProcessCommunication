#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

char *first_string(char* line){
    static char string[50];
    for(int i=0;i<sizeof(line);i++){
        if((int)line[i]!=32){
            string[i]=line[i];
        }
        else{
            break;
        }
    }
    return string;
}

char *middle_string(char* line){
    static char string[100];
    int start,stop;
    for (int i = 0; i < strlen(line); i++){
        if((int)line[i]==32){

        start=i+1;
        break;
        }
    }
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--){
        if((int)line[i]==32){
        stop=i-1;
        break;
        }
    }
    for (int i = start,j=0; i <= stop; i++){
        string[j]=line[i];
        j++;
    }
    return string;
}

char *last_string(char* line){
    static char string[50];
    int start;
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--){
        if ((int)line[i]==32){
            start=i+1;
            break;
        }
        
    }
    int j=0;
    for (int i = start; i < strlen(line); i++){
        string[j]=line[i];
        j++;
    }
    return string;
    
}

char *command_n_pipe(char* line){
    static char string[100];
    int stop;
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--){
        if ((int)line[i]==32){
            stop=i+1;
            break;
        }
    }
    for (int i = 0; i < stop; i++){
        string[i]=line[i];
    }
    return string;
}


int main(int argc, char *argv[]){

    char *plikZKolejkami= "plik.txt";            
    FILE *fp = fopen(plikZKolejkami, "r");
    if (fp == NULL){
    perror("fopen error");
    }
    char line[256];
    char fifo_name[256];
    while (fgets(line, sizeof(line), fp) != NULL){
        char process_name[256];
        sscanf(line, "%s : %s", process_name, fifo_name);
        if (strcmp(process_name, argv[1]) == 0){
            int id = mkfifo(fifo_name, 0666);
            break;
        }
    }
    fclose(fp);

int f = fork();                    
    if (f==0)//potomek                   
    {
        int ff=open(fifo_name, O_RDONLY);
        char buf[100];
        char fifo_temp[100];
        char command[100];
        if (ff!=-1){
            while(1){
                memset(command,0,sizeof(command));
                memset(fifo_temp,0,sizeof(fifo_temp));
                memset(buf,0,sizeof(buf));

                int r = read(ff, buf,sizeof(buf));

                if(r>0){
                    // printf("buf: %s\n",buf);
                    strcpy(command,command_n_pipe(buf));
                    strcpy(fifo_temp,last_string(buf));
                    // printf("%s\n",command);    
                    // printf("%s\n",fifo_temp);

                    int fork2 =fork();
                    if (fork2 ==0){
                        int id = open(fifo_temp,O_WRONLY);
                        close(1);
                        dup(id);
                        execl("/bin/sh", "/bin/sh", "-c", command, NULL);
                     }
                }
            }
        }
    }
    else{//rodzic                
        char process_name_2[256];
        char command[256];
        char fifo_name_temp[256];
        char buf[100];
        char input[100];

        while (1){
            memset(input,0,sizeof(input));
            scanf("%[^\n]%*c", input); 

            if(strcmp(input,"quit")!=0){
            strcpy(process_name_2,first_string(input));
            strcpy(command,middle_string(input));           
            strcpy(fifo_name_temp,last_string(input));

            int tempfd = mkfifo(fifo_name_temp,0666);        

            FILE *fp_2 = fopen(plikZKolejkami, "r");
            if (fp_2 == NULL)
            {
            perror("fopen error");
            }
            char line_2[256];
            char fifo_name_2[256];
            while (fgets(line_2, sizeof(line_2), fp_2) != NULL) {
                char process_name[256];
                sscanf(line_2, "%s : %s", process_name, fifo_name_2);       
                if (strcmp(process_name, process_name_2) == 0) {
                    int id = mkfifo(fifo_name_2, 0666);
                    break;
                }
            }
            int fifo2 = open(fifo_name_2,O_WRONLY);         
            strcat(command, " ");
            strcat(command,fifo_name_temp);
            int wr =  write(fifo2,command,strlen(command)); 
            memset(command,0,sizeof(command));
            memset(buf,0,sizeof(buf));
            int temp = open(fifo_name_temp,O_RDONLY);
            read(temp,buf,sizeof(buf));
            close(temp);
            unlink(fifo_name_temp);
            printf("%s\n",buf);

            }
            else{
                unlink(fifo_name);      
            }
        }
    }
return 0;
}