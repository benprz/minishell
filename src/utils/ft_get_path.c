/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:38:18 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/28 17:30:38 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_path(t_shell *shell)
{
	int		i;
	char	*save;

	i = 0;
	while (shell->all_path[i])
	{
		save = ft_strjoin(shell->all_path[i], "/");
		save = ft_strjoin(save, shell->sp_prompt[shell->save_position]);
		if (access(save, X_OK) == 0)
			return (save);
		free(save);
		i++;
	}
	return (NULL);
}
