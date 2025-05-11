/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:17:51 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 23:33:58 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <string.h>
# include "libft.h"
# include "linux/limits.h"
# include "limits.h"

// ======== Global variable ========

extern int	g_exit_status;

//========** Enums **========

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

typedef enum e_exit_code
{
	EXIT_GENERAL_ERROR = 1,
	EXIT_BUILTIN_ERROR = 2,
	EXIT_SYNTAX_ERROR = 258,
	EXIT_SIGNAL_INTERRUPT = 130,
	EXIT_SIGNAL_QUIT = 131,
}	t_exit_code;

//========** Structures **========

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

typedef struct s_stdio_backup
{
	int	stdin_copy;
	int	stdout_copy;
}	t_stdio_backup;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_status	quote_status;
	struct s_token	*next;
}	t_token;

typedef struct s_parse_ctx
{
	char			*input;
	size_t			*i;
	char			*buf;
	size_t			*len;
	t_quote_status	quote;
}	t_parse_ctx;

typedef struct s_data
{
	t_command	*cmd;
	t_envp_list	env_data;
	t_token		*token;
}	t_data;

//==========** Token functions **==========
t_token			*tokenizer(char *input, t_envp *envp);
t_token			*new_token(t_token_type namecode, char *start,
					size_t len, t_quote_status status);
t_quote_status	ft_lasttoken_status(t_token *lst);
void			add_token(t_token **src, t_token *dest);
int				token_word(char *input, size_t *i, t_token **tokens,
					t_envp *envp);
size_t			estimate_token_size(char *input, size_t i, t_envp *envp);
void			free_tokens(t_token *list);
int				append_var_value(t_parse_ctx *ctx, char *key, t_envp *envp);

//==========** Parse utilities **==========

size_t			handle_dollar(char *input, size_t *i, t_envp *envp);
int				handle_quotes(t_parse_ctx *ctx, t_envp *envp,
					t_quote_status *qs);
int				handle_variable(t_parse_ctx *ctx, t_envp *envp);
int				handle_special_var(t_parse_ctx *ctx);
void			handle_quote_content(t_parse_ctx *ctx, t_envp *envp,
					char quote);
int				ft_isspace(char c);
int				is_special(char c);
void			append_to_buf(t_parse_ctx *ctx, const char *str, size_t len);
char			*ft_strndup(const char *s, size_t n);
void			handle_redir_out(char *input, size_t *i, t_token **tkns);
void			handle_redir_in(char *input, size_t *i, t_token **tokens);
void			accolade_gestion(char *input, size_t *i, char **tmp);
void			get_pid_var(char **value, size_t *i, t_token **tkn,
					t_quote_status q_st);
t_parse_ctx		init_parse_ctx(char *input, size_t *i, char *buf, size_t *len);
int				has_pipe(t_token *tokens);

// ==========** Lexer functions **==========

t_command		*lexer(t_token *tokens);
t_command		*new_command(void);
t_redir			*new_redir(int type, const char *filename);
void			add_arg(t_command *cmd, const char *word);
void			add_redir(t_command *cmd, t_redir *redir);
void			free_command(t_command *cmd);
void			token_dollar_inside_word(t_parse_ctx *ctx, t_envp *envp);

// ==========** Built-in commands **==========

void			exec(t_data *data);
void			ft_echo(t_command *cmd);
void			ft_pwd(void);
void			ft_cd(t_command *cmd, t_envp *envp);
void			ft_export(t_command *cmd, t_envp *envp);
int				set_key(t_envp *envp, char *key, char *value);
void			ft_unset(t_command *cmd, t_envp *envp);
int				ft_exit(t_command *cmd, t_data *data);
void			print_exit_error(char *arg);
void			exit_with_number(char *arg, t_data *data);
long long		ft_atoll(const char *str, int *overflow);
int				handle_too_many_args(void);
void			exit_invalid_number(char *arg, t_data *data);
void			ft_env(t_command *cmd, t_envp *envp);
int				is_builtin(char *cmd);
void			exec_builtin(t_command *cmd, t_data *data);

// ==========** Environment variables functions **==========

t_envp			*new_envp(const char *key, const char *value, bool exprt);
t_envp			*get_env(char **env);
int				len_until_char(const char *str, char c);
void			add_envp_back(t_envp **head, t_envp *new_node);
char			*get_value(t_envp *envp, char *key);
char			**envp_to_array(t_envp *envp);
void			free_envp_list(t_envp_list *envp_list);

// ==========** Execution functions **==========

void			parse_and_execute(char *input, t_data *data);
void			run_command(t_data *data);
void			error(char *error_msg);
void			exec_piped_commands(t_data *data);
void			exec_builtin_or_real(t_data *data);
void			exec_piped_loop(t_command *cmd, t_data *data, int *status);
void			exec_child(t_command *cmd, int in_fd, int *fd, t_data *data);
int				pipe_and_fork(t_command *cmd, int *pid, int *fd);
void			restore_parent_signals(struct sigaction *old_int,
					struct sigaction *old_quit);
void			setup_parent_signals(struct sigaction *old_int,
					struct sigaction *old_quit);

// ==========** Path and command utilities **==========

char			*get_path_from_list(t_envp *env_list);
char			*find_executable(char **chemins, char *cmd);
char			*parsing(t_envp *env_list, char *cmd);
int				check_cmd_path(char **path, t_command *cmd);
int				not_directory(char **path, t_command *cmd);

// ==========** Redirection functions **==========

int				prepare_heredocs(t_data *data);
int				handle_input_redir(t_redir *redir);
int				handle_output_redir(t_redir *redir);
int				handle_append_redir(t_redir *redir);
int				handle_heredoc_redir(t_redir *redir);
int				handle_redirections(t_command *cmd);
int				handle_here_doc(char *limiter, t_data *data);
int				has_heredoc(t_command *cmd);

// ==========** Pipe and process functions **==========

void			handle_parent(t_command *current, int *in_fd, int *fd);
void			exec_command_children(t_command *current,
					int in_fd, t_data *data);
void			prepare_child(t_command *current, int in_fd, int *fd);
void			here_doc_child(int *fd, char *limiter, t_data *data);
void			here_doc_parent(int *fd);
void			write_to_pipe(int fd, char *line);

// ==========** Signal handlers **==========

void			handle_sigint(int signo);
void			heredoc_sigint_handler(int signo);
void			setup_signals(void);
void			setup_exec_signals(void);
void			setup_heredoc_signals(void);
void			reset_signals(void);
void			update_exit_status(int status);

// ==========** Utility functions **==========

char			*ft_strjoin_char(char *str, char c);
int				get_next_line(char **line);
int				is_valid(const char *str);
void			ft_free_split(char **tab);
void			free_tab(char **tab);
void			save_stdio(t_stdio_backup *backup);
void			restore_stdio(t_stdio_backup *backup);
void			free_all(t_command *cmd, t_data *data);

#endif