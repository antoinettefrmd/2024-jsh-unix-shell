# jsh — Unix Shell

Un shell Unix (`jsh`) implémenté en C, avec exécution de commandes, gestion des jobs, redirections, pipes et signaux.

## Fonctionnalités

- **Exécution de commandes** externes, en avant-plan et en arrière-plan (`&`)
- **Builtins** : `cd`, `pwd`, `exit`, `jobs`, `kill`, `fg`, `bg`
- **Gestion des jobs** : suivi des processus en arrière-plan, reprise en avant/arrière-plan
- **Redirections** : `>`, `<`, `>>`, etc.
- **Pipes** : `|`
- **Gestion des signaux** : interruption (Ctrl+C), suspension (Ctrl+Z)

## Compilation

```bash
make
```

## Exécution

```bash
./jsh
```

## Architecture

Le projet est développé par jalons successifs, chacun ajoutant une couche de fonctionnalité :

1. Exécution de commande simple
2. Gestion des jobs en arrière-plan
3. Redirections et pipes
4. Gestion des signaux

Voir `ARCHITECTURE.md` pour le détail de la conception et de la répartition du travail en équipe.

## Auteurs

Projet réalisé en équipe de 3 dans le cadre d'un cours de systèmes/Unix.
