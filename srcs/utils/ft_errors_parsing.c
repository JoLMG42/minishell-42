/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:28:56 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/10 17:07:01 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_errors_parsing(int err, char *msg, t_shell *shell, char *supp)
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
		g_return_satus = 1;
		return ;
	}
}
