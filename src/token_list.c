/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:56:42 by slebik            #+#    #+#             */
/*   Updated: 2025/04/16 13:56:42 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token(t_token **src, t_token *dest)
{
	t_token *tmp;

	if (!*src)
		*src = dest;
	else
	{
		tmp = *src;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = dest;
	}
}

void free_tokens(t_token *list)
{
    t_token *tmp;

    while (list) 
	{
        tmp = list->next;
        free(list->value);
        free(list);
        list = tmp;
    }
}

t_token *new_token(t_token_type namecode, char* start, size_t len, t_quote_status quote_status)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	new->type = namecode;
	new->value = strndup(start, len); //ft 
	new->quote_status = quote_status;
	new->next = NULL;
	return (new);
}

t_quote_status	ft_lasttoken_status(t_token *lst)
{
    t_token *temp;

    if (lst == NULL)
        return (QUOTE_NONE);
    temp = lst;
    while (temp->next != NULL)
        temp = temp->next;
    return (temp->quote_status);
}
 