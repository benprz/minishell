/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <ngeschwi@stutent.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 09:03:31 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/29 10:59:34 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void exit_else(t_shell *shell)
{
	write(1, "exit\n", 6);
	exit(shell->last_exit_status);
}

int	ft_exit(t_shell *shell)
{
	int		nbr_exit;

	if (shell->command_list->argv[1]
		|| ft_strlen(shell->command_list->argv[1]) == 0)
	{
		nbr_exit = ft_atoi(shell->command_list->argv[1]);
		if ((nbr_exit == 0 && ft_strlen(shell->command_list->argv[1]) != 1)
			|| (nbr_exit > 2147483647 || nbr_exit < -2147483648))
		{
			write(1, "exit\n", 6);
			printf("bash : exit: numeric argument required\n");
			exit(255);
		}
		else if (ft_tablen(shell->command_list->argv) > 2)
			printf("bash : exit: too many arguments\n");
		else
		{
			write(1, "exit\n", 6);
			exit(nbr_exit);
		}
	}
	else
		exit_else(shell);
	return (SUCCESS);
}
