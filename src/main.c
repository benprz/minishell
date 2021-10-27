/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neben <neben@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:41:31 by bperez            #+#    #+#             */
/*   Updated: 2021/10/26 18:13:36 by neben            ###   ########lyon.fr   */
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

#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_history
{
	char				*command_line;
	struct s_history	*next;
	struct s_history	*prev;
}	t_history;

typedef struct s_shell
{
	t_history	*commands_history;
}	t_shell;

int	main(void)
{
	t_shell	shell;
	char	*prompt;

	while (1)
	{
		prompt = readline("minishell> ");
		add_history(prompt);
		free(prompt);
	}
	return (0);
}