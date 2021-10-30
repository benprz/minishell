/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:07:27 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/10/30 19:15:10 by ngeschwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>

typedef struct s_shell
{
	int		launched;
	char	*prompt;
	char	**sp_prompt;
	int		position;
	int		save_position;
	int		pipe;
	int		redi_in;
	int		fd_in;
	int		redi_out;
	int		fd_out;
	char	**all_path;
	int		pipe_fd[2];
	char	**arg;
	char	**env;
}				t_shell;

# define ERROR 1
# define SUCCESS 0

int		parse_command(t_shell *shell);
int		parse_redi_pipe(t_shell *shell);
int		ft_check_options(t_shell *shell);
void	check_redi_in(t_shell *shell);
int		ft_execute_cmd(t_shell *shell);
int		ft_pwd(t_shell *shell);
int		ft_echo(t_shell *shell);
int		ft_cd(t_shell *shell);
int		ft_env(t_shell *shell);
int		ft_export(t_shell *shell);
int		ft_unset(t_shell *shell);
char	*ft_get_path(t_shell *shell);
char	**ft_get_arg(t_shell *shell);
char	*ft_get_arg_2(t_shell *shell);

char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(const char *s1, const char *s2);
int		ft_strlen(const char *str);
char	*ft_strdup(char *src);
void	ft_free_tab(t_shell *shell);
int		ft_error(t_shell *shell, char *str);

#endif