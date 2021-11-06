/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 18:48:16 by bperez            #+#    #+#             */
/*   Updated: 2021/11/06 05:16:00 by ben              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_set(const char *set, const char c)
{
	while (*set)
	{
		if (*set++ == c)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	len;

	while (*s1 && check_set(set, *s1))
		s1++;
	len = ft_strlen(s1);
	while (len && check_set(set, s1[len - 1]))
		len--;
	return (ft_strndup(s1, len));
}
