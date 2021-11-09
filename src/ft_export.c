/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:11:30 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/09 20:07:26 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// doit gerer si la valeur existe deja
// voir pour malloc env au debut

static void	change_env(t_shell *shell, int i)
{
	char	**save_env;
	int		j;

	j = 0;
	printf("test2\n");
	save_env = NULL;
	printf("test3\n");
	while (shell->env[j])
	{
		save_env[j] = ft_strdup(shell->env[j]);
		j++;
	}
	printf("test4\n");
	free_tab(shell->env);
	shell->env = malloc(sizeof(char) * (j + 1));
	j = 0;
	printf("test5\n");
	while (save_env[j])
	{
		shell->env[j] = ft_strdup(save_env[j]);
		j++;
	}
	printf("test6\n");
	shell->env[j] = ft_strdup(shell->command_list->argv[i]);
	free_tab(save_env);
	printf("test7\n");
}

int	ft_export(t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	if (!shell->command_list->next)
	{
		while (shell->command_list->argv[i])
		{
			j = 0;
			while (shell->command_list->argv[i][j])
			{
				if (shell->command_list->argv[i][j] == ' ')
					ft_error("Error export not a valid identifier");
				j++;
			}
			if (shell->command_list->argv[i][0] == '=')
				ft_error("Error export not a valid identifier");
			change_env(shell, i);
			printf("test1\n");
		}
	}
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	return (SUCCESS);
}
