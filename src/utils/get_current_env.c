/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_current_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 14:57:24 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/21 17:14:08 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_current_env_int(t_shell *shell, char *str)
{
	int		index;
	char	**tab;

	index = 0;
	while (shell->env[index])
	{
		tab = ft_split(shell->env[index], '=');
		if (!ft_strcmp(tab[0], str))
		{
			free_tab(tab);
			return (index);
		}
		free_tab(tab);
		index++;
	}
	return (-1);
}
