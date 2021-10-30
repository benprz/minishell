/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:38:18 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/30 19:09:33 by ngeschwi         ###   ########.fr       */
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
		save = ft_strjoin(save, shell->sp_prompt[shell->save_position]);
		if (access(save, X_OK) == 0)
			return (save);
		free(save);
		i++;
	}
	return (NULL);
}

char	*ft_get_arg_2(t_shell *shell)
{
	char 	*str;

	str = NULL;
	while (shell->sp_prompt[shell->position] && parse_redi_pipe(shell) == ERROR)
	{
		str = ft_strjoin(str, shell->sp_prompt[shell->position]);
		shell->position++;
		if (shell->sp_prompt[shell->position])
			str = ft_strjoin(str, " ");
	}
	return (str);
}

char	**ft_get_arg(t_shell *shell)
{
	char	**arg;
	int		size;

	size = 1;
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
			arg[2] = "";
		arg[3] = NULL;
	}
	else
	{
		arg = malloc(sizeof(char *) * 3);
		arg[0] = ft_strdup(shell->sp_prompt[shell->position]);
		shell->position++;
		if (shell->sp_prompt[shell->position])
			arg[1] = ft_get_arg_2(shell);
		else
			arg[1] = "";
		arg[2] = NULL;
	}
	return (arg);
}
