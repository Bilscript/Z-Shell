/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:49:22 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/03 14:00:29 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned long long	ft_atoull(const char *str)
{
	unsigned long long	nb;
	int					i;
	int					sign;

	sign = 1;
	nb = 0;
	i = 0;
	while ((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

static void	print_exit_error(char *arg)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_command *cmd)
{
	unsigned long long	num;
	unsigned char		ret;

	printf("exit\n");
	if (!cmd->args[1])
		exit(EXIT_SUCCESS);
	if (!is_numeric(cmd->args[1]))
	{
		print_exit_error(cmd->args[1]);
		exit(EXIT_BUILTIN_ERROR);
	}
	num = ft_atoull(cmd->args[1]);
	if ((cmd->args[1][0] != '-' && num > LONG_MAX)
		|| (cmd->args[1][0] == '-' && num - 1 > LONG_MAX))
	{
		print_exit_error(cmd->args[1]);
		exit(EXIT_BUILTIN_ERROR);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		g_exit_status = EXIT_GENERAL_ERROR;
		return (1);
	}
	ret = (unsigned char)num;
	exit(ret);
}

