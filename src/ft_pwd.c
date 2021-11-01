/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/01 19:25:07 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_shell *shell)
{
	char	*str = NULL;
	size_t	i;

	i = 1;
	if (!shell->sp_prompt[shell->position])
	{
		if (dup2(1, shell->pipe_fd[0]) == -1)
		{
			perror("Error dup2 cmd");
			exit(EXIT_FAILURE);
		}
	}
	str = malloc(sizeof(char) * (i + 1));
	while (getcwd(str, i) == NULL)
	{
		free(str);
		i++;
		str = malloc(sizeof(char) * (i + 1));
	}
	str = ft_strjoin(str, "\n");
	write(shell->pipe_fd[0], str, ft_strlen(str));
	return (SUCCESS);
}
