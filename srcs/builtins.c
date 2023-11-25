#include "jsh.h"

typedef struct cmd { //surement qu'il faut la changer de place jsh.h ?
    int val_retour;
} cmd; 

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
void get_val_ret(struct cmd c) {
    write(1, c.val_retour, strlen(c.val_retour));
}

// Change de répertoire courant en le répertoire ref, (réf valide)
// en le précédent répertoire si le paramètre est -
// La valeur de retour est 0 en cas de succès, 1 en cas d'échec.
int cd (char *ref) {
    // Si le chdir ne prends pas en argument
    // if (strcmp(ref, "-") == 0) {
    //     // char *tmp = pwd mais pas en entier; 
    //     // str cut ou cat ou qui prends la chaine de caractère sans le com du dossier courant
    //     return 0;

    // ou alors juste path = .. 
    // }
    if (chdir(ref) == 0) {
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
        }
        return 1;
    }
    return 0;
}

// Le répertoire courant se mets à la racine
// val_retour = 0 en cas de succès, 1 en cas d'échec.
int cd () { // est-ce qu'on ne ferait pas seulement un appel à cd normal avec /
    return chdir("/");
}

int exit (struct cmd c) {
    return c.val_retour;
}

