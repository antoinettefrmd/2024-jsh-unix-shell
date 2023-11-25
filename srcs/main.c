#include "shell.h"

int main(int argc, char **argv, char **envp)
{
    if (argc <= 1)
    {
        perror("give at least one argument\n");
        exit(EXIT_FAILURE);
    }
    process(argv[1], envp);
    return 0;
}