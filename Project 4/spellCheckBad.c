/*
    Thisara Wijesundera

    Algorithm
        Take the fileInput and pipe that to the read end of lex.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void runLex(int pfd[], char * file);
void runSort(int pfd[]);
void handler(int signum);

char * cmdSort[] = {"sort", "-f"};
char * cmdUniq[] = {"uniq"};

int main(int argc, char * argv[], char * envp[])
{
    FILE * fromParent;
    FILE * toChild;
    pid_t pid;
    int fd[2];
    int status;
    char file[256];

    //signal(SIGCHLD, handler);
    
    strcpy(file, argv[1]);
    //printf("%s", file);

    if (pipe(fd) < 0){ exit(1);}

    runLex(fd, file);
    //runSort(fd);
    /*
    runUniq(fd);
    runCompare(fd);
    */
    close(fd[0]); close(fd[1]);
    while (pid = wait(&status) != -1) // Picks up all the dead childeren
        fprintf(stderr, "processes %d exits with %d\n", pid, WEXITSTATUS(status));
    exit(0);
}

void runLex(int pfd[], char * file)
{
    pid_t pidLex;
    int status;
    char * arguments[] = {"lex.out", file, NULL};

    char * nlPtr = strchr(file, '\n');
    if (nlPtr != NULL)
        *nlPtr = '\0';

    pidLex = fork();
    switch(pidLex)
    {
        case 0: // child
            dup2(pfd[0], 0);
            close(pfd[1]);
            printf("Lex - Child: %d Parent: %d", getpid(), getppid());
            execv("./lex.out", arguments);
            puts("lex\n");
        case -1:
            puts("fork");
            // do stuff
        default:
            close(pfd[0]);
            waitpid(pidLex, &status, WEXITSTATUS(status));
            //printf("break\n");
            //printf("%d", pidLex);
            break; //parent does nothing
            // do stuff
    }
}

void runSort(int pfd[])
{
    pid_t pidSort;
    int status;
    char * arguments[] = {"sort", "-f", NULL};

    pidSort = fork();
    switch(pidSort)
    {
        case 0:
            dup2(pfd[0], 0);
            close(pfd[1]);
            printf("sort - Child: %d Parent: %d\n", getpid(), getppid());
            execvp("sort", arguments);
            puts("sort");
        case -1:
            perror("fork");
        default:
            close(pfd[0]);
            waitpid(pidSort, &status, WEXITSTATUS(status));
            break;
    }

}

void handler(int signum)
{
    int pidReturned = 0;
    int status;

    if(signum == SIGCHLD)
    {
        pidReturned = wait(NULL);
        printf("\nChild process pid: %d\n", pidReturned);
    }
    signal(SIGCHLD, handler);
}