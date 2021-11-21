/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:12:00 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/21 16:00:48 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_line_in_env(t_shell *shell, int index)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	new_env = malloc(sizeof(char *) * (ft_tablen(shell->env)));
	while (shell->env[j])
	{
		if (j != index)
		{
			new_env[i] = ft_strdup(shell->env[j]);
			i++;
		}
		j++;
	}
	new_env[i] = NULL;
	free(shell->env);
	shell->env = malloc(sizeof(char *) * (ft_tablen(new_env) + 1));
	i = -1;
	while (new_env[++i])
		shell->env[i] = new_env[i];
	shell->env[i] = NULL;
	free(new_env);
}

static int	check_where(t_shell *shell, char *value)
{
	int		i;
	char	**tab;

	tab = ft_split(value, '=');
	i = get_current_env(shell, tab[0]);
	free_tab(tab);
	return (i);
}

int	ft_unset(t_shell *shell)
{
	int		index;
	char	*value;

	value = shell->command_list->argv[1];
	index = check_where(shell, value);
	if (index == -1)
		return (EXIT_CMD);
	remove_line_in_env(shell, index);
	return (SUCCESS);
}
