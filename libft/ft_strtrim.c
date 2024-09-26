/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:48:31 by zchtaibi          #+#    #+#             */
/*   Updated: 2023/11/17 17:46:39 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strcontains(int c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*chaine;

	start = 0;
	end = 0;
	i = 0;
	if (s1 == 0 || set == 0)
		return (NULL);
	while (s1[i] && strcontains(s1[i++], set))
		start++;
	i = 0;
	end = ft_strlen(s1);
	while (s1[i++] && strcontains(s1[end - 1], set) && end > start)
		end--;
	chaine = (char *)malloc(end - start + 1);
	if (chaine != NULL)
		ft_strlcpy(chaine, s1 + start, end - start + 1);
	return (chaine);
}
