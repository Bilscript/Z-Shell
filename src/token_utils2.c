/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:14:09 by slebik            #+#    #+#             */
/*   Updated: 2025/04/16 11:10:56 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_double_quote(char *input, size_t *i, t_token **tokens)
{
	char 	quote;
	size_t	start;

	quote = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
	{
		add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_DOUBLE));
		(*i)++;
	}
	else
		printf("\033[0;31mSyntax error: unclosed quote\033[0m\n");
}

void parse_simple_quote(char *input, size_t *i, t_token **tokens)
{
	char 	quote;
	size_t	start;

	quote = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
	{
		add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_SINGLE));
		(*i)++;
	}
	else
		printf("\033[0;31mSyntax error: unclosed quote\033[0m\n");
}

char	*get_env_variable(char **envp, char *token_value, t_quote_status quote_status)
{
	int		i;
	char	*var_name;
	size_t	len;

	if (quote_status == QUOTE_DOUBLE || quote_status == QUOTE_NONE)
	{
		if (!envp || !token_value || token_value[0] != '$')
			return (perror("invalid token"), NULL);
		var_name = token_value + 1;
		len = strlen(var_name); //ft
		i = 0;
		while (envp[i])
		{
			if (strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=') //ft
				return (envp[i] + len + 1);
			i++;
		}
		perror("\033[0;31mvariable not found\033[0m\n");
		return (NULL);
	}
	else
		return (token_value);
}

