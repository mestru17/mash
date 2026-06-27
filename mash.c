#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    for (;;) {
        printf("$ ");

        char line[64];
        if (fgets(line, sizeof(line), stdin) == NULL) {
            perror("failed to read line");
            return EXIT_FAILURE;
        }

        char *cmd;
        size_t cmdlen;
        for (cmd = line; isspace(*cmd); cmd++)
            ;
        for (cmdlen = 0; cmd[cmdlen] != '\0' && !isspace(cmd[cmdlen]); cmdlen++)
            ;

        if (cmdlen == 4 && strncmp(cmd, "exit", cmdlen) == 0)
            return EXIT_SUCCESS;

        if (cmdlen == 0) {
            // do nothing
        } else if (cmdlen == 4 && strncmp(cmd, "echo", cmdlen) == 0) {
            char *next;
            for (next = cmd + cmdlen; isspace(*next); next++)
                ;
            for (; *next != '\0' && *next != '\n'; next++)
                if (!isspace(*(next - 1)) || !isspace(*next))
                    putchar(*next);
            printf("\n");
        } else if (cmdlen == 3 && strncmp(cmd, "pwd", cmdlen) == 0) {
            char cwd[64];
            getcwd(cwd, 64);
            printf("%s\n", cwd);
        } else if (cmdlen == 2 && strncmp(cmd, "cd", cmdlen) == 0) {
            char path[64];
            if (sscanf(cmd + cmdlen, "%63s", path) == EOF)
                perror("cd: failed to parse path");
            else if (chdir(path) != 0)
                perror("cd: failed to change dir");
        } else {
            printf("%.*s: command not found\n", (int)cmdlen, cmd);
        }

        printf("\n");
    }
}
