/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 18:27:34 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/16 19:57:01 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	check_same_delimiter(t_shell *shell, char *buf)
{
	int	i;

	while (buf[i] && buf[i] != '\n')
	{
		if (buf[i] != shell->command_list->delimiter[i])
			return (ERROR);
		i++;
	}
	if (shell->command_list->delimiter[i] == '\0')
		return (SUCCESS);
	return (ERROR);
}

void	get_std_in(t_shell *shell)
{
	int		ret;
	char	buf[1024 + 1];
	char	*str;

	str = NULL;
	ret = read(0, buf, 1024);
	buf[ret] = '\0';
	while (check_same_delimiter(shell, buf))
	{
		str = ft_strjoin(str, buf);
		ret = read(0, buf, 1024);
		buf[ret] = '\0';
	}
	close(shell->pipe_fd_redi_din[0]);
	write(shell->pipe_fd_redi_din[1], str, ft_strlen(str));
	close(shell->pipe_fd_redi_din[1]);
	free(str);
}

void	do_redirection_out(t_shell *shell)
{
	if (shell->command_list->redirection == 2
		|| shell->command_list->redirection == 4)
	{
		if (dup2(shell->command_list->fd, STDOUT_FILENO) == -1)
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
	if (shell->command_list->redirection == 1)
	{
		if (dup2(shell->command_list->fd, STDIN_FILENO) == -1)
			ft_error_fork("Error, Bad file descriptor");
	}
	else if (shell->command_list->redirection == 3)
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
		pid = fork();
		if (pid == -1)
			ft_error_fork("Error fork execute_command");
		else if (pid == 0)
			do_redirection_in(shell, status);
		else
		{
			if (shell->command_list->redirection == 3)
				get_std_in(shell);
			wait(&status);
		}
	}
	if (shell->command_list->next)
	{
		shell->command_list = shell->command_list->next;
		if (!shell->command_list->next)
			close(shell->pipe_fd[1]);
		execute_command(shell);
	}
}
