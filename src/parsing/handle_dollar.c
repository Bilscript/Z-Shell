/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:38:02 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 21:39:22 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_var_start(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*extract_key(char *input, size_t *i, size_t *j)
{
	*j = *i + 1;
	while (is_valid_var_start(input[*j]))
		(*j)++;
	return (ft_strndup(input + *i + 1, *j - (*i + 1)));
}

static size_t	get_val_length(char *key, t_envp *envp)
{
	char	*val;
	size_t	len;

	val = get_value(envp, key);
	if (val)
		len = ft_strlen(val);
	else if (ft_isdigit(key[0]))
		len = ft_strlen(key + 1);
	else
		len = 0;
	return (len);
}

size_t	handle_dollar(char *input, size_t *i, t_envp *envp)
{
	size_t	j;
	char	*key;
	size_t	len;

	if (input[*i + 1] == '?')
	{
		*i += 2;
		return (10);
	}
	if (input[*i + 1] == '\0' || !is_valid_var_start(input[*i + 1]))
	{
		(*i)++;
		return (1);
	}
	key = extract_key(input, i, &j);
	if (!key)
		return (0);
	*i = j;
	len = get_val_length(key, envp);
	free(key);
	return (len);
}
