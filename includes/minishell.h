/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:35:53 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/03 18:44:30 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "structs.h"
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/random.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char	*get_content_env(t_env **env, char *find);
char	*expander(char *str, t_env **env, int i, char *res);
int		parse_input(char *input, t_shell *shell);
void	freetab(char **tab);
int		exec(t_shell *datas);
void	get_tmp_file(t_data *datas);
void	heredoc(char *eof, char *file_name);
void	ft_pwd(t_shell *shell);
void	exec_built_in(t_shell *shell);
int		is_built_in(t_shell *shell);
int		check_if_redir(t_data *datas);
void	exec_simple_cmd(t_shell *datas);
void	handle_redir(t_data *datas);
char	*get_cmd_path(t_data *data, t_shell *shell);
char	**create_char_env(t_env *env, int env_size);
int		get_env_list_size(t_env *list);

/*			UTILS			*/

char	**ft_split_quotes(char *s, char c);
void	exec_pipe(t_data *datas, t_shell *shell);
void	ft_env(t_shell *shell);
void	print_env(t_env *list);
int		ft_strslen_tab_until(char **tab, int pos);
