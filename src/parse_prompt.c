/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neben <neben@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 23:00:13 by bperez            #+#    #+#             */
/*   Updated: 2021/11/08 04:40:15 by neben            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*** Nouveau système de parsing
*** en cours de création
*/

t_command	*goto_first_command(t_command *current)
{
	if (current)
		while (current->prev)
			current = current->prev;
	return (current);
}

void	print_commands(t_shell *shell)
{
	t_command *current;
	int	i;

	current = goto_first_command(shell->command_list);
	while (current)
	{
		printf("\ncurrent = %p\ncurrent->prev = %p\ncurrent->next = %p\n", current, current->prev, current->next);
		i = 0;
		while (current->argv[i])
		{
			printf("current->argv[%d] = %s\n", i, current->argv[i]);
			i++;
		}
		printf("current->argv[%d] = %s\n", i, current->argv[i]);
		current = current->next;
	}
}

void	check_quotes(char c, int *quote, int *double_quote)
{
	if (c == '"' && *quote == 0)
	{
		if (*double_quote == 0)
			*double_quote = 1;
		else
			*double_quote = 0;
	}	
	else if (c == '\'' && *double_quote == 0)
	{
		if (*quote == 0)
			*quote = 1;
		else
			*quote = 0;
	}
}

void	replace_split_spaces(char *command)
{
	int		quote;
	int		double_quote;	
	
	quote = 0;
	double_quote = 0;
	while (*command)
	{
		check_quotes(*command, &quote, &double_quote);
		if (*command == ' ' && quote == 0 && double_quote == 0)
		{
			*command = 1;
			// break ; // si on veut split la commande en deux parties : programme et paramètres
		}
		command++;
	}
}

int get_variable_name_length(char *command, int i)
{
	int	var_name_length;

	var_name_length = 0;
	while (command[i + var_name_length])
	{
		if (!ft_isalnum(command[i + var_name_length]))
			break ;
		var_name_length++;
	}
	return (var_name_length);
}

int	expand_env_variable(char **split_command, int i)
{
	char	*var;
	int		var_name_length;
	char	*new_command;

	var_name_length = get_variable_name_length(*split_command, i);
	var = ft_substr(*split_command, i, var_name_length);
	if (var)
	{
		var = ft_tmp(var, getenv(var));
		new_command = ft_substr(*split_command, 0, i - 1);
		new_command = ft_tmp(new_command, ft_strjoin(new_command, var));
		i += var_name_length;
		var = ft_substr(*split_command, i, ft_strlen(*split_command + i));
		if (var)
		{
			new_command = ft_tmp(new_command, ft_strjoin(new_command, var));
			if (new_command)
			{
				free(*split_command);
				*split_command = new_command;
				return (SUCCESS);
			}
		}
	}
	return (ERROR);
}

/*
void	remove_interpreted_quotes(char *split_command)
{
	int	i;
	int	quote;
	int	double_quote;

	quote = 0;
	double_quote = 0;
}
*/

int	parse_argv(t_command *current_command, char **split_command)
{
	int	i;
	int	quote;
	int	double_quote;

	quote = 0;
	double_quote = 0;
	while (*split_command)
	{
		i = 0;
		while ((*split_command)[i])
		{
			check_quotes((*split_command)[i], &quote, &double_quote);
			if ((*split_command)[i] == '$' && quote == 0)
			{
				if (expand_env_variable(split_command, i + 1) == ERROR)
					return (ERROR);
			}
			i++;
		}
		remove_interpreted_quotes(*split_command);
		printf("%s\n", *split_command);
		split_command++;
	}
	return (SUCCESS);
}

int	parse_command(t_command *current_command, char *command)
{
	char	**split_command;

	while (*command == ' ')
		command++;
	replace_split_spaces(command);
	split_command = ft_split(command, 1);
	if (split_command)
	{
		if (parse_argv(current_command, split_command) == SUCCESS)
		{
			current_command->argv = split_command;
			return (SUCCESS);
		}
		ft_free_2d((void **)split_command, ft_strlen_2d(split_command));
	}
	return (ERROR);
}

int	add_command(t_shell *shell, char *command)
{
	t_command	*current_command;
	
	//printf("command = %s\n", command);
	current_command = malloc(sizeof(t_command));
	if (current_command)
	{
		ft_bzero(current_command, sizeof(t_command));
		if (shell->command_list != NULL)
		{
			current_command->prev = shell->command_list;
			current_command->prev->next = current_command;
		}
		shell->command_list = current_command;
		if (parse_command(current_command, command) == SUCCESS)
		{
			free(command);
			return (SUCCESS);
		}
	}
	free(command);
	return (ERROR);
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
			check_quotes(prompt[i], &quote, &double_quote);
			if (prompt[i] == '\0' || (prompt[i] == '|' && quote == 0 && double_quote == 0))
			{
				if (add_command(shell, ft_substr(prompt, 0, i)) == ERROR)
					return (ERROR);
				prompt += i + 1;
				i = -1;
			}
			i++;
		}
		//print_commands(shell);
	}
	return (SUCCESS);
}