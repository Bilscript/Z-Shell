/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:14:09 by slebik            #+#    #+#             */
/*   Updated: 2025/04/15 15:39:53 by slebik           ###   ########.fr       */
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
		printf("Syntax error: unclosed quote\n");
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
		printf("Syntax error: unclosed quote\n");
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
		perror("variable not found");
		return (NULL);
	}
	else
		return (token_value);
}

