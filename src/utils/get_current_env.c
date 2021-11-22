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

char	*get_current_env(t_shell *shell, char *str)
{
	int		index;
	int		index_equal;

	index = 0;
	if (*str)
	{
		while (shell->env[index])
		{
			index_equal = ft_strclen(shell->env[index], '=');
			if (!ft_strcmp(shell->env[index], str))
			{
				printf("env:%s\n", shell->env[index]);
				printf("strcmp=%d equal=%d\n", ft_strcmp(shell->env[index], str), index_equal);
				return (shell->env[index] + index_equal + 1);
			}
			index++;
		}
	}
	return ("");
}
