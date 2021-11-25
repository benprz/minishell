/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:12:00 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/25 18:12:40 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_new_env(t_shell *shell, char **new_env)
{
	int	i;

	i = -1;
	while (new_env[++i])
		shell->env[i] = new_env[i];
	shell->env[i] = NULL;
}

static void	remove_line_in_env(t_shell *shell, int index)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = -1;
	new_env = malloc(sizeof(char *) * (ft_tablen(shell->env)));
	if (!new_env)
		return ;
	while (shell->env[++j])
	{
		if (j != index)
		{
			new_env[i] = ft_strdup(shell->env[j]);
			i++;
		}
	}
	new_env[i] = NULL;
	free(shell->env);
	shell->env = malloc(sizeof(char *) * (ft_tablen(new_env) + 1));
	if (!shell->env)
		return ;
	set_new_env(shell, new_env);
	free(new_env);
}

static int	check_where(t_shell *shell, char *value)
{
	int		i;
	char	**tab;

	tab = ft_split(value, '=');
	i = get_current_env_int(shell, tab[0]);
	free_tab(tab);
	return (i);
}

static int	check_value(char *value)
{
	int	i;

	i = 0;
	if (ft_isdigit(value[0]))
		return (0);
	while (value[i])
	{
		if (!ft_isalnum(value[i]) && value[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(t_shell *shell)
{
	int		i;
	int		index;
	char	*value;

	i = 1;
	while (shell->command_list->argv[i])
	{
		value = shell->command_list->argv[i];
		if (!check_value(value))
			printf("Error export not a valid identifier\n");
		index = check_where(shell, value);
		if (index != -1)
			remove_line_in_env(shell, index);
		i++;
	}
	return (SUCCESS);
}
