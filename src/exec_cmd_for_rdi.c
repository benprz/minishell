/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_for_rdi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:20:40 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/19 10:35:12 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_same_delimiter(t_shell *shell, char *buf)
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

static void	get_std_in(t_shell *shell)
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
	exit(EXIT_SUCCESS);
}

void	exec_cmd_for_rdi(t_shell *shell)
{
	pid_t	pid_redi_din;
	int		status;

	if (pipe(shell->pipe_fd_redi_din) == -1)
		ft_error("Pipe", EXIT_CMD);
	pid_redi_din = fork();
	if (pid_redi_din == -1)
		ft_error_fork("Error fork execute_command");
	else if (pid_redi_din == 0)
		get_std_in(shell);
	else
	{
		wait(&status);
		close(shell->pipe_fd_redi_din[1]);
	}
}

void	close_pipe_rdi(t_shell *shell)
{
	close(shell->pipe_fd_redi_din[0]);
	close(shell->pipe_fd_redi_din[1]);
}
