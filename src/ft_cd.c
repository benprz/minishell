/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:06:03 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/29 15:12:29 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**arg;

	shell->save_position = shell->position;
	arg = ft_get_arg(shell);
	pid = fork();
	if (pid == -1)
		perror("Error fork echo");
	else if (pid == 0)
	{
		if (shell->save_position != 0)
		{
			if (dup2(shell->pipe_fd[0], 0) == -1)
				perror("Error dup2 echo");
		}
		if (shell->sp_prompt[shell->position] != NULL)
		{
			if (dup2(shell->pipe_fd[1], 1) == -1)
				perror("Error dup2 echo");
		}
		if (close(shell->pipe_fd[0]) == -1)
			perror("Error close echo");
		if (execve(ft_get_path(shell), arg, shell->env) == -1) 
			perror("Error execve echo");
	}
	else
	{
		wait(&status);
		ft_free_tab(&arg);
		if (shell->sp_prompt[shell->position] != NULL)
		{
			shell->position++;
			if (parse_command(shell) == ERROR)
				perror("Error command not found");
		}
	}
	return (SUCCESS);
}
