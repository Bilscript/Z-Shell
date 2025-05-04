/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:37:44 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/04 13:22:52 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp_array(char **envp_array, size_t count)
{
	while (count--)
		free(envp_array[count]);
	free(envp_array);
}

static char	**alloc_envp_array(t_envp *envp, size_t *count)
{
	t_envp	*current;
	char	**array;

	*count = 0;
	current = envp;
	while (current)
	{
		(*count)++;
		current = current->next;
	}
	array = (char **)malloc(sizeof(char *) * (*count + 1));
	if (!array)
		return (NULL);
	return (array);
}

char	**envp_to_array(t_envp *envp)
{
	char		**envp_array;
	size_t		count;
	size_t		len;
	t_envp		*current;

	envp_array = alloc_envp_array(envp, &count);
	if (!envp_array)
		return (NULL);
	current = envp;
	count = 0;
	while (current)
	{
		len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
		envp_array[count] = malloc(sizeof(char) * len);
		if (!envp_array[count])
			return (free_envp_array(envp_array, count), NULL);
		ft_strlcpy(envp_array[count], current->key, len);
		ft_strlcat(envp_array[count], "=", len);
		ft_strlcat(envp_array[count], current->value, len);
		current = current->next;
		count++;
	}
	envp_array[count] = NULL;
	return (envp_array);
}

void	free_envp_list(t_envp_list *envp_list)
{
	t_envp	*current;
	t_envp	*next;

	if (!envp_list)
		return ;
	free_tab(envp_list->lenv);
	current = envp_list->head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
