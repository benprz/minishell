/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/27 18:02:00 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_command(t_shell *shell, char **env)
{
	shell->sp_prompt = ft_split(shell->prompt, ' ');
	if (!ft_strcmp(shell->sp_prompt[0], "pwd"))
		ft_pwd(shell, env);
	return (SUCCESS);
}
