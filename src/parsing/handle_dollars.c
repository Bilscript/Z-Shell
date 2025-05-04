/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:06:19 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/04 12:06:34 by bhamani          ###   ########.fr       */
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

void	append_to_buf(t_parse_ctx *ctx, const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		ctx->buf[(*ctx->len)++] = str[i++];
}

static void	handle_exit_status(t_parse_ctx *ctx)
{
	char	*status_str;

	status_str = ft_itoa(g_exit_status);
	if (!status_str)
		return ;
	append_to_buf(ctx, status_str, ft_strlen(status_str));
	free(status_str);
	(*ctx->i) += 2;
}

void	token_dollar_inside_word(t_parse_ctx *ctx, t_envp *envp)
{
	size_t	start;
	char	*key;
	char	*val;

	if (ctx->input[*ctx->i + 1] == '?')
		return (handle_exit_status(ctx));
	start = ++(*ctx->i);
	if (!ctx->input[*ctx->i] || (!ft_isalnum(ctx->input[*ctx->i])
			&& ctx->input[*ctx->i] != '_'))
		return (ctx->buf[(*ctx->len)++] = '$', (void)0);
	while (ctx->input[*ctx->i] && (ft_isalnum(ctx->input[*ctx->i])
			|| ctx->input[*ctx->i] == '_'))
		(*ctx->i)++;
	key = ft_strndup(ctx->input + start, *ctx->i - start);
	if (!key)
		return ;
	val = get_value(envp, key);
	free(key);
	if (!val)
		return ;
	append_to_buf(ctx, val, ft_strlen(val));
}
