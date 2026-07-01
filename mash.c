#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IS_LITERAL(string, length, literal)                                    \
    length == sizeof(literal) - 1 && memcmp(string, literal, length) == 0

static void print_prompt(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        printf("dang, I done lost track of where we at\n$ ");
    else
        printf("%s\n$ ", cwd);
}

static void read_line(char *line, size_t len) {
    if (!fgets(line, len, stdin)) {
        fprintf(stderr, "ay yo, couldn't catch what you sayin', my bad\n");
        exit(EXIT_FAILURE);
    }
}

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
        fprintf(stderr, "cd: yo dawg, you gotta tell me where we rollin'\n");
        return;
    }

    char path[1024];
    if (length >= sizeof(path)) {
        fprintf(
            stderr,
            "cd: that path too long dawg, we ain't tryna overflow the hood\n");
        return;
    }
    memcpy(path, arg, length);
    path[length] = '\0';

    if (chdir(path) != 0)
        perror("cd: nah homie, that hood don't exist");
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
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("pwd: dang, I done lost track of where we at");
    else
        printf("%s\n", cwd);
}

static void run_command(char *current, char *command, size_t len) {
    if (IS_LITERAL(command, len, "cd"))
        cd(current);
    else if (IS_LITERAL(command, len, "echo"))
        echo(current);
    else if (IS_LITERAL(command, len, "exit"))
        exit(EXIT_SUCCESS);
    else if (IS_LITERAL(command, len, "pwd"))
        pwd();
    else if (len != 0)
        printf("%.*s: that ain't no command I know, playa\n", (int)len,
               command);
    printf("\n");
}

int main(void) {
    for (;;) {
        print_prompt();

        char line[1024];
        read_line(line, sizeof(line));

        char *current = line;
        char *command;
        size_t command_len = next_word(&current, &command);

        run_command(current, command, command_len);
    }
}
