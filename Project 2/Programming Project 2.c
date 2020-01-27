/*
    Name: Thisara Wijesundera
    CS-231
    Program - Working with Strings
*/

/*
    The purpose of this program is to understand how strings
    are used in C. By taking an input of at max 100 strings
    that can have 100 chars at max. For each input we must
    output the line number, the original text, the amount
    of words in that line, how often each character appears
    in the line, the number of characters in the string,
    and the string in reverse.


	Test file must have a line more than 100 chars
	One or more lines with greater than 100 charsat least 1 line with exactly 100 chars 


    Program Algorithm
        1: Print the title and program description
        2: Read user line input until they press CRTL + D (get input from a test file)
        3: Read the first line and print the reverse of the line
        4: Read the first line and calculate the amount of words in that string
        5: Read the first line and calculate the number of times each character appears
        6: Read the first line and calculate the number of characters in the line
        7: Repeat steps 3-6 for all lines given as input

    The external data table 
    NAME              DESCRIPTION 

    TRUE              1 is true in C
    FALSE             0 is false in C 
    MAX_CHAR_ALLOWED  The maximum chars allowed for fgets input
    X                 The max chars allowed in the string array
    Y                 The max allowed lines in the string array
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define MAX_CHAR_ALLOWED 101 // Only 100 characers allowed in the string.
#define MAX_LINES 101
#define MAX_CHARS 101


void print_title_author(char * title, char * proginfo)
{
/*
    print_title_author prints the title of the program, the
    author information, including, and some information
    about the program.

    Data Table

    NAME               DESCRIPTION
    title              parameter - the title of the output.
    proginfo           parameter - identifies programming task.

*/

    printf("\t%s\n\n", title);
    printf("\tThisara Wijesundera\n\tCS-231\n\t%s\n\n", proginfo);
}

int find_number_of_words(char *string)
{
    /*
        Count the number of words in a line by counting the number of spaces
        exist within the line

        Data Table

        NAME               DESCRIPTION
        string             parameter - pointer pointing to current line that is being used
        wordcount          int holding the amount of words in the given line
        *token             value holding line information up to the next white space character
    */
    int wordcount = 0;
    char *token;
    
    token = strtok(string," "); // Initial string token
    if(token==NULL) {return(1);}
    while(token)
    {
        wordcount++;
        token = strtok(NULL," ");
    }
    return (wordcount);
}

void character_in_line(char *string)
{
    /*
        This function takes in a string, counts the amount of times each
        alphabetical character appears in the string and it counts the
        amount of character in the string
        

        Data Table

        NAME               DESCRIPTION
        string             parameter - pointer pointing to current line that is being used
        count              array of asciiConverted characters (into decimal)
        charLocation       counter for while loop to interate over string array
        asciiConverted     int to hold ascii characters that are converted into decimal (max 256)
        characterCount     The number of characters in the line (including special characters)
        length             Length of the line passed to the function
        currentChar        The current character that is being tested/converted
    */
	int count[256] = {0};
	int charLocation = 0, asciiConverted = 0, characterCount = 0;
    int length = strlen(string);
    char currentChar;
	
	while (charLocation < length)
	{
		string[charLocation] = tolower(string[charLocation]);
		
		if (string[charLocation] != '\0' || string[charLocation] != '\t')
		{
            if(string[charLocation] != ' ')
            {
                characterCount++; // Count everything but blank spaces
            }

			if(string[charLocation] >= 'a' && string[charLocation] <= 'z')
			{  
				asciiConverted = (int)(string[charLocation]); // convert char to ascii int value
				count[asciiConverted]++;
			}
		}
		charLocation++;
	}
    
	for(int i=0; i < 256; i++)
	{
		if(count[i] > 0) 
        {
            if(count[i] > 100)
            {printf("\t\t%c occurs %i times in the string\n", i, 100);}
            else
            {printf("\t\t%c occurs %i times in the string\n", i, count[i]);}
        }
	}
	if (characterCount >= 100)
	{printf("\t\tThere are %d characters in this line\n", 100);}
	else
	{printf("\t\tThere are %d characters in this line\n", characterCount-1);}
	
	int wordcount = find_number_of_words(string);
	printf("\t\tThere is/are %d words in this line\n", wordcount);
}

void reverse_words(char * start, char * end)
{
    /*
        This function is used to reverse the characters in a word
        
        Data Table

        NAME               DESCRIPTION
        start              parameter - pointer value pointing to the start of the string 
        end                parameter - Pointer pointing to the end of the string
        temp               Temporary value used to store the start character that's being pointed at
    */
    while(start < end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

char * word_start_index(char * p)
{
    /*
        This function is used to find the beginning of the word
        and it returns a pointer to that character
        
        Data Table

        NAME               DESCRIPTION
        p                  parameter - pointer value pointing to 
    */
    while((*p != '\0') && (*p == ' '))
    {++p;}

    if(*p == '\0')
        return NULL;
    else
        return p;
}

char * word_end_index(char * p)
{
    /*
        This function is used to find the last character in a word 
        and it returns a pointer to that chracter
        
        Data Table

        NAME               DESCRIPTION
        p                  parameter - Pointer pointing to 
    */
    while((*p != '\0') && (*p != ' '))
    {++p;}

    return p-1;
}


int main(int argc, char * argv[])
{
    /*  
        main is the controlling function of the program
        main will print out all the stdin lines with their line numbers
        at the front. Then all strings will be reversed and printed out.
        Next the characters with their alphabetical statistics are printed out,
        the amount of characters in the line, and the number of words in the line
        Finally, the total amount of words, chracters and alphabetical statistics
        will be printed out.
        
        
        main Data Table

        NAME               DESCRIPTION
        stringArray        Array where all fgets input is stored
        copiedString       Char array where the current line will be copied into
        arraySize          Size of the stringArray array
        nlPtr              Used to remove the newline character from a line.
        arrayPoint         Pointer pointing to the current string that needs to be reversed
        curWordStart       Pointer that points to the current starting character
        curWordEnd         Pointer that points to the current ending character
        totalWordCount     Total word count of the document
        totalCharacterCount Total character count of the document
        alphaCharCount     int array that hold the number of ascii characters that appear
        asciiConverted     int the hold the decimal value of an ascii character
    */
    char stringArray [MAX_LINES][MAX_CHARS]; // 100 lines 100 chars each // Change X and Y
    char copiedString[101]; // A container to copy strings into
    int arraySize = 0; //Size of the array

    print_title_author("Lab Assignment 2", "Second program in C Language");
    printf("Keyboard Input\n");

    // while there is input coming into the stdin keep adding to the stringArray
    // allow only 100 chars into the stdin
    while(arraySize < MAX_LINES && fgets(stringArray[arraySize], MAX_CHAR_ALLOWED, stdin ) != NULL) ++arraySize;
    printf("\n"); // print newline for pretty output
    
    // Print out all lines that were given as stdin
    printf("All strings inputed\n");
	for (int i = 0; i < arraySize; i++ ) 
    {
        if(strlen(stringArray[i]) == 1)
        {continue;}
        else
        // Print out the current line (from the stdin) with a line number (starting with 1)
        {printf("\t%d: %s", i+1, stringArray[i]);} 
    }
    
    // Print out all stings from stdin but with each word reversed
    printf("The reversed strings\n");
    for (int i=0; i < arraySize; i++)
    { 
        strcpy(copiedString, stringArray[i]);// Copy the string to avoid modifying the original string
        // Remove the new line character and replace with a NULL character
        char * nlPtr = strchr(copiedString, '\n');
        if (nlPtr != NULL)
        *nlPtr = '\0';
        
        char * arrayPoint = copiedString;
        
        if(strlen(stringArray[i]) == 1)
        {continue;}
        else
        {
            // Keep reading stdin until NULL is reached
            while(arrayPoint != "\0")
            {
                char* curWordStart = word_start_index(arrayPoint); // Find start of string and place in pointer
                if (curWordStart == NULL)
                    break;
                char* curWordEnd = word_end_index(curWordStart); // Find end of string and place in pointer
                reverse_words(curWordStart, curWordEnd); 
                arrayPoint = curWordEnd + 1; // Get the next word
            }
            printf("\t%d: %s\n", i+1, copiedString); // Print the reversed string 
        }
    }

    // Print the alphabetical total and the number of characters in the line
    printf("Stats of each line\n");
    for (int i=0; i < arraySize; i++)
    {
        strcpy(copiedString, stringArray[i]);
        printf("\tStats for line %d\n", i+1);
        character_in_line(copiedString);
    }    
    
    // Print stats for the total document
    printf("Total file stats\n");
    int totalWordCount = 0;
    int totalCharacterCount = 0;
    int alphaCharCount[256] = {0};
    int asciiConverted = 0;
    
    for(int i=0; i < arraySize; i++)
    {
        strcpy(copiedString, stringArray[i]);
        for (int character=0; character < strlen(copiedString); character++)
        {
            copiedString[character] = tolower(copiedString[character]);
		
		    if (copiedString[character] != '\0' || copiedString[character] != '\t')
		    {
                if(copiedString[character] != ' ')
                {
                    totalCharacterCount++; // Count everything but blank spaces
                }

			    if(copiedString[character] >= 'a' && copiedString[character] <= 'z')
			    {  
				    asciiConverted = (int)(copiedString[character]); // convert char to ascii int value
				    alphaCharCount[asciiConverted]++;
			    }
            }
        }
        totalWordCount += find_number_of_words(stringArray[i]);
    }
    
    for(int i=0; i < 256; i++)
	{
		if(alphaCharCount[i] > 0) 
        {
            printf("\t%c occurs %i times in the string\n", i, alphaCharCount[i]);
        }
	}
    printf("\tThe total words in this file is: %d\n", totalWordCount); // Print out the total words
    
    return 0;
}
