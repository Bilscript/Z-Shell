/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:57:58 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/19 16:17:18 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_redir_to_cmd(t_command *cmd, t_token *tokens)
{
    if (tokens->next && tokens->next->type == TOKEN_WORD)
        add_redir(cmd, new_redir(tokens->type, tokens->next->value));
    else
        printf("\033[0;31mSyntax error: no redirection file\033[0m\n");
}

void    process_token(t_token *tokens, t_command **current)
{
    if (tokens->type == TOKEN_WORD)
        add_arg(*current, tokens->value);
    else if (tokens->type == TOKEN_REDIR_IN
        || tokens->type == TOKEN_REDIR_OUT
        || tokens->type == TOKEN_APPEND
        || tokens->type == TOKEN_HEREDOC)
        add_redir_to_cmd(*current, tokens);
    else if (tokens->type == TOKEN_PIPE)
    {
        (*current)->next = new_command();
        *current = (*current)->next;
    }
}

t_command    *lexer(t_token *tokens)
{
    t_command    *head;
    t_command    *current;

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
        process_token(tokens, &current);
        if (tokens->type == TOKEN_REDIR_IN
            || tokens->type == TOKEN_REDIR_OUT
            || tokens->type == TOKEN_APPEND
            || tokens->type == TOKEN_HEREDOC)
            tokens = tokens->next;
        tokens = tokens->next;
    }
    return (head);
}