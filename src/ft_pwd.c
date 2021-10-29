/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/29 14:27:24 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**arg;
	
	shell->save_position = shell->position;
	arg = ft_get_arg(shell);
	arg[1] = NULL;
	pid = fork();
	if (pid == -1)
		perror("Error fork pwd");
	else if (pid == 0)
	{
		if (shell->save_position != 0)
		{
			if (dup2(shell->pipe_fd[0], 0) == -1)
				perror("Error dup2 pwd");
		}
		if (shell->sp_prompt[shell->position] != NULL)
		{
			if (dup2(shell->pipe_fd[1], 1) == -1)
				perror("Error dup2 pwd");
		}
		if (close(shell->pipe_fd[0]) == -1)
			perror("Error close pwd");
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
