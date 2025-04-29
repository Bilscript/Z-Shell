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

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	print_shell(void)
{
	printf("\033[1;34m");
	printf("██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗    ████████╗ ██████╗     ████████╗██╗  ██╗███████╗\n\
██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝    ╚══██╔══╝██╔═══██╗    ╚══██╔══╝██║  ██║██╔════╝\n\
██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗         ██║   ██║   ██║       ██║   ███████║█████╗  \n\
██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝         ██║   ██║   ██║       ██║   ██╔══██║██╔══╝  \n\
╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗       ██║   ╚██████╔╝       ██║   ██║  ██║███████╗\n\
 ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝       ╚═╝    ╚═════╝        ╚═╝   ╚═╝  ╚═╝╚══════╝\n\
");
	printf("\033[1;31m");
	printf("                            ███████╗      ███████╗██╗  ██╗███████╗██╗     ██╗\n\
                            ╚══███╔╝      ██╔════╝██║  ██║██╔════╝██║     ██║\n\
                              ███╔╝ █████╗███████╗███████║█████╗  ██║     ██║\n\
                             ███╔╝  ╚════╝╚════██║██╔══██║██╔══╝  ██║     ██║\n\
                            ███████╗      ███████║██║  ██║███████╗███████╗███████╗\n\
                            ╚══════╝      ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
	printf("\033[1;36m");
	printf("                                                			by Bilscript & SLK  |  Z-Corp®\n");
	printf("\033[1;30m");
	printf("===================================================================================================================\n");
	printf("\033[0m");
}

void	parse_and_execute(char *input, t_envp_list *env_data)
{
	t_token		*token;
	t_command	*command;

	token = tokenizer(input, env_data->head);
	//print_tokens(token);
	command = lexer(token);
	//print_commands(command);
	exec(command, env_data, token);
	free_tab(env_data->lenv);
	env_data->lenv = envp_to_array(env_data->head);
	free_tokens(token);
	free_command(command);
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	t_envp_list	env_data;

//	print_shell();
	(void)ac;
	(void)av;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
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
	free_envp_list(&env_data);
	rl_clear_history();
	return (0);
}
