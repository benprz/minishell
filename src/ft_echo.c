/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:24:48 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/20 17:49:57 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	do_redirection(t_shell *shell)
{
	if (shell->command_list->redirection_out == 2)
	{
		close(shell->pipe_fd[0]);
		if (dup2(shell->command_list->fd_out, shell->pipe_fd[1]) == -1)
			ft_error_fork(shell,  "Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		close(shell->pipe_fd[0]);
		if (dup2(1, shell->pipe_fd[1]) == -1)
			ft_error_fork(shell,  "Error dup2 cmd");
	}
}

int	ft_echo(t_shell *shell)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	do_redirection(shell);
	while (shell->command_list->argv[i])
	{
		if (ft_strcmp(shell->command_list->argv[i], "-n"))
			check++;
		if (shell->command_list->argv[i + 1])
			shell->command_list->argv[i]
				= ft_strjoin(shell->command_list->argv[i], " ");
		write(shell->pipe_fd[1], shell->command_list->argv[i],
			ft_strlen(shell->command_list->argv[i]));
		i++;
	}
	if (check != 0)
		write(shell->pipe_fd[1], "\n", 1);
	close(shell->pipe_fd[1]);
	return (SUCCESS);
}
