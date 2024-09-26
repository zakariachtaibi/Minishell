/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:09:32 by zchtaibi          #+#    #+#             */
/*   Updated: 2023/11/17 17:30:52 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pt;

	if (!nmemb && !size && (nmemb >= SIZE_MAX / size))
		return (NULL);
	pt = malloc(nmemb * size);
	if (pt != NULL)
		ft_bzero(pt, nmemb * size);
	else
		return (NULL);
	return (pt);
}
