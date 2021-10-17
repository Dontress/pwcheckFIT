#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 100  // maximalni delka hesla

void stdinToString();
void stringToStdout();
bool containsLower();
bool containsUpper();
bool containsNumber();
bool containsSpecialChar();
bool containsSequence(int lenOfSeq);
bool containsTwoSameSubstrings(int lenOfString);
int lenOfPassword();
bool firstLevelSecurity();
bool secondLevelSecurity(int param);

// globalni promenne na ulozeni hesla, informace o EOF, jestli neni heslo moc velke a PARAM 
char passwdString[MAXLEN];
bool endOfFile = false;
bool isNormalSize;
int param = 1;

// stats
int varChar = 0;
int shortest = 0;
int avgLen = 0;

//--------------
// FUNKCE MAIN
//--------------
int main(){
    param = 1;

    do{
        stdinToString();
        
        

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
        if( (passwdString[i] >= 32 && passwdString[i] < '0')  || 
            (passwdString[i] > '9' && passwdString[i] < 'A')  ||
            (passwdString[i] > 'Z' && passwdString[i] < 'a')  ||
            (passwdString[i] > 'z' && passwdString[i] <= 12)   )
                return true;
    }
    
return false;
}

// vraci, jestli obsahuje sekvenci lenOfSeq stejnych znaku
bool containsSequence(int lenOfSeq){

    int lenOfPasswd = lenOfPassword();
    bool seqIsSame;

    if( lenOfPasswd >= lenOfSeq){

        for (int i = 0; i <= lenOfPasswd - lenOfSeq; i++)
        {
            seqIsSame = true;
            for (int j = 1; j < lenOfSeq; j++)
            {
                if( passwdString[i] != passwdString[j + i])
                    seqIsSame = false;
            }

            if( seqIsSame == true)
                return true;
        }
    }
return false;
}

// vraci delku hesla
int lenOfPassword(){
    int len = 0;

    for (len = 0; passwdString[len] != '\0'; len++){    
    }
    
return len;
}

// vraci, jestli heslo obsahuje dva stejne podretezce
bool containsTwoSameSubstrings(int lenOfCommonString){
    int lenOfPasswd = lenOfPassword();
    int streak = 0;                                                         

    if( lenOfPasswd >= lenOfCommonString * 2){

        // Prochazi pole od 0 do (maximalni delky minus delky spolecneho retezce)
        // a hleda stejny znak, pokud ho najde, testujou se dvojce znaku na i+1 a j+1.
        // Vzdy kdyz se znaky rovnaji, zvysi se streak a pokud streak == lenOfCommonString funkce vraci true.
        // Pokud se dalsi znaky v rade nerovnaji a streak != lenOfCommonString, i se vrati zpet kde bylo pri detekovani  
        // prvniho spolecneho znaku, ale j zustane a dale se hleda prvni spolecny znak.

        for (int i = 0; i <= lenOfPasswd - lenOfCommonString; i++)      
        {
            for (int j = lenOfCommonString + i; j < lenOfPasswd; j++)
            {
                if( passwdString[i] == passwdString[j])
                {
                    streak++; 
                    i++;                // zvednuti i a j zaroven pro testovani dalsich spolecnych znaku
                }
                else{
                    i = i - streak;     // vraceni i na pozici kde byl nalezen prvni spolecny znak, j zustava stejne
                    streak = 0;
                }
                     
                if(streak >= lenOfCommonString)
                    return true;
            }
        }
    }
return false;
}

// vraci, jestli plati pro uroven bezpecnosti 1
bool firstLevelSecurity(){
    if( containsLower() && containsUpper())
        return true;
    else    
        return false;
}

// vraci, jestli plati pro uroven bezpecnosti 2
bool secondLevelSecurity(int param){
    
    switch (param)
    {
        case 3:
            if( firstLevelSecurity() && ( containsNumber() || containsSpecialChar() ))
                return true;
            else 
                return false;
        
        case 4:
            if( firstLevelSecurity() && containsNumber() && containsSpecialChar() )\
                return true;
            else
                return false;
        
        default:
            if( firstLevelSecurity() )
                return true;
            break;
                return false;
    }
    
}