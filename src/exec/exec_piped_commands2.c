/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped_commands2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:57:03 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 23:58:58 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_piped_commands(t_data *data)
{
	t_command			*current;
	struct sigaction	old_int;
	struct sigaction	old_quit;
	int					status;

	current = data->cmd;
	if (!prepare_heredocs(data))
		return ;
	setup_parent_signals(&old_int, &old_quit);
	exec_piped_loop(current, data, &status);
	restore_parent_signals(&old_int, &old_quit);
	update_exit_status(status);
}

void	update_exit_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}
