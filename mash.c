#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IS_LITERAL(string, length, literal)                                    \
    length == sizeof(literal) - 1 && strncmp(string, literal, length) == 0

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

        char *command = skip_whitespace(line);
        size_t command_length;
        for (command_length = 0; command[command_length] != '\0' &&
                                 !is_whitespace(command[command_length]);
             command_length++)
            ;

        if (IS_LITERAL(command, command_length, "exit"))
            return EXIT_SUCCESS;

        if (IS_LITERAL(command, command_length, "echo")) {
            char *next = skip_whitespace(command + command_length);
            for (; *next != '\0' && *next != '\n'; next++)
                if (!is_whitespace(*(next - 1)) || !is_whitespace(*next))
                    putchar(*next);
            printf("\n");
        } else if (IS_LITERAL(command, command_length, "pwd")) {
            char cwd[64];
            getcwd(cwd, 64);
            printf("%s\n", cwd);
        } else if (IS_LITERAL(command, command_length, "cd")) {
            char path[64];
            if (sscanf(command + command_length, "%63s", path) == EOF)
                perror("cd: failed to parse path");
            else if (chdir(path) != 0)
                perror("cd: failed to change directory");
        } else if (command_length != 0) {
            printf("%.*s: command not found\n", (int)command_length, command);
        }

        printf("\n");
    }
}
