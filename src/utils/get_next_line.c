/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: bperez <bperez@student.le-101.fr>          +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/20 18:12:51 by bperez       #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 15:40:47 by bperez      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "get_next_line.h"
#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

static char	*update_line(char *line, char *buffer)
{
	char	*tmp;

	tmp = malloc(ft_strlen(line) + ft_strclen(buffer, DELIM) + 1);
	if (tmp)
		*((char *)ft_strccpy(ft_strcpy(tmp, line), buffer, DELIM)) = '\0';
	free(line);
	return (tmp);
}

int	get_next_line(int fd, char **line, char *remainder, long read_length)
{
	static char	buffer[BUFFER_SIZE + 1];

	*line = NULL;
	while (1)
	{
		*line = update_line(*line, buffer);
		if (!line)
			break ;
		remainder = ft_strchr(buffer, DELIM);
		if (!remainder)
		{
			read_length = read(fd, buffer, BUFFER_SIZE);
			if (read_length < 0)
				break ;
			if (read_length == 0 && buffer[0] == '\0')
				return (END_READ);
			buffer[read_length] = '\0';
		}
		else
		{
			ft_strcpy(buffer, remainder + 1);
			return (END_LINE);
		}
	}
	return (-1);
}
