/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:51:17 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/20 00:24:03 by ben              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		if (g_process_section == 0)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (g_process_section == 2)
		{
			exit(EXIT_FAILURE);
		}
	}
}

void	init_signals()
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
}

void	init_shell(t_shell *shell, char **env)
{
	int	i;

	init_signals();
	ft_bzero(shell, sizeof(t_shell));
	shell->all_path = ft_split(getenv("PATH"), ':');
	shell->env = malloc(sizeof(char *) * (ft_tablen(env) + 1));
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		i++;
	}
	shell->env[i] = NULL;
}
