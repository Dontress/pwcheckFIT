#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 100  // maximalni delka hesla


typedef struct{
    char passwdString[ MAXLEN ];
    bool endOfFile;
    bool isNormalSize;
    int passwdLen;
}Password;

typedef struct{
    int varChar;
    int shortest;
    int totalLen;
    int passwdCount;
}Stats;

// Function prototypes
Password stdinToString();
void stringToStdout( Password password );
bool containsLower();
bool containsUpper();
bool containsNumber();
bool containsSpecialChar();
bool containsSequence( char passwdString[ MAXLEN ], int lenOfPasswd, int lenOfSeq );
bool containsTwoSameSubstrings(char passwdString[ MAXLEN ], int lenOfPasswd, int lenOfCommonString);
bool firstLevelSecurity( Password password);
bool secondLevelSecurity(Password password, int param);
bool thirdLevelSecurity(Password password, int param);
bool fourthLevelSecurity(Password password, int param);
void setStats(Password passwd, Stats* stats);
bool paramCheck(int argCount, int level,int param, char* statsString);
bool stringCompare(char* str1, char* str2);
int stringLen(char *str);

//--------------
// FUNKCE MAIN
//--------------
int main(int argc, char *argv[]){
    int level = 0, param = 0;
    level = atoi(argv[1]);
    param =  atoi(argv[2]);

    if( !paramCheck(argc, level, param, argv[3]) )      // kontrola platnosti parametru
      return 1;

    // deklarovani struktury pro praci s heslem
    Password password;

    // deklarovani a vynulovani Stats
    Stats stats;
    stats.shortest = MAXLEN;
    stats.totalLen = 0;
    stats.varChar = 0;
    stats.passwdCount = 0;

    do{
        password = stdinToString();    // nacteni hesla
        setStats(password, &stats);

        switch ( level )
        {
        case 1 :
            if( firstLevelSecurity( password ) )
                stringToStdout( password );       // vypsani hesla
            break;
        
        case 2 :
            if( secondLevelSecurity( password, param ) )
                stringToStdout( password );
            break;
        
        case 3 :
            if( thirdLevelSecurity( password, param ) )
                stringToStdout( password );
            break;
        
        case 4 :
            if( fourthLevelSecurity( password, param ) )
                stringToStdout( password );
            break;

        default :
            printf("\nerror with parameter LEVEL\n");
            return 1;
        }

    }while( password.endOfFile == false ); 

    // zjistovani zadani parametru --stats a pripadne vypsani statistik
    if( stringCompare( argv[3] , "--stats" ) )
        printf("\nStatistika:\nMinimalni delka: %d\nPrumerna delka: %0.1f", stats.shortest, stats.totalLen/(float)stats.passwdCount);

return 0;
}

// naplni globalni pole znaku heslem 
Password stdinToString(){
    int c;
    int i;

    Password passwd;
    passwd.endOfFile = false;

    passwd.isNormalSize = true;           
    
    for (i = 0; ( c = getchar() ) != '\n'; i++)
    {
         if( c == EOF ){
            passwd.endOfFile = true;       
            break;                  // diky break se EOF nezapise do hesla
        }

        if( i >= MAXLEN ){            // osetreni presahnuti maximalni mozne delky => heslo je povazovano za neplatne
            passwd.isNormalSize = false;   // nastaveni globalni promenne 
            continue;
        }

        passwd.passwdString[i] = c;
    }

    passwd.passwdLen = i;
                        // zavolani funkce na vypocteni statistik o heslech

    if( passwd.isNormalSize )
        passwd.passwdString[i] = '\0';      

    return passwd;                  
}

// posila prave nactene heslo (globalni string) do stdout
void stringToStdout(Password passwd){

    static int passwdNumber = 0; //zajistuje ze prvni heslo nebude odradkovane

    if( passwdNumber > 0 ) // zajistuje odradkovani hesel na vystupu
        putchar( '\n' );

    for (int i = 0; passwd.passwdString[i] != '\0' ; i++)
        putchar( passwd.passwdString[i] );

    passwdNumber++;       // pocitani vypsanych hesel
}

// vraci, jestli heslo obsahuje alespon jedno male pismeno
bool containsLower( char passwdString[ MAXLEN ] ){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= 'a' && passwdString[i] <= 'z' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno velke pismeno
bool containsUpper( char passwdString[ MAXLEN ] ){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= 'A' && passwdString[i] <= 'Z' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno cislo
bool containsNumber( char passwdString[ MAXLEN ]){
    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= '0' && passwdString[i] <= '9' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jeden specialni znak
bool containsSpecialChar( char passwdString[ MAXLEN ]){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( (passwdString[i] >= 32 && passwdString[i] < '0')  ||    // magicke cisla 32 a 126 vznikla v zadani projektu
            (passwdString[i] > '9' && passwdString[i] < 'A')  ||
            (passwdString[i] > 'Z' && passwdString[i] < 'a')  ||
            (passwdString[i] > 'z' && passwdString[i] <= 126)   )
                return true;
    }
return false;
}

// vraci, jestli obsahuje sekvenci lenOfSeq stejnych znaku
bool containsSequence( char passwdString[ MAXLEN ], int lenOfPasswd, int lenOfSeq ){

    bool seqIsSame;

    if( lenOfPasswd >= lenOfSeq ){   // jestlize je sekvence delsi nez heslo, heslo sekvenci nemuze obsahovat

        // X = (lenOfSeq - 1)
        // kazdy znak v heslu testujeme s X znaky za nim
        // pokud alespon jeden neni stejny jako pocatecni znak => sekvence neni stejna
        // po projeti X znaku za pocatecnim znakem se pocatecni znak zveda o 1, seqIsSame se prepise na true
        // a opet porovnavame jestli jsou dalsi znaky stejne

        for (int i = 0; i <= lenOfPasswd - lenOfSeq; i++)       
        {
            seqIsSame = true;
            for (int j = 1; j < lenOfSeq; j++)
            {
                if( passwdString[i] != passwdString[j + i] )
                    seqIsSame = false;
            }

            if( seqIsSame == true )  
                return true;
        }
    }
return false;
}

// vraci, jestli heslo obsahuje dva stejne podretezce
bool containsTwoSameSubstrings(char passwdString[ MAXLEN ], int lenOfPasswd, int lenOfCommonString){
    int streak = 0;                                                         

    if( lenOfPasswd >= lenOfCommonString * 2 ){

        // Prochazi pole od 0 do (maximalni delky minus delky spolecneho retezce)
        // a hleda stejny znak, pokud ho najde, testujou se dvojce znaku na i+1 a j+1.
        // Vzdy kdyz se znaky rovnaji, zvysi se streak a pokud streak == lenOfCommonString funkce vraci true.
        // Pokud se dalsi znaky v rade nerovnaji a streak != lenOfCommonString, i se vrati zpet kde bylo pri detekovani  
        // prvniho spolecneho znaku, ale j zustane a dale se hleda prvni spolecny znak.

        for (int i = 0; i <= lenOfPasswd - lenOfCommonString; i++)      
        {
            for (int j = lenOfCommonString + i; j < lenOfPasswd; j++)
            {
                if( passwdString[i] == passwdString[j] )
                {
                    streak++; 
                    i++;                // zvednuti i a j zaroven pro testovani dalsich spolecnych znaku
                }
                else{
                    i = i - streak;     // vraceni i na pozici kde byl nalezen prvni spolecny znak, j zustava stejne
                    streak = 0;
                }
                     
                if( streak >= lenOfCommonString )
                    return true;
            }
        }
    }
return false;
}

// vraci, jestli plati pro uroven bezpecnosti 1
bool firstLevelSecurity( Password password){
    if( containsLower( password.passwdString ) && containsUpper( password.passwdString ) )
        return true;
    else    
        return false;
}

// vraci, jestli plati pro uroven bezpecnosti 2
bool secondLevelSecurity(Password password, int param){
    
    switch ( param )   // param ma hodnotu vstupniho parametru PARAM
    {
        case 1 :
        case 2 :{
            if( firstLevelSecurity( password ) )
                return true;
            else 
                return false;
        }
        case 3 :{
            if( firstLevelSecurity( password ) && ( containsNumber( password.passwdString ) || containsSpecialChar( password.passwdString) ) )
                return true;
            else 
                return false;
        }
        case 4 :{
            if( firstLevelSecurity( password ) && containsNumber( password.passwdString ) && containsSpecialChar( password.passwdString ) )
                return true;
            else
                return false;
        }
        default :{       // predpokladame ze PARAM > 4, dalsi jina situace diky paramCheck() jiz nemuze nastat
            if( firstLevelSecurity( password ) && containsNumber( password.passwdString ) && containsSpecialChar( password.passwdString ) )
                return true;
            else
                return false;
        }
    }   
}

// vraci, jestli plati pro uroven bezpecnosti 3
bool thirdLevelSecurity(Password password, int param){

    if( secondLevelSecurity( password, param ) && containsSequence( password.passwdString, password.passwdLen, param ) == false )
        return true;
    else
        return false;
}

// vraci, jestli plati pro uroven bezpecnosti 4
bool fourthLevelSecurity(Password password, int param){

    if( thirdLevelSecurity( password, param ) && containsTwoSameSubstrings( password.passwdString, password.passwdLen, param ) == false )
        return true;
    else
        return false;
}

// prace s daty a zapisovani statistik
void setStats(Password passwd, Stats* stats){
    
    stats->totalLen += passwd.passwdLen;                // pocitani prumerne delky
    stats->passwdCount++;

    if( passwd.passwdLen < stats->shortest)                // zjisteni nejkratsiho hesla
        stats->shortest = passwd.passwdLen ;
}

// kontrola zadanych parametru programu, vraci true v pripade spravneho zadani
bool paramCheck(int argCount, int level, int param, char *statsString){
    char statsStringModel[] = "--stats";                        // vzorove pole znaku pro porovnani dobrovolneho parametru --stats
    bool parametersRight = true;

    // testovani spravneho poctu argumentu
    if( argCount < 3 || argCount > 4 ){
        printf("\nerror, invalid number of parameters => /.pwcheck LEVEL PARAM [--stats]\n");
        parametersRight = false;
    }

    // kontrola hodnoty parametru urcujici uroven bezpecnosti
    if( level < 1 || level > 4 ){
        printf("/.pwcheck LEVEL PARAM [--stats]\n");
        printf("            ^ \n");
        printf("            | \n");
        printf("error, LEVEL parameter can only be >= 1 && <= 4 \n");
        parametersRight = false;
    }

    // kontrola hodnoty parametru PARAM urcujici dodatecny parametr
    if( param < 1 || param > 4){
        printf("/.pwcheck LEVEL PARAM [--stats]\n");
        printf("                  ^ \n");
        printf("                  | \n");
        printf("error, PARAM parameter can only be >= 1 && <= 4\n");
        parametersRight = false;
    }

    // kontrola tvaru zadani nepovinneho parametru --stats
    if( argCount == 4){                                         // pokud jsou 4 argumenty => byl zadan nepovinny --stats
       if( !stringCompare(statsString, statsStringModel)){
            printf("/.pwcheck LEVEL PARAM [--stats]\n");
            printf("                          ^ \n");
            printf("                          | \n");
            printf("error, expected \"--stats\" instead of \"%s\"\n", statsString);
            parametersRight = false;   
        }
    } 
return parametersRight;
}

// vraci jestli jsou retezce stejne nebo ne
bool stringCompare(char* str1, char* str2){

    int areSame = true;

    if( stringLen( str1 ) == stringLen( str2 ) ){           // pokud jsou stejne dlouhe, zaciname porovnavat znak za znakem
        for (int i = 0; str1[i] != '\0'; i++)
        {
            if( str1[i] != str2[i] )
                areSame = false;
        }
    }
    else areSame = false;
    
return areSame;
}

// vraci delku retezce
int stringLen(char *str){
    int i;

    for ( i = 0; str[i] != '\0'; i++){      
    }
    
return i;
}