/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_change_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:36:44 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/19 10:40:33 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_line_pwd(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'P' && shell->env[i][1] == 'W')
			break ;
		i++;
	}
	return (i);
}

void	change_env_cd(t_shell *shell)
{
	int		i;
	char	*save_pwd;

	i = get_line_pwd(shell);
	save_pwd = ft_strdup(shell->env[i]);
	free(shell->env[i]);
	shell->env[i] = NULL;
	shell->env[i] = ft_strjoin(shell->env[i], "PWD=");
	shell->env[i] = ft_strjoin(shell->env[i], shell->command_list->argv[1]);
	i = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'O' && shell->env[i][1] == 'L')
			break ;
		i++;
	}
	free(shell->env[i]);
	shell->env[i] = NULL;
	shell->env[i] = ft_strjoin(shell->env[i], "OLD");
	shell->env[i] = ft_strjoin(shell->env[i], save_pwd);
	free(save_pwd);
}
