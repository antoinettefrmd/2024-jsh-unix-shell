#include "shell.h"

int main(int argc, char **argv, char **envp)
{
    if (argc <= 1)
    {
        perror("give at least one argument\n");
        exit(EXIT_FAILURE);
    }
    cmd c;
    c.val_retour = is_builtins(argv[1]);
    printf("%d", c.val_retour);
    if ( c.val_retour == 0) {}
    else {process(argv[1], envp);}
    return 0;
}