/*
	Program 3
	Thisara Wijesundera
	CS-231
	Recreate the 'cat' command
	
	Purpose
	    C program to recreate the UNIX cat command. It is used to output the
	    contents of a file to the stdout for user reading       
	
	
	Algorithm
		Get command line argument input
		Check with a switch for the options
		
		If there less than one argument 
			take stdin and after each line print out the input
			Read from stdin with fgetc and putchar to the stdout?
		
		If there is 1 or more inputs check if the file exists
		before trying to printf
			Print error message if the file does not exist
		
		Using a switch read if there are any command switches //might not be able to use switch. Instead check if there is a - and see what comes after it.
			-b: prints the line number before each output
			-E: Write a $ at the end of each output of line (i.e hello -> hello$)
			-n: prints the line number before each output, but blanks count as a line
			(when b and n are used together use only b)
		
		Use fopen to open the file into a pointer FILE * fp
		While loop over the contents of the file with fgetc and 
		puts each character to output 
	
	External Data Table 
    NAME              DESCRIPTION 
    MAX_CHAR_FILE     Max amount the file name can be
    TRUE              True value; 1 is true in C
    FALSE             FALSE value; 0 is false in C	
	argE              Used to check if the E flag is set
	argB              Used to check if b flag is set
	argN	          Used to check if n flag is set
*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_CHARS_FILE 255
#define TRUE 1
#define FALSE 0

static int argE = FALSE;
static int argB = FALSE;
static int argN = FALSE;

void check_for_arguments(char * string)
{
    /*
        This function takes the arguments from the command line
        Finds the '-' character and checks whether the character is a
        b, n, or E. If they are then the static vairables are set to true 
        for future use.
        
        Data Table

        NAME               DESCRIPTION 
        argument           Character to search string for
        argPtr             Pointer pointing to location of found character
    */
    char argument = '-';
    char * argPtr = strchr(string, argument);
     
    if(argPtr == NULL)
    {return;}
    else
    {
        //printf(argPtr);
        int position = string - argPtr;
        //printf("%c\n",string[position+1]);
        if(string[position+1] == 'b')
        {
            //printf("b");
            argB = TRUE;
            if(string[position+2] == 'E')
            {
                //printf("E");
                argE = TRUE;
                if(string[position+3] == 'n')
                {
                    //printf("n");
                    argN = TRUE;
                }
            }
            else if(string[position+2] == 'n')
            {
                //printf("n");
                argN = TRUE;
                if(string[position+3] == 'E')
                {
                    //printf("E");
                    argE = TRUE;
                }
            }
        }
        if(string[position+1] == 'E')
        {
            //printf("E");
            argE = TRUE;
            if(string[position+2] == 'b')
            {
                //printf("b");
                argB = TRUE;
                if(string[position+3] == 'n')
                {
                    //printf("n");
                    argN = TRUE;
                }
            }
            else if (string[position+2] == 'n')
            {
                //printf("n");
                argN = TRUE;
                if(string[position+3] == 'b')
                {
                    //printf("b");
                    argB = TRUE;
                }
            }
        }
        if(string[position+1] == 'n')
        {
            //printf("n");
            argN = TRUE;
            if(string[position+2] == 'b')
            {
                //printf("b");
                argB = TRUE;
                if(string[position+3] == 'E')
                {
                    //printf("E");
                    argE = TRUE;
                }
            }
            else if(string[position+2] == 'E')
            {
                //printf("E");
                argE = TRUE;
                if(string[position+3] == 'b')
                {
                    //printf("b");
                    argB = TRUE;
                }
            } 
        }
    }
}

int normal_file_print(char * string)
{
   /*
        Given a command line argument and tries to open the file using fopen
        If successfull in opening then it will take the file contents and 
        output them to the stdout and returns TRUE. If unsuccessful the
        function will return false.
   
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    
            
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            printf("%s\n", line);
            //printf("%c",'$');
        }
    }
    
    else
        {return FALSE;}
        
    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
}

int print_line_numbers_blanks(char * string)
{
    /*
        This function is used to print line numbers in addition to the file
        contents, however, this function does not print line numbers for
        blank lines in the file given to it.
        
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
        lineNumber         Used to print the line number to output
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    int lineNumber = 1;
    
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            //printf("%s\n", line);
            if(strlen(line) > 0)
            {
                printf("\t%d  ", lineNumber);
                for(int i=0; i < strlen(line); i++)
                {
                    if(line[i] != ' ')
                        printf("%c", line[i]);
                    else
                        printf("%c", ' ');
                }
                printf("\n");   
                lineNumber++;
            }
            else
            {
                printf("\n");
                continue;
            }
        }
    }

    else
        {return FALSE;} // file is invalid
    
    if(fp != NULL)
        {
            fclose(fp);
            return TRUE;
        }
}

int print_blank_with_dollar(char * string)
{
    /*
        This function is used to print a dollar sign at the end of each line
        in the file given as a command line argument. This will also print
        the line number of each line, but not for blank lines.
        
        
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
        lineNumber         Used to print the line number to output
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    int lineNumber = 1;
    
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            //printf("%s\n", line);
            if(strlen(line) > 0)
            {
                printf("\t%d  ", lineNumber);
                for(int i=0; i < strlen(line); i++)
                {
                    if(line[i] != ' ')
                        printf("%c", line[i]);
                    else
                        printf("%c", ' ');
                }
                printf("\n");   
                lineNumber++;
            }
            else
            {
                printf("$\n");
                continue;
            }
        }
    } 
    else
        {return FALSE;}
    
    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
}

int print_with_all_line_numbers(char * string)
{
    /*
        This function is used to print the lines in the input file
        provided and prints the line number next to it. This is done
        for blank lines as well
        
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    int lineNumber = 1;
            
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            printf("\t%d  %s\n",lineNumber, line);
            //printf("%c",'$');
            lineNumber++;
        }
    }
    else
        {return FALSE;}
        
    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
}

int print_with_dollar(char * string)
{
    /*
        This function prints all lines for a given file input with
        dollar signs at the end of each line
    
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            printf("%s$\n",line);
        }
    }
    else
        {return FALSE;}

    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
}

int print_all_with_dollar(char * string)
{
    /*
        This function is used to print all line numbers and a dollar sign
        at the end of each line.
        
        Data Table

        NAME               DESCRIPTION
        string             parameter - Passes along the file name
        fp                 Pointer that points to a FILE in the current directory
        line               String to hold the current line of the file
        nlPtr              Pointer used to set the new line character to null
        lineNumber         Used to count the number of lines in the file
    */
    
    FILE *fp; // pointer pointing to the file
    char line[256];
    fp = fopen(string, "r"); // open as read only
    int lineNumber = 1;
    
    if(fp != NULL)
    {
        while((fgets(line, 256, fp)) != NULL)
        {
            char * nlPtr = strchr(line, '\n');
            if (nlPtr != NULL)
            *nlPtr = '\0'; // remove the new line chracter from the string
            printf("\t%d  %s$\n",lineNumber,line);
            lineNumber++;
        }
    }
   
    else
        {return FALSE;}
    
    if(fp != NULL)
    {
        fclose(fp);
        return TRUE;
    }
}

int argument_print(char * fileName)
{  
    /*
        The purpose of this function is to pass the input file
        to the correct line writing function. Additionally it is used
        to check whether or not the argument passed is a valid file
        or just a switch for the program.
        
        Data Table

        NAME               DESCRIPTION
        isFileValid        Boolean that returns whether or not the file is a valid file
    */
    
    int isFileValid;
    
    if(strchr(fileName, '-'))
    {
        if(strchr(fileName, 'b'))
        {
            isFileValid = TRUE;
            return isFileValid;
        }
        else if(strchr(fileName, 'n'))
        {
            isFileValid = TRUE;
            return isFileValid;    
        }
        else if(strchr(fileName, 'E'))
        {
            isFileValid = TRUE;
            return isFileValid;    
        }
        else
        {
            isFileValid = FALSE;
            return isFileValid;    
        }
    }
     
    if(argB == TRUE)
    {
        if(argE == TRUE)
        {
            //printf("argE is true\n");
            /*
            print out \t1  Hello World$
            $
            \t2  This is a test$
            */
            isFileValid = print_blank_with_dollar(fileName);
            return isFileValid;
        }   
        else
        {
            //printf("argB is the only one true");
            isFileValid = print_line_numbers_blanks(fileName);
            return isFileValid;
        }     
    }

    else if(argN == TRUE)
    {
        if(argB == TRUE)
        {
            if(argE == TRUE)
            {
                /*
                print out \t1: Hello World$
                $
                \t2: This is a test$
                */
                isFileValid = print_blank_with_dollar(fileName);
                return isFileValid;
            }
            else
            {
                /*
                print out \t1: Hello World

                \t2: This is a test
                */
                isFileValid = print_line_numbers_blanks(fileName);
                return isFileValid;
            }
        }
        else
        {
            if(argE == TRUE)
            {
                isFileValid = print_all_with_dollar(fileName);
                return isFileValid;
            }
            else
            {
                /*
                print out \t1: Hello World
                            2: 
                          \t3: This is a test
                */
                isFileValid = print_with_all_line_numbers(fileName);
                return isFileValid;
            }               
        }        
    }

    else // argE
    {
        if(argB == TRUE)
        {
            // doesnt matter if n is switched
            isFileValid = print_blank_with_dollar(fileName);
            return isFileValid;
        }
        else if(argN == TRUE)
        {
            isFileValid = print_all_with_dollar(fileName);
            return isFileValid;
        }
        else
        {
            isFileValid = print_with_dollar(fileName);
            return isFileValid;
        }
    }
}

int main(int argc, char * argv[])
{
    /*
        main is the controlling function of the program.
        main will check whether there is more than 1 argument.
        If there is less then it will read from stdin and immediate print
        to stdout. If there is more than 1 argument then check all the 
        arguments in the command and check what switches are turned on
        If there are switches that are true then print out the file with the 
        proper switches. Otherwise, print out the files without switches
        
        Data Table

        NAME               DESCRIPTION
        fileName           Holds the name of the file
        fileValid          Default is TRUE, boolean to tell if the argument is a valid file         
        input              Variable used to store user input
    */
    
    char fileName[MAX_CHARS_FILE]; // String that hold the name of the file
    int fileValid = TRUE;
    char input[500]; // Used to hold user input
    
    if(argc <= 1)
    {
        while(fgets(input, 500, stdin))
        {
            printf("%s\n", input); // print the user input
            memset(input, 0, strlen(input)); // reset the memory to 0 to avoid memory leaking
        }
    }    
    
    if(argc >= 2)
    {
        argB = FALSE;
        argN = FALSE;
        argE = FALSE;
        for(int j=1; j < argc; j++)
            {check_for_arguments(argv[j]);} // Set all the arguments first
        
        for(int i=1; i < argc; i++)
        {
            strcpy(fileName, argv[i]); // get the first file (file at i)
            
            if(argB == TRUE || argN == TRUE || argE == TRUE)
                {fileValid = argument_print(fileName);}
            else
                {fileValid = normal_file_print(fileName);}
            
            // Get a return variable telling the user if the arguments worked or not
            if(fileValid == FALSE)
                {printf("%s: No such file or directory\n", fileName);}
        }
    }	
	return 0;
}
