/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:16:43 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/14 12:16:43 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	parse_and_execute(char *input, t_envp_list *env_data)
{
	t_token		*token;
	t_command	*command;
	bool		token_on;

	token_on = false;
	token = tokenizer(input, env_data->head);
	if (token->type != TOKEN_EOF)
	{
		command = lexer(token);
		exec(command, env_data, token);
		token_on = true;
	}
	free_tab(env_data->lenv);
	env_data->lenv = envp_to_array(env_data->head);
	free_tokens(token);
	if (token_on == true)
		free_command(command);
}

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
		input = readline("Z-Shell> ");
		if (!input)
		{
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
	return (free_envp_list(&env_data), rl_clear_history(), 0);
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
