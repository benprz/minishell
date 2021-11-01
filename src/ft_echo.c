/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:24:48 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/01 19:24:08 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_shell *shell)
{
	if (!shell->sp_prompt[shell->position])
	{
		if (dup2(1, shell->pipe_fd[0]) == -1)
		{
			perror("Error dup2 cmd");
			exit(EXIT_FAILURE);
		}
	}
	if (shell->arg[1] == NULL || ft_strcmp(shell->arg[1], "-n"))
	{
		shell->arg[1] = ft_strjoin(shell->arg[1], "\n");
		write(shell->pipe_fd[0], shell->arg[1], ft_strlen(shell->arg[1]));
	}
	else
		write(shell->pipe_fd[0], shell->arg[2], ft_strlen(shell->arg[2]));
	return (SUCCESS);
}
