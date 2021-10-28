/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:24:48 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/28 17:51:58 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_get_arg(t_shell *shell)
{
	char	**arg;
	int		size;

	size = 1;
	if (shell->sp_prompt[shell->position + 1] &&
		!ft_strcmp(shell->sp_prompt[shell->position + 1], "-n"))
	{
		arg = malloc(sizeof(char *) * 4);
		arg[0] = ft_strdup("echo");
		arg[1] = ft_strdup("-n");
		if (shell->sp_prompt[shell->position + 1])
			arg[2] = ft_strdup(shell->sp_prompt[shell->position + 2]);
		else
			arg[2] = "";
		arg[3] = NULL;
		shell->position += 2;
	}
	else
	{
		arg = malloc(sizeof(char *) * 3);
		arg[0] = ft_strdup("echo");
		if (shell->sp_prompt[shell->position + 1])
			arg[1] = ft_strdup(shell->sp_prompt[shell->position + 1]);
		else
			arg[1] = "";
		arg[2] = NULL;
		shell->position++;
	}
	return (arg);
}

int	ft_echo(t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**arg;

	shell->save_position = shell->position;
	arg = ft_get_arg(shell);
	pid = fork();
	if (pid == -1)
		perror("Error fork echo");
	else if (pid == 0)
	{
		if (shell->save_position != 0)
			if (dup2(shell->pipe_fd[0], 0) == -1)
				perror("Error dup2 echo");
		if (shell->sp_prompt[shell->position + 1] != NULL)
			if (dup2(shell->pipe_fd[1], 1) == -1)
				perror("Error dup2 echo");
		if (close(shell->pipe_fd[0]) == -1)
			perror("Error close echo");
		if (execve(ft_get_path(shell), arg, shell->env) == -1)
			perror("Error execve echo");
	}
	else
	{
		wait(&status);
		if (shell->sp_prompt[shell->position + 1] != NULL)
		{
			shell->position += 2;
			if (parse_command(shell) == ERROR)
				perror("Error command not found");
		}
	}
	return (SUCCESS);
}
