/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:05:43 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/02 16:39:06 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_spaces_to_pipes(char *command)
{
	int new_command_length;
	char	*new_command;
	int	i;
	int j;

	new_command_length = ft_strlen(command);
	i = 0;
	while (command[i])
	{
		if (command[i] == '|')
			new_command_length += 2;
		i++;
	}
	new_command = malloc(sizeof(char) * new_command_length + 1);
	i = 0;
	j = 0;
	while (command[i])
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
		i++;
		j++;
	}
	new_command[new_command_length] = '\0';
	free(command);
	return (new_command);
}

void	print_prompt(t_shell *shell)
{
	//printf("prompt = '%s'\n", shell->prompt);
	for (int i = 0; shell->sp_prompt[i]; i++)
	{
		printf("sp_prompt[%d] = %s\n", i, shell->sp_prompt[i]);
	}
}

char	*interpret_command(char *command)
{
	command = add_spaces_to_pipes(command);
	return (command);
}

int	check_redi_in(t_shell *shell)
{
	shell->prompt = interpret_command(shell->prompt);
	shell->sp_prompt = ft_split(shell->prompt, ' ');
	// print_prompt(shell);
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
	// else if (!ft_strcmp(shell->sp_prompt[shell->position], ">>"))
	// 	shell->redirection = 1;
	// else if (!ft_strcmp(shell->sp_prompt[shell->position], "<<"))
	// 	shell->redirection = 1;
	else
		return (ERROR);
	return (SUCCESS);
}

int	parse_command(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->save_position], "pwd"))
		return(ft_pwd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "echo"))
		return(ft_echo(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "cd"))
		return(ft_cd(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "export"))
		return(ft_export(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "unset"))
		return(ft_unset(shell));
	else if (!ft_strcmp(shell->sp_prompt[shell->save_position], "env"))
		return(ft_env(shell));
	else
		return (ERROR);
}

int	ft_check_options(t_shell *shell)
{
	if (!ft_strcmp(shell->sp_prompt[shell->position], "echo"))
	{
		if (shell->sp_prompt[shell->position + 1] &&
			!ft_strcmp(shell->sp_prompt[shell->position + 1], "-n"))
			return (SUCCESS);
	}
	else if (shell->sp_prompt[shell->position + 1] &&
		shell->sp_prompt[shell->position + 1][0] == '-')
		return (SUCCESS);
	return (ERROR);
}
