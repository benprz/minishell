/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:41:31 by bperez            #+#    #+#             */
/*   Updated: 2021/11/01 19:36:02 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history,
// printf, malloc, free, write, access, open, read,
// close, fork, wait, waitpid, wait3, wait4, signal,
// sigaction, kill, exit, getcwd, chdir, stat,
// lstat, fstat, unlink, execve, dup, dup2, pipe,
// opendir, readdir, closedir, strerror, perror,
// isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr,
// tcgetattr, tgetent, tgetflag, tgetnum, tgetstr,
// tgoto, tputs

#include "minishell.h"

t_shell	g_shell;

void	handle_program_signals(int signal)
{
}

void	init_program_signals(void)
{
	int i = 0;
	
	while (++i <= 31)
	{
		signal(i, handle_program_signals);
	}
}

void	exit_shell(void)
{
	write(1, "exit\n", 6);
	exit(EXIT_SUCCESS);
}

void	break_current_loops(void)
{
	/*
	write(0, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	*/
}

void	handle_shell_signals(int signal)
{
	if (signal == SIGINT)
		break_current_loops();
}

void	init_shell_signals(void)
{
	signal(SIGINT, handle_shell_signals);
	signal(SIGQUIT, handle_shell_signals);
}

static void	ft_free_prompt(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->sp_prompt)
	{
		while (shell->sp_prompt[i])
		{
			free(shell->sp_prompt[i]);
			i++;
		}
		free(shell->sp_prompt);
	}
	free(shell->prompt);
}

static void	init_shell_data()
{
	if (pipe(g_shell.pipe_fd) == -1)
		perror("Pipe");
	g_shell.all_path = ft_split(getenv("PATH"), ':');
}

void	launch_shell()
{
	bzero(&g_shell, sizeof(g_shell));
	init_shell_data();
	init_shell_signals();
	while (1)
	{
		g_shell.prompt = readline("minishell> ");
		if (g_shell.prompt == NULL || !strcmp(g_shell.prompt, "exit"))
			exit_shell();
		if (ft_strlen(g_shell.prompt) != 0)
		{
			add_history(g_shell.prompt);
			check_redi_in(&g_shell);
		}
		ft_free_prompt(&g_shell);
	}
}

int	main(int argc, char **argv, char **env)
{
	pid_t	shell_pid;
	int		shell_status;

	shell_pid = fork();
	if (shell_pid == -1)
		printf("Error making shell's process\n");
	else if (shell_pid == 0)
	{
		g_shell.env = env;
		launch_shell();
	}
	else
	{
		init_program_signals();
		wait(&shell_status);
	}
	return (shell_status);
}
