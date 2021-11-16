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

void	free_prompt(t_shell *shell, char *prompt)
{
	shell->command_list = goto_first_command(shell->command_list);
	while (shell->command_list)
	{
		ft_free_2d((void **)shell->command_list->argv, \
					shell->command_list->argc);
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
	free(prompt);
	close(g_shell.pipe_fd[0]);
	close(g_shell.pipe_fd[1]);
	close(g_shell.pipe_fd_redi_din[0]);
	close(g_shell.pipe_fd_redi_din[1]);
}

static void	init_shell_data(char **env)
{
	int	i;

	i = 0;
	g_shell.all_path = ft_split(getenv("PATH"), ':');
	while (env[i])
		i++;
	g_shell.env = malloc(sizeof(char*) * (i + 1));
	i = 0;
	while (env[i])
	{
		g_shell.env[i] = ft_strdup(env[i]);
		i++;
	}
	g_shell.env[i] = NULL;
}

void	launch_shell()
{
	char	*prompt;

	while (1)
	{
		if (pipe(g_shell.pipe_fd) == -1)
			perror("Pipe");
		if (pipe(g_shell.pipe_fd_redi_din) == -1)
			perror("Pipe");
		prompt = readline("minishell> ");
		if (prompt == NULL || !strcmp(prompt, "exit"))
			exit_shell();
		prompt = ft_tmp(prompt, ft_strtrim(prompt));
		if (prompt)
		{
			add_history(prompt);
			if (parse_prompt(&g_shell, prompt) == SUCCESS)
			{
				g_shell.command_list = goto_first_command(g_shell.command_list);
				execute_command(&g_shell);
			}
			free_prompt(&g_shell, prompt);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	pid_t	shell_pid;
	int		shell_status;

	(void)argc;
	(void)argv;
	shell_status = 0;
	shell_pid = fork();
	if (shell_pid == -1)
		perror("Error making shell's process\n");
	else if (shell_pid == 0)
	{
		init_shell_signals();
		init_shell_data(env);
		launch_shell();
	}
	else
	{
		init_program_signals();
		wait(&shell_status);
	}
	return (shell_status);
}
/*
int main(int argc, char **argv, char **env)
{
	pid_t	pid;
	int		status;
	char	c;

	init_shell_data(env);
	g_shell.arg = argv + 1;
	pid = fork();
	if (pid == 0)
	{
		g_shell.fd_in = open("a", O_RDWR);
		dup2(1, g_shell.fd_in);
		execve(ft_get_path(&g_shell), g_shell.arg, env);
	}
	else
	{
		wait(&status);
		while (read(g_shell.fd_in, &c, 1) == 1)
		{
			printf("%c\n", c);
		}
		close(g_shell.fd_in);
	}
	return (0);
}
*/