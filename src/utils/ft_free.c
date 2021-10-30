/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:11:14 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/30 19:14:46 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->sp_prompt[i])
	{
		free(shell->sp_prompt[i]);
		i++;
	}
	free(shell->sp_prompt);
	free(shell->prompt);
}

int	ft_error(t_shell *shell, char *str)
{
	perror(str);
	ft_free_tab(shell);
	return (ERROR);
}
