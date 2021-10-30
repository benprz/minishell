/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/30 19:28:28 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_redi_in(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
	{
		shell->redi_in = 1;
		shell->position++;
		shell->fd_in = open(shell->sp_prompt[shell->position], O_RDWR);
		if (shell->fd_in == -1)
			perror("Error infile doesn't exist");
	}
	ft_execute_cmd(shell);
}

int	parse_redi_pipe(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "|"))
		shell->pipe = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
		shell->redi_out = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
		shell->redi_in = 1;
	// else if (!ft_strcmp(shell->sp_prompt[shell->position], ">>"))
	// 	shell->redirection = 1;
	// else if (!ft_strcmp(shell->sp_prompt[shell->position], "<<"))
	// 	shell->redirection = 1;
	else
		return (ERROR);
	return (SUCCESS);
}

int	parse_command(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->save_position], "pwd"))
		return(ft_pwd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "echo"))
		return(ft_echo(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "cd"))
		return(ft_cd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "export"))
		return(ft_export(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "unset"))
		return(ft_unset(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "env"))
		return(ft_env(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->save_position], "exit"))
	// 	return(ft_exit(shell));
	else
		return (ERROR);
}

int	ft_check_options(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "echo"))
		if (shell->sp_prompt[shell->position + 1] &&
			!ft_strcmp(shell->sp_prompt[shell->position + 1], "-n"))
			return (SUCCESS);
	return (ERROR);
}
