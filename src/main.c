/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:16:43 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/08 19:33:36 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	parse_and_execute(char *input, t_data *data)
{
	bool		token_on;

	token_on = false;
	data->token = tokenizer(input, data->env_data.head);
	if (data->token == NULL)
		return ; // potentiel leak free data ici
	if (data->token->type != TOKEN_EOF)
	{
		data->cmd = lexer(data->token);
		exec(data);
		token_on = true;
	}
	free_tokens(data->token);
	free_tab(data->env_data.lenv);
	data->env_data.lenv = envp_to_array(data->env_data.head);
	if (token_on == true)
		free_command(data->cmd);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->cmd = NULL;
	data->token = NULL;
	data->env_data.head = get_env(envp);
	data->env_data.lenv = envp_to_array(data->env_data.head);
	return (data);
}

int	main(int ac, char **av, char **envp) 
{
	char	*input;
	t_data	*data;

	(void)ac;
	(void)av;
	setup_signals();
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
	free_envp_list(&data->env_data);
	rl_clear_history();
	free_all(NULL, data);
	return (0);
}

//MAIN POUR TESTER 
/*
int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_envp_list	env_data;

	(void)ac;
	(void)av;
	g_exit_status = 0;
	setup_signals();
	env_data.head = get_env(envp);
	env_data.lenv = envp_to_array(env_data.head);

	while (1)
	{
		if (isatty(STDIN_FILENO))
			input = readline("Z-Shell> ");
		else
			input = readline("");

		if (!input)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			parse_and_execute(input, &env_data);
		}
		free(input);
	}
	free_envp_list(&env_data);
	rl_clear_history();
	return (0);
}*/
