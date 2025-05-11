/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:22:04 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/14 18:22:04 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '"' || c == '\'' || c == '$');
}

void	free_all(t_command *cmd, t_data *data)
{
	if (data)
	{
		if (data->cmd)
		{
			free_command(data->cmd);
			data->cmd = NULL;
		}
		if (data->token)
		{
			free_tokens(data->token);
			data->token = NULL;
		}
		free_envp_list(&(data->env_data));
		free(data);
	}
	if (cmd)
	{
		free_command(cmd);
		cmd = NULL;
	}
}
