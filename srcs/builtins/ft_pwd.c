/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:35:20 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/19 22:23:10 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_data *data, t_shell *shell, char **args)
{
	char	cwd[PATH_MAX];

	(void)shell;
	(void)data;
	(void)args;
	if (getcwd(cwd, PATH_MAX))
		ft_putendl_fd(cwd, data->fdout);
	else
		perror("pwd");
}
