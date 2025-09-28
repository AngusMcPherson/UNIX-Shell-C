#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define NV 20  /* max number of command tokens */
#define NL 100 /* input buffer size */
#define backgroundMax 50
char line[NL]; /* command input buffer */ /*
                                          shell prompt
                                          */
typedef struct
{
    pid_t pid;
    char line[NL];
    int index;
} processStatus;
processStatus background[backgroundMax];
int backgroundCount = 0;
int backgroundReset = 0;

void prompt(void)
{
    fflush(stdout);
}

void childHandler()
{
    int status;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG);
    if (pid == -1)
    {
        if (backgroundCount > 0)
        {
            perror("waitpid");
        }
    }
    else
    {
        if (WIFEXITED(status))
        {
            for (int i = 0; i <= backgroundCount; i++)
            {
                if (background[i].pid == pid)
                {
                    printf("[%d]+ Done  %s\n", background[i].index, background[i].line);
                    backgroundReset--;
                    if (backgroundReset == 0)
                    {
                        backgroundCount = 0;
                    }
                    break;
                }
            }
        }
    }
}
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
int main(int argk, char *argv[], char *envp[])
{
    int frkRtnVal; /* value returned by fork sys call */
    int waitPid;
    char *v[NV];         /* array of pointers to command line tokens
                          */
    char *sep = " \t\n"; /* command line token separators */
    int i;               /* parse index */
    signal(SIGCHLD, childHandler);

    /* prompt for and process one command line at a time */

    while (1)
    { /* do Forever */
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);
        // This if() required for gradescope
        if (feof(stdin))
        { /* non-zero on EOF */
            exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
        {
            continue; /* to prompt */
        }

        int bg = 0;
        size_t len = strlen(line);
        if (line[len - 2] == '&')
        {
            bg = 1;
            line[len - 2] = '\0';
        }

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++)
        {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL)
            {
                break;
            }
        }
        /* assert i is number of tokens + 1 */
        if (!strcmp(v[0], "cd"))
        {
            if (v[1])
            {
                if (chdir(v[1]))
                {
                    perror("chdir");
                }
            }
            else
            {
                perror("no address");
            }
            continue;
        }
        /* fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork())
        {
        case -1: /* fork returns error to parent process */
        {
            break;
        }
        case 0: /* code executed only by child process */
        {
            execvp(v[0], v);
        }
        default: /* code executed only by parent process */
        {
            if (bg == 0)
            {
                waitPid = wait(0);
                if (waitPid == -1)
                {
                    perror("wait");
                }
            }
            else
            {
                background[backgroundCount].pid = frkRtnVal;
                strncpy(background[backgroundCount].line, line, NL);
                background[backgroundCount].index = backgroundCount + 1;
                printf("[%d] %d\n", background[backgroundCount].index, frkRtnVal);
                backgroundCount++;
                backgroundReset++;
            }
        }
        } /* switch */
    } /* while */
} /* main */