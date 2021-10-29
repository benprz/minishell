/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:10:56 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/29 15:22:23 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**arg;

	shell->save_position = shell->position;
	arg = ft_get_arg(shell);
	arg[1] = NULL;
	pid = fork();
	if (pid == -1)
		perror("Error fork env");
	else if (pid == 0)
	{
		if (shell->save_position != 0)
			if (dup2(shell->pipe_fd[0], 0) == -1)
				perror("Error dup2 env");
		if (shell->sp_prompt[shell->position] != NULL)
			if (dup2(shell->pipe_fd[1], 1) == -1)
				perror("Error dup2 env");
		if (close(shell->pipe_fd[0]) == -1)
			perror("Error close ecenvho");
		if (execve(ft_get_path(shell), arg, shell->env) == -1) 
			perror("Error execve env");
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
