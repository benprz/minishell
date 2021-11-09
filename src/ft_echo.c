/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:24:48 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/09 19:45:00 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_shell *shell)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	if (!shell->command_list->next)
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error("Error dup2 cmd");
	while (shell->command_list->argv[i])
	{
		if (ft_strcmp(shell->command_list->argv[i], "-n"))
		{
			check++;
			write(shell->pipe_fd[0], shell->command_list->argv[i],
				ft_strlen(shell->command_list->argv[i]));
		}
		i++;
	}
	if (check != 0)
		printf("\n");
	return (SUCCESS);
}
