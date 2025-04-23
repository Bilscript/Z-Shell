/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:40:38 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/14 17:40:38 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_double_quote(char *input, size_t *i, char *buf, size_t *len)
{
	(*i)++;
	while (input[*i] && input[*i] != '"')
		buf[(*len)++] = input[(*i)++];
	if (input[*i] == '"')
		(*i)++;
}

static void	handle_single_quote(char *input, size_t *i, char *buf, size_t *len)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
		buf[(*len)++] = input[(*i)++];
	if (input[*i] == '\'')
		(*i)++;
}

void	token_word(char *input, size_t *i, t_token **tokens)
{
	char	*buf;
	size_t	len;

	buf = malloc(strlen(input + *i) + 1);
	if (!buf)
		return ;
	len = 0;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '"')
			handle_double_quote(input, i, buf, &len);
		else if (input[*i] == '\'')
			handle_single_quote(input, i, buf, &len);
		else
			buf[len++] = input[(*i)++];
	}
	buf[len] = '\0';
	add_token(tokens, new_token(TOKEN_WORD, buf, len, QUOTE_NONE));
	free(buf);
}

void	get_pid_var(char **value, size_t *i, t_token **tkn, t_quote_status q_st)
{
	(*i)++;
	*value = ft_itoa(getpid());
	add_token(tkn, new_token(TOKEN_WORD, *value, ft_strlen(*value), q_st));
	free(*value);
}

void	accolade_gestion(char *input, size_t *i, char **tmp)
{
	size_t	start;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '}')
		(*i)++;
	if (input[*i] == '}')
	{
		*tmp = ft_substr(input, start, *i - start);
		(*i)++;
	}
	else
	{
		*tmp = NULL;
		printf("\033[0;31mSyntax error: unclosed ${}\033[0m\n");
	}
}

void	token_dollar(char *input, size_t *i, t_token **tkn, t_envp *envp, t_quote_status sta)
{
	size_t	start;
	char	*tmp;
	char	*value;

	start = *i;
	(*i)++;
	if (input[*i] == '$')
	{
		get_pid_var(&value, i, tkn, sta);
		return ;
	}
	if (input[*i] == '{')
		accolade_gestion(input, i, &tmp);
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		tmp = strndup(&input[start + 1], *i - start - 1);
	}
	if (tmp)
	{
		value = get_env_variable(envp, tmp, sta);
		free(tmp);
		if (value)
			add_token(tkn, new_token(TOKEN_WORD, value, ft_strlen(value), sta));
	}
}

static void	handle_redir_in(char *input, size_t *i, t_token **tokens)
{
	if (input[*i + 1] == '<')
	{
		add_token(tokens, new_token(TOKEN_HEREDOC, &input[*i], 2, QUOTE_NONE));
		*i += 2;
	}
	else
	{
		add_token(tokens, new_token(TOKEN_REDIR_IN, &input[*i], 1, QUOTE_NONE));
		(*i)++;
	}
}

static void	handle_redir_out(char *input, size_t *i, t_token **tkns)
{
	if (input[*i + 1] == '>')
	{
		add_token(tkns, new_token(TOKEN_APPEND, &input[*i], 2, QUOTE_NONE));
		*i += 2;
	}
	else
	{
		add_token(tkns, new_token(TOKEN_REDIR_OUT, &input[*i], 1, QUOTE_NONE));
		(*i)++;
	}
}

void	tokenize_special(char *input, size_t *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, &input[*i], 1, QUOTE_NONE));
		(*i)++;
	}
	else if (input[*i] == '<')
		handle_redir_in(input, i, tokens);
	else if (input[*i] == '>')
		handle_redir_out(input, i, tokens);
}

t_token	*tokenizer(char *input, t_envp *envp)
{
	size_t	i;
	t_token	*tokens;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '$')
			token_dollar(input, &i, &tokens, envp, QUOTE_NONE);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			tokenize_special(input, &i, &tokens);
		else
			token_word(input, &i, &tokens);
	}
	add_token(&tokens,
		new_token(TOKEN_EOF, NULL, 0, QUOTE_NONE));
	return (tokens);
}
