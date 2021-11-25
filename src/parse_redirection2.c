/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez <bperez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 05:22:24 by bperez            #+#    #+#             */
/*   Updated: 2021/11/25 07:21:20 by bperez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	goto_eof(t_command *command)
{
	char	buf[1024];
	int		ret;

	ret = read(command->fd_out, buf, 1024);
	while (ret > 0)
		ret = read(command->fd_out, buf, 1024);
}

int	open_redirection_file(t_command *command, char *arg, int type)
{
	struct stat	sct_stat;

	if (type == REDIRECTION_INPUT)
		command->fd_in = open(arg, O_RDWR);
	else if (command->redirection_out == REDIRECTION_DOUTPUT)
		command->fd_out = open(arg, O_CREAT | O_RDWR, O_APPEND, S_IRWXU);
	else if (command->redirection_out == REDIRECTION_OUTPUT)
	{
		stat(arg, &sct_stat);
		if (sct_stat.st_atime != 0)
			unlink(arg);
		command->fd_out = open(arg, O_CREAT | O_RDWR, S_IRWXU);
	}
	if (command->fd_in == -1)
		return (ft_error("Error no such file or directory", ERROR));
	return (SUCCESS);
}

int	add_delimiter_to_list(t_command *command, char **split_command)
{
	int		len;
	char	**delimiters;
	int		i;

	len = ft_tablen(command->delimiters);
	delimiters = malloc(sizeof(char *) * (len + 2));
	if (delimiters)
	{
		i = 0;
		while (command->delimiters[i])
		{
			delimiters[i] = command->delimiters[i];
			i++;
		}
		free(command->delimiters);
		delimiters[i] = *split_command;
		delimiters[i + 1] = NULL;
		command->delimiters = delimiters;
		return (SUCCESS);
	}
	return (ERROR);
}
