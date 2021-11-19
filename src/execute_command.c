/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 18:27:34 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/19 10:30:38 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_redirection_out(t_shell *shell)
{
	if (shell->command_list->redirection_out == REDIRECTION_OUTPUT
		|| shell->command_list->redirection_out == REDIRECTION_DOUTPUT)
	{
		if (dup2(shell->command_list->fd_out, STDOUT_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	else if (shell->command_list->next)
	{
		if (dup2(shell->pipe_fd[1], STDOUT_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	if (check_commad_2(shell) == ERROR)
	{
		if (execve(ft_get_path(shell),
				shell->command_list->argv, shell->env) == -1)
			ft_error_fork("Error command not found");
	}
	else
		exit(EXIT_SUCCESS);
}

void	do_redirection_in(t_shell *shell, int status)
{
	if (shell->command_list->redirection_in == REDIRECTION_INPUT)
	{
		if (dup2(shell->command_list->fd_in, STDIN_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	else if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
	{
		if (dup2(shell->pipe_fd_redi_din[0], STDIN_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	else if (shell->command_list->prev)
	{
		if (dup2(shell->pipe_fd[0], STDIN_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	do_redirection_out(shell);
}

void	execute_command(t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (check_commad_1(shell) == ERROR)
	{
		if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
			exec_cmd_for_rdi(shell);
		pid = fork();
		if (pid == -1)
			ft_error_fork("Error fork execute_command");
		else if (pid == 0)
			do_redirection_in(shell, status);
		else
			wait(&status);
	}
	if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
		close_pipe_rdi(shell);
	if (shell->command_list->next)
	{
		shell->command_list = shell->command_list->next;
		if (!shell->command_list->next)
			close(shell->pipe_fd[1]);
		execute_command(shell);
	}
}
