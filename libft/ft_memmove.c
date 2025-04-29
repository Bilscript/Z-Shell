/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:05:27 by bhamani           #+#    #+#             */
/*   Updated: 2024/10/16 19:05:27 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*ptr_dest;
	const unsigned char	*ptr_src;

	ptr_dest = (unsigned char *) dest;
	ptr_src = (const unsigned char *) src;
	i = 0;
	if (!dest && !src)
		return (NULL);
	if (ptr_dest > ptr_src)
	{
		while (n-- > 0)
			ptr_dest[n] = ptr_src[n];
	}
	else
	{
		while (i < n)
		{
			ptr_dest[i] = ptr_src[i];
			i++;
		}
	}
	return (dest);
}
/*
#include <string.h>
int main() {
    char str[50] = "123456789bewyir";
    char buffer[50];
    
    printf("Résultat avant memmove: %s\n", str);
    // Copie avec chevauchement: source et destination se chevauchent
    memmove(str + 5, str, 9);

    printf("Résultat après memmove: %s\n", str);
    return 0;
}*/