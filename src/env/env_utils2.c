/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:37:44 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 13:13:50 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_to_array(t_envp *envp)
{
	t_envp	*current;
	size_t	count;
	char	**envp_array;
	size_t	len;

	current = envp;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	envp_array = (char **)malloc((count + 1) * sizeof(char *));
	if (!envp_array)
		return (NULL);
	current = envp;
	count = 0;
	while (current)
	{
		len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
		envp_array[count] = (char *)malloc(len * sizeof(char));
		if (!envp_array[count])
		{
			while (count > 0)
			{
				free(envp_array[count - 1]);
				count--;
			}
			free(envp_array);
			return (NULL);
		}
		ft_strlcpy(envp_array[count], current->key, len);
		ft_strlcat(envp_array[count], "=", len);
		ft_strlcat(envp_array[count], current->value, len);
		count++;
		current = current->next;
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
