/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 17:06:33 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/06 05:02:49 by ben              ###   ########lyon.fr   */
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
					ft_error(shell, "Error dup2 cmd");
			}
			else
				if (dup2(shell->pipe_fd[0], 0) == -1)
					ft_error(shell, "Error dup2 cmd");
		}
		if (shell->sp_prompt[shell->position] != NULL)
		{
			if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
			{
				if (shell->sp_prompt[shell->position + 1])
				{
					shell->fd_out = open(shell->sp_prompt[shell->position + 1],
							O_CREAT | O_RDWR, S_IRWXU);
					if (shell->fd_out == -1)
						ft_error(shell, "Error open fd_out");
					if (dup2(shell->fd_out, 1) == -1)
						ft_error(shell, "Error dup2 cmd");
				}
				else
					exit(EXIT_FAILURE);
			}
			else if (dup2(shell->pipe_fd[1], 1) == -1)
				ft_error(shell, "Error dup2 cmd");
		}
		if (close(shell->pipe_fd[0]) == -1)
			ft_error(shell, "Error close cmd");
		if (parse_program(shell) == ERROR)
		{
			if (execve(ft_get_path(shell), shell->arg, shell->env) == -1)
				ft_error(shell, "Error command not found");
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
