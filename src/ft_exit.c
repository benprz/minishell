/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 09:03:31 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/25 09:08:28 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *shell)
{
	int		nbr_exit;

	if (shell->command_list->argv[1])
	{
		nbr_exit = ft_atoi(shell->command_list->argv[1]);
		if (nbr_exit == 0 && ft_strlen(shell->command_list->argv[1]) != 1)
		{
			write(1, "exit\n", 6);
			printf("bash : exit: numeric argument required\n");
			exit(EXIT_FAILURE);
		}
		else if (ft_tablen(shell->command_list->argv) > 2)
			printf("bash : exit: too many arguments\n");
		else
		{
			write(1, "exit\n", 6);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		write(1, "exit\n", 6);
		exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}
