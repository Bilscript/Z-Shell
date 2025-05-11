/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:00:00 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 16:51:16 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exit_error(char *arg)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static int	check_overflow(const char *str, int sign)
{
	size_t		len;
	const char	*max_str;

	len = ft_strlen(str);
	if (sign == 1)
		max_str = "9223372036854775807";
	else
		max_str = "9223372036854775808";
	if (len > ft_strlen(max_str))
		return (1);
	if (len < ft_strlen(max_str))
		return (0);
	return (ft_strncmp(str, max_str, len) > 0);
}

long long	ft_atoll(const char *str, int *overflow)
{
	int			sign;
	long long	res;

	res = 0;
	sign = 1;
	*overflow = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (check_overflow(str, sign))
	{
		*overflow = 1;
		return (0);
	}
	while (*str && ft_isdigit(*str))
		res = res * 10 + (*str++ - '0');
	if (*str != '\0')
		*overflow = 1;
	return (res * sign);
}

static int	is_valid_number(const char *str, int *overflow)
{
	const char	*ptr;

	ptr = str;
	if (*ptr == '-' || *ptr == '+')
		ptr++;
	if (!*ptr)
		return (0);
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
			return (0);
		ptr++;
	}
	ft_atoll(str, overflow);
	return (!(*overflow));
}

int	ft_exit(t_command *cmd, t_data *data)
{
	int	overflow;

	printf("exit\n");
	if (!cmd->args[1])
	{
		free_all(NULL, data);
		exit(EXIT_SUCCESS);
	}
	overflow = 0;
	if (!is_valid_number(cmd->args[1], &overflow))
		exit_invalid_number(cmd->args[1], data);
	if (cmd->args[2])
		return (handle_too_many_args());
	exit_with_number(cmd->args[1], data);
	return (0);
}
