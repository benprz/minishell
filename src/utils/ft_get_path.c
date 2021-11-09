/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:38:18 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/03 21:40:50 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_path(t_shell *shell)
{
	int		i;
	char	*save;

	i = 0;
	while (shell->all_path[i])
	{
		save = ft_strjoin(shell->all_path[i], "/");
		save = ft_strjoin(save, shell->command_list->argv[0]);
		if (access(save, X_OK) == 0)
			return (save);
		free(save);
		i++;
	}
	return (NULL);
}

static char	*ft_get_arg_2(t_shell *shell)
{
	char	*str;

	str = NULL;
	while (shell->sp_prompt[shell->position] && parse_redi_pipe(shell) == ERROR)
	{
		if (!ft_strcmp(shell->sp_prompt[shell->position], "'|'"))
			str = ft_strjoin(str, "|");
		else if (!ft_strcmp(shell->sp_prompt[shell->position], "'>'"))
			str = ft_strjoin(str, ">");
		else
			str = ft_strjoin(str, shell->sp_prompt[shell->position]);
		shell->position++;
		if (shell->sp_prompt[shell->position])
			str = ft_strjoin(str, " ");
	}
	return (str);
}

static char	**get_arg_no_option(t_shell *shell)
{
	char	**arg;

	arg = malloc(sizeof(char *) * 3);
	arg[0] = ft_strdup(shell->sp_prompt[shell->position]);
	shell->position++;
	if (shell->sp_prompt[shell->position])
		arg[1] = ft_get_arg_2(shell);
	else
		arg[1] = NULL;
	arg[2] = NULL;
	return (arg);
}

char	**ft_get_arg(t_shell *shell)
{
	char	**arg;

	if (ft_check_options(shell) == SUCCESS)
	{
		arg = malloc(sizeof(char *) * 4);
		arg[0] = ft_strdup(shell->sp_prompt[shell->position]);
		shell->position++;
		arg[1] = ft_strdup(shell->sp_prompt[shell->position]);
		shell->position++;
		if (shell->sp_prompt[shell->position])
			arg[2] = ft_get_arg_2(shell);
		else
			arg[2] = NULL;
		arg[3] = NULL;
	}
	else
		arg = get_arg_no_option(shell);
	printf("%s\n", arg[1]);
	return (arg);
}
