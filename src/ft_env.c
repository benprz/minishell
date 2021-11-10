/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:10:56 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/10 12:02:19 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell)
{
	int		i;
	char	*str;
	char	*save;

	i = 0;
	str = NULL;
	save = NULL;
	if (!shell->command_list->next)
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error("Error dup2 cmd");
	while (shell->env[i])
	{
		save = ft_strjoin(str, shell->env[i]);
		str = ft_strjoin(save, "\n");
		free(save);
		save = NULL;
		i++;
	}
	write(shell->pipe_fd[0], str, ft_strlen(str));
	free(str);
	return (SUCCESS);
}
