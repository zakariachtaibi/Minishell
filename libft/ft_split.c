/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:54:26 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/23 22:03:06 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(const char *str, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] != c) && ((str[i + 1] == c) || str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

void	*free_all(char **sp)
{
	size_t	j;

	j = 0;
	while (sp[j])
	{
		free(sp[j]);
		j++;
	}
	free(sp);
	return (NULL);
}

static void	fill_split(char **split, const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	wordlen;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			wordlen = 0;
			while (s[i + wordlen] && s[i + wordlen] != c)
				wordlen++;
			split[j] = malloc(wordlen + 1);
			if (!split[j])
				free_all(split);
			ft_strlcpy(split[j], s + i, wordlen + 1);
			i += wordlen;
			j++;
		}
	}
	split[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**split;

	if (!s)
		return (NULL);
	split = malloc((word_count(s, c) + 1) * sizeof(char *));
	if (split == NULL)
		return (NULL);
	fill_split(split, s, c);
	return (split);
}
