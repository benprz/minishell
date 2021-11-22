/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_for_rdi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:20:40 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/22 08:36:20 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_same_delimiter(char *delimiter, char *buf)
{
	int	i;

	while (buf[i] && buf[i] != '\n')
	{
		if (buf[i] != delimiter[i])
			return (ERROR);
		i++;
	}
	if (delimiter[i] == '\0')
		return (SUCCESS);
	return (ERROR);
}

static void	get_std_in(t_shell *shell)
{
	int		ret;
	char	buf[1024 + 1];
	char	*str;
	int		i;

	i = 0;
	str = NULL;
	ret = read(0, buf, 1024);
	buf[ret] = '\0';
	while (shell->command_list->delimiter[i])
	{
		while (check_same_delimiter(shell->command_list->delimiter[i], buf))
		{
			str = ft_strjoin(str, buf);
			ret = read(0, buf, 1024);
			buf[ret] = '\0';
		}
		i++;
	}
	close(shell->pipe_fd_redi_din[0]);
	if (str)
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
		ft_error_fork(shell, "Error pipe() execute_command");
	else if (pid_redi_din == 0)
		get_std_in(shell);
	else
	{
		wait(&status);
		close(shell->pipe_fd_redi_din[1]);
	}
	printf("execcommand rdi wait()=%d\n", status);
}

void	close_pipe_rdi(t_shell *shell)
{
	close(shell->pipe_fd_redi_din[0]);
	close(shell->pipe_fd_redi_din[1]);
}
