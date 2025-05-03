/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:57:58 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/03 15:47:45 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir_to_cmd(t_command *cmd, t_token *tokens)
{
	if (tokens->next && tokens->next->type == TOKEN_WORD)
		add_redir(cmd, new_redir(tokens->type, tokens->next->value));
	else
		printf("\033[0;31mSyntax error: no redirection file\033[0m\n");
}

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
		printf("\033[0;31mSyntax error: no redirection file\033[0m\n");
		return (NULL);
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
			fprintf(stderr, "Syntax error: empty command\n");
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
	if (check_empty_pipe(head))
	{
		free_command(head);
		return (NULL);
	}
	return (head);
}

