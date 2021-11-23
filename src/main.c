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

void	exit_shell(void)
{
	write(1, "exit", 5);
	exit(EXIT_FAILURE);
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
}

void	launch_shell(t_shell *shell)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("minishell> ");
		if (prompt == NULL || !ft_strcmp(prompt, "exit"))
			exit_shell();
		prompt = ft_tmp(prompt, ft_strtrim(prompt));
		if (prompt)
		{
			add_prompt_to_history(prompt);
			if (parse_prompt(shell, prompt) == SUCCESS)
			{
				if (pipe(shell->pipe_fd) == -1)
					perror("Pipe");
				shell->command_list = goto_first_command(shell->command_list);
				execute_command(shell);
				close(shell->pipe_fd[0]);
				close(shell->pipe_fd[1]);
			}
			free_prompt(shell, prompt);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	pid_t	shell_pid;
	int		shell_status;

	shell_status = 0;
	while (shell_status == 0)
	{
		ft_bzero(&shell, sizeof(t_shell));
		shell_pid = fork();
		if (shell_pid == -1)
			perror("Error making shell's process\n");
		else if (shell_pid == 0)
		{
			init_prompt_history();
			init_shell_signals();
			init_shell_data(&shell, env);
			launch_shell(&shell);
		}
		else
		{
			init_program_signals();
			wait(&shell_status);
		}
		write(1, "\n", 1);
	}
	return (shell.last_exit_status);
}