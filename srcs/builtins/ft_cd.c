/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:21:58 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/10 23:23:28 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_shell *shell, t_data *data)
{
	char	**new_tab;
	char	*tmp;
	char	cwd[PATH_MAX];

	new_tab = malloc(sizeof(char *) * 4);
	if (!new_tab)
		return ;
    //ft_error("malloc : ", strerror(errno), 1, data)
	new_tab[0] = ft_strdup("export");
	if (!new_tab[0])
		return ;
    //safe_malloc(new_tab, data)
	tmp = ft_strdup(get_key_value(shell->envp, "PWD"));
	if (!tmp)
		return ;
    // safe_malloc(new_tab, data), free(tmp)
	new_tab[1] = ft_strdup(ft_strjoin("OLDPWD=", tmp));
	free(tmp);
	if (!new_tab[1])
		return ;
    // safe_malloc(new_tab, data)
	new_tab[2] = ft_strdup(ft_strjoin("PWD=", getcwd(cwd, PATH_MAX)));
	if (!new_tab[2])
		return ;
    // safe_malloc(new_tab, data
	new_tab[3] = NULL;
	ft_export(data, shell, new_tab);
	freetab(new_tab);
}

void	ft_cd(t_data *data, t_shell *shell, char **args)
{
	int	cd_ret;
    (void)cd_ret;
    (void)args;
	if (count_args(data->args) >= 3)
		return ;
        //ft_error("cd: too many arguments", NULL, 1, data)
	if (!data->args[1])
	{
		if (check_key(shell->envp, "HOME"))
			return ;
        //ft_error("cd : HOME not set", NULL, 1, data)
		cd_ret = chdir(get_path_value(shell, "HOME"));
		//if (cd_ret != 0)
			//ft_error("cd : ", strerror(errno), 1, data);
		update_pwd(shell, data);
		return ;
	}
	cd_ret = chdir(data->args[1]);
	//if (cd_ret != 0)
	//	ft_error("cd : ", strerror(errno), 1, data);
	update_pwd(shell, data);
}