/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 10:46:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/16 10:13:05 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		if (str[i] == '\0')
			return (-1);
		while (str[i] == 'n')
			i++;
		if (str[i] != '\0')
			return (-1);
		return (1);
	}
	return (-1);
}

void	ft_echo(t_data *data, t_shell *shell, char **args)
{
	int	i;
	int	n_option;

	(void)data;
	(void)shell;
	i = 1;
	n_option = 0;
	while (args[i] && check_arg(args[i]) == 1)
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
}
