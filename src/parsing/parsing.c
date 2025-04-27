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

char	*get_value(t_envp *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

void	token_word(char *input, size_t *i, t_token **tokens, t_envp *envp)
{
	char		*buf;
	size_t		len;
	t_parse_ctx	ctx;

	buf = malloc(strlen(input + *i) + 1);
	if (!buf)
		return ;
	len = 0;
	ctx = init_parse_ctx(input, i, buf, &len);
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '"')
			handle_double_quote(&ctx, envp);
		else if (input[*i] == '\'')
			handle_single_quote(&ctx);
		else if (input[*i] == '$')
			handle_variable(&ctx, envp);
		else
			buf[len++] = input[(*i)++];
	}
	buf[len] = '\0';
	add_token(tokens, new_token(TOKEN_WORD, buf, len, QUOTE_NONE));
	free(buf);
}

void	token_dollar(t_parse_ctx *ctx, t_token **tkn, t_envp *envp,
					t_quote_status sta)
{
	size_t	start;
	char	*tmp;
	char	*value;

	start = *ctx->i;
	(*ctx->i)++;
	if (ctx->input[*ctx->i] == '$')
	{
		get_pid_var(&value, ctx->i, tkn, sta);
		return ;
	}
	if (ctx->input[*ctx->i] == '{')
		accolade_gestion(ctx->input, ctx->i, &tmp);
	else if (!ctx->input[*ctx->i] || ft_isspace(ctx->input[*ctx->i])
		|| ctx->input[*ctx->i] == '|' || ctx->input[*ctx->i] == '<'
		|| ctx->input[*ctx->i] == '>')
	{
		add_token(tkn, new_token(TOKEN_WORD, "$", 1, sta));
		return ;
	}
	else
	{
		while (ctx->input[*ctx->i] && (ft_isalnum(ctx->input[*ctx->i])
				|| ctx->input[*ctx->i] == '_'))
			(*ctx->i)++;
		tmp = strndup(&ctx->input[start + 1], *ctx->i - start - 1);
	}
	if (tmp)
	{
		value = get_value(envp, tmp);
		free(tmp);
		if (value)
			add_token(tkn, new_token(TOKEN_WORD, value, ft_strlen(value), sta));
	}
}

void	tokenize_special(char *input, size_t *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, &input[*i], 1, QUOTE_NONE));
		(*i)++;
	}
	else if (input[*i] == '<')
		handle_redir_in(input, i, tokens);
	else if (input[*i] == '>')
		handle_redir_out(input, i, tokens);
}

t_token	*tokenizer(char *input, t_envp *envp)
{
	size_t		i;
	t_token		*tokens;
	t_parse_ctx	ctx;

	tokens = NULL;
	i = 0;
	ctx.input = input;
	ctx.i = &i;
	ctx.buf = NULL;
	ctx.len = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '$')
			token_dollar(&ctx, &tokens, envp, QUOTE_NONE);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			tokenize_special(input, &i, &tokens);
		else
			token_word(input, &i, &tokens, envp);
	}
	add_token(&tokens, new_token(TOKEN_EOF, NULL, 0, QUOTE_NONE));
	return (tokens);
}
