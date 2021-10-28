/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/28 17:51:40 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_redi_pipe(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "|"))
		shell->pipe = 1;
	if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
		shell->redirection = 1;
	if (!ft_strcmp(shell->sp_prompt[shell->position], ">>"))
		shell->redirection = 1;
	if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
		shell->redirection = 1;
	if (!ft_strcmp(shell->sp_prompt[shell->position], "<<"))
		shell->redirection = 1;
	else
		return (ERROR);
	return (SUCCESS);
}

int	parse_command(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "pwd"))
		return(ft_pwd(shell));
	if (!ft_strcmp(shell->sp_prompt[shell->position], "echo"))
		return(ft_echo(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "cd"))
	// 	return(ft_echo(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "export"))
	// 	return(ft_echo(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "unset"))
	// 	return(ft_echo(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "env"))
	// 	return(ft_echo(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "exit"))
	// 	return(ft_echo(shell));
	if (shell->sp_prompt[shell->position] == NULL)
		return (SUCCESS);
	else
		return (ERROR);
	return (SUCCESS);
}
