/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/29 15:09:37 by slebik           ###   ########.fr       */
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
	//else if (!ft_strcmp(cmd->cmd, "unset"))
		//ft_unset(cmd);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, env_data->head);
	//else if (!ft_strcmp(cmd->cmd, "exit"))
		//ft_exit(cmd);
}

void exec_builtin_or_real(t_command *cmd, t_envp_list *env_data)
{
    pid_t			pid;
    int				status;
    t_stdio_backup	backup;

	if (!prepare_heredocs(cmd))
		return ;
    if (is_builtin(cmd->cmd) && !has_heredoc(cmd))
    {
        save_stdio(&backup);
        handle_redirections(cmd);
        exec_builtin(cmd, env_data);
        restore_stdio(&backup);
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            handle_redirections(cmd);
            if (is_builtin(cmd->cmd))
                exec_builtin(cmd, env_data);
            else
                run_command(cmd, env_data);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
            error("fork failed");
        waitpid(pid, &status, 0);
    }
}

void save_stdio(t_stdio_backup *backup)
{
    backup->stdin_copy = dup(STDIN_FILENO);
    if (backup->stdin_copy == -1)
        perror("dup stdin");

    backup->stdout_copy = dup(STDOUT_FILENO);
    if (backup->stdout_copy == -1)
        perror("dup stdout");
}

void restore_stdio(t_stdio_backup *backup)
{
    if (backup->stdin_copy != -1)
    {
        dup2(backup->stdin_copy, STDIN_FILENO);
        close(backup->stdin_copy);
    }
    if (backup->stdout_copy != -1)
    {
        dup2(backup->stdout_copy, STDOUT_FILENO);
        close(backup->stdout_copy);
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
