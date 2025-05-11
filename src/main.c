/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:16:43 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 23:27:50 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

int	set_value(t_envp *envp, const char *key, const char *value)
{
	char	*new_value;

	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
		{
			free(envp->value);
			if (value)
				new_value = ft_strdup(value);
			else
				new_value = ft_strdup("");
			if (!new_value)
				return (0);
			envp->value = new_value;
			return (1);
		}
		envp = envp->next;
	}
	return (0);
}

void	set_shlvl(t_data *data)
{
	int		shlvl;
	char	*value;
	char	*new_value;

	shlvl = 0;
	value = get_value(data->env_data.head, "SHLVL");
	if (value)
		shlvl = ft_atoi(value);
	shlvl += 1;
	new_value = ft_itoa(shlvl);
	if (!new_value)
		return ;
	set_value(data->env_data.head, "SHLVL", new_value);
	free(new_value);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	setup_signals();
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->cmd = NULL;
	data->token = NULL;
	data->env_data.head = get_env(envp);
	if (!data->env_data.head)
	{
		free(data);
		return (NULL);
	}
	data->env_data.lenv = envp_to_array(data->env_data.head);
	if (!data->env_data.lenv)
	{
		free_envp_list(&data->env_data);
		free(data);
		return (NULL);
	}
	set_shlvl(data);
	return (data);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_data		*data;

	(void)ac;
	(void)av;
	data = init_data(envp);
	if (!data)
		return (1);
	while (1)
	{
		input = readline("Z-Shell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			parse_and_execute(input, data);
		}
		free(input);
	}
	free_all(NULL, data);
	return (0);
}
