/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 10:46:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/10 22:47:24 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i = 1;
		while (str[i] == 'n')
			i++;
	}
	if (str[i])
		return (-1);
	return (1);
}

void	ft_echo(t_data *data, t_shell *shell, char **args)
{
	int	i;
	int	n_option;
	(void)shell;
	(void)args;
	i = 0;
	n_option = 0;
	while (data->args[++i] && check_arg(data->args[i]) == 1)
		n_option = 1;
	while (data->args[i] && ft_strncmp(data->args[i], "-n", ft_strlen(data->args[i])) == 0)
	{
		n_option = 1;
		i++;
	}
	while (data->args[i])
	{
		ft_putstr_fd(data->args[i], 1);
		if (data->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
}
