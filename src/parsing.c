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

void token_word(char *input, size_t *i, t_token **tokens)
{
	size_t	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special(input[*i]))
		(*i)++;
	add_token(tokens, new_token(TOKEN_WORD, &input[start], *i - start, QUOTE_NONE));
}

void get_pid_var(char **value, size_t *i, t_token **tokens, t_quote_status quote_status)
{
    (*i)++;
    *value = ft_itoa(getpid());
    add_token(tokens, new_token(TOKEN_WORD, *value, ft_strlen(*value), quote_status));
    free(*value);
}

void accolade_gestion(char *input, size_t *i, char **tmp)
{
    size_t start;

    (*i)++;
    start = *i;
    while (input[*i] && input[*i] != '}')
        (*i)++;
    if (input[*i] == '}')
    {
        *tmp = ft_substr(input, start, *i - start);
        (*i)++;
    }
    else
    {
        *tmp = NULL;
        printf("\033[0;31mSyntax error: unclosed ${}\033[0m\n");
    }
}

void token_dollar(char *input, size_t *i, t_token **tokens, char **envp, t_quote_status quote_status)
{
    size_t start;
    char *tmp;
    char *value;

    start = *i;
    (*i)++;
    if (input[*i] == '$')
    {
        get_pid_var(&value, i, tokens, quote_status);
        return;
    }
    if (input[*i] == '{')
        accolade_gestion(input, i, &tmp);
    else
    {
        while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
            (*i)++;
        tmp = strndup(&input[start + 1], *i - start - 1);  // Extraire le nom de la variable
    }
    if (tmp)
    {
        value = get_env_variable(envp, tmp, quote_status);
        free(tmp);
        if (value)
            add_token(tokens, new_token(TOKEN_WORD, value, ft_strlen(value), quote_status));
    }
}

void tokenize(char *input, size_t *i, t_token **tokens, char **envp)
{
	if (input[*i] == '|')
		add_token(tokens, new_token(TOKEN_PIPE, &input[(*i)++], 1, QUOTE_NONE));
	else if (input[*i] == '$')
		token_dollar(input, i, tokens, envp, QUOTE_NONE);
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
		parse_double_quote(input, i ,tokens, envp);
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
	add_token(&tokens, new_token(TOKEN_EOF, NULL, 0, QUOTE_NONE));
	return tokens;
}
