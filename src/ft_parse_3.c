/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:20:33 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/03 22:00:38 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_quotes_on_cmd(t_shell *shell)
{
	int	i;
	int	count_space;

	i = 0;
	count_space = 0;
	if (shell->prompt[0] == '\'')
	{
		while (shell->prompt[i] && shell->prompt[i] != '\'')
		{
			if (shell->prompt[i] == ' ')
				count_space++;
			i++;
		}
	}
}