ARCHITECTURE.md


Sommaire :

Cahier des charges et répartition du travail
Architecture logicielle
Structures de données
Les différents algorithmes 


I.  Cahier des charges et répartition du travail

Jalon 1 : 
compilation sans erreur avec make à la racine du dépôt
exécution de jsh à la racine du dépôt
Ronen : exécution des commandes externes simples à l'avant-plan (avec arguments mais sans redirection ni possibilité de basculer à l'arrière-plan) 
Antoinette : bon fonctionnement de cd, pwd, ?, exit (en l'absence de jobs) 
Georges : conformité du prompt (en l'absence de jobs) 

Jalon 2 : 
Ronen : exécution des commandes externes simples à l'arrière-plan avec & (sans possibilité de basculer à l'avant-plan, et seulement pour les commandes qui ne cherchent pas à lire ou écrire sur le terminal)
Georges : bon fonctionnement de jobs (sans option), kill et exit
adaptation du prompt selon le nombre de jobs en cours
Antoinette : redirections autres que | et <( )

Jalon 3 : 
Antoinette : Gestion des signaux et ARCHITECTURE.md
Georges : Fg, Bg et Gestion des jobs avec options
Ronen : Gestion des pipes |


II. Architecture logicielle

Notre projet se décompose en deux sous-répertoires include et srcs et un Makefile.

Le Makefile

Le Makefile est destiné à compiler notre projet nommé "jsh". Il récupère tous les fichiers de compilation .o et les met dans un dossier objects. On peut exécuter les commandes make : créé un exécutable et ou recompile les les derniers fichiers , make re : recompile tous les fichier , make clean : efface tous les .o, make fclean : efface les .o et l'exécutable.

Notre shell.h dans include

Notre shell.h contient toutes les informations nécessaires à la communication entres les différents fichiers de srcs. Il s’agit donc de l’ensemble des en-têtes de fonctions, des structures cmd et job ainsi que de trois variables globales représentant une sauvegarde de l’entrée/sortie standard/erreur. Chaque fichier de srcs a inclus shell.h et peut donc appeler les différentes fonctions et utiliser cmd et job.

Les fichiers dans srcs

main.c et prompt.c :
main.c initialise nos structures et variables puis interagit avec le prompt en lançant une boucle qui lui demande la nouvelle ligne de commande arrivant sur l’entrée standard. La boucle permet également de gérer de faire appelle aux fonctions de gestion des signaux, aux gestions d’erreur du prompt et à la réinitialisation des descripteurs de la commande ainsi qu’à leur fermeture. 
prompt.c s’occupe également de l’affichage de notre terminal à l’aide de “fmt_cwd”, qui récupère le chemin et l’affiche sur le bon format et de “cat_maison”. 

split.c, parsing_pipes.c, parsing_redir.c, pipe_split.c, lib.c : Tous ces fichiers aident au parsing de la commande afin de vérifier s’il y a des redirections, des pipes, une mise à l’arrière plan et d’appeler certaines fonctions en conséquence en plus de remplir le champs char **str_opt de la commande. lib.c regroupe toutes les petites fonctions auxiliaires de traitement de chaînes ou tableaux de caractères. (cf. III) a))

exec.c : contient les fonctions utilisées lors de l’exécution d’une fonction externe, et contient également la fonction execloop qui s’occupe de l’exécution des lignes de commande, notamment le fait de lancer plusieurs processus lors d’un pipe.

builtins.c : contient une fonction de vérification “is_builtins”, une qui traite la commande avec ses différents paramètres et appelle les builtins correspondant, on a donc cd, pwd, ?, exit,fg, bg, kill, et job.

process.c : contient les fonctions permettant de lancer de nouveaux processus avec fork(), s’occupe de la gestion des pipes et d’ajouter des jobs.

jobs.c : contient une fonction print_job permettant d’afficher un job, et une fonction check_job, qui est chargée de prendre des nouvelles des jobs en cours.

signaux.c : deux fonctions permettant pour l’une de modifier le comportement en mode IGN pour les signaux SIGINT, SIGTERM, SIGTTIN, SIGQUIT, SIGTTOU et SIGTSTP pouvant être envoyé à jsh et pour l’autre de rétablir le comportement par défaut surtout dans le cas de l'exécution d’une commande externe.

Flux d'exécution d’une commande

Une ligne de commande est donc reçue par le prompt, envoyée au main qui se charge d’appeler execloop pour la mettre au format de notre structure commande avec le nombre de vraies commandes effectives (pipe). execloop vérifie ensuite pour chaque sous-commande si c’est un builtin ou une commande externe et appelle en conséquence les fonctions d'exécutions dans les fichiers buitlins.c ou process.c qui se chargent également chacun des redirections. 
	

III) Structures de données

La structure cmd

Nous avons opté pour une structure commande afin de pouvoir traiter la ligne à travers plusieurs fichiers et mémoriser différentes informations importantes.

typedef struct cmd {
	int val_retour; (= la valeur de retour de la fonction)
	char **str_opts; (= la commande splitté dans un tableau pour analyser chaque partie)
	int bg; (= savoir si la commande est en background)
	char *chem_jsh; (= mémoriser le chemin du répertoire précédent)
	int nb_jobs; (= le nombre de jobs en cours)
	pid_t all_jobs; (= le nombre de jobs créés en tout)
	job *jobs; (= un tableau contenant tous les jobs)
	int fd_in; (= redirection de l’entrée standard de la commande spécifiquement)
	int fd_out; (= ””””” sortie ””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””)
	int fd_err; (= ”””””” sortie erreur ””””””””””””””””””””””””””””””””””””””””””””””””””””””””””””"”)
	struct cmd *next; (= pointeur vers la prochaine commande (pipe))
    	struct cmd *origin; (= pointeur vers la struct cmd d’origine (pipe))
	int nb_c; (= nombre de sous-commandes (pipe)) 
} cmd;


La structure job

typedef struct job {
    int groupe; (= le numéro du job)
    pid_t *pid; (= pid[0] est le pid du job, les autres sont les pid des processus du job)
    char **etat; (= etat[0] est l’état du job, les autres sont les états des processus)
    char **ligne; (= ligne[0] est la ligne du job, les autres sont les commandes de chaque processus)
    int nb_process; (= le nombre de processus du job)
} job;


IV)  Les différents algorithmes 

Gestion des pipes

Si une commande contient des tubes, notre shell va créer un tableau où chaque commande
est reliée à son tube. Lorsqu’une des commandes de la pipeline est ensuite envoyée à la fonction process, nous allons d’abord rediriger les entrées et sorties standard vers le tube auquel est associé la commande, puis close le tableau de tubes (comme on est dans un processus enfant, la table des fd est copiée, il faut donc tous les fermer afin de ne pas bloquer l’exécution), avant de l’exécuter. Plus précisément, chaque commande va écrire sur la sortie du tube de la commande suivante (mis à part évidemment la dernière commande de la pipeline qui écrit sur la sortie standard), et si besoin lire le résultat de la commande précédente sur l’entrée du tube (mis à part cette fois la première commande qui lit sur l’entrée standard).

Une fois sorti du processus enfant, il faut encore close les fd inutiles (car ceux qu’on a close auparavant étaient des copies). La sortie du tube est fermée systématiquement (sauf encore une fois lors de la dernière itération de la boucle) car on n’en a jamais besoin pour la commande suivante. En ce qui concerne l’entrée, on ferme (sauf pour la première commande) celle du tube précédent car on n'en a plus besoin.

La dernière étape consiste à faire attendre les process pour avec waitpid afin qu’elles puissent s’exécuter dans le bon ordre.



Gestion des redirections

On parcourt c->str_opts :

Si une chaîne à la position i correspond à une certaine redirection, on ouvre le fichier du nom de la chaîne à la position i+1 avec les différents flags correspondant à la redirection. Ce file descriptor créé est stocké pour l’instant dans les entrée et sortie de la commande.

S’il n’y a pas eu d’erreur, on appelle redir_fic qui fait réellement la redirection des entrée/sorties standard/erreur avec des dup2 pointant sur les fd de la commande. On close les fd de la commande pour ne pas gêner le reste de l'exécution.

La valeur de retour, s’il n’y a pas d’erreur, est la position i de la première redirection. Celle-ci sera en argument de la fonction petit_tab qui permet de vider/free le contenu d’un tableau à partir d’un indice i. Les chaînes de caractères contenant les redirections et noms de fichier dans str_opts vont être effacées pour avoir une commande bien parsée et pouvoir exécuter la commande.

Dans shell.h on a déclaré trois variables globales qui sont sauvegardées des entrée/sorties standard/erreur afin qu’après avoir redirigé on puisse revenir sur le comportement habituel. Ce sont des sauvegardes qu’on a initialisé à l’aide de dup dans le main. On les a mises globales pour qu’à chaque instant on puisse les close à partir de n’importe quel fichier dans le cas où une erreur surviendrait.


Gestion des jobs

Un nouveau job est créé à chaque appel de la fonction process, il est stocké dans le tableau de jobs de la structure cmd. Ensuite, à chaque fois qu’une commande est exécutée, jsh fait un appel à check_jobs pour prendre des nouvelles des jobs en cours et afficher ceux dont l’état a changé. Les identifiants des jobs sont stockés dans le champ groupe, ce qui permet de les retrouver facilement lors des exécutions de jobs %n ou kill %n. Chaque job possède des champs pid_t *pid, char **etat, char **ligne, dans lesquels le premier emplacement de chaque tableau permet d’obtenir des informations sur le job, alors que les autres donnent des informations sur les processus du job.
