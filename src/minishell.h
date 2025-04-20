#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "linux/limits.h"
# include <stdbool.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,			// |
	TOKEN_REDIR_IN,		// < Ça remplace l'entrée standard par le contenu d'un fichier par ex command < fichier
	TOKEN_REDIR_OUT,	// > redirection vers un fichier qui sera cree ou qui sera ecrase si il existe deja par ex command > fichier
	TOKEN_APPEND,		// >> ajt a la fin d'un fichier
	TOKEN_HEREDOC,		// << heredoc
	TOKEN_DQUOTE,		// " quasi comme simple quote sauf que ex echo '$USER' affiche la valeur de la variable et non "$USER".
	TOKEN_SQUOTE,		// ' tt ce qui est entre simple quote est pris litteralement, par ex echo '$USER' affiche $USER et non la valeur de la variable.
	TOKEN_VAR,			// $
	TOKEN_EOF
}	t_token_type;

typedef enum e_quote_status {
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_status;

typedef struct s_redir
{
	int				type;			// REDIR_IN, REDIR_OUT, APPEND, HEREDOC
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				*cmd;			// "echo", "ls", "cat", etc.
	char				**args;			// tableau de tous les arguments (cmd inclus)
	t_redir				*redirs;		// liste de redirections
	int					append;			// 0 = truncate 1 = append
	struct s_command	*next;			// pour les pipes (commande suivante)
}	t_command;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_status	quote_status;
	struct s_token	*next;
}	t_token;

//================TOKEN UTILS================
t_token			*tokenizer(char *input, char **envp);
t_token			*new_token(t_token_type namecode, char* start, size_t len, t_quote_status quote_status);
t_quote_status	ft_lasttoken_status(t_token *lst);
void			add_token(t_token **src, t_token *dest);
void			token_dollar(char *input, size_t *i, t_token **tokens, char **envp, t_quote_status quote_status);
void			token_word(char *input, size_t *i, t_token **tokens);
void			free_tokens(t_token *list);
void			print_tokens(t_token *list);

int				ft_isspace(char c);
int				is_special(char c);
void			parse_simple_quote(char *input, size_t *i, t_token **tokens);
void			parse_double_quote(char *input, size_t *i, t_token **tokens, char **envp);
char			*get_env_variable(char **envp, char *token_value, t_quote_status quote_status);

//================LEXER UTILS================
t_command		*lexer(t_token *tokens);
t_command		*new_command(void);
t_redir			*new_redir(int type, const char *filename);
void			add_arg(t_command *cmd, const char *word);
void			add_redir(t_command *cmd, t_redir *redir);
void			print_commands(t_command *cmd_list);
void			free_command(t_command *cmd);

//================LEXER UTILS================
void			exec(t_command *cmd_line, char **envp);
void			ft_echo(t_command *cmd);
void		    ft_pwd(void);
void			ft_cd(t_command *cmd_line);
void			ft_env(t_command *cmd, char **envp);

#endif
