/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:10:56 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/02 16:25:55 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell)
{
	int		i;
	char	*str = NULL;

	i = 0;
	if (!shell->sp_prompt[shell->position])
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error(shell, "Error dup2 cmd");
	while (shell->env[i])
	{
		str = ft_strjoin(str, shell->env[i]);
		str = ft_strjoin(str, "\n");
		i++;
	}
	printf("test\n");
	write(shell->pipe_fd[0], str, ft_strlen(str));
	return (SUCCESS);
}
