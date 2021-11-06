/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/06 05:03:26 by ben              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_env_variables(t_shell *shell, char *command, int i)
{
	char	*var;
	size_t	var_name_length;
	char	*new_command;

	var_name_length = ft_strclen(command + i + 1, ' ');
	var = ft_substr(command, i + 1, var_name_length);
	var = ft_tmp(var, getenv(var));
	if (var == NULL) 
	{
		var_name_length = ft_strclen(command + i + 1, '"');
		var = ft_substr(command, i + 1, var_name_length);
		var = ft_tmp(var, getenv(var));
	}	
	command[i] = '\0';
	new_command = ft_strjoin(command, var);
	new_command = ft_tmp(new_command, ft_strjoin(new_command, \
					command + i + var_name_length + 1));
	free(command);
	shell->prompt = new_command;
	return (ft_strlen(var));
}

void	add_spaces_to_redirection_chars(t_shell *shell, char *command, int i)
{
	char	*new_command;
	int		j;
	int		k;

	new_command = malloc(sizeof(char) * (ft_strlen(command) + 2 + 1));
	j = 0;
	k = 0;
	while (command[j])
	{
		new_command[k] = command[j];
		if (j == i)
		{
			new_command[k] = ' ';
			new_command[k + 1] = command[i];
			new_command[k + 2] = ' ';
			k += 2;
		}
		j++;
		k++;
	}
	new_command[k] = '\0';
	ft_tmp(shell->prompt, new_command);
}

void	interpret_command(t_shell *shell)
{
	int	i;
	int	double_quotes;
	int	single_quotes;

	i = 0;
	double_quotes = 0;
	single_quotes = 0;
	while (shell->prompt[i])
	{
		if (shell->prompt[i] == '\'' && double_quotes == 0)
			single_quotes = 1;
		else if (shell->prompt[i] == '"' && single_quotes == 0)
			double_quotes = 1;
		if (shell->prompt[i] == '$' && single_quotes == 0)
			i += expand_env_variables(shell, shell->prompt, i) - 1;
		else if ((shell->prompt[i] == '|' || shell->prompt[i] == '<' || shell->prompt[i] == '>') && single_quotes == 0 && double_quotes == 0)
		{
			add_spaces_to_redirection_chars(shell, shell->prompt, i);
			i += 2;
		}
		i++;
	}
}

int	check_redi_in(t_shell *shell)
{
	interpret_command(shell);
	//rm_quotes_on_cmd(shell);
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

int	parse_program(t_shell *shell)
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
