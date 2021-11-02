/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:11:14 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/02 16:34:10 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(t_shell *shell, char *str)
{
	int	i;

	i = 0;
	perror(str);
	while (shell->arg[i])
	{
		free(shell->arg[i]);
		i++;
	}
	free(shell->arg);
	exit(EXIT_FAILURE);
}
