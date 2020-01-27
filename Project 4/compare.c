/*
    Thisara Wijesundera
    Program 4
    CS-231
    Use pipes and signals to check if a word is spelled correctly

    
    Algortihm
        Take 2 input files
        Scan in the first word of the file input
        Scan in the second word of the file input
        Use strcasecmp() to check if we get 0
            if we get 0 then the words are the same and the word is spelled correctly
            otherwise keep going through the dictionary until there are o
                words left in the dictionary
    
    Purpose
        This program is used to compare a word from stdin
        to a word from the dictionary. The dictionary will be traversed
        until a match is found. If a match is found then "CORRECT" 
        will be printed. Otherwise "INCORRECT" will be printed.
        
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char * argv[])
{
    /*
        main is the cotrolling function of the program. This will
        scan a word from the stdin and compare it with a word from the
        dictionary. If the strcmp is == 0 then "CORRECT" will be printed.
        Otherwise "INCORRECT" will be printed

        Data Table

        NAME               DESCRIPTION
        dictionaryPointer  Holds a pointer to the dictionary.txt file.
        line               Line from stdin
        copyLine           Stores a copy of line
        dictLine           Line from the dictionary file
        copyDictLine       Stores a compy of dictLine
        spelledCorrectly   variable used to take the comparison from strcmp
        scanFReturn        int that sotres the number reutned from scanf
        fgetsReturn        Char pointer that stores the return from fgets
    */
    FILE *dictionaryPointer;
    char line[256];
    char copyLine[256];
    char dictLine[256];
    char copyDictLine[256];
    int spelledCorrectly = 0;
    int scanFReturn;
    char * fgetsReturn;

    if(dictionaryPointer != NULL)
    {   
        scanFReturn = scanf("%s", line); // returns char count if true, negative if none
        while(scanFReturn > 0)
        {
            dictionaryPointer = fopen(argv[1], "r");
            strcpy(copyLine, line); // Copy the stdin

            //Change the line to lowercase
            for(int i=0; i < strlen(copyLine); i++)
                {copyLine[i] = tolower(copyLine[i]);}
            
            char * nlPtr = strchr(copyLine, '\n');
                if (nlPtr != NULL)
                    *nlPtr = '\0'; // remove new line character from the string
            //printf("%s\n", copyLine);

            fgets(dictLine, 255, dictionaryPointer);
            strcpy(copyDictLine, dictLine);
            
            //Change the line to lowercase
            for(int i=0; i < strlen(dictLine); i++)
                {copyDictLine[i] = tolower(copyDictLine[i]);}
            
            nlPtr = strchr(copyDictLine, '\n');
                if (nlPtr != NULL)
                    *nlPtr = '\0'; // remove the new line chracter from the string
            
            //printf("%s\n", copyDictLine);

            spelledCorrectly = strcmp(copyLine, copyDictLine);
            //printf("%d\n",spelledCorrectly);

            if (spelledCorrectly == 0)
            {printf("%s --- CORRECT\n", line);}
            else 
            {
                fgetsReturn = fgets(dictLine, 255, dictionaryPointer);
                while(fgetsReturn != NULL)
                {
                    strcpy(copyDictLine, dictLine);
            
                    //Change the line to lowercase
                    for(int i=0; i < strlen(dictLine); i++)
                        {copyDictLine[i] = tolower(copyDictLine[i]);}
                    
                    nlPtr = strchr(copyDictLine, '\n');
                        if (nlPtr != NULL)
                            *nlPtr = '\0'; // remove the new line chracter from the string                  

                    spelledCorrectly = strcmp(copyLine, copyDictLine);

                    if(spelledCorrectly == 0)
                    {
                        printf("%s --- CORRECT\n", line);
                        break;
                    }
                    fgetsReturn = fgets(dictLine, 255, dictionaryPointer); 
                }
                
                if (spelledCorrectly != 0)
                {
                    printf("%s --- INCORRECT\n", line);
                }
                
            }
            scanFReturn = scanf("%s", line);
            fclose(dictionaryPointer);
        }
        
    }        
    return 0;
}
