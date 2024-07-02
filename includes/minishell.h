/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:35:53 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/02 17:44:34 by jtaravel         ###   ########.fr       */
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
void	exec(t_shell *datas);
void	get_tmp_file(t_data *datas);
void	heredoc(char *eof, char *file_name);
void	ft_pwd(t_data *datas);
void	exec_built_in(t_data *datas);
int		is_built_in(t_data *datas);
int		check_if_redir(t_data *datas);
void	exec_simple_cmd(t_data *datas);
void	handle_redir(t_data *datas);
char	**ft_split_quotes(char *s, char c);
