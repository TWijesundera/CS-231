/*
	Thisara Wijesundera
	Program 4
	CS-231
	Use pipes and signals to check if a word is spelled correctly

	Algorithm
		Take name of file as command line argument
		Write all the words in that file to stdout
			only print out alphabetical characters (isAlpha())
			No change to case is allowed
	
	Purpose
		The purpose of this program is to print out to stdout
		all all the lines of the file input but only those that are alphabetical

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
		main is the controlling function of the program
		Here we iterate over the file input and printf each
		character after we make sure it is an alphabetical charcter
		
		Data Table

        NAME               DESCRIPTION
		fp				   Pointer that points to the input file
		line			   Used to store the first line from file input
		nlPtr			   Pointer that points to the new line character in a string
	*/
	
	FILE *fp;
	char line[256];
    fp = fopen(argv[1], "r"); // open as read only
        
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            	*nlPtr = '\0'; // remove the new line chracter from the string
            //printf("%s\n", line);
			for(int i=0; i < strlen(line); i++)
            {
				if(isalpha(line[i]))
				{
					printf("%c", line[i]);
				}
				else
				{
					printf("\n");
				}
			}
			printf("\n");
        }
    }
    
    else
        {return FALSE;}
        
    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
	
	return 0;
}
