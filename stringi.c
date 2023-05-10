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
    for (int i = 0; i < strlen(line); i++)
    {
        if((int)line[i]==32){

        start=i+1;
        break;
        }
    }
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--)
    {
        if((int)line[i]==32){
        stop=i-1;
        break;
        }
    }
    int j=0;
    for (int i = start; i <= stop; i++)
    {
        string[j]=line[i];
        j++;
    }
    return string;
    
    
}

char *last_string(char* line){
    static char string[50];
    int start;
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--)
    {
        if ((int)line[i]==32)
        {
            start=i+1;
            break;
        }
        
    }
    int j=0;
    for (int i = start; i < strlen(line); i++)
    {
        string[j]=line[i];
        j++;
    }
    return string;
    
}

char *command_n_pipe(char* line){
    static char string[100];
    int stop;
    int end=strlen(line)-1;
    for (int i = end; i >=0; i--)
    {
        if ((int)line[i]==32)
        {
            stop=i+1;
            break;
        }
        
    }
    for (int i = 0; i < stop; i++)
    {
        string[i]=line[i];
    }
    return string;
}

int main(){
    char lines[]="pierwsze jakiś tekst nie ma znaczenia co ostatnie";
    char charrr[50];
    strcpy(charrr,first_string(lines));
    printf("%s\n",charrr);
    strcpy(charrr,middle_string(lines));
    printf("%s\n",charrr);
    strcpy(charrr,last_string(lines));
    printf("%s\n",charrr);
    strcpy(charrr,command_n_pipe(lines));
    printf("%s\n",charrr);


}
