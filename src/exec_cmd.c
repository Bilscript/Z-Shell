/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:12:19 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/21 19:07:21 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	path[PATH_MAX];

	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
		perror("pwd");
}

void	ft_cd(t_command *cmd_line)
{
	char	*path;

	if (cmd_line->args[2])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (!cmd_line->args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME is not set\n", 21);
			return ;
		}
	}
	else
		path = cmd_line->args[1];
	if (chdir(path) != 0)
	{
		write(2, "cd: ", 4);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		perror("");
	}
}

bool	skip_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	ft_echo(t_command *cmd)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	while (cmd->args[i] && skip_n_flag(cmd->args[i]))
	{
		newline = false;
		i++;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_env(t_command *cmd, t_envp *envp)
{
	if (cmd->args[1])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	while (envp)
	{
		printf("%s=%s\n", envp->key, envp->value);
		envp = envp->next;
	}
}

void	ft_export(t_command *cmd, t_envp *envp)
{
	t_envp	*temp;
	int		len;

	temp = envp;
	if (!cmd->args[1])
	{
		while (temp)
		{
			if (temp->export == true)
				printf("export %s=%s\n", temp->key, temp->value);
			temp = temp->next;
		}
		return ;
	}
	len = len_until_char(cmd->args[1], '=');
	add_envp_back(&envp, new_envp(ft_strndup(cmd->args[1],
				len), cmd->args[1] + len + 1, true));
}
