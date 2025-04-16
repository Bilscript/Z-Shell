#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"


typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,         // |
    TOKEN_REDIR_IN,     // < Ça remplace l'entrée standard par le contenu d'un fichier par ex command < fichier
    TOKEN_REDIR_OUT,    // > redirection vers un fichier qui sera cree ou qui sera ecrase si il existe deja par ex command > fichier
    TOKEN_APPEND,       // >> ajt a la fin d'un fichier
    TOKEN_HEREDOC,      // << heredoc
    TOKEN_DQUOTE,       // " quasi comme simple quote sauf que ex echo '$USER' affiche la valeur de la variable et non "$USER".
    TOKEN_SQUOTE,       // ' tt ce qui est entre simple quote est pris litteralement, par ex echo '$USER' affiche $USER et non la valeur de la variable.
    TOKEN_VAR,			// $
	TOKEN_EOF
}   t_token_type;

typedef enum e_quote_status {
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_status;

typedef struct s_token {
    t_token_type    type;
    char            *value;
	t_quote_status quote_status;
    struct s_token  *next;
}   t_token;

t_token	*tokenizer(char *input, char **envp);
void    print_tokens(t_token *list);
void    free_tokens(t_token *list);
void	token_word(char *input, size_t *i, t_token **tokens);
void	add_token(t_token **src, t_token *dest);
int		ft_isspace(char c);
int		is_special(char c);
void 	parse_simple_quote(char *input, size_t *i, t_token **tokens);
void 	parse_double_quote(char *input, size_t *i, t_token **tokens);
t_token *new_token(t_token_type namecode, char* start, size_t len, t_quote_status quote_status);
char	*get_env_variable(char **envp, char *token_value, t_quote_status quote_status);


#endif
