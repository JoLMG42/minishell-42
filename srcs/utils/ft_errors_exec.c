/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:05:22 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 13:10:12 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_errors_exec(int err, char *msg, t_shell *shell, char *supp)
{
	(void)shell;
	if (err == 0)
		return (write(2, "minishell: ", 11), ft_putstr_fd(msg, 2));
	if (err == 1)
	{
		write(2, "minishell: ", 11);
		ft_putstr_fd(supp, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(msg, 2);
		g_return_satus = 127;
		return ;
	}
}