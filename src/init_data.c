/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:51:17 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/25 18:07:10 by bperez           ###   ########lyon.fr   */
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
		else if (g_process_section == 1)
			g_process_section = 0;
	}
}

void	init_signals(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
}

void	init_shell(t_shell *shell, char **env)
{
	int	i;

	init_signals();
	ft_bzero(shell, sizeof(t_shell));
	shell->env = malloc(sizeof(char *) * (ft_tablen(env) + 1));
	if (!shell->env)
		return ;
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		i++;
	}
	shell->env[i] = NULL;
}

int	init_pipe(t_shell *shell)
{
	if (pipe(shell->pipe_fd[0]) == -1)
		return (ERROR);
	if (pipe(shell->pipe_fd[1]) == -1)
		return (ERROR);
	if (pipe(shell->pipe_fd[2]) == -1)
		return (ERROR);
	return (SUCCESS);
}
