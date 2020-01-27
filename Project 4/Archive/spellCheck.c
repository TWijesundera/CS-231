/*
    Thisara Wijesundera
    Spell Check program
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void runLex(char *);
void runSort();
void runUniq();

int lexPid;
int sortPid;
int uniqPid;

int lexSortFd[2];
int sortUniqFd[2];

int main(int argc, char * argv[])
{
    int pid;
    int status;
    char file[256];

    strcpy(file, argv[1]);
    runLex(file);
    runSort();
    runUniq();
    wait(&status);
    wait(&status);
    wait(&status);
}

void runLex(char * file)
{
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
    char * arguments[] = {"uniq", "-i", NULL};
    
    uniqPid = fork();
    
    if(uniqPid == 0) // child
    {
        dup2(sortUniqFd[0], STDIN_FILENO);
        close(sortUniqFd[0]);
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
    }
}
