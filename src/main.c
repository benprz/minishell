/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:41:31 by bperez            #+#    #+#             */
/*   Updated: 2021/10/29 15:23:01 by ngeschwi         ###   ########.fr       */
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

void	exit_shell(void)
{
	g_shell.launched = 0;
	write(1, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

void	break_current_loops(void)
{
	//write(1, "\n", 1);
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
}

void	init_program_signals(void)
{
	signal(SIGINT, NULL);
}

static void	ft_free_prompt(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->sp_prompt[i])
	{
		free(shell->sp_prompt[i]);
		i++;
	}
	free(shell->sp_prompt);
	free(shell->prompt);
}

static void	ft_init_struct()
{
	g_shell.position = 0;
	g_shell.pipe = 0;
	g_shell.redirection = 0;
	g_shell.all_path = ft_split(getenv("PATH"), ':');
	g_shell.pipe_fd[0] = 0;
	g_shell.pipe_fd[1] = 0;
	if (pipe(g_shell.pipe_fd) == -1)
		perror("Pipe");
	g_shell.sp_prompt = ft_split(g_shell.prompt, ' ');
}

void	launch_shell()
{
	g_shell.launched = 1;
	init_shell_signals();
	while (1)
	{
		g_shell.prompt = readline("minishell> ");
		ft_init_struct();
		if (g_shell.prompt == NULL || !strcmp(g_shell.prompt, "exit"))
			exit_shell();
		add_history(g_shell.prompt);
		if (parse_command(&g_shell) == ERROR)
			perror("Error command not found");
		ft_free_prompt(&g_shell);
	}
}

int	main(int argc, char **argv, char **env)
{
	pid_t	shell_pid;
	int		shell_status;

	g_shell.env = env;
	shell_pid = fork();
	if (shell_pid == -1)
		printf("Error making shell's process\n");
	else if (shell_pid == 0)
		launch_shell();
	else
	{
		init_program_signals();
		wait(&shell_status);
	}
	return (shell_status);
}
