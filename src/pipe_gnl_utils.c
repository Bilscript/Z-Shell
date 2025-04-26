/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_gnl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:04:13 by slebik            #+#    #+#             */
/*   Updated: 2025/04/26 16:04:13 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(ft_strlen(str) + 2);
	if (!result)
		return (NULL);
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	free(str);
	return (result);
}

int	get_next_line(char **line)
{
	char	*str;
	char	c;
	int		ret;

	str = malloc(1);
	if (!str)
		return (-1);
	str[0] = '\0';
	ret = read(0, &c, 1);
	while (ret > 0 && c != '\n')
	{
		str = ft_strjoin_char(str, c);
		if (!str)
			return (-1);
		ret = read(0, &c, 1);
	}
	*line = str;
	return (ret);
}
