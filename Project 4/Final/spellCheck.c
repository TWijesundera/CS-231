/*
    Program 3
	Thisara Wijesundera
	CS-231
	Use pipes and signals to check if a word is spelled correctly

    Purpose
        C program to show the use of pipes and signals to
        check if a word from a file input is spelled correctly


    Algorithm
        Open the file for printing out to spellCheck.log
        Copy the input files into new variables
        Run functions realted to running the pipes
        and exec files that are used in this program
        Loop until all the processes are finished
        Close spellcheck.log
    
    External Data Table 
    NAME              DESCRIPTION 
    lexPid            Used to store the pid returned when forking from lex
    sortPid           Used to store the pid returned when forking from sort
    uniqPid           Used to store the pid returned when forking from uniq
    comparePid        Used to store the pid returned when forking from compare
    numberOfProcess   Used to store the number of completed processes
    openFile          Used to open the spellCheck.log for writing
    lexSortFd         Used as file descripters for piping from lex to sort
    sortUniqFd        Used as file descripters for piping from sort to uniq
    uniqCompareFd     Used as file descripters for piping from uniq to compare
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

// Define all the functions being used in this program
void runLex(char *);
void runSort();
void runUniq();
void runCompare(char *);
void handler(int);

int lexPid;
int sortPid;
int uniqPid;
int comparePid;
int numberOfProcess = 4;
FILE * openFile;

int lexSortFd[2];
int sortUniqFd[2];
int uniqCompareFd[2];

int main(int argc, char * argv[])
{
    /*
        main is the controlling function of the program.
        main will open spellCheck.log for writing. Copy the first argument
        file input to a variable called file. Copy the second argument to a 
        variable called dictionary file. Then it will call all the functions
        required for this program. Lex, sort, uniq, and compare. Then there is
        a while loop that checks how many processes are still open. Finally
        it will close spellCheck.log

        Data Table

        NAME               DESCRIPTION
        file               Hold the file argument for the file input
        dictionaryFile     Hold the file argument for the dictionary input
    */
    char file[256];
    char dictionaryFile[256];
    
    openFile = fopen("spellCheck.log", "w");
    signal(SIGCHLD, handler);

    strcpy(file, argv[1]);
    strcpy(dictionaryFile, argv[2]);

    // run all the commands
    runLex(file);
    runSort();
    runUniq();
    runCompare(dictionaryFile);

    /*
    printf("%d\n", lexPid);
    printf("%d\n", sortPid);
    printf("%d\n", uniqPid);
    printf("%d\n", comparePid);
    */
        
    while(numberOfProcess > 0)
    {
     //   fprintf(stderr,"%d ", numberOfProcess);
    }
    
    // close spellCheck.log
    fclose(openFile);
    return 0;
}

void runLex(char * file)
{
    /*
        This function forks a process to execute lex.out
        and pipe its output to sort.

        Data Table

        NAME               DESCRIPTION
        arguments          Array that holds the commands used to input into execvp 
    */
    char * arguments[] = {"lex.out", file, NULL};
    
    pipe(lexSortFd);
    lexPid = fork();
    
    if(lexPid == 0) // child
    {
        dup2(lexSortFd[1], STDOUT_FILENO);
        close(lexSortFd[0]);
        close(lexSortFd[1]);
        //fprintf(stderr, "in lex child");
        execv("./lex.out", arguments);
    }
    else if (lexPid == -1) // failure to create fork
    {
        fprintf(stderr, "lex fork failed");
    }
    else
    {
        close(lexSortFd[1]);
    }
}

void runSort()
{
    /*
        This function is used to execute the sort commmand
        on file input. It's output is piped to uniq

        Data Table

        NAME               DESCRIPTION
        arguments          Array that holds the commands used to input into execvp 
    */
    char * arguments[] = {"sort", "-f", NULL};

    pipe(sortUniqFd);
    sortPid = fork();

    if(sortPid == 0)
    {
        dup2(lexSortFd[0], STDIN_FILENO);
        close(lexSortFd[0]);
        //fprintf(stderr, "in sort child");
        dup2(sortUniqFd[1], STDOUT_FILENO);
        close(sortUniqFd[0]);
        close(sortUniqFd[1]);
        execvp("sort", arguments);
    }
    else if (sortPid == -1)
    {
        fprintf(stderr, "sort fork failed");
    }
    else
    {
        close(lexSortFd[0]);
        close(sortUniqFd[1]);
    }
}

void runUniq()
{
    /*
        This function is used to execute the uniq commmand
        on file input. It's output is piped to compare

        Data Table

        NAME               DESCRIPTION
        arguments          Array that holds the commands used to input into execvp 
    */
    char * arguments[] = {"uniq", "-i", NULL};
    
    pipe(uniqCompareFd);
    uniqPid = fork();
    
    if(uniqPid == 0) // child
    {
        dup2(sortUniqFd[0], STDIN_FILENO);
        close(sortUniqFd[0]);
        dup2(uniqCompareFd[1], STDOUT_FILENO);
        close(uniqCompareFd[0]);
        close(uniqCompareFd[1]);
        //fprintf(stderr, "in uniq child");
        execvp("uniq", arguments);
    }
    else if (uniqPid == -1) // failure to create fork
    {
        fprintf(stderr, "uniq fork failed");
    }
    else
    {
        close(sortUniqFd[0]);
        close(uniqCompareFd[1]);
    }
}

void runCompare(char * file)
{
    /*
        This function is used to execute the compare.out commmand
        on file input.

        Data Table

        NAME               DESCRIPTION
        arguments          Array that holds the commands used to input into execvp 
    */
    char * arguments[] = {"compare.out", file, NULL};

    comparePid = fork();

    if(comparePid == 0)
    {
        dup2(uniqCompareFd[0], STDIN_FILENO);
        close(uniqCompareFd[0]);
        close(uniqCompareFd[1]);
        //fprintf(stderr, "in lex child");
        execv("./compare.out", arguments);
    }
    else if(comparePid == -1)
    {
        fprintf(stderr, "compare fork failed");
    }
    else
    {
        close(uniqCompareFd[1]);
        //waitpid(comparePid, &status, 0); 
    }
}

void handler(int signum)
{
    /*
        This function is used to handle the signal SIGCHLD
        It waits for a pid and store the pid as a variable.
        Then it compares that pid to one of the global pids 
        to check if that thread was finished. Then subracts
        1 from the number of processes still running.

        Data Table

        NAME               DESCRIPTION
        pid                Used to store the pid that was finished
        status             Used to store the status of the pid
    */
    int pid; int status;

    pid = waitpid(-1, &status, WNOHANG);
    //fprintf(stderr,"%d\n", pid);
    if(pid == lexPid)
    {
        fprintf(openFile, "Child process lex.out with pid %d terminated\n", pid);
        numberOfProcess--;
    }
    else if (pid == sortPid)
    {
        fprintf(openFile, "Child process sort with pid %d terminated\n", pid);
        numberOfProcess--;
    }
    else if (pid == uniqPid)
    {
        fprintf(openFile, "Child process uniq with pid %d terminated\n", pid);
        numberOfProcess--;
    }
    else if (pid == comparePid)
    {
        fprintf(openFile, "Child process compare.out with pid %d terminated\n", pid);
        numberOfProcess--;
    }
}