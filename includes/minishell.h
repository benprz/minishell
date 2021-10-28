/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:07:27 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/28 17:51:48 by ngeschwi         ###   ########.fr       */
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
	int		position;
	int		save_position;
	int		pipe;
	int		redirection;
	char	**all_path;
	int		pipe_fd[2];
	char	**env;
}				t_shell;

# define ERROR 1
# define SUCCESS 0

int		parse_command(t_shell *shell);
int		parse_redi_pipe(t_shell *shell);
int		ft_pwd(t_shell *shell);
int		ft_echo(t_shell *shell);

char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(const char *s1, const char *s2);
int		ft_strlen(const char *str);
char	*ft_strdup(char *src);
char	*ft_get_path(t_shell *shell);

#endif