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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	free_envp_list(t_envp_list *envp_list)
{
	t_envp	*current;
	t_envp	*next;
	int		i = 0;

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
		printf("free env alloc n°%d\n", i++);
	}
}

void	parse_and_execute(char *input, t_envp_list *env_data)
{
	t_token		*token;
	t_command	*command;

	token = tokenizer(input, env_data->head);
	print_tokens(token);
	command = lexer(token);
	print_commands(command);
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

	print_shell();
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
