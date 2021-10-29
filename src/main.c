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

static void	ft_init_struct(t_shell *shell)
{
	shell->position = 0;
	shell->pipe = 0;
	shell->redirection = 0;
	shell->all_path = ft_split(getenv("PATH"), ':');
	shell->pipe_fd[0] = 0;
	shell->pipe_fd[1] = 0;
	if (pipe(shell->pipe_fd) == -1)
		perror("Pipe");
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	shell.env = env;
	while (1)
	{
		ft_init_struct(&shell);
		shell.prompt = readline("minishell> ");
		add_history(shell.prompt);
		shell.sp_prompt = ft_split(shell.prompt, ' ');
		if (parse_command(&shell) == ERROR)
			perror("Error command not found");
		ft_free_prompt(&shell);
	}
	return (0);
}
