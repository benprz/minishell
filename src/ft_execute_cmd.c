/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 17:06:33 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/30 17:52:39 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_cmd(t_shell *shell)
{
	pid_t	pid;
	int		status;

	shell->save_position = shell->position;
	shell->arg = ft_get_arg(shell);
	if (!ft_strcmp(shell->sp_prompt[shell->save_position], "env") ||
		!ft_strcmp(shell->sp_prompt[shell->save_position], "pwd"))
		shell->arg[1] = NULL;
	pid = fork();
	if (pid == -1)
	{
		perror("Error fork cmd");
		return (ERROR);
	}
	else if (pid == 0)
	{
		if (shell->save_position != 0)
		{
			if (shell->redi_in == 1)
			{
				if (dup2(shell->fd_in, 0) == -1)
				{
					perror("Error dup2 cmd");
					return (ERROR);
				}
			}
			else
			{
				if (dup2(shell->pipe_fd[0], 0) == -1)
				{
					perror("Error dup2 cmd");
					return (ERROR);
				}
			}
		}
		if (shell->sp_prompt[shell->position] != NULL)
		{
			if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
			{
				shell->fd_out = open(shell->sp_prompt[shell->position + 1], O_CREAT | O_RDWR, S_IRWXU);
				if (dup2(shell->fd_out, 1) == -1)
				{
					perror("Error dup2 cmd");
					return (ERROR);
				}
			}
			else
				if (dup2(shell->pipe_fd[1], 1) == -1)
				{
					perror("Error dup2 cmd");
					return (ERROR);
				}
		}
		if (close(shell->pipe_fd[0]) == -1)
		{
			perror("Error close cmd");
			return (ERROR);
		}
		if (parse_command(shell) == ERROR)
		{
			if (execve(ft_get_path(shell), shell->arg, shell->env) == -1) 
			{
				perror("Error execve cmd");
				return (ERROR);
			}
		}
	}
	else
	{
		wait(&status);
		ft_free_tab(&(shell->arg));
		if (shell->sp_prompt[shell->position] != NULL)
		{
			shell->position++;
			if (parse_command(shell) == ERROR)
				perror("Error command not found");
		}
	}
	return (SUCCESS);
}
