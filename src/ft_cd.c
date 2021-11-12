/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:06:03 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/12 00:09:16 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cas d'erreur ????

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

static char	*cd_back(t_shell *shell, char *pwd)
{
	int	i;
	int	j;

	i = ft_strlen(pwd);
	j = 0;
	while (i && pwd[i] != '/')
		i--;
	free(shell->command_list->argv[1]);
	shell->command_list->argv[1] = malloc(sizeof(char) * (i + 1));
	while (j < i)
	{
		shell->command_list->argv[1][j] = pwd[j];
		j++;
	}
	shell->command_list->argv[1][j] = '\0';
	pwd = ft_strdup(shell->command_list->argv[1]);
	return (pwd);
}

static void	do_cd_one(t_shell *shell, char **split_path, char *pwd)
{
	int		i;
	int		j;

	pwd = get_pwd(shell);
	if (!ft_strcmp(split_path[0], ".."))
		pwd = cd_back(shell, pwd);
	else
	{
		pwd = ft_strjoin(pwd, "/");
		shell->command_list->argv[1] = ft_strjoin(pwd,
				shell->command_list->argv[1]);
	}
}

static void	do_cd_else(t_shell *shell, char **split_path, char *pwd)
{
	int		i;

	i = 0;
	if (!ft_strcmp(split_path[i], "."))
		i++;
	while (split_path[i] && !ft_strcmp(split_path[i], ".."))
	{
		pwd = cd_back(shell, pwd);
		i++;
	}
	if (split_path[i])
	{
		free(shell->command_list->argv[1]);
		pwd = ft_strjoin(pwd, "/");
		shell->command_list->argv[1] = ft_strjoin(pwd, split_path[i]);
		while (split_path[i + 1])
		{
			shell->command_list->argv[1]
				= ft_strjoin(shell->command_list->argv[1], "/");
			shell->command_list->argv[1]
				= ft_strjoin(shell->command_list->argv[1], split_path[i + 1]);
			i++;
		}
	}
}

int	ft_cd(t_shell *shell)
{
	char	**split_path;
	int		size_split;
	char	*pwd;

	if (shell->command_list->next)
		return (EXIT_CMD);
	split_path = ft_split(shell->command_list->argv[1], '/');
	size_split = ft_tablen(split_path);
	pwd = get_pwd(shell);
	if (size_split == 1)
		do_cd_one(shell, split_path, pwd);
	else
		do_cd_else(shell, split_path, pwd);
	free(pwd);
	if (chdir(shell->command_list->argv[1]) == -1)
		ft_error("Error No such file or directory", EXIT_CMD);
	change_env(shell);
	return (SUCCESS);
}
