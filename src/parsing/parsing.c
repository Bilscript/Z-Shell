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

char	*get_value(t_envp *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

void	append_to_buf(t_parse_ctx *ctx, const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		ctx->buf[(*ctx->len)++] = str[i++];
}

void	token_dollar_inside_word(t_parse_ctx *ctx, t_envp *envp)
{
	size_t	start;
	char	*key;
	char	*val;

	start = ++(*ctx->i);
	if (!ctx->input[*ctx->i] || !(ft_isalnum(ctx->input[*ctx->i])
			|| ctx->input[*ctx->i] == '_'))
	{
		ctx->buf[(*ctx->len)++] = '$';
		return ;
	}
	while (ctx->input[*ctx->i] && (ft_isalnum(ctx->input[*ctx->i])
			|| ctx->input[*ctx->i] == '_'))
		(*ctx->i)++;
	key = ft_strndup(ctx->input + start, *ctx->i - start);
	if (!key)
		return ;
	val = get_value(envp, key);
	free(key);
	if (!val)
		return ;
	append_to_buf(ctx, val, ft_strlen(val));
}

int	token_word(char *input, size_t *i, t_token **tokens, t_envp *envp)
{
	char			*buf;
	size_t			len;
	t_parse_ctx		ctx;
	t_quote_status	quote_status;

	quote_status = QUOTE_NONE;
	buf = malloc(strlen(input + *i) + 1);
	if (!buf)
		return (0);
	len = 0;
	ctx = init_parse_ctx(input, i, buf, &len);
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '"')
		{
			quote_status = QUOTE_DOUBLE;
			if (!handle_double_quote(&ctx, envp))
				return (free(buf), 0);
		}
		else if (input[*i] == '\'')
		{
			quote_status = QUOTE_SINGLE;
			if (!handle_single_quote(&ctx))
				return (free(buf), 0);
		}
		else if (input[*i] == '$')
			token_dollar_inside_word(&ctx, envp);
		else
			ctx.buf[(*ctx.len)++] = input[(*ctx.i)++];
	}
	ctx.buf[*ctx.len] = '\0';
	if (*ctx.len == 0 && quote_status == QUOTE_NONE)
	{
		free(buf);
		return (1);
	}
	add_token(tokens, new_token(TOKEN_WORD, ctx.buf, *ctx.len, quote_status));
	return (1);
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
	size_t		i;
	t_token		*tokens;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			tokenize_special(input, &i, &tokens);
		else
		{
			if (!token_word(input, &i, &tokens, envp))
				return (NULL);
		}
	}
	add_token(&tokens, new_token(TOKEN_EOF, NULL, 0, QUOTE_NONE));
	return (tokens);
}
