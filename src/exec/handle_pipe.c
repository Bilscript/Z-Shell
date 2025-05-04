/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:01:23 by slebik            #+#    #+#             */
/*   Updated: 2025/04/26 16:01:23 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	prepare_child(t_command *current, int in_fd, int *fd)
{
	setup_exec_signals();
	// Gérer stdin (depuis le pipe précédent ou depuis fd standard)
	if (in_fd != 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
		close(in_fd);
	}
	// Gérer stdout (vers le pipe suivant si non dernière commande)
	if (current->next)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(1);
		}
		close(fd[1]);
	}
	// Gérer les redirections spécifiques à la commande
	// Les heredocs sont déjà préparés à ce stade
	if (handle_redirections(current) == -1)
		exit(1);
}

void	exec_command_children(t_command *current, t_envp_list *env_data, int fd)
{
	char	*path;

	(void)fd;
	if (is_builtin(current->cmd))
	{
		exec_builtin(current, env_data);
		exit(g_exit_status);
	}
	
	path = parsing(env_data->head, current->cmd);
	if (!path)
	{
		ft_putstr_fd(current->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	
	execve(path, current->args, env_data->lenv);
	perror("execve failed");
	free(path);
	exit(1);
}

void	handle_parent(t_command *current, int *in_fd, int *fd)
{
	t_redir	*redir;

	// Fermer les descripteurs de fichier inutilisés
	if (*in_fd != 0)
		close(*in_fd);

	// Configurer l'entrée pour la prochaine commande
	if (current->next)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
	
	// Fermer les descripteurs de fichier heredoc inutilisés
	redir = current->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
		{
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}

void	exec_piped_commands(t_command *cmd, t_envp_list *env_data)
{
	int			fd[2];
	int			in_fd;
	pid_t		pid;
	int			status;
	t_command	*current;

	in_fd = 0;
	// Préparer tous les heredocs avant de créer les processus
	if (!prepare_heredocs(cmd))
	{
		// g_exit_status est déjà défini à 130 par prepare_heredocs
		return ;
	}
	
	// Si un heredoc a été interrompu (g_exit_status == 130), on ne continue pas
	if (g_exit_status == 130)
		return ;
	
	current = cmd;
	while (current)
	{
		if (current->next && pipe(fd) == -1)
			error("pipe failed");
		
		pid = fork();
		if (pid == 0)
		{
			setup_exec_signals();
			if (current->next)
				prepare_child(current, in_fd, fd);
			else if (in_fd != 0) // Dernière commande
			{
				if (dup2(in_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 stdin");
					exit(1);
				}
				close(in_fd);
				if (handle_redirections(current) == -1)
					exit(1);
			}
			else if (handle_redirections(current) == -1)
				exit(1);
			
			exec_command_children(current, env_data, in_fd);
		}
		else if (pid < 0)
			error("fork failed");
		
		handle_parent(current, &in_fd, fd);
		current = current->next;
	}
	
	// Attendre que tous les processus enfants terminent
	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				g_exit_status = 130;
			else if (WTERMSIG(status) == SIGQUIT)
				g_exit_status = 131;
		}
		else
			g_exit_status = WEXITSTATUS(status);
	}
	setup_signals(); // Réinitialiser les signaux après que tous les processus aient terminé
}
