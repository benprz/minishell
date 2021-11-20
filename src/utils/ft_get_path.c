/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:38:18 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/20 17:05:33 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_path(t_shell *shell)
{
	int		i;
	char	*save;

	i = 0;
			int a;
	while (shell->all_path[i])
	{
		save = ft_strjoin(shell->all_path[i], "/");
		save = ft_strjoin(save, shell->command_list->argv[0]);
		if ((a = access(save, X_OK)) == 0)
			return (save);
		free(save);
		i++;
	}
	printf("access=%d\n", a);
	return (NULL);
}
