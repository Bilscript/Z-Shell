/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:50:40 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 11:59:18 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_ctx	init_parse_ctx(char *input, size_t *i, char *buf, size_t *len)
{
	t_parse_ctx	ctx;

	ctx.input = input;
	ctx.i = i;
	ctx.buf = buf;
	ctx.len = len;
	return (ctx);
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
