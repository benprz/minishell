/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 18:27:34 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/24 13:58:32 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	care_pipe(t_shell *shell)
{
	if (shell->index == 2)
		shell->index = 0;
	else
		shell->index++;
	if (shell->index == 2)
	{
		close(shell->pipe_fd[0][0]);
		close(shell->pipe_fd[0][1]);
		if (pipe(shell->pipe_fd[0]) == -1)
			perror("Error pipe initialisation");
	}
	else
	{
		close(shell->pipe_fd[shell->index + 1][0]);
		close(shell->pipe_fd[shell->index + 1][1]);
		if (pipe(shell->pipe_fd[shell->index + 1]) == -1)
			perror("Error pipe initialisation");
	}
}

static void	get_env_export(t_shell *shell)
{
	int		ret;
	char	buff[1024 + 1];
	char	*str;

	str = NULL;
	close(shell->pipe_export[1]);
	ret = read(shell->pipe_export[0], buff, 1024);
	buff[ret] = '\0';
	while (ret > 0)
	{
		str = ft_strjoin(str, buff);
		ret = read(shell->pipe_export[0], buff, 1024);
		buff[ret] = '\0';
	}
	close(shell->pipe_export[0]);
	free_tab(shell->env);
	shell->env = ft_split(str, '\n');
}

static void	do_redirection_out(t_shell *shell)
{
	close(shell->pipe_fd[shell->index][0]);
	if (shell->command_list->redirection_out == REDIRECTION_OUTPUT
		|| shell->command_list->redirection_out == REDIRECTION_DOUTPUT)
	{
		if (dup2(shell->command_list->fd_out, STDOUT_FILENO) == -1)
			ft_error_fork(shell, "Error, Bad file descriptor");
	}
	else if (shell->command_list->next)
	{
		if (dup2(shell->pipe_fd[shell->index][1], STDOUT_FILENO) == -1)
			ft_error_fork(shell, "Error, Bad file descriptor");
	}
	if (check_commad_2(shell) == ERROR)
	{
		if (execve(shell->command_list->program_path,
				shell->command_list->argv, shell->env) == -1)
			ft_error_fork(shell, "Error command not found");
	}
	exit(EXIT_SUCCESS);
}

static void	do_redirection_in(t_shell *shell, int status)
{
	g_process_section = 2;
	if (shell->index == 0)
		close(shell->pipe_fd[2][1]);
	else
		close(shell->pipe_fd[shell->index - 1][1]);
	if (shell->command_list->redirection_in == REDIRECTION_INPUT)
	{
		if (dup2(shell->command_list->fd_in, STDIN_FILENO) == -1)
			ft_error_fork(shell, "Error, Bad file descriptor");
	}
	else if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
	{
		if (dup2(shell->pipe_fd_redi_din[0], STDIN_FILENO) == -1)
			ft_error_fork(shell, "Error, Bad file descriptor");
	}
	else if (shell->command_list->prev)
	{
		if (shell->index == 0)
		{
			if (dup2(shell->pipe_fd[2][0], STDIN_FILENO) == -1)
				ft_error_fork(shell, "Error, Bad file descriptor");
		}
		else
			if (dup2(shell->pipe_fd[shell->index - 1][0], STDIN_FILENO) == -1)
				ft_error_fork(shell, "Error, Bad file descriptor");
	}
	do_redirection_out(shell);
}

void	execute_command(t_shell *shell)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (check_commad_1(shell) == ERROR)
	{
		if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
			exec_cmd_for_rdi(shell);
		g_process_section = 1;
		pid = fork();
		if (pid == -1)
			ft_error_fork(shell, "Error fork() execute_command");
		else if (pid == 0)
			do_redirection_in(shell, status);
		else
		{
			if (shell->index == 0)
			{
				close(shell->pipe_fd[2][0]);
				close(shell->pipe_fd[2][1]);
			}
			else
			{
				close(shell->pipe_fd[shell->index - 1][0]);
				close(shell->pipe_fd[shell->index - 1][1]);
			}
			wait(&status);
		}
	}
	g_process_section = 0;
	care_pipe(shell);
	if (shell->command_list->redirection_in == REDIRECTION_DINPUT)
		close_pipe_rdi(shell);
	if (!ft_strcmp(shell->command_list->argv[0], "export"))
		get_env_export(shell);
	if (shell->command_list->next)
	{
		shell->command_list = shell->command_list->next;
		if (!shell->command_list->next)
		{
			if (shell->index == 2)
			{
				close(shell->pipe_fd[0][0]);
				close(shell->pipe_fd[0][1]);
			}
			else
			{
				close(shell->pipe_fd[shell->index + 1][0]);
				close(shell->pipe_fd[shell->index + 1][1]);
			}
			close(shell->pipe_fd[shell->index][0]);
			close(shell->pipe_fd[shell->index][1]);
		}
		execute_command(shell);
	}
}
