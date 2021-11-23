/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 23:00:13 by bperez            #+#    #+#             */
/*   Updated: 2021/11/22 18:47:31 by ngeschwi         ###   ########.fr       */
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
		printf("program_path = %s\nargc = %d\ntype_in = %d\ntype_out = %d\nfd_in = %d\nfd_out = %d\n", current->program_path, current->argc, current->redirection_in, current->redirection_out, current->fd_in, current->fd_out);
		i = 0;
		if (current->delimiters)
		{
		while (current->delimiters[i])
			{
				printf("current->delimiters[%d] = %s\n", i, current->delimiters[i]);
				i++;
			}
		}
		printf("\n");
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
			*command = 1;
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

int	expand_tilde(t_command *command, char **split_command, int i)
{
	char	*var;
	char	*new_command;

	var = get_current_env(command->shell, "HOME");
	if (var[0] != '\0')
	{
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
	}
	return (ERROR);
}

int	expand_env_variable(t_command *command, char **split_command, int i)
{
	char	*var;
	int		var_name_length;
	char	*new_command;

	var_name_length = get_variable_name_length(*split_command, i);
	var = ft_substr(*split_command, i, var_name_length);
	if (var)
	{
		var = ft_tmp(var, get_current_env(command->shell, var));
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

int	get_redirection_type(t_command *cmd, char *command)
{
	int	check_redirection;
	
	if (*command == '<')
	{
		cmd->redirection_in = REDIRECTION_INPUT;
		check_redirection = ft_strcmp(command, "<<");
		if (check_redirection == 0)
			cmd->redirection_in = REDIRECTION_DINPUT;
		return (cmd->redirection_in);
	}
	else
	{
		cmd->redirection_out = REDIRECTION_OUTPUT;
		check_redirection = ft_strcmp(command, ">>");
		if (check_redirection == 0)
			cmd->redirection_out = REDIRECTION_DOUTPUT;
		return (cmd->redirection_out);
	}
}

void	goto_eof(t_command *command)
{
	char	buf[1024];
	int		ret;
	
	ret = read(command->fd_out, buf, 1024);
	while (ret > 0)
		ret = read(command->fd_out, buf, 1024);
}

int	open_redirection_file(t_command *command, char *redirection_argument, int redirection_type)
{
	struct stat	sct_stat;

	if (redirection_type == REDIRECTION_INPUT)
	{
		if (command->redirection_in == REDIRECTION_INPUT)
		{
			command->fd_in = open(redirection_argument, O_RDWR);
			if (command->fd_in == -1)
				return (ft_error("Error no such file or directory", ERROR)); 
		}
	}
	else
	{
		if (command->redirection_out == REDIRECTION_OUTPUT)
		{
			stat(redirection_argument, &sct_stat);
			if (sct_stat.st_atime != 0)
				unlink(redirection_argument);
			command->fd_out = open(redirection_argument, O_CREAT | O_RDWR, S_IRWXU);
			if (command->fd_out == -1)
				return (ft_error("Error no such file or directory", ERROR));
		}
		else if (command->redirection_out == REDIRECTION_DOUTPUT)
		{
			stat(redirection_argument, &sct_stat);
			command->fd_out = open(redirection_argument, O_CREAT | O_RDWR, S_IRWXU);
			if (command->fd_out == -1)
				return (ft_error("Error no such file or directory", ERROR));
			if (sct_stat.st_atime != 0)
				goto_eof(command);
		}
	}
	printf("fd_out=%d\nfd_in=%d\n", command->fd_out, command->fd_in);
	return (SUCCESS);
}

void	free_redirection(int current_type, char **split_command)
{
	int	i;
	
	i = 0;
	free(*split_command);
	if (current_type != REDIRECTION_DINPUT)
		free(*(split_command + 1));
	while (split_command[i + 2])
	{
		split_command[i] = split_command[i + 2];
		i++;
	}
	split_command[i] = NULL;
	split_command[i + 1] = NULL;
}

int	parse_redirection_argument(int type, t_command *cmd, char **split_cmd)
{
	int	i;
	int	quote;
	int	double_quote;

	if (*split_cmd)
	{
		i = 0;
		quote = 0;
		double_quote = 0;
		while (*split_cmd && i < ft_strlen(*split_cmd))
		{
			if (interpret_quotes(split_cmd, i, &quote, &double_quote))
				return (ERROR);
			if (type != REDIRECTION_DINPUT && quote == 0 && \
				(*split_cmd)[i] == '$' && ft_isalnum((*split_cmd)[i + 1]))
			{
				if (expand_env_variable(cmd, split_cmd, i + 1) == ERROR)
					return (ERROR);
					i -= -1;
			}
			i++;
		}
		return (SUCCESS);
	}
	return (ERROR);
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

int	parse_delimiter(t_command *command, char **split_command)
{
	char	**delimiters;

	if (command->delimiters)
	{
		if (add_delimiter_to_list(command, split_command) == SUCCESS)
			return (SUCCESS);
	}
	else
	{
		delimiters = malloc(sizeof(char *) * 2);
		if (delimiters)
		{
			delimiters[0] = *split_command;
			delimiters[1] = NULL;
			command->delimiters = delimiters;
			return (SUCCESS);
		}
	}
	return (ERROR);
}

int	parse_redirection(t_command *command, char **split_command)
{
	int		current_type;
	int		ret;

	ret = ERROR;
	current_type = get_redirection_type(command, *split_command);
	if (parse_redirection_argument(current_type, command, \
		split_command + 1) == SUCCESS)
	{
		if (current_type == REDIRECTION_DINPUT)
		{
			if (parse_delimiter(command, split_command + 1) == SUCCESS)
				ret = SUCCESS;
		}
		else
		{
			if (open_redirection_file(command, *(split_command + 1), current_type) == SUCCESS)
				ret = SUCCESS;
		}
		free_redirection(current_type, split_command);
		command->argc -= 2;
	}
	return (ret);
}

int	interpret_the_rest(t_command *cmd, char **split_cmd, int *i, int dq)
{
	if ((*split_cmd)[*i] == '$' && ft_isalnum((*split_cmd)[*i + 1]))
	{
		if (expand_env_variable(cmd, split_cmd, *i + 1) == ERROR)
			return (ERROR);
			*i -= 1;
	}
	else if ((*split_cmd)[*i] == '~' && dq == 0)
	{
		if (*i == 0 && ((*split_cmd)[*i + 1] == '\0' || \
			(*split_cmd)[*i + 1] == '/'))
		{
			if (expand_tilde(cmd, split_cmd, *i) == ERROR)
				return (ERROR);
		}
	}
	else if (((*split_cmd)[*i] == '<' || (*split_cmd)[*i] == '>') && dq == 0)
	{
		if (parse_redirection(cmd, split_cmd) == ERROR)
			return (ERROR);
		*i = -1;
	}
	return (SUCCESS);
}

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
		while (*split_command && (*split_command)[i])
		{
			if (interpret_quotes(split_command, i, &quote, &double_quote))
				return (ERROR);
			if ((*split_command)[i] && (*split_command)[i] != '"' && \
				(*split_command)[i] != '\'')
			{
				if (quote == 0 && interpret_the_rest(current_command, \
						split_command, &i, double_quote) == ERROR)
					return (ERROR);
				i++;
			}
		}
		split_command++;
	}
	return (SUCCESS);
}

char	*add_char(char *str, int index, char c)
{
	char	*new_str;
	int		new_len;
	int		i;

	new_len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (new_len + 1 + 1));
	if (new_str)
	{
		i = 0;
		while (*str)
		{
			if (i == index)
				new_str[i] = c;
			else
			{
				new_str[i] = *str;
				str++;
			}
			i++;
		}
		new_str[i] = '\0';
	}
	return (new_str);
}

int	add_delimiters_to_redirection(char **command, int *i)
{
	if (*i > 0 && (*command)[*i - 1] != SPLIT_DELIMITER)
	{
		*command = ft_tmp(*command, add_char(*command, *i, SPLIT_DELIMITER));
		if (*command == NULL)
			return (ERROR);
		*i += 1;
	}
	if ((*command)[*i + 1] == '<' || (*command)[*i + 1] == '>')
	{
		if (((*command)[*i] == '<' && (*command)[*i + 1] == '>') || \
			((*command)[*i] == '>' && (*command)[*i + 1] == '<'))
			return (ERROR);
		*i += 1;
	}
	if ((*command)[*i + 1] != '\0' && (*command)[*i + 1] != ' ')
	{
		*command = ft_tmp(*command, add_char(*command, *i + 1, SPLIT_DELIMITER));
		if (command == NULL)
			return (ERROR);
		*i += 1;
	}
	return (SUCCESS);
}

int	split_command(char **command)
{
	int		i;
	int		quote;
	int		double_quote;

	i = 0;
	quote = 0;
	double_quote = 0;
	while ((*command)[i])
	{
		check_quotes((*command)[i], &quote, &double_quote);
		if ((*command)[i] == ' ' && quote == 0 && double_quote == 0)
			(*command)[i] = SPLIT_DELIMITER;
		else if (((*command)[i] == '<' || (*command)[i] == '>') && \
					quote == 0 && double_quote == 0)
		{
			if (add_delimiters_to_redirection(command, &i) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

char	*get_program_path(t_shell *shell, t_command *command)
{
	int		i;
	char	*path;

	i = 0;
	while (command->argv[0][i])
	{
		if (command->argv[0][i] == '/')
			return (ft_strdup(command->argv[0]));
		i++;
	}
	i = 0;
	while (shell->all_path[i])
	{
		path = ft_strjoin(shell->all_path[i], "/");
		path = ft_strjoin(path, command->argv[0]);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

int	is_program_builtin(char *program)
{
	if (!ft_strcmp(program, "cd"))
		return (1);
	if (!ft_strcmp(program, "export"))
		return (1);
	if (!ft_strcmp(program, "unset"))
		return (1);
	if (!ft_strcmp(program, "echo"))
		return (1);
	if (!ft_strcmp(program, "pwd"))
		return (1);
	if (!ft_strcmp(program, "env"))
		return (1);
	return (0);
}

int	parse_command(t_shell *shell, t_command *current_command, char **command)
{
	char	**ret;

	if (ft_strlen(*command) > 0 && split_command(command) == SUCCESS)
	{
		ret = ft_split(*command, SPLIT_DELIMITER);
		if (ret)
		{
			current_command->shell = shell;
			current_command->argc = ft_strlen_2d(ret);
			if (parse_argv(current_command, ret) == SUCCESS)
			{
				current_command->argv = ret;
				if (!is_program_builtin(current_command->argv[0]))
				{
					current_command->program_path = get_program_path(shell, \
													current_command);
				}
				return (SUCCESS);
			}
			ft_free_2d((void **)ret, current_command->argc);
		}
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
		}
		shell->command_list = current_command;
		if (parse_command(shell, current_command, &command) == SUCCESS)
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
	int	q;
	int	dq;
	int	prompt_length;

	if (prompt)
	{
		i = 0;
		q = 0;
		dq = 0;
		prompt_length = ft_strlen(prompt);
		while (prompt_length-- >= 0)
		{
			check_quotes(prompt[i], &q, &dq);
			if (prompt[i] == '\0' || (prompt[i] == '|' && q == 0 && dq == 0))
			{
				if (add_command(shell, ft_substr(prompt, 0, i)) == ERROR)
				{
					printf("Error parsing\n");
					return (ERROR);
				}
				prompt += i + 1;
				i = -1;
			}
			i++;
		}
		//print_commands(shell);
	}
	return (SUCCESS);
}