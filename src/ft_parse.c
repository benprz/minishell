/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 19:38:26 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/09 19:49:20 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmmd(t_shell *shell)
{
	if (!ft_strcmp(shell->command_list->argv[0], "echo"))
		return (ft_echo(shell));
	if (!ft_strcmp(shell->command_list->argv[0], "cd"))
		return (ft_cd(shell));
	if (!ft_strcmp(shell->command_list->argv[0], "export"))
		return (ft_export(shell));
	if (!ft_strcmp(shell->command_list->argv[0], "unset"))
		return (ft_unset(shell));
	if (!ft_strcmp(shell->command_list->argv[0], "pwd"))
		return (ft_pwd(shell));
	if (!ft_strcmp(shell->command_list->argv[0], "env"))
		return (ft_env(shell));
	return (ERROR);
}
