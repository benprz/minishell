/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:07:27 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/27 18:03:15 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shell
{
	char	*prompt;
	char	**sp_prompt;
	char	**all_path;
	int		pipe_fd[2];
}				t_shell;

# define ERROR 1
# define SUCCESS 0

int		parse_command(t_shell *shell, char **env);
int		ft_pwd(t_shell *shell, char **env);

char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(const char *s1, const char *s2);
int		ft_strlen(const char *str);
char	*ft_get_path(t_shell *shell);

#endif