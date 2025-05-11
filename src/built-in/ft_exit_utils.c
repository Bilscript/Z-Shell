/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:49:54 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 16:50:18 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_number(char *arg, t_data *data)
{
	long long		num;
	unsigned char	ret;
	int				overflow;

	overflow = 0;
	num = ft_atoll(arg, &overflow);
	ret = (unsigned char)(num % 256);
	free_all(NULL, data);
	exit(ret);
}

int	handle_too_many_args(void)
{
	ft_putstr_fd("bash: exit: too many arguments\n", 2);
	g_exit_status = EXIT_GENERAL_ERROR;
	return (1);
}

void	exit_invalid_number(char *arg, t_data *data)
{
	print_exit_error(arg);
	free_all(NULL, data);
	exit(EXIT_BUILTIN_ERROR);
}
