/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngeschwi <nathan.geschwind@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:07:27 by ngeschwi          #+#    #+#             */
/*   Updated: 2021/11/23 15:48:54 by ngeschwi         ###   ########.fr       */
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
# include <sys/errno.h>

typedef struct s_command
{
	int					argc;
	char				**argv;
	char				*program_path;
	int					redirection_in;
	int					redirection_out;
	int					fd_in;
	int					fd_out;
	char				**delimiters;
	struct s_shell		*shell;
	struct s_command	*prev;
	struct s_command	*next;
}	t_command;

typedef struct s_shell
{
	t_command	*command_list;
	char		**env;
	char		**all_path;
	int			pipe_fd[2];
	int			pipe_export[2];
	int			pipe_fd_redi_din[2];
	int			last_exit_status;
}				t_shell;

extern int	errno;

# define SUCCESS 0
# define ERROR 1
# define EXIT_CMD 2
# define REDIRECTION_INPUT 1
# define REDIRECTION_OUTPUT 2
# define REDIRECTION_DINPUT 3
# define REDIRECTION_DOUTPUT 4
# define SPLIT_DELIMITER 1

// ***** Init data and signals *****
void		init_shell_data(t_shell *shell, char **env);
void		init_shell_signals(void);
void		init_program_signals(void);
void		exit_shell(void);
void		break_current_loops(void);

// ***** Parsing *****
int			parse_prompt(t_shell *shell, char *prompt);
t_command	*goto_first_command(t_command *current);
int			check_commad_1(t_shell *shell);
int			check_commad_2(t_shell *shell);
char		*add_spaces_to_pipes(char *command);
void		rm_quotes_on_cmd(t_shell *shell);

// ***** Execution *****
void		execute_command(t_shell *shell);
void		exec_cmd_for_rdi(t_shell *shell);
void		free_prompt(t_shell *shell, char *prompt);
void		close_pipe_rdi(t_shell *shell);
int			ft_pwd(t_shell *shell);
int			ft_echo(t_shell *shell);
int			ft_cd(t_shell *shell);
int			ft_env(t_shell *shell);
int			ft_export(t_shell *shell);
int			ft_unset(t_shell *shell);
char		*ft_get_path(t_shell *shell);
char		**ft_get_arg(t_shell *shell);
char		*get_pwd(t_shell *shell);

// ***** Utils *****
void		*ft_tmp(void *a, void *b);
size_t		ft_strclen(const char *str, const char c);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(const size_t count, const size_t size);
void		*ft_memset(void *ptr, const unsigned int c, size_t len);
char		*ft_substr(const char *s, unsigned int start, size_t len);
char		**ft_split(char const *s, char c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strjoin(const char *s1, const char *s2);
int			ft_strlen(const char *str);
int			ft_tablen(char **tab);
char		*ft_strdup(char *src);
void		ft_error_fork(t_shell *shell, char *str);
int			ft_error(char *str, int	wich_one);
void		free_tab(char **tab);
char		*ft_strtrim(const char *s1);
char		*ft_strndup(const char *s1, size_t len);
void		ft_free_2d(void **array, size_t size);
size_t		ft_strlen_2d(char **array);
int			ft_isalnum(const int c);
int			ft_isalpha(const int c);
int			ft_isdigit(const int c);
int			ft_strncmp(const char *s1, const char *s2, const size_t n);
int			get_current_env_int(t_shell *shell, char *str);
char		*get_current_env(t_shell *shell, char *str);
void		check_pwd(t_shell *shell);

#endif
