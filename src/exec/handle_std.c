/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_std.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:57:52 by slebik            #+#    #+#             */
/*   Updated: 2025/05/04 15:59:22 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_stdio(t_stdio_backup *backup)
{
	backup->stdin_copy = dup(STDIN_FILENO);
	if (backup->stdin_copy == -1)
		perror("dup stdin");
	backup->stdout_copy = dup(STDOUT_FILENO);
	if (backup->stdout_copy == -1)
		perror("dup stdout");
}

/* void	save_stdio(t_stdio_backup *backup)
{
	backup->stdin_backup = dup(STDIN_FILENO);
	if (backup->stdin_backup == -1)
		perror("dup stdin");
	backup->stdout_backup = dup(STDOUT_FILENO);
	if (backup->stdout_backup == -1)
		perror("dup stdout");
	backup->stderr_backup = dup(STDERR_FILENO);
	if (backup->stderr_backup == -1)
		perror("dup stderr");
} */

void	restore_stdio(t_stdio_backup *backup)
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

/* void	restore_stdio(t_stdio_backup *backup)
{
	if (backup->stdin_backup != -1)
	{
		if (dup2(backup->stdin_backup, STDIN_FILENO) == -1)
			perror("dup2 stdin restore");
		close(backup->stdin_backup);
	}
	if (backup->stdout_backup != -1)
	{
		if (dup2(backup->stdout_backup, STDOUT_FILENO) == -1)
			perror("dup2 stdout restore");
		close(backup->stdout_backup);
	}
	if (backup->stderr_backup != -1)
	{
		if (dup2(backup->stderr_backup, STDERR_FILENO) == -1)
			perror("dup2 stderr restore");
		close(backup->stderr_backup);
	}
} */
