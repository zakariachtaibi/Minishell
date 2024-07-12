/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 15:29:47 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/08 17:40:01 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_el(char **elements)
{
	int	i;

	i = 0;
	if (elements)
	{
		while (elements[i])
		{
			free(elements[i]);
			i++;
		}
		free(elements);
	}
}

void	errornl(const char *msg, char **elements)
{
	write(2, msg, ft_strlen(msg));
	free_el(elements);
	exit(1);
}

int	ft_atoi(const char *nptr, char **elements)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -sign;
	if (nptr[i] < '0' || nptr[i] > '9')
		errornl("Error\n", elements);
	while (nptr[i] && nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = nbr * 10 + (nptr[i] - '0');
		if ((nbr > 2147483648 && sign == -1) || (nbr > 2147483647 && sign == 1))
			errornl("Error\n", elements);
		i++;
	}
	if (nptr[i])
		errornl("Error\n", elements);
	return (nbr * sign);
}
