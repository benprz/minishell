/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:41:13 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/27 16:41:46 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		size_s1;
	int		size_s2;
	int		i;
	char	*strcat;

	i = 0;
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	strcat = malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (strcat == NULL)
		return (NULL);
	while (i < size_s1)
	{
		strcat[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < size_s2)
	{
		strcat[size_s1 + i] = s2[i];
		i++;
	}
	strcat[size_s1 + i] = '\0';
	return (strcat);
}
