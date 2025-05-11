/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 17:24:29 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 17:26:53 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_data *data)
{
	t_command	*current;
	t_command	*next;

	if (has_pipe(data->token))
		exec_piped_commands(data);
	else
	{
		current = data->cmd;
		while (current)
		{
			next = current->next;
			if (current->cmd)
				exec_builtin_or_real(data);
			current = next;
		}
	}
}

static int	handle_tokenizer_error(t_data *data)
{
	if (!data->token || data->token->type == TOKEN_EOF)
	{
		free_tokens(data->token);
		data->token = NULL;
		free_command(data->cmd);
		data->cmd = NULL;
		return (1);
	}
	return (0);
}

static int	handle_lexer_error(t_data *data)
{
	if (!data->cmd)
	{
		free_tokens(data->token);
		data->token = NULL;
		return (1);
	}
	return (0);
}

static void	execute_and_cleanup(t_data *data, t_command *cmd_save)
{
	exec(data);
	free_tokens(data->token);
	data->token = NULL;
	if (data->env_data.lenv)
	{
		free_tab(data->env_data.lenv);
		data->env_data.lenv = NULL;
	}
	data->env_data.lenv = envp_to_array(data->env_data.head);
	free_command(data->cmd);
	data->cmd = cmd_save;
}

void	parse_and_execute(char *input, t_data *data)
{
	t_command	*cmd_save;

	if (!input || !data)
		return ;
	data->token = tokenizer(input, data->env_data.head);
	if (handle_tokenizer_error(data))
		return ;
	cmd_save = data->cmd;
	data->cmd = lexer(data->token);
	if (handle_lexer_error(data))
		return ;
	execute_and_cleanup(data, cmd_save);
}
