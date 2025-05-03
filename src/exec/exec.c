/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/03 13:41:51 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

void	exec_builtin(t_command *cmd, t_envp_list *env_data)
{
	prepare_heredocs(cmd);
	if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(cmd);
}

void exec_builtin_or_real(t_command *cmd, t_envp_list *env_data)
{
    pid_t			pid;
    int				status;
    t_stdio_backup	backup;

	if (!prepare_heredocs(cmd))
	{
		g_exit_status = 130;
		return ;
	}
    if (is_builtin(cmd->cmd) && !has_heredoc(cmd))
    {
        save_stdio(&backup);
        if (handle_redirections(cmd) == -1)
		{
			restore_stdio(&backup);
			return ;
		}
        exec_builtin(cmd, env_data);
        restore_stdio(&backup);
    }
	else
	{
		pid = fork();
		if (pid == 0)
		{
			setup_exec_signals();
			if (handle_redirections(cmd) == -1)
				exit(1);
			if (is_builtin(cmd->cmd))
				exec_builtin(cmd, env_data);
			else
				run_command(cmd, env_data);
			exit(g_exit_status);
		}
		else if (pid < 0)
			error("fork failed");
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				g_exit_status = 130;
				write(1, "\n", 1);
			}
			else if (WTERMSIG(status) == SIGQUIT)
			{
				g_exit_status = 131;
				ft_putendl_fd("Quit (core dumped)", 2);
			}
		}
		else
			g_exit_status = WEXITSTATUS(status);
	}
}



void	exec(t_command *cmd_line, t_envp_list *env_data, t_token *token)
{
	if (has_pipe(token))
		exec_piped_commands(cmd_line, env_data);
	else
	{
		while (cmd_line)
		{
			if (cmd_line->cmd)
				exec_builtin_or_real(cmd_line, env_data);
			cmd_line = cmd_line->next;
		}
	}
}

void	run_command(t_command *cmd, t_envp_list *env_data)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (!cmd || !cmd->cmd)
		return ;
	path = parsing(env_data->head, cmd->cmd);
	if (!path)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		printf("free path\n");
		return ;
	}
	if (pid == 0)
	{
		execve(path, cmd->args, env_data->lenv);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
	free(path);
}
