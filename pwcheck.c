#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void stdinToString();
bool containsChar(char c);
void stringToStdout();

char passwdString[100];

int main(int argc, char *argv[]){

    bool condition = true;

    do{
        stdinToString();
        stringToStdout();

    }while(  containsChar(EOF) != true);                // posledni heslo pouzijeme jako nosic pro informaci o konci souboru 
                                                        // pokud obsahuje EOF => proud skoncil a prestaneme nacitat hesla    
return 0;
}

// naplni globalni pole znaku heslem 
void stdinToString(){
    char c;
    int i = 0;
    
    while( (c = getchar()) != '\n' ){
        passwdString[i] = c;                            //globalni promenna
        i++;

        if(c == EOF){                                   // pokud nastane => {'passwdString[0]',...,'passwdString[i - 1]',EOF,'\0'}
            break;                                      // to znamena ze ze posledni znak v poli nam poslouzi jako nosic informace ze proud skoncil
        }                                               // coz zjistime pomoci containsChar( pole, EOF ) - zjistime ze obsahuje a ukoncime cyklus nacitani hesel v main
    }  
    passwdString[i] = '\0';
}

// vraci, jestli se znak nachazi v poli znaku
bool containsChar(char c){

    for (int i = 0; passwdString[i] != '\0' ; i++)
    {
        if( passwdString[i] == c){
            return true;
        }
    }
return false; 
}

// posila prave nactene heslo (globalni string) do stdout
void stringToStdout(){

    for (int i = 0; passwdString[i] != '\0' ; i++)
    {
        if(passwdString[i] == EOF)                      // diky podmince nevypiseme EOF, ktery je na konci posledniho hesla
            return;  
                                               // kvuli rozpoznani konce proudu a uzavreni cyklu v main
        putchar( passwdString[i] );
    }
    putchar('\n');
}


