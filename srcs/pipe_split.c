#include "shell.h"

int pipecount (char *str) {
    int c = 1;
    int i = 0;
    while (str[i]) {
        if (i && str[i] == '|' && str[i - 1] != '>' && str[i - 1] != '<')
            c++;
        i++;
    }
    return c;
}

int pipelen(char *str) {
    int i = 0;

    while (str[i]) {
        if (i && str[i] == '|' && str[i - 1] != '>' && str[i - 1] != '<')
            break;
        i++;
    }
    return i;
}

char **pipe_split(char *str) {
    int len;
    int size = pipecount(str);
    char **tab = malloc((size + 1) * (sizeof(char *)));
    for (int i = 0; i < size; i++) {
        len = pipelen(str);
        tab[i] = malloc((len + 1) * sizeof(char));
        tab[i][len] = '\0';
        for (int j = 0; j < len; j++) {
            tab[i][j] = *str++;
        }
        str++;
    }
    tab[size] = '\0';
    return tab;
}