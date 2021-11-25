/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prompt_tonorm.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 23:00:13 by bperez            #+#    #+#             */
/*   Updated: 2021/11/25 05:25:07 by bperez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
