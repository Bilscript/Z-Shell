/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:26 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/19 16:01:23 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *new_command(void)
{
	t_command *cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return cmd;
}

t_redir *new_redir(int type, const char *filename)
{
	t_redir *redir = malloc(sizeof(t_redir));
	if (!redir)
		return NULL;
	redir->type = type;
	redir->filename = strdup(filename);
	redir->next = NULL;
	return redir;
}

void add_redir(t_command *cmd, t_redir *redir)
{
	t_redir *tmp;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

void add_arg(t_command *cmd, const char *word)
{
	int i;
	int j;
	char **new_args;

	i = 0;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return;
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = strdup(word);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	if (cmd->cmd == NULL)
		cmd->cmd = new_args[0];
}

void    free_args(char **args)
{
    int    i;

    if (args)
    {
        i = 0;
        while (args[i])
        {
            free(args[i]);
            i++;
        }
        free(args);
    }
}

void    free_redirs(t_redir *redir)
{
    t_redir    *tmp_redir;

    while (redir)
    {
        tmp_redir = redir;
        free(redir->filename);
        redir = redir->next;
        free(tmp_redir);
    }
}

void    free_command(t_command *cmd)
{
    t_command    *next;

    while (cmd)
    {
        next = cmd->next;
        free_args(cmd->args);
        free_redirs(cmd->redirs);
        free(cmd->cmd);
        free(cmd);
        cmd = next;
    }
}

void print_commands(t_command *cmd_list)
{
    const char *redir_types[] = {
        "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC"
    };

    while (cmd_list)
    {
        printf("=== Command ===\n");

        // Affichage de la commande principale
        printf("Command: %s\n", cmd_list->cmd ? cmd_list->cmd : "(null)");

        // Affichage des arguments
        printf("Arguments:\n");
        if (cmd_list->args)
        {
            for (int i = 0; cmd_list->args[i]; i++)
                printf("  - [%s]\n", cmd_list->args[i]);
        }
        else
            printf("  (none)\n");
        printf("Redirections:\n");
        t_redir *redir = cmd_list->redirs;
        if (!redir)
            printf("  - (Aucune redirection)\n");
        while (redir)
        {
            printf("  - Type: %s, File: [%s]\n", redir_types[redir->type - TOKEN_REDIR_IN], redir->filename);
            redir = redir->next;
        }
        printf("\n");
        cmd_list = cmd_list->next;
    }
}