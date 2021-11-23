/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez <bperez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 12:31:20 by bperez            #+#    #+#             */
/*   Updated: 2021/11/23 12:31:23 by bperez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "minishell.h"

#include <fcntl.h>
#include <stdlib.h>

void	init_prompt_history(void)
{
	char	*line;
	int		fd;
	int		ret;

	line = NULL;
	fd = open(".minishell_history", O_RDONLY);
	if (fd > 0)
	{
		while (1)
		{
			ret = get_next_line(fd, &line, NULL, 0);
			if (ret == 1)
			{
				add_history(line);
				free(line);
			}
			else if (ret == 0)
			{
				free(line);
				break ;
			}
		}
	}
}

void	add_prompt_to_history(char *prompt)
{
	int	fd;

	fd = open(".minishell_history", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd > 0)
	{
		add_history(prompt);
		write(fd, prompt, ft_strlen(prompt));
		write(fd, "\n", 1);
	}
}