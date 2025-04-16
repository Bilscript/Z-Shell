/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:41:30 by bhamani           #+#    #+#             */
/*   Updated: 2024/10/30 19:41:30 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	dsize;
	size_t	ssize;
	size_t	i;

	dsize = ft_strlen(dest);
	ssize = ft_strlen(src);
	if (size <= dsize)
		return (size + ssize);
	i = 0;
	while (src[i] && (dsize + i < size - 1))
	{
		dest[dsize + i] = src[i];
		i++;
	}
	dest[dsize + i] = '\0';
	return (dsize + ssize);
}
