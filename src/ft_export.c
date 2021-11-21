/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:11:30 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/21 15:59:06 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_already_here(t_shell *shell, char *value)
{
	int		i;
	char	**tab;

	tab = ft_split(value, '=');
	i = get_current_env(shell, tab[0]);
	free_tab(tab);
	if (i == -1)
		return (ERROR);
	else
	{
		free(shell->env[i]);
		shell->env[i] = ft_strdup(value);
		return (SUCCESS);
	}
}

static void	change_env(t_shell *shell, int i)
{
	char	**save_env;
	int		j;

	save_env = malloc(sizeof(char *) * (ft_tablen(shell->env) + 1));
	j = 0;
	while (shell->env[j])
	{
		save_env[j] = ft_strdup(shell->env[j]);
		j++;
	}
	save_env[j] = NULL;
	free_tab(shell->env);
	shell->env = malloc(sizeof(char *) * (ft_tablen(save_env) + 2));
	j = 0;
	while (save_env[j])
	{
		shell->env[j] = ft_strdup(save_env[j]);
		j++;
	}
	shell->env[j] = ft_strdup(shell->command_list->argv[i]);
	shell->env[j + 1] = NULL;
	free_tab(save_env);
}

int	ft_export(t_shell *shell)
{
	int	i;
	int	j;
	int	check;

	i = 1;
	if (shell->command_list->next)
		return (EXIT_CMD);
	while (shell->command_list->argv[i])
	{
		j = -1;
		check = 0;
		while (shell->command_list->argv[i][++j])
			if (shell->command_list->argv[i][j] == '=')
				check++;
		if (check == 0)
			return (EXIT_CMD);
		if (check_already_here(shell, shell->command_list->argv[i]) == ERROR)
			change_env(shell, i);
		i++;
	}
	return (SUCCESS);
}
