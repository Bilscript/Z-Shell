/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:14:09 by slebik            #+#    #+#             */
/*   Updated: 2025/04/23 18:36:20 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_and_free(char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}

char	*extract_dollar(char *input, size_t *i, t_envp *envp, t_quote_status sta)
{
	char	*key;
	char	*value;
	size_t	start;

	(*i)++;
	if (input[*i] == '$')
	{
		(*i)++;
		return (ft_itoa(getpid()));
	}
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	if (start == *i)
		return (ft_strdup("$"));
	key = ft_substr(input, start, *i - start);
	if (!key)
		return (NULL);
	value = get_env_variable(envp, key, sta);
	free(key);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

void	parse_double_quote(char *input, size_t *i, t_token **tkn, t_envp *envp)
{
	char	*joined;
	char	*tmp;
	size_t	start;

	(*i)++;
	joined = ft_strdup("");
	if (!joined)
		return ;
	if (input[*i] == '"')
	{
		add_token(tkn, new_token(TOKEN_WORD, "", 0, QUOTE_DOUBLE));
		(*i)++;
		free(joined);
		return ;
	}
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			tmp = extract_dollar(input, i, envp, QUOTE_DOUBLE);
		else
		{
			start = *i;
			while (input[*i] && input[*i] != '"' && input[*i] != '$')
				(*i)++;
			tmp = ft_substr(input, start, *i - start);
		}
		if (!tmp)
			return ;
		joined = strjoin_and_free(joined, tmp);
		free(tmp);
	}
	if (input[*i] == '"')
		(*i)++;
	else
		printf("\033[0;31mSyntax error: unclosed double quote\033[0m\n");
	add_token(tkn, new_token(TOKEN_WORD, joined, ft_strlen(joined), QUOTE_DOUBLE));
	free(joined);
}

void	parse_simple_quote(char *input, size_t *i, t_token **tokens)
{
	size_t	start;

	if (input[*i] == '\'' && input[*i + 1] == '\'')
	{
		add_token(tokens, new_token(TOKEN_WORD, "", 0, QUOTE_SINGLE));
		(*i) += 2;
		return ;
	}
	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_SINGLE));
		(*i)++;
	}
	else
		printf("\033[0;31mSyntax error: unclosed single quote\033[0m\n");
}


char	*get_env_variable(t_envp *env, char *key, t_quote_status status)
{
	int		i;
	size_t	len;
	char	**envp;

	envp = env_list_to_array(env);
	if (status == QUOTE_SINGLE)
		return (NULL);
	if (!envp || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
