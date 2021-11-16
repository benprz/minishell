/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:10:56 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/16 18:53:56 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell)
{
	int		i;
	char	*str;
	char	*save;

	i = 0;
	str = NULL;
	save = NULL;
	if (shell->command_list->redirection == 2)
	{
		close(shell->pipe_fd[0]);
		if (dup2(shell->command_list->fd, shell->pipe_fd[1]) == -1)
			ft_error_fork("Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		close(shell->pipe_fd[0]);
		if (dup2(1, shell->pipe_fd[1]) == -1)
			ft_error_fork("Error dup2 cmd");
	}
	while (shell->env[i])
	{
		save = ft_strjoin(str, shell->env[i]);
		str = ft_strjoin(save, "\n");
		free(save);
		save = NULL;
		i++;
	}
	write(shell->pipe_fd[1], str, ft_strlen(str));
	close(shell->pipe_fd[1]);
	free(str);
	return (SUCCESS);
}
