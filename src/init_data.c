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

void	init_shell_data(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	ft_bzero(shell, sizeof(t_shell));
	shell->all_path = ft_split(getenv("PATH"), ':');
	while (env[i])
		i++;
	shell->env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		i++;
	}
	shell->env[i] = NULL;
}

void	handle_program_signals(int signal)
{
	(void)signal;
}

void	init_program_signals(void)
{
	int	i;

	i = 0;
	while (++i <= 31)
	{
		signal(i, handle_program_signals);
	}
}

void	handle_shell_signals(int signal)
{
	if (signal == SIGINT)
		exit(SUCCESS);
}

void	init_shell_signals(void)
{
	signal(SIGINT, handle_shell_signals);
	signal(SIGQUIT, handle_shell_signals);
}
