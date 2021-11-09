/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neben <neben@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:41:31 by bperez            #+#    #+#             */
/*   Updated: 2021/11/08 00:35:58by neben            ###   ########lyon.fr   */
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
	int	i;

	i = 0;
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
	exit_shell();
	// write(0, "\n", 1);
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_redisplay();
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

static void	free_prompt(t_shell *shell)
{
	shell->command_list = goto_first_command(shell->command_list);
	while (shell->command_list)
	{
		if (shell->command_list->next)
		{
			shell->command_list = shell->command_list->next;
			//printf("free(%p)\n", shell->command_list->prev);
			free(shell->command_list->prev);
		}
		else
		{
			//printf("(end) free(%p)\n", shell->command_list);
			free(shell->command_list);
			shell->command_list = NULL;
		}
	}
	free(shell->prompt);
}

static void	init_shell_data(char **env)
{
	if (pipe(g_shell.pipe_fd) == -1)
		perror("Pipe");
	g_shell.all_path = ft_split(getenv("PATH"), ':');
	g_shell.env = env;
}


void	launch_shell(char **env)
{
	init_shell_signals();
	while (1)
	{
		ft_bzero(&g_shell, sizeof(g_shell));
		init_shell_data(env);
		g_shell.prompt = readline("minishell> ");
		if (g_shell.prompt == NULL || !strcmp(g_shell.prompt, "exit"))
			exit_shell();
		g_shell.prompt = ft_tmp(g_shell.prompt, ft_strtrim(g_shell.prompt));
		if (g_shell.prompt)
		{
			add_history(g_shell.prompt);
			if (parse_prompt(&g_shell, g_shell.prompt) == SUCCESS)
			{
				g_shell.command_list = goto_first_command(g_shell.command_list);
				execute_command(&g_shell);
			}
		}
		//free_prompt(&g_shell);
	}
}

int	main(int argc, char **argv, char **env)
{
	pid_t	shell_pid;
	int		shell_status;

	shell_pid = fork();
	if (shell_pid == -1)
		perror("Error making shell's process\n");
	else if (shell_pid == 0)
		launch_shell(env);
	else
	{
		init_program_signals();
		wait(&shell_status);
	}
	return (shell_status);
}

/*
int main(void)
{
	parse_prompt(&g_shell, "echo world");
	return (0);
}
*/