/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 20:29:29 by zchtaibi          #+#    #+#             */
/*   Updated: 2023/11/06 15:27:31 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	index;
	char	*res;

	res = NULL;
	index = 0;
	while (s[index])
	{
		if (s[index] == (unsigned char)c)
			res = (char *)(s + index);
		index++;
	}
	if (s[index] == (unsigned char)c)
	{
		res = (char *)(s + index);
	}
	return (res);
}
