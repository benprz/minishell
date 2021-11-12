/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:11:30 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/12 00:08:00 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change_value(t_shell *shell, char *value, int index)
{
	free(shell->env[index]);
	shell->env[index] = ft_strdup(value);
	return (SUCCESS);
}

static int	check_already_here(t_shell *shell, char *value)
{
	int	i;
	int	j;

	i = 0;
	while (shell->env[i])
	{
		j = 0;
		while (shell->env[i][j] && value[j] && shell->env[i][j] == value[j])
		{
			if (shell->env[i][j] == '=')
				break ;
			j++;
		}
		if (value[j] == '=')
			return (change_value(shell, value, i));
		i++;
	}
	return (ERROR);
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
			return (ft_error("Error export not a valid identifier", EXIT_CMD));
		if (shell->command_list->argv[i][0] == '=')
			return (ft_error("Error export not a valid identifier", EXIT_CMD));
		if (check_already_here(shell, shell->command_list->argv[i]) == ERROR)
			change_env(shell, i);
		i++;
	}
	return (SUCCESS);
}
