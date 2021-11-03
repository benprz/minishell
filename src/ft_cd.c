/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:06:03 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/03 19:37:29 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cd .. || cd ../dir || cd ../../../dir

static char	*get_path_cd(t_shell *shell, char *save)
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
	while (shell->arg[1][1] && shell->arg[1][0] == '.')
	{
		i = 3;
		j = 0;
		save = ft_strdup(shell->arg[1]);
		free(shell->arg[1]);
		shell->arg[1] = NULL;
		shell->arg[1] = malloc(sizeof(char) * (ft_strlen(save) - 2));
		while (save[i])
		{
			shell->arg[1][j] = save[i];
			i++;
			j++;
		}
		shell->arg[1][j] = '\0';
		free(save);
		path = get_path_cd(shell, path);
	}
	if (shell->arg[1][0] != '\0')
		path = ft_strjoin(path, "/");
	shell->arg[1] = ft_strjoin(path, shell->arg[1]);
}

int	ft_cd(t_shell *shell)
{
	int	cd;

	if (shell->arg[1][0] == '.')
	{
		if (shell->arg[1][1] == '.' && shell->arg[1][2] == '/')
			get_arg_cd(shell);
		else
			ft_error(shell, "Error No such file or directory");
	}
	else if (shell->arg[1][0] != '/')
	{
		shell->arg[1] = ft_strjoin("/", shell->arg[1]);
		shell->arg[1] = ft_strjoin(getenv("PWD"), shell->arg[1]);
	}
	cd = chdir(shell->arg[1]);
		if (cd == -1)
			ft_error(shell, "Error No such file or directory");
	return (SUCCESS);
}
