/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 23:00:13 by bperez            #+#    #+#             */
/*   Updated: 2021/11/07 05:52:11 by bperez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*** Nouveau système de parsing
*** en cours de création
*/

int	parse_command(t_command *current_command, char *command)
{
	char	**split_command;
	int		i;

	split_command = ft_split(command, ' ');
	if (split_command)
	{
		i = 0;
		while (split_command[i])
		{
			printf("%s\n", split_command[i]);
			i++;
		}
		return (SUCCESS);
	}
	return (ERROR);
}

int	add_command(t_shell *shell, char *command)
{
	printf("command = '%s'\n", command);
	return (SUCCESS);
	/*
	t_command	*current_command;
	
	current_command = malloc(sizeof(t_command));
	if (current_command)
	{
		ft_bzero(current_command, sizeof(t_command));
		if (shell->current_command)
		{
			current_command->prev = shell->current_command;
			current_command->prev->next = current_command;
		}
		if (parse_command(current_command, command) == SUCCESS)
			return (SUCCESS);
	}
	free(command);
	return (ERROR);
	*/
}

int	parse_prompt(t_shell *shell, char *prompt)
{
	int	i;
	int	quote;
	int	double_quote;
	int	prompt_length;

	if (prompt)
	{
		i = 0;
		quote = 0;
		double_quote = 0;
		prompt_length = ft_strlen(prompt);
		while (prompt_length-- >= 0)
		{
			if (prompt[i] == '"' && quote == 0)
			{
				if (double_quote == 0)
					double_quote = 1;
				else
					double_quote = 0;
			}	
			else if (prompt[i] == '\'' && double_quote == 0)
			{
				if (quote == 0)
					quote = 1;
				else
					quote = 0;
			}
			else if (prompt[i] == '\0' || (prompt[i] == '|' && quote == 0 && double_quote == 0))
			{
				if (add_command(shell, ft_substr(prompt, 0, i)) == ERROR)
					return (ERROR);
				prompt += i;
				i = -1;
			}
			i++;
		}
	}
	return (SUCCESS);
}
