/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 23:00:13 by bperez            #+#    #+#             */
/*   Updated: 2021/11/17 18:58:40 by ngeschwi         ###   ########.fr       */
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
	t_command	*current;
	int			i;

	current = goto_first_command(shell->command_list);
	while (current)
	{
		printf("\ncurrent = %p\ncurrent->prev = %p\ncurrent->next = %p\n", current, current->prev, current->next);
		printf("current->argc = %d\ncurrent->redirection_in = %d\ncurrent->redirection_out = %d\ncurrent->fd_in = %d\ncurrent->fd_out = %d\ncurrent->delimiter = %s\n", current->argc, current->redirection_in, current->redirection_out, current->fd_in, current->fd_out, current->delimiter);
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

int	get_variable_name_length(char *command, int i)
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

int	expand_tilde(char **split_command, int i)
{
	char	*var;
	char	*new_command;

	var = getenv("HOME");
	new_command = ft_substr(*split_command, 0, i);
	new_command = ft_tmp(new_command, ft_strjoin(new_command, var));
	var = ft_substr(*split_command, i + 1, ft_strlen(*split_command + i + 1));
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
	return (ERROR);
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

void	get_redirection_type(t_command *cmd, char **split_cmd, int i)
{
	int	check_redirection;

	if ((*split_cmd)[i] == '<')
	{
		cmd->redirection_in = REDIRECTION_INPUT;
		check_redirection = strncmp(*split_cmd + i, "<<", 2);
		if (check_redirection == 0)
			cmd->redirection_in = REDIRECTION_DINPUT;
	}
	else if ((*split_cmd)[i] == '>')
	{
		cmd->redirection_out = REDIRECTION_OUTPUT;
		check_redirection = strncmp(*split_cmd + i, ">>", 2);
		if (check_redirection == 0)
			cmd->redirection_out = REDIRECTION_DOUTPUT;
	}
}

int	get_redirection_fd(t_shell *shell, t_command *cmd, char **split_cmd)
{
	if (cmd->redirection_in == REDIRECTION_INPUT)
	{
		cmd->fd_in = open(*(split_cmd + 1), O_RDWR);
		if (cmd->fd_in == -1)
			return (ft_error("Error no such file or directory", ERROR));
	}
	else if (cmd->redirection_out == REDIRECTION_OUTPUT)
	{
		stat(*(split_cmd + 1), &shell->sct_stat);
		if (shell->sct_stat.st_atime != 0)
			unlink(*(split_cmd + 1));
		cmd->fd_out = open(*(split_cmd + 1), O_CREAT | O_RDWR, S_IRWXU);
		if (cmd->fd_out == -1)
			return (ft_error("Error no such file or directory", ERROR));
	}
	else if (cmd->redirection_out == REDIRECTION_DOUTPUT)
	{
		stat(*(split_cmd + 1), &shell->sct_stat);
		if (shell->sct_stat.st_atime == 0)
		{
			cmd->fd_out = open(*(split_cmd + 1), O_CREAT | O_RDWR, S_IRWXU);
			if (cmd->fd_out == -1)
				return (ft_error("Error no such file or directory", ERROR));
		}
	}
	return (SUCCESS);
}

int	get_redirection_argument(t_shell *shell, t_command *cmd, char **split_cmd, int i)
{
	int	j;

	if (get_redirection_fd(shell, cmd, split_cmd) == ERROR)
		return (ERROR);
	cmd->delimiter = *(split_cmd + 1);
	j = 0;
	free(*split_cmd);
	//free(*(split_cmd + 1));
	while (split_cmd[j + 2])
	{
		split_cmd[j] = split_cmd[j + 2];
		j++;
	}
	split_cmd[j] = NULL;
	split_cmd[j + 1] = NULL;
	return (SUCCESS);
	return (ERROR);
}

int	parse_redirection(t_shell *shell, t_command *cmd, char **split_cmd, int *i)
{
	get_redirection_type(cmd, split_cmd, *i);
	if (cmd->redirection_in == 3 || cmd->redirection_out == 4)
		*i += 1;
	if (get_redirection_argument(shell, cmd, split_cmd, *i) == SUCCESS)
	{
		return (SUCCESS);
	}
	return (ERROR);
}

int	interpret_the_rest(t_shell *shell, t_command *cmd, char **split_cmd, int *i, int *dq)
{
	if ((*split_cmd)[*i] == '$')
	{
		if (expand_env_variable(split_cmd, *i + 1) == ERROR)
			return (ERROR);
	}
	else if ((*split_cmd)[*i] == '~')
	{
		if ((i == 0 || (*split_cmd)[*i - 1] == ' ') && \
			((*split_cmd)[*i + 1] == '\0' || (*split_cmd)[*i + 1] == ' '))
		{
			if (expand_tilde(split_cmd, *i) == ERROR)
				return (ERROR);
		}
	}
	else if (((*split_cmd)[*i] == '<' || (*split_cmd)[*i] == '>') && *dq == 0)
	{
		if (parse_redirection(shell, cmd, split_cmd, i) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

char	*remove_char(char *str, int i)
{
	char	*new_str;
	int		new_length;
	int		j;

	new_length = ft_strlen(str) - 1;
	new_str = malloc(sizeof(char) * (new_length + 1));
	if (new_str)
	{
		j = new_length + 1;
		new_str[new_length] = '\0';
		while (new_length--)
		{
			j--;
			if (j == i)
				j--;
			new_str[new_length] = str[j];
		}
	}
	return (new_str);
}

int	interpret_quotes(char **split_command, int i, int *quote, int *dq)
{
	check_quotes((*split_command)[i], quote, dq);
	if (((*split_command)[i] == '"' && *quote == 0) || \
		((*split_command)[i] == '\'' && *dq == 0))
	{
		*split_command = ft_tmp(*split_command, remove_char(*split_command, i));
		if (*split_command == NULL)
			return (ERROR);
	}	
	return (SUCCESS);
}

int	parse_argv(t_shell *shell, t_command *current_command, char **split_command)
{
	int	i;
	int	quote;
	int	double_quote;

	quote = 0;
	double_quote = 0;
	while (*split_command)
	{
		i = 0;
		while (*split_command && (*split_command)[i])
		{
			if (interpret_quotes(split_command, i, &quote, &double_quote))
				return (ERROR);
			if ((*split_command)[i])
			{
				if (quote == 0 && interpret_the_rest(shell, current_command, \
						split_command, &i, &double_quote) == ERROR)
					return (ERROR);
				i++;
			}
		}
		split_command++;
	}
	return (SUCCESS);
}

int	parse_command(t_shell *shell, t_command *current_command, char *command)
{
	char	**split_command;

	while (*command == ' ')
		command++;
	replace_split_spaces(command);
	split_command = ft_split(command, 1);
	if (split_command)
	{
		current_command->argc = ft_strlen_2d(split_command);
		if (parse_argv(shell, current_command, split_command) == SUCCESS)
		{
			current_command->argv = split_command;
			return (SUCCESS);
		}
		ft_free_2d((void **)split_command, current_command->argc);
	}
	return (ERROR);
}

int	add_command(t_shell *shell, char *command)
{
	t_command	*current_command;

	current_command = malloc(sizeof(t_command));
	if (current_command)
	{
		ft_bzero(current_command, sizeof(t_command));
		if (shell->command_list != NULL)
		{
			current_command->prev = shell->command_list;
			current_command->prev->next = current_command;
			current_command->redirection_out = 0;
			current_command->redirection_in = 0;
		}
		shell->command_list = current_command;
		if (parse_command(shell, current_command, command) == SUCCESS)
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
		// print_commands(shell);
	}
	return (SUCCESS);
}
