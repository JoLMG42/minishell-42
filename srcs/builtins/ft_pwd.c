/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:35:20 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/16 10:13:57 by jsarda           ###   ########.fr       */
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
		printf("%s\n", cwd);
	else
		perror("pwd");
}
