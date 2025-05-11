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

int	handle_variable(t_parse_ctx *ctx, t_envp *envp)
{
	size_t	start;
	char	*key;
	char	*val;

	if (ctx->input[*ctx->i + 1] == '?')
		return (handle_special_var(ctx));
	(*ctx->i)++;
	if (!ctx->input[*ctx->i])
		return (ctx->buf[(*ctx->len)++] = '$', 1);
	start = *ctx->i;
	while (ft_isalnum(ctx->input[*ctx->i]) || ctx->input[*ctx->i] == '_')
		(*ctx->i)++;
	key = ft_strndup(ctx->input + start, *ctx->i - start);
	if (!key)
		return (0);
	val = get_value(envp, key);
	if (val)
		append_to_buf(ctx, val, ft_strlen(val));
	else if (ft_isdigit(key[0]))
		append_to_buf(ctx, key + 1, ft_strlen(key) - 1);
	free(key);
	return (1);
}

int	fill_token_buf(t_parse_ctx *ctx, t_envp *envp, t_quote_status *quote_status)
{
	char	c;

	while (ctx->input[*ctx->i]
		&& !ft_isspace(ctx->input[*ctx->i])
		&& ctx->input[*ctx->i] != '|'
		&& ctx->input[*ctx->i] != '<'
		&& ctx->input[*ctx->i] != '>')
	{
		if (ctx->input[*ctx->i] == '"' || ctx->input[*ctx->i] == '\'')
		{
			if (!handle_quotes(ctx, envp, quote_status))
				return (0);
		}
		else if (ctx->input[*ctx->i] == '$')
			handle_variable(ctx, envp);
		else
		{
			c = ctx->input[*ctx->i];
			ctx->buf[(*ctx->len)++] = c;
			(*ctx->i)++;
		}
	}
	return (1);
}

int	token_word(char *input, size_t *i, t_token **tokens, t_envp *envp)
{
	t_quote_status	quote_status;
	size_t			len;
	size_t			est_len;
	char			*buf;
	t_parse_ctx		ctx;

	est_len = estimate_token_size(input, *i, envp);
	buf = malloc(est_len + 1);
	len = 0;
	quote_status = QUOTE_NONE;
	if (!buf)
		return (0);
	ctx = init_parse_ctx(input, i, buf, &len);
	if (!fill_token_buf(&ctx, envp, &quote_status))
		return (free(buf), 0);
	ctx.buf[*ctx.len] = '\0';
	if (*ctx.len == 0 && quote_status == QUOTE_NONE)
		return (free(buf), 1);
	add_token(tokens, new_token(TOKEN_WORD, ctx.buf, *ctx.len, quote_status));
	free(ctx.buf);
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
