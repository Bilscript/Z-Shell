/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:36:42 by bhamani           #+#    #+#             */
/*   Updated: 2024/11/12 21:37:21 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_countnbr(int nb)
{
	int	len;

	len = 0;
	if (nb < 0)
		len++;
	while (nb)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*check_limit(int nb)
{
	if (nb == 0)
		return (ft_strdup("0"));
	if (nb == -2147483648)
		return (ft_strdup("-2147483648"));
	return (0);
}

char	*ft_itoa(int nb)
{
	int		len;
	int		i;
	char	*res;

	i = 0;
	len = ft_countnbr(nb);
	res = check_limit(nb);
	if (res)
		return (res);
	res = (char *)malloc((len + 1) * sizeof (char));
	if (!res)
		return (0);
	if (nb < 0)
	{
		res[0] = '-';
		nb = -nb;
	}
	while (nb)
	{
		res[len - i++ - 1] = ((nb % 10) + '0');
		nb /= 10;
	}
	res[len] = '\0';
	return (res);
}
