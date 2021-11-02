/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:06:03 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/02 16:09:59 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cd .. || cd ../dir

int	ft_cd(t_shell *shell)
{
	int	cd;

	if (shell->arg[1][0] == '/')
	{
		cd = chdir(shell->arg[1]);
		if (cd == -1)
			ft_error(shell, "Error No such file or directory");
	}
	else
	{
		shell->arg[1] = ft_strjoin("/", shell->arg[1]);
		shell->arg[1] = ft_strjoin(getenv("PWD"), shell->arg[1]);
		cd = chdir(shell->arg[1]);
		if (cd == -1)
			ft_error(shell, "Error No such file or directory");
	}
	return (SUCCESS);
}
