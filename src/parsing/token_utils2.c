/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:14:09 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 19:44:54 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(t_parse_ctx *ctx, t_envp *envp, t_quote_status *qs)
{
	char	quote;

	quote = ctx->input[*ctx->i];
	if (quote == '"')
		*qs = QUOTE_DOUBLE;
	else
		*qs = QUOTE_SINGLE;
	(*ctx->i)++;
	handle_quote_content(ctx, envp, quote);
	if (!ctx->input[*ctx->i])
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		g_exit_status = 2;
		return (0);
	}
	(*ctx->i)++;
	return (1);
}

size_t	handle_dollar_double_quote(char *input, size_t *i, t_envp *envp)
{
	size_t	j;
	char	*key;
	char	*val;
	size_t	len;

	len = 0;
	j = *i + 1;
	if (input[j] == '\0' || input[j] == '"')
	{
		(*i)++;
		return (1);
	}
	while (input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
		j++;
	key = ft_strndup(input + *i + 1, j - (*i + 1));
	val = get_value(envp, key);
	if (val)
		len = ft_strlen(val);
	free(key);
	*i = j;
	return (len);
}

size_t	handle_double_quote(char *input, size_t *i, t_envp *envp)
{
	size_t	size;
	char	quote;

	size = 0;
	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '$')
			size += handle_dollar_double_quote(input, i, envp);
		else
		{
			size++;
			(*i)++;
		}
	}
	if (input[*i])
		(*i)++;
	return (size);
}

size_t	handle_dollar(char *input, size_t *i, t_envp *envp)
{
	size_t	j;
	char	*key;
	char	*val;

	if (input[*i + 1] == '?')
	{
		*i += 2;
		return (10);
	}
	j = *i + 1;
	while (ft_isalnum(input[j]) || input[j] == '_')
		j++;
	key = ft_strndup(input + *i + 1, j - (*i + 1));
	if (!key)
		return (0);
	val = get_value(envp, key);
	*i = j;
	if (val)
		j = ft_strlen(val);
	else if (ft_isdigit(key[0]))
		j = ft_strlen(key + 1);
	else
		j = 0;
	free(key);
	return (j);
}

size_t	estimate_token_size(char *input, size_t i, t_envp *envp)
{
	size_t	size;

	size = 0;
	while (input[i] && !ft_isspace(input[i])
		&& input[i] != '|' && input[i] != '<' && input[i] != '>')
	{
		if (input[i] == '\'' || input[i] == '"')
			size += handle_double_quote(input, &i, envp);
		else if (input[i] == '$')
			size += handle_dollar(input, &i, envp);
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}
