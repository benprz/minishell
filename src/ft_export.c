/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:11:30 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/10 13:28:04 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// doit gerer si la valeur existe deja

static void	change_env(t_shell *shell, int i)
{
	char	**save_env;
	int		j;

	j = 0;
	printf("tets\n");
	while (shell->env[j])
		j++;
	save_env = malloc(sizeof(char*) * (j + 1));
	j = 0;
	while (shell->env[j])
	{
		save_env[j] = ft_strdup(shell->env[j]);
		j++;
	}
	save_env[j] = NULL;
	free_tab(shell->env);
	shell->env = malloc(sizeof(char) * (j + 2));
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
	check = 0;
	if (shell->command_list->next)
		return (SUCCESS);
	while (shell->command_list->argv[i])
	{
		j = 0;
		while (shell->command_list->argv[i][j])
		{
			if (shell->command_list->argv[i][j] == '=')
				check++;
			j++;
		}
		if (check == 0)
		{
			perror("Error export not a valid identifier");
			return (SUCCESS);
		}
		if (shell->command_list->argv[i][0] == '=')
		{
			perror("Error export not a valid identifier");
			return (SUCCESS);
		}
		change_env(shell, i);
		i++;
	}
	return (SUCCESS);
}
