#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void stdinToString();
bool containsChar(char c);
void stringToStdout();

char passwdString[100];
bool endOfFile = false;

int main(int argc, char *argv[]){

    do{
        stdinToString();
        stringToStdout();

    }while( endOfFile == false );                       
 
return 0;
}

// naplni globalni pole znaku heslem 
void stdinToString(){
    char c;
    int i;
    
    for (i = 0; (c = getchar()) != '\n'; i++)
    {
        if(c == EOF){
            endOfFile = true;
            break;
        }
        passwdString[i] = c;
    }
    
    passwdString[i] = '\0';                              
}

// posila prave nactene heslo (globalni string) do stdout
void stringToStdout(){

    for (int i = 0; passwdString[i] != '\0' ; i++)
        putchar( passwdString[i] );
    
    if(endOfFile != true)
        putchar('\n');
}



