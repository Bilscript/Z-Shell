/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_std.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:57:52 by slebik            #+#    #+#             */
/*   Updated: 2025/05/05 14:04:45 by slebik           ###   ########.fr       */
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
