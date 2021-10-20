#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 100  // maximalni delka hesla

// globalni promenne na ulozeni hesla, informace o EOF, jestli neni heslo moc velke a PARAM 
char passwdString[ MAXLEN ];
bool endOfFile = false;
bool isNormalSize;

// stats
int varChar = 0;
int shortest = MAXLEN;
int totalLen = 0;
int passwdCount = 0;

// Function prototypes
void stdinToString();
void stringToStdout();
bool containsLower();
bool containsUpper();
bool containsNumber();
bool containsSpecialChar();
bool containsSequence(int lenOfSeq);
bool containsTwoSameSubstrings(int lenOfString);
bool firstLevelSecurity();
bool secondLevelSecurity(int param);
bool thirdLevelSecurity(int param);
bool fourthLevelSecurity(int param);
void stats(int i);
bool paramCheck(int argCount, int level,int param, char* statsString);
bool stringCompare(char* str1, char* str2);
int stringLen(char *str);

//--------------
// FUNKCE MAIN
//--------------
int main(int argc, char *argv[]){

    int level = atoi(argv[1]);
    int param =  atoi(argv[2]);

    if( !paramCheck(argc, level, param, argv[3]) )      // kontrola platnosti parametru
      return 1;

    do{
        stdinToString();    // nacteni hesla

        switch ( level )
        {
        case 1 :
            if( firstLevelSecurity() )
                stringToStdout();       // vypsani hesla
            break;
        
        case 2 :
            if( secondLevelSecurity( param ) )
                stringToStdout();
            break;
        
        case 3 :
            if( thirdLevelSecurity( param ) )
                stringToStdout();
            break;
        
        case 4 :
            if( fourthLevelSecurity( param ) )
                stringToStdout();
            break;

        default :
            printf("\nerror with parameter LEVEL\n");
            return 1;
        }

    }while( endOfFile == false ); 

    // zjistovani zadani parametru --stats a pripadne vypsani statistik
    if( stringCompare( argv[3] , "--stats" ) )
        printf("\nStatistika:\nMinimalni delka: %d\nPrumerna delka: %0.1f", shortest, totalLen/(float)passwdCount);

return 0;
}

// naplni globalni pole znaku heslem 
void stdinToString(){
    int c;
    int i;

    isNormalSize = true;            // vyresetovani globalni promenne pro kazde heslo
    
    for (i = 0; ( c = getchar() ) != '\n'; i++)
    {
         if( c == EOF ){
            endOfFile = true;       // nastaveni globalni promenne na true pro ukonceni cyklu nacitani hesel v main
            break;                  // diky break se EOF nezapise do hesla
        }

        if( i >= MAXLEN ){            // osetreni presahnuti maximalni mozne delky => heslo je povazovano za neplatne
            isNormalSize = false;   // nastaveni globalni promenne 
            continue;
        }

        passwdString[i] = c;
    }

    stats( i );                     // zavolani funkce na vypocteni statistik o heslech

    if( isNormalSize )
        passwdString[i] = '\0';                        
}

// posila prave nactene heslo (globalni string) do stdout
void stringToStdout(){

    static int passwdNumber = 0; //zajistuje ze prvni heslo nebude odradkovane

    if( passwdNumber > 0 ) // zajistuje odradkovani hesel na vystupu
        putchar( '\n' );

    for (int i = 0; passwdString[i] != '\0' ; i++)
        putchar( passwdString[i] );

    passwdNumber++;       // pocitani vypsanych hesel
}

// vraci, jestli heslo obsahuje alespon jedno male pismeno
bool containsLower(){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= 'a' && passwdString[i] <= 'z' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno velke pismeno
bool containsUpper(){

    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= 'A' && passwdString[i] <= 'Z' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jedno cislo
bool containsNumber(){
    for (int i = 0; passwdString[i] != '\0'; i++)
    {
        if( passwdString[i] >= '0' && passwdString[i] <= '9' )
            return true;
    }
    
return false;
}

// vraci, jestli heslo obsahuje alespon jeden specialni znak
bool containsSpecialChar(){

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
bool containsSequence(int lenOfSeq){

    int lenOfPasswd = stringLen( passwdString );
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
bool containsTwoSameSubstrings(int lenOfCommonString){
    int lenOfPasswd = stringLen( passwdString );
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
bool firstLevelSecurity(){
    if( containsLower() && containsUpper() )
        return true;
    else    
        return false;
}

// vraci, jestli plati pro uroven bezpecnosti 2
bool secondLevelSecurity(int param){
    
    switch ( param )   // param ma hodnotu vstupniho parametru PARAM
    {
        case 1 :
        case 2 :{
            if( firstLevelSecurity() )
                return true;
            else 
                return false;
        }
        case 3 :{
            if( firstLevelSecurity() && ( containsNumber() || containsSpecialChar() ) )
                return true;
            else 
                return false;
        }
        case 4 :{
            if( firstLevelSecurity() && containsNumber() && containsSpecialChar() )
                return true;
            else
                return false;
        }
        default :{       // predpokladame ze PARAM > 4, dalsi jina situace diky paramCheck() jiz nemuze nastat
            if( firstLevelSecurity() && containsNumber() && containsSpecialChar() )
                return true;
            else
                return false;
        }
    }   
}

// vraci, jestli plati pro uroven bezpecnosti 3
bool thirdLevelSecurity(int param){

    if( secondLevelSecurity( param ) && containsSequence( param ) == false )
        return true;
    else
        return false;
}

// vraci, jestli plati pro uroven bezpecnosti 4
bool fourthLevelSecurity(int param){

    if( thirdLevelSecurity( param ) && containsTwoSameSubstrings( param ) == false )
        return true;
    else
        return false;
}

// prace s daty a zapisovani statistik
void stats(int passwdLen){
    
    totalLen += passwdLen;                // pocitani prumerne delky
    passwdCount++;

    if(passwdLen < shortest)                // zjisteni nejkratsiho hesla
        shortest = passwdLen ;
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