/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_gestion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:49:19 by slebik            #+#    #+#             */
/*   Updated: 2025/05/05 15:52:21 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// rename le fichier find path

#include "minishell.h"

void	error(char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
	exit(1);
}

char	*get_path_from_list(t_envp *env_list)
{
	t_envp	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*find_executable(char **chemins, char *cmd)
{
	int		i;
	char	*part_chem;
	char	*final_chem;

	if (!chemins)
		return (NULL);
	i = 0;
	while (chemins[i])
	{
		part_chem = ft_strjoin(chemins[i], "/");
		if (!part_chem)
			return (ft_free_split(chemins), NULL);
		final_chem = ft_strjoin(part_chem, cmd);
		free(part_chem);
		if (!final_chem)
			return (ft_free_split(chemins), NULL);
		if (access(final_chem, F_OK | X_OK) == 0)
			return (ft_free_split(chemins), final_chem);
		free(final_chem);
		i++;
	}
	return (ft_free_split(chemins), NULL);
}

char	*parsing(t_envp *env_list, char *cmd)
{
	char	*path_val;
	char	**chemins;

	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	if (!cmd || !cmd[0])
		return (NULL);
	path_val = get_value(env_list, "PATH");
	if (!path_val)
		return (NULL);
	chemins = ft_split(path_val, ':');
	if (!chemins)
		return (NULL);
	return (find_executable(chemins, cmd));
}
