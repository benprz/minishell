/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:20:06 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/03 17:58:59 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_length_new_cmd(char *command)
{
	int	i;
	int	new_command_length;

	i = 0;
	new_command_length = ft_strlen(command);
	while (command[i])
	{
		if (command[i] == '|')
			new_command_length += 2;
		i++;
	}
	return (new_command_length);
}

char	*add_spaces_to_pipes(char *command)
{
	int		new_command_length;
	char	*new_command;
	int		i;
	int		j;

	new_command_length = get_length_new_cmd(command);
	new_command = malloc(sizeof(char) * new_command_length + 1);
	i = -1;
	j = 0;
	while (command[++i])
	{
		if (command[i] == '|')
		{
			new_command[j] = ' ';
			new_command[j + 1] = '|';
			new_command[j + 2] = ' ';
			j += 2;
		}
		else
			new_command[j] = command[i];
		j++;
	}
	new_command[new_command_length] = '\0';
	free(command);
	return (new_command);
}
