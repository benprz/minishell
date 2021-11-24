/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neben <neben@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:41:31 by bperez            #+#    #+#             */
/*   Updated: 2021/11/24 14:45:19 by bperez           ###   ########lyon.fr   */
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

#include <stdio.h>

void	exit_shell(void)
{
	write(1, "exit\n", 6);
	exit(EXIT_FAILURE);
}

void	close_pipe(t_shell *shell)
{
	if (shell->pipe_fd[0][0])
		close(shell->pipe_fd[0][0]);
	if (shell->pipe_fd[0][1])
		close(shell->pipe_fd[0][1]);
	if (shell->pipe_fd[1][0])
		close(shell->pipe_fd[1][0]);
	if (shell->pipe_fd[1][1])
		close(shell->pipe_fd[1][1]);
	if (shell->pipe_fd[2][0])
		close(shell->pipe_fd[2][0]);
	if (shell->pipe_fd[2][1])
		close(shell->pipe_fd[2][1]);
}

void	free_prompt(t_shell *shell, char *prompt)
{
	shell->command_list = goto_first_command(shell->command_list);
	while (shell->command_list)
	{
		free(shell->command_list->program_path);
		ft_free_2d((void **)shell->command_list->argv, \
					shell->command_list->argc);
		if (shell->command_list->delimiters)
			ft_free_2d((void **)shell->command_list->delimiters, \
						ft_tablen(shell->command_list->delimiters));
		if (shell->command_list->next)
		{
			shell->command_list = shell->command_list->next;
			free(shell->command_list->prev);
		}
		else
		{
			free(shell->command_list);
			shell->command_list = NULL;
		}
	}
	// free(shell->pipe_fd);
	close_pipe(shell);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*prompt;

	init_shell(&shell, env);
	while (1)
	{
		printf("process_section=%d\n", g_process_section);
		prompt = readline("minishell> ");
		if (prompt == NULL || !ft_strcmp(prompt, "exit"))
			exit_shell();
		prompt = ft_tmp(prompt, ft_strtrim(prompt));
		if (prompt)
		{
			add_history(prompt);
			if (parse_prompt(&shell, prompt) == SUCCESS)
			{
				if (init_pipe(&shell) == ERROR)
					perror("Error pipe creation");
				shell.command_list = goto_first_command(shell.command_list);
				execute_command(&shell);
			}
			free_prompt(&shell, prompt);
		}
	}
}
