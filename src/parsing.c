/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:40:38 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/14 17:40:38 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token(t_token_type namecode, char* start, size_t len, t_quote_status quote_status)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	new->type = namecode;
	new->value = strndup(start, len);
	new->quote_status = quote_status;
	new->next = NULL;
	return (new);
}

void token_word(char *input, size_t *i, t_token **tokens)
{
	size_t	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special(input[*i]))
		(*i)++;
	add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_NONE));
}

void token_dollar(char *input, size_t *i, t_token **tokens, char **envp)
{
	size_t start = *i;
	char *tmp;
	char *value;
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	tmp = strndup(&input[start], *i - start); // ft  $USER
	value = get_env_variable(envp, tmp, QUOTE_NONE); // ou je passe un quote_status réel
	free(tmp);
	if (value)
		add_token(tokens, new_token(TOKEN_WORD, value, ft_strlen(value), QUOTE_NONE));
}

void tokenize(char *input, size_t *i, t_token **tokens, char **envp)
{
	if (input[*i] == '|')
		add_token(tokens, new_token(TOKEN_PIPE, &input[(*i)++], 1, QUOTE_NONE));
	else if (input[*i] == '$')
		token_dollar(input, i, tokens, envp);
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			add_token(tokens, new_token(TOKEN_HEREDOC, &input[(*i)], 2, QUOTE_NONE)), (*i) += 2;
		else
			add_token(tokens, new_token(TOKEN_REDIR_IN, &input[(*i)++], 1, QUOTE_NONE));
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
			add_token(tokens, new_token(TOKEN_APPEND, &input[(*i)], 2, QUOTE_NONE)), (*i) += 2;
		else
			add_token(tokens, new_token(TOKEN_REDIR_OUT, &input[(*i)++], 1, QUOTE_NONE));
	}
	else if (input[*i] == '"')
		parse_double_quote(input, i ,tokens);
	else if (input[*i] == '\'')
		parse_simple_quote(input, i ,tokens);
	else
		token_word(input, i, tokens);
}

t_token *tokenizer(char *input, char **envp)
{
	t_token *tokens;
	size_t i;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue ;
		}
		tokenize(input, &i, &tokens, envp);
	}
	return tokens;
}
