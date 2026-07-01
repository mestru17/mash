#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IS_LITERAL(s, len, lit)                                                \
    len == sizeof(lit) - 1 && memcmp(s, lit, len) == 0

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

static char *skip_whitespace(char *s) {
    while (is_whitespace(*s))
        s++;
    return s;
}

static char *skip_arg(char *s) {
    while (*s != '\0' && !is_whitespace(*s))
        s++;
    return s;
}

static size_t get_arg(char *s, char **arg) {
    *arg = skip_whitespace(s);
    return skip_arg(*arg) - *arg;
}

static void cd(char *input) {
    char *arg;
    size_t len = get_arg(input, &arg);
    if (len == 0) {
        fprintf(stderr, "cd: yo dawg, you gotta tell me where we rollin'\n");
        return;
    }

    char path[1024];
    if (len >= sizeof(path)) {
        fprintf(
            stderr,
            "cd: that path too long dawg, we ain't tryna overflow the hood\n");
        return;
    }
    memcpy(path, arg, len);
    path[len] = '\0';

    if (chdir(path) != 0)
        perror("cd: nah homie, that hood don't exist");
}

static void echo(char *input) {
    char *arg;
    size_t len = get_arg(input, &arg);
    if (len != 0) {
        printf("%.*s", (int)len, arg);
        for (input = arg + len; (len = get_arg(input, &arg)) != 0;
             input = arg + len)
            printf(" %.*s", (int)len, arg);
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

static void run_command(char *command, size_t len, char *input) {
    if (IS_LITERAL(command, len, "cd"))
        cd(input);
    else if (IS_LITERAL(command, len, "echo"))
        echo(input);
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

        char *command;
        size_t len = get_arg(line, &command);

        run_command(command, len, command + len);
    }
}
