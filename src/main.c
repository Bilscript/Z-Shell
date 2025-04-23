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


// copier envp pr pouvoir le modif

void handle_sigint(int sig)
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

int main(int ac, char **av, char **envp)
{
	char *input;

	print_shell();
	(void)ac;
	(void)av;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	t_envp *env = get_env(envp);
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
			t_token *token = tokenizer(input, envp);
			print_tokens(token);
			t_command *command = lexer(token);
			print_commands(command);
			exec(command, env);
			free_tokens(token);
			free_command(command);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
