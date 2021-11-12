/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:24:48 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/12 01:48:40 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_shell *shell)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	if (shell->command_list->redirection == 2)
	{
		if (dup2(shell->command_list->fd, shell->pipe_fd[0]) == -1)
			ft_error_fork("Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error_fork("Error dup2 cmd");
	}
	while (shell->command_list->argv[i])
	{
		if (ft_strcmp(shell->command_list->argv[i], "-n"))
			check++;
		if (shell->command_list->argv[i + 1])
			shell->command_list->argv[i] = ft_strjoin(shell->command_list->argv[i], " ");
		write(shell->pipe_fd[0], shell->command_list->argv[i],
			ft_strlen(shell->command_list->argv[i]));
		i++;
	}
	if (check != 0)
		write(shell->pipe_fd[0], "\n", 1);
	return (SUCCESS);
}
