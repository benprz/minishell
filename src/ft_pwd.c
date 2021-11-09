/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 18:01:39 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/09 19:54:25 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_shell *shell)
{
	char	*str;
	size_t	i;

	i = 1;
	str = NULL;
	if (!shell->command_list->next)
		if (dup2(1, shell->pipe_fd[0]) == -1)
			ft_error("Error dup2 cmd");
	str = ft_strdup(getenv("PWD"));
	str = ft_strjoin(str, "\n");
	write(shell->pipe_fd[0], str, ft_strlen(str));
	free(str);
	return (SUCCESS);
}
