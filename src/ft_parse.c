/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/29 15:15:41 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_redi_pipe(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "|"))
		shell->pipe = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
		shell->redirection = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], ">>"))
		shell->redirection = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
		shell->redirection = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], "<<"))
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
	if (!ft_strcmp(shell->sp_prompt[shell->position], "cd"))
		return(ft_cd(shell));
	if (!ft_strcmp(shell->sp_prompt[shell->position], "export"))
		return(ft_export(shell));
	if (!ft_strcmp(shell->sp_prompt[shell->position], "unset"))
		return(ft_unset(shell));
	if (!ft_strcmp(shell->sp_prompt[shell->position], "env"))
		return(ft_env(shell));
	// if (!ft_strcmp(shell->sp_prompt[shell->position], "exit"))
	// 	return(ft_exit(shell));
	if (shell->sp_prompt[shell->position] == NULL)
		return (SUCCESS);
	else
		return (ERROR);
}

int	ft_check_options(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "echo"))
		if (!ft_strcmp(shell->sp_prompt[shell->position + 1], "-n"))
			return (SUCCESS);
	return (ERROR);
}
