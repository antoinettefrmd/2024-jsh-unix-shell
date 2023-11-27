#include "jsh.h"
#include "shell.h"

int is_builtins(char *cmd) { // faut prendre la struct commande aussi dcp ?
    if (strncmp("cd ", cmd, 3) == 0) {
        if (strlen(cmd) < 4) {
            return cd("/"); 
        }
        if(strcmp("-", (cmd+3)) == 0) {
            return cd("..");
        }
        return cd((cmd+3));
    }
    if (strcmp("pwd", cmd) == 0) {
        return pwd();
    }
    // if (strncmp("?", cmd, 1) == 0) { //je ne sais pas
    //     print_val_ret();
    //     return;
    // }
    // if (strncmp("exit", cmd, 4) == 0) {
    //     exit_maison();
    // }
    return 1; // jsp on verra
}

// Affiche la référence physique absolue du répertoire de travail courant
int pwd () {
    char chemin[PATH_MAX];
    if (getcwd(chemin, sizeof(chemin)) == NULL) { // stocke le pwd dans chemin et regarde s'il n'y a pas eu d'erreur
        switch (errno)
        {
            case EACCES : 
                write(2, "Vous n'avez pas les droits nécessaires pour accéder au dossier.", 66); // ecrit sur la sortie erreur
                break;
            case EFAULT : 
                write(2, "Le pointeur buffer chemin est invalide.", 33); 
                break;
            case EINVAL :
                write(2,"La taille spécifiée est inférieure ou égale à zéro (Invalid Argument).", 77);
                break;
            case EIO :
                write(2,"Une erreur d'entrée/sortie s'est produite.", 44);
                break;
            case ERANGE :
                write(2,"La taille du buffer spécifiée est positive mais pas suffisamment grande pour contenir le chemin (et le caractère '\0').", 122);
                break;
            default : 
                write(2,"Il y a une erreur", 18);
                break;
        }
        perror("Erreur lors de la récupération du répertoire courant");
        return 1;
    } else {
        write(1, chemin, strlen(chemin)); // ecrit le pwd sur la sortie standard
        write(1, "\n", 1); 
    }
    return 0;
}

// Affiche la valeur de retour de la dernière commande exécutée
void print_val_ret(struct cmd c) {
    int length = snprintf(NULL, 0, "%d", c.val_retour);
    char *str = (char *)malloc((length + 1) * sizeof(char));
    if (str != NULL) {
        snprintf(str, length + 1, "%d", c.val_retour);
        write(1, str, length);
        free(str);
    } else {
        printf("Erreur d'allocation mémoire\n");
    }
}

// Change de répertoire courant en le répertoire ref, (réf valide)
// en le précédent répertoire si le paramètre est -
// La valeur de retour est 0 en cas de succès, 1 en cas d'échec.
int cd (char *ref) {
    if (chdir(ref) == -1) {
        switch (errno) {
            case EACCES : //en vrai, j'ai l'impression que c'est le seul vraiment utile non ?
                write(2, "Vous n'avez pas les droits nécessaires pour accéder au dossier.", 66); // ecrit sur la sortie erreur
                break;
            case EFAULT : 
                write(2, "Le pointeur buffer chemin est invalide.", 33); // à enlever quand le code fonctionnera je pense
                break;
            case ENAMETOOLONG : 
                write(2, "Le chemin spécifié dépasse PATH_MAX caractères, ou l'un de ses éléments constitutifs dépasse NAME_MAX", 109);
                break;
            case ENOENT :
                write(2, "Le chemin spécifié n'existe pas.", 35);
                break;
            case ELOOP :
                write(2, "Le chemin spécifié contient une référence circulaire, certainement au travers d'un lien symbolique.",104);
                break;
            default : 
                write(2, "Autre erreur",13);
        }   
        return 1;
    }
    return 0;
}

void exit_maison () {
    exit(0);
    // Si un ou plusieurs jobs sont en cours d'exécution ou suspendus, affiche
    // un message d'avertissement; dans ce cas, la valeur de retour est 1, et
    // jsh affiche une nouvelle invite de commande.
}

