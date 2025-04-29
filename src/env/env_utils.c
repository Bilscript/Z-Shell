/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:21:26 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/29 12:08:31 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_until_char(const char *str, char c)
{
	char	*ptr;

	ptr = ft_strchr(str, c);
	if (!ptr)
		return (-1);
	return (ptr - str);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (s[i] && i < n)
		i++;
	dup = (char *)malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_envp	*new_envp(const char *key, const char *value, bool exprt)
{
	t_envp	*node;

	node = (t_envp *)malloc(sizeof(t_envp));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->export = exprt;
	node->next = NULL;
	return (node);
}

void	add_envp_back(t_envp **head, t_envp *new_node)
{
	t_envp	*tmp;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

t_envp	*get_env(char **env)
{
	int		i;
	int		len;
	t_envp	*res;
	char	*key;
	char	*value;

	i = 0;
	len = 0;
	res = NULL;
	while (env[i])
	{
		len = len_until_char(env[i], '=');
		key = ft_strndup(env[i], len);
		if (!key)
			return (res);
		value = ft_strdup(env[i] + len + 1);
		if (!value)
		{
			free(key);
			return (res);
		}
		add_envp_back(&res, new_envp(key, value, false));
		free(key);
		free(value);
		i++;
	}
	return (res);
}
