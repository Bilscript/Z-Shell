/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:16:43 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 21:45:35 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

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
