/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/04 04:07:37 by ben              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*interpret_env_variables(char *command)
{
	int	i;
	char	*var;
	size_t	var_name_length;
	char	*new_command;

	i = 0;
	var = NULL;
	while (command[i])
	{
		if (command[i] == '$')
		{
			var_name_length = ft_strclen(command + i + 1, ' ');
			var = ft_substr(command, i + 1, var_name_length);
			var = ft_tmp(var, getenv(var));
			command[i] = '\0';
			new_command = ft_strjoin(command, var);
			new_command = ft_tmp(new_command, ft_strjoin(new_command, \
							command + i + var_name_length + 1));
			free(command);
			command = new_command;
			i += ft_strlen(var) - 1;
		}
		i++;
	}
	return (command);
}

char	*interpret_command(char *command)
{
	command = add_spaces_to_pipes(command);
	command = interpret_env_variables(command);
	// command = interpret_single_quotes(command);
	// command = interpret_double_quotes(command);
	return (command);
}

int	check_redi_in(t_shell *shell)
{
	shell->prompt = interpret_command(shell->prompt);
	rm_quotes_on_cmd(shell);
	shell->sp_prompt = ft_split(shell->prompt, ' ');
	if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
	{
		shell->redi_in = 1;
		shell->position++;
		shell->fd_in = open(shell->sp_prompt[shell->position], O_RDWR);
		if (shell->fd_in == -1)
		{
			perror("Error infile doesn't exist");
			return (ERROR);
		}
	}
	return (ft_execute_cmd(shell));
}

int	parse_redi_pipe(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "|"))
		shell->pipe = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], ">"))
		shell->redi_out = 1;
	else if (!ft_strcmp(shell->sp_prompt[shell->position], "<"))
		shell->redi_in = 1;
	else
		return (ERROR);
	return (SUCCESS);
}

int	parse_command(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->save_position], "pwd"))
		return (ft_pwd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "echo"))
		return (ft_echo(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "cd"))
		return (ft_cd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "export"))
		return (ft_export(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "unset"))
		return (ft_unset(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "env"))
		return (ft_env(shell));
	else
		return (ERROR);
}

int	ft_check_options(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "echo"))
	{
		if (shell->sp_prompt[shell->position + 1]
			&& !ft_strcmp(shell->sp_prompt[shell->position + 1], "-n"))
			return (SUCCESS);
	}
	else if (shell->sp_prompt[shell->position + 1]
		&& shell->sp_prompt[shell->position + 1][0] == '-')
		return (SUCCESS);
	return (ERROR);
}
