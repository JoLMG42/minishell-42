/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:21:58 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/17 15:00:12 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_shell *shell, t_data *data, char cwd[])
{
	char	**new_tab;
	char	*tmp;
	char	*tmp_str;

	new_tab = malloc(sizeof(char *) * 4);
	if (!new_tab)
		return (ft_errors_exec(1, strerror(errno), "malloc", errno));
	new_tab[0] = ft_strdup("export");
	if (!new_tab[0])
		return (ft_errors_exec(1, strerror(errno), "malloc", errno),
			freetab(new_tab));
	tmp = ft_strdup(get_key_value(shell->envp, "PWD"));
	if (!tmp)
		return (ft_errors_exec(1, strerror(errno), "malloc", errno),
			freetab(new_tab));
	tmp_str = ft_strjoin("OLDPWD=", tmp);
	free(tmp);
	if (!tmp_str)
		return (ft_errors_exec(1, strerror(errno), "malloc", errno),
			freetab(new_tab));
	new_tab[1] = tmp_str;
	tmp_str = ft_strjoin("PWD=", getcwd(cwd, PATH_MAX));
	if (!tmp_str)
		return (ft_errors_exec(1, strerror(errno), "malloc", errno),
			freetab(new_tab));
	new_tab[2] = tmp_str;
	new_tab[3] = NULL;
	ft_export(data, shell, new_tab);
	freetab(new_tab);
}

void	ft_cd(t_data *data, t_shell *shell, char **args)
{
	int		cd_ret;
	char	cwd[PATH_MAX];

	(void)cd_ret;
	(void)args;
	if (count_args(data->args) >= 3)
		return (ft_errors_exec(1, " too many arguments", "cd", 1));
	if (!data->args[1])
	{
		if (check_key(shell->envp, "HOME"))
			return (ft_errors_exec(1, " HOME not set", "cd", 1));
		cd_ret = chdir(get_path_value(shell, "HOME"));
		if (cd_ret != 0)
			return (ft_errors_exec(1, strerror(errno), "chdir", errno));
		update_pwd(shell, data, cwd);
		return ;
	}
	cd_ret = chdir(data->args[1]);
	if (cd_ret != 0)
		return (ft_errors_exec(1, strerror(errno), "chdir", errno));
	update_pwd(shell, data, cwd);
}
