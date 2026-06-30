#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IS_LITERAL(string, length, literal)                                    \
    length == sizeof(literal) - 1 && memcmp(string, literal, length) == 0

static int is_whitespace(char c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
           c == '\v';
}

static size_t next_word(char **current, char **start) {
    while (is_whitespace(**current))
        (*current)++;
    *start = *current;
    while (**current != '\0' && !is_whitespace(**current))
        (*current)++;
    return *current - *start;
}

static void cd(char *input) {
    char *arg;
    size_t length = next_word(&input, &arg);
    if (length == 0) {
        fprintf(stderr, "usage: cd <path>\n");
        return;
    }

    char path[length + 1];
    memcpy(path, arg, length);
    path[length] = '\0';
    if (chdir(path) != 0)
        perror("cd: failed to change directory");
}

static void echo(char *input) {
    char *arg;
    size_t length = next_word(&input, &arg);
    if (length != 0) {
        printf("%.*s", (int)length, arg);
        while ((length = next_word(&input, &arg)) != 0)
            printf(" %.*s", (int)length, arg);
    }
    printf("\n");
}

static void pwd(void) {
    size_t size = 1024;
    char cwd[size];
    if (getcwd(cwd, size) == NULL)
        perror("pwd: failed to get working directory");
    else
        printf("%s\n", cwd);
}

int main(void) {
    for (;;) {
        printf("$ ");

        char line[1024];
        if (!fgets(line, sizeof(line), stdin)) {
            fputs("failed to read line\n", stderr);
            return EXIT_FAILURE;
        }

        char *current = line;
        char *command;
        size_t command_length = next_word(&current, &command);

        if (IS_LITERAL(command, command_length, "cd"))
            cd(current);
        else if (IS_LITERAL(command, command_length, "echo"))
            echo(current);
        else if (IS_LITERAL(command, command_length, "exit"))
            exit(EXIT_SUCCESS);
        else if (IS_LITERAL(command, command_length, "pwd"))
            pwd();
        else if (command_length != 0)
            printf("%.*s: command not found\n", (int)command_length, command);

        printf("\n");
    }
}
