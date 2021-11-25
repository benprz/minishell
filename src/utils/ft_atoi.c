/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 08:44:11 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/25 08:46:51 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		total;
	int		nbrless;

	nbrless = 1;
	i = 0;
	total = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			nbrless = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		total = total * 10 + (str[i] - '0');
		i++;
	}
	return (total * nbrless);
}
