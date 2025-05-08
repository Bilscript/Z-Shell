/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:49:22 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/08 19:17:44 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exit_error(char *arg)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

int	is_numeric(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

long long	ft_atoll(const char *str)
{
	int			sign;
	long long	res;

	res = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while (*str && ft_isdigit(*str))
		res = res * 10 + (*str++ - '0');
	return (res * sign);
}

void	free_all(t_command *cmd, t_data *data)
{
	if (data)
	{
		if (data->cmd)
			free_command(data->cmd);
		if (data->token)
			free_tokens(data->token);
		free_envp_list(&(data->env_data));
		free(data);
	}
	if (cmd)
		free_command(cmd);
}



int	ft_exit(t_command *cmd, t_data *data)
{
	long long		num;
	unsigned char	ret;

	printf("exit\n");
	if (!cmd->args[1])
	{
		free_all(NULL, data);
		exit(EXIT_SUCCESS);
	}
	if (!is_numeric(cmd->args[1]))
	{
		print_exit_error(cmd->args[1]);
		free_all(NULL, data);
		exit(EXIT_BUILTIN_ERROR);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		g_exit_status = EXIT_GENERAL_ERROR;
		return (1);
	}
	num = ft_atoll(cmd->args[1]);
	ret = (unsigned char)(num % 256);
	free_all(NULL, data);
	exit(ret);
}
