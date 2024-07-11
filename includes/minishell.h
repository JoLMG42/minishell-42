/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:35:53 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/11 19:44:40 by juliensarda      ###   ########.fr       */
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
void		ft_pwd(t_data *data, t_shell *shell, char **args);
void		ft_env(t_data *data, t_shell *shell, char **args);
void		ft_export(t_data *data, t_shell *shell, char **args);
void		ft_echo(t_data *data, t_shell *shell, char **args);
void		ft_exit(t_data *data, t_shell *shell, char **args);
void        ft_cd(t_data *data, t_shell *shell, char **args);
void        ft_unset(t_data *data, t_shell *shell, char **args);

char		*expander(char *str, t_env **env, int i, char *res);
int			parse_input(char *input, t_shell *shell);
void		freetab(char **tab);
int			exec(t_shell *datas);
void		get_tmp_file(t_data *datas);
int			heredoc(t_data *data, t_shell *shell, char *eof, char *file_name);
void		exec_built_in(t_data *datas, t_shell *shell);
int			is_built_in(t_data *data);
int			check_if_redir(t_data *datas);
void		exec_simple_cmd(t_data *data, t_shell *shell);
void		handle_redir(t_shell *shell, t_data *datas);
char		*get_cmd_path(t_data *data, t_shell *shell);
char	    *get_path_value(t_shell *datas, char *key);
char	    *get_key_value(t_env *env, char *key);
int	        check_key(t_env *env, char *key);

/*			PARSING / PARSING UTILS			*/

int		create_list(char *input, t_data **datas, t_shell *shell);
t_data	*parse_block(char *str, t_data *datas, t_shell *shell, int flag);
char	**erase_split_parse_block(char **split, t_shell *shell, char *s, int *i);
t_data	*pre_init_block(void);
t_data	*ft_lstlast_block(t_data *lst);
int	block_add_back(t_data **alst, t_data *new);
char	*ft_recreate_input(char *str, char **tab, char *s, t_shell *shell);
void	init_redir_arrays(t_data *datas, char *tmp_str);
t_data	*end_init_one_block(t_data *datas, char **tab, char *s, t_shell *shell);
int	cut_parse_block_loop_1(char **split, int i, t_data *datas, t_shell *shell);
int	ccut_parse_block_loop_1(char **split, int i, t_data *datas, t_shell *shell);
int	cut_parse_block_loop_2(char **split, int i, t_data *datas, t_shell *shell);
int	ccut_parse_block_loop_2(char **split, int i, t_data *datas, t_shell *shell);
int	check_syntaxes(char *str);
int	count_operators(char *input);
int	cut_delete_quote_loop(char *str, int i, int *dq, int *sq);
int	recup_second_quote(char *str, int i, int mode);
char	*cut_delete_quote(char *str, int pos1, int pos2);
char	*delete_extra_quotes(char *str, int i, int dq, int sq);
int	count_redir_operator(char *str, int mode);
int	count_hd_operator(char *str);
int	open_files(t_data **datas);

/*			UTILS ENV			*/
char		**create_char_env(t_env *env, int env_size);
void		print_env(t_env *list);
int			get_env_list_size(t_env *list);
char		*get_content_env(t_env **env, char *find);
void		ft_print_exp(t_env *exp);
char	*ft_erase(char *str, int pos, int len);

/*			UTILS	STR/TAB		*/

char		**ft_split_quotes(char *s, char c);
void	exec_pipe(t_shell *shell);
int			ft_strslen_tab_until(char **tab, int pos);
char		**ft_erase_in_tab(char **tab, int pos, int pos2);
int			ft_tablen(char **tab);
size_t		count_args(char **args);
void		ft_lstadd_back_env(t_env **alst, t_env *new);
t_env		*ft_lstnew_env(char *line, char *name, char *value);
char		*ft_wildcards(char *str);
void		ft_dup(t_data *datas);

/*			ERRORS			*/
void		ft_errors_parsing(int err, char *msg, t_shell *shell, char *supp);
void	ft_errors_exec(int err, char *msg, t_shell *shell, char *supp, int err_status);

/*			FREE			*/

void		ft_clear_datas(t_data **datas);
void		ft_free_env_list(t_env **env);
void		free_child(t_data *data, t_shell *shell, int exit_status);
void	    free_hd_file(t_data **data, int mode);
extern int	g_return_satus;
