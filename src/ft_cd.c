/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:06:03 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/10 13:31:07 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// mieux gerer .. (tout refaire ?)

static void	change_env(t_shell *shell)
{
	int		i;
	char	*save_pwd;

	i = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'P' && shell->env[i][1] == 'W')
			break ;
		i++;
	}
	save_pwd = ft_strdup(shell->env[i]);
	free(shell->env[i]);
	shell->env[i] = NULL;
	shell->env[i] = ft_strjoin(shell->env[i], "PWD=");
	shell->env[i] = ft_strjoin(shell->env[i], shell->command_list->argv[1]);
	i = 0;
	while (shell->env[i])
	{
		if (shell->env[i][0] == 'O' && shell->env[i][1] == 'L')
			break ;
		i++;
	}
	free(shell->env[i]);
	shell->env[i] = NULL;
	shell->env[i] = ft_strjoin(shell->env[i], "OLD");
	shell->env[i] = ft_strjoin(shell->env[i], save_pwd);
	free(save_pwd);
}

static char	*get_path_cd(char *save)
{
	char	*path;
	int		i;
	int		j;
	int		size;

	size = ft_strlen(save);
	i = size - 1;
	j = 0;
	while (save[i] && save[i] != '/')
		i--;
	path = malloc(sizeof(char) * (i + 1));
	while (j < i)
	{
		path[j] = save[j];
		j++;
	}
	path[j] = '\0';
	return (path);
}

static void	get_arg_cd(t_shell *shell)
{
	char	*save;
	char	*path;
	int		i;
	int		j;

	path = getenv("PWD");
	while (shell->command_list->argv[1][1] && shell->command_list->argv[1][0] == '.')
	{
		i = 3;
		j = 0;
		save = ft_strdup(shell->command_list->argv[1]);
		free(shell->command_list->argv[1]);
		shell->command_list->argv[1] = NULL;
		shell->command_list->argv[1] = malloc(sizeof(char) * (ft_strlen(save) - 2));
		while (save[i])
		{
			shell->command_list->argv[1][j] = save[i];
			i++;
			j++;
		}
		shell->command_list->argv[1][j] = '\0';
		free(save);
		path = get_path_cd(path);
	}
	if (shell->command_list->argv[1][0] != '\0')
		path = ft_strjoin(path, "/");
	shell->command_list->argv[1] = ft_strjoin(path, shell->command_list->argv[1]);
}

int	ft_cd(t_shell *shell)
{
	int	cd;

	if (shell->command_list->next)
		return (SUCCESS);
	if (shell->command_list->argv[1][0] == '.')
	{
		if (shell->command_list->argv[1][1] == '.'
			&& (shell->command_list->argv[1][2] == '/'
				|| !shell->command_list->argv[1][2]))
			get_arg_cd(shell);
		else
		{
			perror("Error No such file or directory");
			return (SUCCESS);
		}
	}
	else if (shell->command_list->argv[1][0] != '/')
	{
		shell->command_list->argv[1] = ft_strjoin("/",
				shell->command_list->argv[1]);
		shell->command_list->argv[1] = ft_strjoin(getenv("PWD"),
				shell->command_list->argv[1]);
	}
	cd = chdir(shell->command_list->argv[1]);
	if (cd == -1)
	{
		perror("Error No such file or directory");
		return (SUCCESS);	
	}
	change_env(shell);
	return (SUCCESS);
}
