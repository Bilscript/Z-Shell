/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:17:51 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/26 22:17:51 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_VAR,
	TOKEN_EOF
}	t_token_type;

typedef enum e_quote_status
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_status;

typedef struct s_redir
{
	int				type;
	char			*filename;
	int				heredoc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	t_redir				*redirs;
	int					append;
	struct s_command	*next;
}	t_command;

typedef struct s_envp
{
	char			*key;
	char			*value;
	bool			export;
	struct s_envp	*next;
}	t_envp;

typedef struct s_envp_list
{
	t_envp	*head;
	char	**lenv;
}	t_envp_list;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_status	quote_status;
	struct s_token	*next;
}	t_token;

typedef struct s_parse_ctx
{
	char	*input;
	size_t	*i;
	char	*buf;
	size_t	*len;
}	t_parse_ctx;

//================TOKEN UTILS================
t_token			*tokenizer(char *input, t_envp *envp);
t_token			*new_token(t_token_type namecode, char *start,
					size_t len, t_quote_status status);
t_quote_status	ft_lasttoken_status(t_token *lst);
void			add_token(t_token **src, t_token *dest);
void			token_dollar(t_parse_ctx *ctx, t_token **tkn, t_envp *envp,
					t_quote_status sta);
void			token_word(char *input, size_t *i,
					t_token **tokens, t_envp *envp);
void			free_tokens(t_token *list);
void			print_tokens(t_token *list);
int				ft_isspace(char c);
int				is_special(char c);
void			parse_simple_quote(char *input, size_t *i, t_token **tokens);
void			parse_double_quote(char *input, size_t *i, t_token **tkn,
					t_envp *envp);
char			*get_env_variable(t_envp *env, char *key,
					t_quote_status status);
char			*ft_strndup(const char *s, size_t n);

//================LEXER UTILS================
t_command		*lexer(t_token *tokens);
t_command		*new_command(void);
t_redir			*new_redir(int type, const char *filename);
void			add_arg(t_command *cmd, const char *word);
void			add_redir(t_command *cmd, t_redir *redir);
void			print_commands(t_command *cmd_list);
void			free_command(t_command *cmd);

//================BUILT-IN UTILS================
void			exec(t_command *cmd_line, t_envp_list *envp, t_token *token);
void			ft_echo(t_command *cmd);
void			ft_pwd(void);
void			ft_cd(t_command *cmd_line);
void			ft_export(t_command *cmd, t_envp *envp);
t_envp			*new_envp(const char *key, const char *value, bool exprt);
t_envp			*get_env(char **env);
void			ft_env(t_command *cmd, t_envp *envp);
int				len_until_char(const char *str, char c);
void			add_envp_back(t_envp **head, t_envp *new_node);
t_envp			*new_envp(const char *key, const char *value, bool exprt);
char			*get_value(t_envp *envp, char *key);
char			**envp_to_array(t_envp *envp);

//================EXEC UTILS================
void			parse_and_execute(char *input, t_envp_list *env_data);
void			handle_pipeline(char **cmds, t_envp *env);
void			ft_free_split(char **tab);
char			*get_path_from_list(t_envp *env_list);
char			*find_executable(char **chemins, char *cmd);
char			*parsing(t_envp *env_list, char *cmd);
void			run_command(t_command *cmd, t_envp_list *env_data);
void			error(char *error_msg);
void			errorcmd(const char *cmd, int exit_code);
char			**env_list_to_array(t_envp *env);
t_command		*parse_simple_command(char *input);
void			exec_piped_commands(t_command *cmd, t_envp_list *env_data);
void			exec_builtin(t_command *cmd, t_envp_list *env_data);
int				is_builtin(char *cmd);

char			*ft_strjoin_char(char *str, char c);
int				get_next_line(char **line);

void			here_doc_child(int *fd, char *limiter);
void			here_doc_parent(int *fd);
int				handle_here_doc(char *limiter);
void			write_to_pipe(int fd, char *line);
void			exec_piped_commands(t_command *cmd, t_envp_list *env_data);
void			handle_parent(t_command *current, int *in_fd, int *fd);
void			exec_command_children(t_command *current, t_envp_list *env_data,
					int in_fd);
void			prepare_child(t_command *current, int in_fd, int *fd);
void			prepare_heredocs(t_command *cmd);
void			handle_input_redir(t_redir *redir);
void			handle_output_redir(t_redir *redir);
void			handle_append_redir(t_redir *redir);
void			handle_heredoc_redir(t_redir *redir);
void			handle_redirections(t_command *cmd);

void			handle_redir_out(char *input, size_t *i, t_token **tkns);
void			handle_redir_in(char *input, size_t *i, t_token **tokens);
void			accolade_gestion(char *input, size_t *i, char **tmp);
void			get_pid_var(char **value, size_t *i, t_token **tkn,
					t_quote_status q_st);
void			handle_single_quote(t_parse_ctx *ctx);
void			handle_double_quote(t_parse_ctx *ctx, t_envp *envp);
void			handle_variable(t_parse_ctx *ctx, t_envp *envp);
t_parse_ctx		init_parse_ctx(char *input, size_t *i, char *buf, size_t *len);
int				has_pipe(t_token *tokens);
void			free_envp_list(t_envp_list *envp_list);
void			free_tab(char **tab);

#endif
