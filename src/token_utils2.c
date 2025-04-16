/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:14:09 by slebik            #+#    #+#             */
/*   Updated: 2025/04/16 18:20:43 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_double_quote(char *input, size_t *i, t_token **tokens, char **envp)
{
	size_t start;

	(*i)++;
	while (input[*i] && input[*i] != '"')
	{
		start = *i;
		if (input[*i] == '$')
		{
			token_dollar(input, i, tokens, envp, QUOTE_DOUBLE);
		}
		else
		{
			while (input[*i] && input[*i] != '$' && input[*i] != '"')
				(*i)++;
			add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_DOUBLE));
		}
	}
	if (input[*i] == '"')
		(*i)++;
	else
		printf("\033[0;31mSyntax error: unclosed double quote\033[0m\n");
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

char *get_env_variable(char **envp, char *token_value, t_quote_status quote_status)
{
	int		i;
	char	*var_name;
	size_t	len;

	if (quote_status == QUOTE_SINGLE)
		return (token_value);
	if (!envp || !token_value || token_value[0] != '$')
		return (perror("invalid token"), NULL);
	var_name = token_value + 1; // pour le skip
	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1); // return la valeur apres le = 
		i++;
	}
	perror("\033[0;31mvariable not found\033[0m\n");
	return (NULL);
}
