/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 17:13:14 by bperez            #+#    #+#             */
/*   Updated: 2021/11/06 05:17:40 by ben              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	*ft_strndup(const char *s1, size_t len)
{
	char	*s2;
	size_t	slen;

	s2 = NULL;
	if (s1 && len > 0)
	{
		slen = ft_strlen(s1);
		if (len > slen)
			len = slen;
		s2 = (char *)ft_calloc(len + 1, sizeof(char));
		if (s2)
		{
			while (len--)
				s2[len] = s1[len];
		}
	}
	return (s2);
}
