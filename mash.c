#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int is_whitespace(char c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
           c == '\v';
}

static char *skip_whitespace(char *c) {
    while (is_whitespace(*c))
        c++;
    return c;
}

int main() {
    for (;;) {
        printf("$ ");

        char line[64];
        if (!fgets(line, sizeof(line), stdin)) {
            fputs("failed to read line\n", stderr);
            return EXIT_FAILURE;
        }

        char *cmd = skip_whitespace(line);
        size_t cmdlen;
        for (cmdlen = 0; cmd[cmdlen] != '\0' && !is_whitespace(cmd[cmdlen]);
             cmdlen++)
            ;

        if (cmdlen == 4 && strncmp(cmd, "exit", cmdlen) == 0)
            return EXIT_SUCCESS;

        if (cmdlen == 4 && strncmp(cmd, "echo", cmdlen) == 0) {
            char *next = skip_whitespace(cmd + cmdlen);
            for (; *next != '\0' && *next != '\n'; next++)
                if (!is_whitespace(*(next - 1)) || !is_whitespace(*next))
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
                perror("cd: failed to change directory");
        } else if (cmdlen != 0) {
            printf("%.*s: command not found\n", (int)cmdlen, cmd);
        }

        printf("\n");
    }
}
