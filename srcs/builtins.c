#include "shell.h"

int is_builtins(cmd *c) { 
    if (strcmp("cd", c->str_opts[0]) == 0) {
        if (c->str_opts[1] == NULL) {
            char *home = getenv("HOME");
            c->val_retour = cd(home); 
        }
        else if(strcmp("-", c->str_opts[1]) == 0) {
            c->val_retour = cd(c->chem_jsh);
        }
        else {
            c->val_retour = cd(c->str_opts[1]);
        } 
        return 1;
    }
    if (strcmp("pwd", c->str_opts[0]) == 0) {
        c->val_retour = pwd();
        return 1;
    }
    if (strncmp("?", c->str_opts[0], 1) == 0) { 
        print_val_ret(c->val_retour);
        c->val_retour = 0;
        return 1;
    }
    if (strcmp("exit", c->str_opts[0]) == 0) {
        exit_maison(c);
        return 1;
    }
    return 0; 
}

// Affiche la référence physique absolue du répertoire de travail courant
int pwd () {
    char chemin[PATH_MAX];
    if (getcwd(chemin, sizeof(chemin)) == NULL) { // stocke le pwd dans chemin et regarde s'il n'y a pas eu d'erreur
        switch (errno)
        {
            case EACCES : 
                write(2, "Vous n'avez pas les droits nécessaires pour accéder au dossier.\n", 66); // ecrit sur la sortie erreur
                break;
            case EFAULT : 
                write(2, "Le pointeur buffer chemin est invalide.\n", 40); 
                break;
            case EINVAL :
                write(2,"La taille spécifiée est inférieure ou égale à zéro (Invalid Argument).\n", 77);
                break;
            case EIO :
                write(2,"Une erreur d'entrée/sortie s'est produite.\n", 44);
                break;
            case ERANGE :
                write(2,"La taille du buffer spécifiée est positive mais pas suffisamment grande pour contenir le chemin (et le caractère '\0').\n", 122);
                break;
            default : 
                write(2,"Il y a une erreur.\n", 18);
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
void print_val_ret(int val) {
    printf("%d\n", val);
}

// Change de répertoire courant en le répertoire ref, (réf valide)
// en le précédent répertoire si le paramètre est -
// La valeur de retour est 0 en cas de succès, 1 en cas d'échec.
int cd (char *ref) {
    if (chdir(ref) == -1) {
        switch (errno) {
            case EACCES : 
                write(2, "Vous n'avez pas les droits nécessaires pour accéder au dossier.\n", 66); // ecrit sur la sortie erreur
                break;
            case EFAULT : 
                write(2, "Le pointeur buffer chemin est invalide.\n", 40); 
                break;
            case ENAMETOOLONG : 
                write(2, "Le chemin spécifié dépasse PATH_MAX caractères, ou l'un de ses éléments constitutifs dépasse NAME_MAX", 109);
                break;
            case ENOENT :
                write(2, "Le chemin spécifié n'existe pas.\n", 35);
                break;
            case ELOOP :
                write(2, "Le chemin spécifié contient une référence circulaire, certainement au travers d'un lien symbolique.\n",104);
                break;
            default : 
                write(2, "Autre erreur",13);
        }   
        return 1;
    }
    return 0;
}

void exit_maison (cmd *c) {
    int tmp = c->val_retour;
    if (c->str_opts[1] != NULL)
    {
        tmp = atoi(c->str_opts[1]);
    }
    free_cmd(c, 1);
    exit(tmp);         
}

