/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/19 10:43:23 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*pwd;

	i = 0;
	j = 4;
	k = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'P' && shell->env[i][1] == 'W')
			break ;
		i++;
	}
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

int	ft_pwd(t_shell *shell)
{
	char	*str;
	size_t	i;

	i = 1;
	str = NULL;
	if (shell->command_list->redirection_out == 2)
	{
		close(shell->pipe_fd[0]);
		if (dup2(shell->command_list->fd_out, shell->pipe_fd[1]) == -1)
			ft_error_fork("Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		close(shell->pipe_fd[0]);
		if (dup2(1, shell->pipe_fd[1]) == -1)
			ft_error_fork("Error dup2 cmd");
	}
	str = get_pwd(shell);
	str = ft_strjoin(str, "\n");
	write(shell->pipe_fd[1], str, ft_strlen(str));
	close(shell->pipe_fd[1]);
	free(str);
	return (SUCCESS);
}
