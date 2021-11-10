/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/10 13:16:16 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_pwd(t_shell *shell, char **pwd)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 4;
	k = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'P' && shell->env[i][1] == 'W')
			break ;
		i++;
	}
	*pwd = malloc(sizeof(char) * (ft_strlen(shell->env[i]) - 3));
	while (shell->env[i][j])
	{
		(*pwd)[k] = shell->env[i][j];
		k++;
		j++;
	}
	(*pwd)[k] = '\0';
}

int	ft_pwd(t_shell *shell)
{
	char	*str;
	size_t	i;

	i = 1;
	str = NULL;
	if (!shell->command_list->next)
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error("Error dup2 cmd");
	get_pwd(shell, &str);
	str = ft_strjoin(str, "\n");
	write(shell->pipe_fd[0], str, ft_strlen(str));
	free(str);
	return (SUCCESS);
}
