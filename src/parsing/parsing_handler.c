/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:57:04 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/04 12:07:52 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_quote_content(t_parse_ctx *ctx, t_envp *envp, char quote)
{
	while (ctx->input[*ctx->i] && ctx->input[*ctx->i] != quote)
	{
		if (quote == '"' && ctx->input[*ctx->i] == '$')
			handle_variable(ctx, envp);
		else
		{
			ctx->buf[*ctx->len] = ctx->input[*ctx->i];
			(*ctx->len)++;
			(*ctx->i)++;
		}
	}
}

int	handle_special_var(t_parse_ctx *ctx)
{
	char	*s;

	s = ft_itoa(g_exit_status);
	if (!s)
		return (0);
	append_to_buf(ctx, s, ft_strlen(s));
	free(s);
	(*ctx->i) += 2;
	return (1);
}
