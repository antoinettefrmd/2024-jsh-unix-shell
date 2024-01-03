#include "shell.h"

// verifie si l'argument est une commande interne
int is_builtins(cmd *c) 
{ 
    
    if (strcmp("cd", c->str_opts[0]) == 0) // Change le repertoire de travail courant
    { 
        if (c->str_opts[1] == NULL) { 
            char *home = getenv("HOME");
            if (home != NULL) 
                c->val_retour = cd(home); // Nous ramène à la racine home/user/
            else 
            {
                write(2, "home undefined\n", 16);
                exit_maison(c);
            }
        }
        else if(strcmp("-", c->str_opts[1]) == 0) 
        {
            c->val_retour = cd(c->chem_jsh); // Nous ramène à la racine du dépot
        }
        else 
        {
            c->val_retour = cd(c->str_opts[1]); // Nous ramène au bout du chemin passé en argument
        } 
        return 1;
    }
    if (strcmp("pwd", c->str_opts[0]) == 0) // Affiche la référence physique absolue du répertoire de travail courant
    {
        c->val_retour = pwd();
        return 1;
    }
    if (strncmp("?", c->str_opts[0], 1) == 0) // Affiche la valeur de la dernière commande executée
    { 
        print_val_ret(c->val_retour);
        c->val_retour = 0;
        return 1;
    }
    if (strcmp("exit", c->str_opts[0]) == 0) // Nous sort du programme en s'assurant d'avoir bien tout free et en renvoyant une valeur qui explique son arrêt
    {
        exit_maison(c);
        return 1;
    }
    if (strcmp("jobs", c->str_opts[0]) == 0)
    {
	    jobs(c);
	    return 1;
    }
    if (strcmp("kill", c->str_opts[0]) == 0)
    {
	    kill_maison(c);
	    return 1;
    }
    return 0; 
}

// Affiche la référence physique absolue du répertoire de travail courant
int pwd () 
{
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
        write(2,"Erreur lors de la récupération du répertoire courant.\n", 58);
        return 1;
    } else {
        write(1, chemin, strlen(chemin)); // ecrit le pwd sur la sortie standard
        write(1, "\n", 1); 
    }
    return 0;
}

// Affiche la valeur de retour de la dernière commande exécutée
void print_val_ret(int val) 
{
    printf("%d\n", val);
}

// Change de répertoire courant en le répertoire ref, (réf valide)
// en le répertoire  du dépot si le paramètre est -
// La valeur de retour est 0 en cas de succès, 1 en cas d'échec.
int cd (char *ref) 
{
    if (chdir(ref) == -1) // Change le repertoire et reagarde s'il y a une erreur
    { 
        switch (errno) 
        {
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
                write(2, "Autre erreur\n",14);
        }   
        return 1;
    }
    return 0;
}

// Sortie de programme qui free les derniers malloc et renvoie la valeur qui explique l'arrêt
void exit_maison (cmd *c) 
{
   for (int i = 0; i < c -> all_jobs; i++) {
	if(strcmp("Stopped", c -> jobs[i].etat) == 0 || strcmp("Running", c -> jobs[i].etat) == 0) {
		printf("Attention : certains jobs sont toujours en cours d'exécution.\n");
		c -> val_retour = 1;
		return;
	}
    }
    int tmp = c->val_retour; // valeur de la dernière commande executée
    if (c->str_opts[1] != NULL)
    {
        tmp = atoi(c->str_opts[1]); // si exit prends une valeur de retour en argument, c'est elle qui est renvoyée
    }
    free_cmd(c, 1); //free la le tableau d'arg ET la strucuture commande
    exit(tmp);    
}

void jobs (cmd *c) {
	for(int i = 0; i < c -> all_jobs; i++) {
		print_job(c -> jobs[i]);
	}
	c -> val_retour = 0;
}

void kill_maison(cmd *c) {
	char *tmp = c -> str_opts[1];
	char *job;
	int sig = 15;
	if(tmp[0] == '-') {
		sig = atoi(tmp) * (-1);
		job = c -> str_opts[2];
	}
	else job = c -> str_opts[1];
	for(int i = 0; i < c -> all_jobs; i++) {
		if(strcmp("Running", c -> jobs[i].etat) == 0 || strcmp("Stopped", c -> jobs[i].etat) == 0) {
			if(job[0] == '%') {
				int j = 1;
				char *groupe = malloc(strlen(job));
				while(job[j] != '\0') {
					groupe[j - 1] = job[j];
					j++;
				}
				groupe[j - 1] = '\0';
				if(c -> jobs[i].nb == atoi(groupe)) {
					kill(c -> jobs[i].pid, sig);
					free(groupe);
					break;
				}
				free(groupe);
			}
			else {
				if(c -> jobs[i].pid == atoi(job)) {
					kill(c -> jobs[i].pid, sig);
					break;
				}
			}
		}
	}
	c -> val_retour = 0;
}
