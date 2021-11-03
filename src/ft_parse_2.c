/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:20:06 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/03 21:35:44 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pipe_in_quote(char *command, int i, int check, int check_d)
{
	if ((command[i] == '|' || command[i] == '>')
		&& (check == 1 || check_d == 1))
	{
		if (command[i - 1] && command[i + 1])
			if (command[i - 1] == ' ' && command[i + 1] == ' ')
				return (1);
	}
	return (0);
}

static void	ft_check_quote(char str, int *check, int *check_d)
{
	if (str == '\'')
	{
		if (*check == 1)
			*check = 0;
		else if (*check_d == 0 && *check == 0)
			*check = 1;
	}
	else if (str == '"')
	{
		if (*check_d == 1)
			*check_d = 0;
		else if (*check == 0 && *check_d == 0)
			*check_d = 1;
	}
}

static int	get_length_new_cmd(char *command)
{
	int	i;
	int	new_command_length;
	int	check;
	int	check_d;

	i = 0;
	check = 0;
	check_d = 0;
	new_command_length = ft_strlen(command);
	while (command[i])
	{
		ft_check_quote(command[i], &check, &check_d);
		if ((command[i] == '|' || command[i] == '>') && (check == 0 || check_d == 0))
			new_command_length += 2;
		if (check_pipe_in_quote(command, i, check, check_d))
			new_command_length += 2;
		i++;
	}
	return (new_command_length);
}

char	*add_spaces_to_pipes(char *command)
{
	char	*new_command;
	int		i;
	int		j;
	int		check;
	int		check_d;

	new_command = malloc(sizeof(char) * get_length_new_cmd(command) + 1);
	i = 0;
	j = 0;
	check = 0;
	check_d = 0;
	while (command[i])
	{
		ft_check_quote(command[i], &check, &check_d);
		if ((command[i] == '|' || command[i] == '>') && (check == 0 && check_d == 0))
		{
			new_command[j] = ' ';
			new_command[j + 1] = command[i];
			new_command[j + 2] = ' ';
			j += 2;
		}
		else if (check_pipe_in_quote(command, i, check, check_d))
		{
			new_command[j] = '\'';
			new_command[j + 1] = command[i];
			new_command[j + 2] = '\'';
			j += 2;
		}
		else
			new_command[j] = command[i];
		j++;
		i++;
	}
	new_command[j] = '\0';
	free(command);
	return (new_command);
}
