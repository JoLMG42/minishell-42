/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:35:53 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/04 15:16:52 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "structs.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/random.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*          BUILTINS        */
void	ft_pwd(t_data *data, t_shell *shell);
void	ft_env(t_data *data, t_shell *shell);
void	ft_export(t_data *data, t_shell *shell);

char	*expander(char *str, t_env **env, int i, char *res);
int		parse_input(char *input, t_shell *shell);
void	freetab(char **tab);
int		exec(t_shell *datas);
void	get_tmp_file(t_data *datas);
void	heredoc(char *eof, char *file_name);
void	exec_built_in(t_data *datas, t_shell *shell);
int		is_built_in(t_data *data);
int		check_if_redir(t_data *datas);
void	exec_simple_cmd(t_data *data, t_shell *shell);
void	handle_redir(t_data *datas);
char	*get_cmd_path(t_data *data, t_shell *shell);

/*			UTILS	ENV*/
char	**create_char_env(t_env *env, int env_size);
void	print_env(t_env *list);
int		get_env_list_size(t_env *list);
char	*get_content_env(t_env **env, char *find);
void	ft_print_exp(t_env *exp);

/*			UTILS	STR/TAB		*/

char	**ft_split_quotes(char *s, char c);
void	exec_pipe(t_data *datas, t_shell *shell);
int		ft_strslen_tab_until(char **tab, int pos);
char	**ft_erase_in_tab(char **tab, int pos, int pos2);
int		ft_tablen(char **tab);
size_t	count_args(char **args);
void	ft_lstadd_back_env(t_env **alst, t_env *new);
t_env	*ft_lstnew_env(char *line, char *name, char *value);

/*			ERRORS			*/
void	ft_errors_parsing(int err, char *msg, t_shell *shell);

/*			FREE			*/

void	ft_clear_datas(t_data **datas);
void	ft_free_env_list(t_env **env);
