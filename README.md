# 🐚 Z-Shell

Z-Shell est un **minishell** conforme au projet de 42, développé en C.
Il reproduit les fonctionnalités de base d’un shell Unix, incluant :

### ✅ Fonctions principales

* Exécution de commandes avec ou sans chemins (`ls`, `/bin/ls`, etc.)
* Gestion des redirections (`>`, `>>`, `<`, `<<`)
* Piping (`|`)
* Variables d’environnement (`$USER`, `$?`, etc.)
* Expansions entre guillemets (`'...'`, `"..."`)
* Built-ins : `cd`, `echo`, `exit`, `export`, `unset`, `env`, `pwd`
* Support du `CTRL+C`, `CTRL+D`, `CTRL+\`
* Gestion du niveau de shell (`SHLVL`)
* Prise en charge des erreurs de syntaxe (quotes non fermées, commandes invalides, etc.)

### 🛠️ Compilation

```bash
make
```

### ▶️ Lancer le shell

```bash
./Z-Shell
```

### ⛔️ Builtins disponibles

* `cd`
* `echo`
* `exit`
* `export`
* `unset`
* `env`
* `pwd`

---

### 📁 Structure du projet

Le code est divisé en plusieurs modules clairs :

* `lexer/` : découpe l’entrée utilisateur en tokens
* `parser/` : organise les tokens en commandes exploitables
* `exec/` : exécute les commandes (builtins ou binaires)
* `expansion/` : gère les variables d’environnement (`$`, quotes, etc.)
* `env/` : manipulation des variables d’environnement
* `signals/` : gestion des signaux

---

### 📌 Remarques

* Norme 42 respectée (fonctions < 25 lignes, pas de ternaires, etc.)
* Aucune fonction interdite utilisée
* Gestion robuste de la mémoire (Valgrind clean)

---

