/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:35:20 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/03 11:18:15 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_shell *shell)
{
	(void)shell;
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
		printf("%s\n", cwd);
	else
		perror("pwd");
}
