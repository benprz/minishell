/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/24 17:03:31 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*pwd;

	j = 4;
	k = 0;
	i = get_current_env_int(shell, "PWD");
	if (i == -1)
		return (NULL);
	pwd = malloc(sizeof(char) * (ft_strlen(shell->env[i]) - 3));
	while (shell->env[i][j])
	{
		pwd[k] = shell->env[i][j];
		k++;
		j++;
	}
	pwd[k] = '\0';
	return (pwd);
}

static void	do_redirection(t_shell *shell)
{
	if (shell->command_list->redirection_out == 2)
	{
		close(shell->pipe_fd[shell->index][0]);
		if (dup2(shell->command_list->fd_out,
				shell->pipe_fd[shell->index][1]) == -1)
			ft_error_fork(shell, "Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		close(shell->pipe_fd[shell->index][0]);
		if (dup2(1, shell->pipe_fd[shell->index][1]) == -1)
			ft_error_fork(shell, "Error dup2 cmd");
	}
}

int	ft_pwd(t_shell *shell)
{
	char	*str;
	size_t	i;

	i = 1;
	str = NULL;
	do_redirection(shell);
	str = get_pwd(shell);
	str = ft_strjoin(str, "\n");
	write(shell->pipe_fd[shell->index][1], str, ft_strlen(str));
	close(shell->pipe_fd[shell->index][1]);
	free(str);
	return (SUCCESS);
}
