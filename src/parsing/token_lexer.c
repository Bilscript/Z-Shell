/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:57:58 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/07 20:07:04 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_token	*process_token(t_token *tok, t_command **cur)
{
	if (tok->type == TOKEN_WORD || tok->type == TOKEN_VAR)
	{
		add_arg(*cur, tok->value);
		return (tok->next);
	}
	if (tok->type == TOKEN_REDIR_IN
		|| tok->type == TOKEN_REDIR_OUT
		|| tok->type == TOKEN_APPEND
		|| tok->type == TOKEN_HEREDOC)
	{
		if (tok->next && tok->next->type == TOKEN_WORD)
		{
			add_redir(*cur, new_redir(tok->type, tok->next->value));
			return (tok->next->next);
		}
		g_exit_status = 2;
		return (printf("Syntax error: no redirection file\n"), NULL);
	}
	if (tok->type == TOKEN_PIPE)
	{
		(*cur)->next = new_command();
		*cur = (*cur)->next;
		return (tok->next);
	}
	return (tok->next);
}

static bool	is_empty_command(t_command *cmd)
{
	return (!cmd->args || !cmd->args[0]);
}

static bool	check_empty_pipe(t_command *cmds)
{
	while (cmds)
	{
		if (is_empty_command(cmds))
		{
			g_exit_status = 2;
			ft_putstr_fd("Syntax error: empty command\n", 2);
			return (true);
		}
		cmds = cmds->next;
	}
	return (false);
}

t_command	*lexer(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_token		*next;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = new_command();
			if (!head)
				head = current;
		}
		next = process_token(tokens, &current);
		if (!next)
			break ;
		tokens = next;
	}
	if (tokens == NULL)
		return (NULL);
	if (check_empty_pipe(head))
		return (free_command(head), NULL);
	return (head);
}
