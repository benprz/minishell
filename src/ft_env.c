/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:10:56 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/01 19:24:50 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell)
{
	int		i;
	char	*str = NULL;

	i = 0;
	if (!shell->sp_prompt[shell->position])
	{
		if (dup2(1, shell->pipe_fd[0]) == -1)
		{
			perror("Error dup2 cmd");
			exit(EXIT_FAILURE);
		}
	}
	while (shell->env[i])
	{
		str = ft_strjoin(str, shell->env[i]);
		str = ft_strjoin(str, "\n");
		i++;
	}
	write(shell->pipe_fd[0], str, ft_strlen(str));
	return (SUCCESS);
}
