/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:11:30 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/24 12:00:45 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	send_env(t_shell *shell)
{
	char	*chaine_env;
	int		i;
	int		size;
	
	i = -1;
	size = 0;
	while (shell->env[++i])
		size += ft_strlen(shell->env[i]) + 1;
	chaine_env = malloc(sizeof(char) * (size + 1));
	chaine_env = NULL;
	i = -1;
	while (shell->env[++i])
	{
		chaine_env = ft_strjoin(chaine_env, shell->env[i]);
		chaine_env = ft_strjoin(chaine_env, "\n");
	}
	chaine_env[size] = '\0';
	close(shell->pipe_export[0]);
	write(shell->pipe_export[1], chaine_env, size);
	close(shell->pipe_export[1]);
}

static int	check_already_here(t_shell *shell, char *value)
{
	int		i;
	int		j;
	int		check;
	char	**tab;

	tab = ft_split(value, '=');
	i = get_current_env_int(shell, tab[0]);
	free_tab(tab);
	if (i == -1)
		return (ERROR);
	else
	{
		j = -1;
		check = 0;
		while (value[++j])
			if (value[j] == '=')
				check++;
		if (check)
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(value);
		}
		return (SUCCESS);
	}
}

static void	change_env(t_shell *shell, int i)
{
	char	**save_env;
	int		j;

	save_env = malloc(sizeof(char *) * (ft_tablen(shell->env) + 1));
	j = 0;
	while (shell->env[j])
	{
		save_env[j] = ft_strdup(shell->env[j]);
		j++;
	}
	save_env[j] = NULL;
	free_tab(shell->env);
	shell->env = malloc(sizeof(char *) * (ft_tablen(save_env) + 2));
	j = 0;
	while (save_env[j])
	{
		shell->env[j] = ft_strdup(save_env[j]);
		j++;
	}
	shell->env[j] = ft_strdup(shell->command_list->argv[i]);
	shell->env[j + 1] = NULL;
	free_tab(save_env);
}

static void	do_redirection(t_shell *shell)
{
	if (shell->command_list->redirection_out == 2)
	{
		close(shell->pipe_fd[shell->index][0]);
		if (dup2(shell->command_list->fd_out, shell->pipe_fd[shell->index][1]) == -1)
			ft_error_fork(shell, "Error, dup2");
	}
	else if (!shell->command_list->next)
	{
		close(shell->pipe_fd[shell->index][0]);
		if (dup2(1, shell->pipe_fd[shell->index][1]) == -1)
			ft_error_fork(shell, "Error dup2 cmd");
	}
}

static int	check_value(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (!ft_isalnum(value[i]) && value[i] != '_' && value[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_shell *shell)
{
	int	i;

	i = -1;
	do_redirection(shell);
	if (shell->command_list->argv[1] == NULL)
		while (shell->env[++i])
			printf("declare -x %s\n", shell->env[i]);
	else
	{
		i = 1;
		while (shell->command_list->argv[i])
		{
			if (ft_isdigit(shell->command_list->argv[i][0]))
				printf("Error export not a valid identifier\n");
			if (!check_value(shell->command_list->argv[i]))
				printf("Error export not a valid identifier\n");
			else if (shell->command_list->argv[i][0] == '=')
				printf("Error export '=' not a valid identifier\n");
			else if (check_already_here(shell, shell->command_list->argv[i]) == ERROR)
				change_env(shell, i);
			i++;
		}
	}
	send_env(shell);
	return (SUCCESS);
}
