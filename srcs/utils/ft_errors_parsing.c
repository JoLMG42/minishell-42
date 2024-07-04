/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:28:56 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/04 12:36:42 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_errors_parsing(int err, char *msg, t_shell *shell)
{
	(void)shell;
	if (err == 0)
		return (write(2, "minishell: ", 11),ft_putstr_fd(msg, 2));
}