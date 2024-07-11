/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:43:07 by juliensarda       #+#    #+#             */
/*   Updated: 2024/07/11 21:46:52 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_error(int err_status, char *msg, char *supp)
{
	write(2, "minishell: ", 12);
	ft_putstr_fd(supp, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	g_return_satus = err_status;
}

void	ft_errors_exec(int err, char *msg, t_shell *shell, char *supp, int err_status)
{
	(void)shell;
	if (err == 0)
		return (write(2, "minishell: ", 12), ft_putstr_fd(msg, 2));
	if (err == 1)
		return (minishell_error(err_status, msg, supp));
}