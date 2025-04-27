/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:57:04 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 11:59:23 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_variable(t_parse_ctx *ctx, t_envp *envp)
{
	size_t	start;
	char	*key;
	char	*val;

	start = ++(*ctx->i);
	while (ctx->input[*ctx->i] && (ft_isalnum(ctx->input[*ctx->i])
			|| ctx->input[*ctx->i] == '_'))
		(*ctx->i)++;
	key = strndup(ctx->input + start, *ctx->i - start);
	val = get_value(envp, key);
	free(key);
	if (!val)
		return ;
	while (*val)
		ctx->buf[(*ctx->len)++] = *val++;
}

void	handle_double_quote(t_parse_ctx *ctx, t_envp *envp)
{
	(*ctx->i)++;
	while (ctx->input[*ctx->i] && ctx->input[*ctx->i] != '"')
	{
		if (ctx->input[*ctx->i] == '$')
			handle_variable(ctx, envp);
		else
			ctx->buf[(*ctx->len)++] = ctx->input[(*ctx->i)++];
	}
	if (ctx->input[*ctx->i] == '"')
		(*ctx->i)++;
}

void	handle_single_quote(t_parse_ctx *ctx)
{
	(*ctx->i)++;
	while (ctx->input[*ctx->i] && ctx->input[*ctx->i] != '\'')
		ctx->buf[(*ctx->len)++] = ctx->input[(*ctx->i)++];
	if (ctx->input[*ctx->i] == '\'')
		(*ctx->i)++;
}

void	handle_redir_in(char *input, size_t *i, t_token **tokens)
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

void	handle_redir_out(char *input, size_t *i, t_token **tkns)
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
