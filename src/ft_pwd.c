/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/27 18:07:57 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_fils(t_shell *shell, char **env)
{
	char	*prompt_pwd[2];
	char	*path_prompt;

	prompt_pwd[0] = shell->sp_prompt[0];
	prompt_pwd[1] = NULL;
	path_prompt = ft_get_path(shell);
	close(shell->pipe_fd[0]);
	dup2(shell->pipe_fd[1], 1);
	execve(path_prompt, prompt_pwd, env);
}

int	ft_pwd(t_shell *shell, char **env)
{
	char	buf[1024];
	int		ret;
	int		status;
	pid_t	pid;

	if (pipe(shell->pipe_fd) == -1)
		return (ERROR);
	pid = fork();
	if (pid == 0)
		ft_fils(shell, env);
	else
	{
		wait(&status);
		close(shell->pipe_fd[1]);
		ret = read(shell->pipe_fd[0], buf, 1024);
		buf[ret] = 0;
		printf("%s", buf);
	}
	return (SUCCESS);
}
