#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char line[64];

    for (;;) {
        printf("$ ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            perror("failed to read line");
            return EXIT_FAILURE;
        }

        char *cmd;
        size_t cmdlen;
        for (cmd = line; isspace(cmd[0]); cmd++)
            ;
        for (cmdlen = 0; cmd[cmdlen] != '\0' && !isspace(cmd[cmdlen]); cmdlen++)
            ;

        if (cmdlen == 0)
            continue;

        if (cmdlen == 4 && strncmp(cmd, "exit", cmdlen) == 0)
            return EXIT_SUCCESS;

        if (cmdlen == 4 && strncmp(cmd, "echo", cmdlen) == 0) {
            char *next;
            for (next = cmd + cmdlen; isspace(*next); next++)
                ;
            for (; *next != '\0' && *next != '\n'; next++)
                if (!isspace(*(next - 1)) || !isspace(*next))
                    putchar(*next);
            printf("\n");
            continue;
        }

        printf("%.*s: command not found\n\n", (int)cmdlen, cmd);
    }
}
