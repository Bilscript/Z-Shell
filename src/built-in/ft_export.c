/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:12:19 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/03 13:48:17 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_key(t_envp *envp, char *key, char *value)
{
	int		len;

	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
		{
			free(envp->value);
			len = len_until_char(value, '=');
			if (len == -1)
				envp->value = ft_strdup("");
			else
				envp->value = ft_strdup(value + len + 1);
			return (1);
		}
		envp = envp->next;
	}
	return (0);
}

static void	print_env(t_envp *envp)
{
	t_envp	*temp;

	temp = envp;
	while (temp)
	{
		if (temp->export == true)
			printf("export %s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return ;
}

int	is_valid(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_export_arg(t_envp **envp, const char *arg)
{
	int		len;
	char	*key;

	len = len_until_char(arg, '=');
	key = ft_strndup(arg, len);
	if (!key)
		return ;
	if (ft_strchr(arg, '='))
	{
		if (!set_key(*envp, key, (char *)arg))
			add_envp_back(envp, new_envp(key, (char *)arg + len + 1, true));
		free(key);
	}
	else
	{
		if (!set_key(*envp, key, ""))
			add_envp_back(envp, new_envp(key, "", true));
		free(key);
	}
}

void	ft_export(t_command *cmd, t_envp *envp)
{
	int		i;

	if (!cmd->args[1])
		return (print_env(envp));
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid(cmd->args[i]))
		{
			write(2, "bash: export: `", 15);
			write(2, cmd->args[i], strlen(cmd->args[i]));
			write(2, "': not a valid identifier\n", 26);
		}
		else
			handle_export_arg(&envp, cmd->args[i]);
		i++;
	}
}
