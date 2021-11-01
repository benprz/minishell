/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 17:06:33 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/01 19:37:58 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_cmd(t_shell *shell)
{
	pid_t	pid;
	int		status;

	shell->save_position = shell->position;
	if (!shell->sp_prompt[shell->position])
		return (ERROR);
	shell->arg = ft_get_arg(shell);
	pid = fork();
	if (pid == -1)
	{
		perror("Error fork cmd");
		exit(EXIT_FAILURE);
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
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				if (dup2(shell->pipe_fd[0], 0) == -1)
				{
					perror("Error dup2 cmd");
					exit(EXIT_FAILURE);
				}
			}
		}
		if (shell->sp_prompt[shell->position] != NULL)
		{
			if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
			{
				if (shell->sp_prompt[shell->position + 1])
				{
					shell->fd_out = open(shell->sp_prompt[shell->position + 1], O_CREAT | O_RDWR, S_IRWXU);
					if (shell->fd_out == -1)
					{
						perror("Error open fd_out");
						exit(EXIT_FAILURE);
					}
					if (dup2(shell->fd_out, 1) == -1)
					{
						perror("Error dup2 cmd");
						exit(EXIT_FAILURE);
					}
				}
				else
					exit(EXIT_FAILURE);
			}
<<<<<<< HEAD
			else
			{
				if (dup2(shell->pipe_fd[1], 1) == -1)
				{
					perror("Error dup2 cmd");
					exit(EXIT_FAILURE);
				}
=======
			else if (dup2(shell->pipe_fd[1], 1) == -1)
			{
				perror("Error dup2 cmd");
				return (ERROR);
>>>>>>> 68e1dfaa38c6761de6f6dfbf7c22508ac56a2ce0
			}
		}
		if (close(shell->pipe_fd[0]) == -1)
		{
			perror("Error close cmd");
			exit(EXIT_FAILURE);
		}
		if (parse_command(shell) == ERROR)
		{
			if (execve(ft_get_path(shell), shell->arg, shell->env) == -1) 
			{
				perror("Error execve cmd");
				exit(EXIT_FAILURE);
			}
		}
		else
			exit(EXIT_SUCCESS);
	}
	else
	{
		wait(&status);
		
		if (shell->redi_out == 1)
			shell->position++;
		shell->redi_in = 0;
		shell->redi_out = 0;
		if (shell->sp_prompt[shell->position])
		{
			shell->position++;
			ft_execute_cmd(shell);
		}
	}
	return (SUCCESS);
}
