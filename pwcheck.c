#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 100

void stdinToString();
void stringToStdout();
bool containsLower();
bool containsUpper();
bool containsNumber();
bool containsSpecialChar();

char passwdString[MAXLEN];
bool endOfFile = false;
bool isNormalSize;

int main(int argc, char *argv[]){

    do{
        stdinToString();

        if( isNormalSize )
            stringToStdout();

    }while( endOfFile == false );                       
 
return 0;
}

// naplni globalni pole znaku heslem 
void stdinToString(){
    char c;
    int i;

    isNormalSize = true;
    
    for (i = 0; (c = getchar()) != '\n'; i++)
    {
         if(c == EOF){
            endOfFile = true;
            break;
        }

        if(i >= MAXLEN){            // osetreni presahnuti maximalni mozne delky => heslo je povazovano za neplatne
            isNormalSize = false;
            continue;
        }

        passwdString[i] = c;
    }

    if(isNormalSize)
        passwdString[i] = '\0';                        
}

// posila prave nactene heslo (globalni string) do stdout
void stringToStdout(){

    static int passwdNumber = 0; //zajistuje ze prvni heslo nebude odradkovane

    if( passwdNumber > 0) // zajistuje odradkovani hesel na vystupu
        putchar('\n');

    for (int i = 0; passwdString[i] != '\0' ; i++)
        putchar( passwdString[i] );

    passwdNumber++;
}

// vraci, jestli heslo obsahuje alespon jedno male pismeno
bool containsLower(){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if(passwdString[i] >= 'a' && passwdString[i] <= 'z')
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno velke pismeno
bool containsUpper(){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if(passwdString[i] >= 'A' && passwdString[i] <= 'Z')
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno cislo
bool containsNumber(){
    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if(passwdString[i] >= '0' && passwdString[i] <= '9')
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jeden specialni znak
bool containsSpecialChar(){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= 32 && passwdString[i] < '0'  || 
            passwdString[i] > '9' && passwdString[i] < 'A'  ||
            passwdString[i] > 'Z' && passwdString[i] < 'a'  ||
            passwdString[i] > 'z' && passwdString[i] <= 126    )
                return true;
    }
    
return false;
}



