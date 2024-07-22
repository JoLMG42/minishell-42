/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 10:54:23 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/22 18:34:17 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned long long int	ft_atol(const char *str)
{
	int						i;
	int						sign;
	unsigned long long int	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while ((str[i] >= 8 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && nb < LONG_MAX)
	{
		nb = nb * 10 + (str[i] - 48);
		i++;
	}
	return (nb * sign);
}

void	print_exit_message(char *message, char *args)
{
	printf("exit\n");
	ft_putstr_fd("minishell: exit: ", 2);
	if (args)
		ft_putstr_fd(args, 2);
	ft_putendl_fd(message, 2);
}

int	ft_isdigit_str(char *c)
{
	int	i;

	i = 0;
	if (c[0] == '-' || c[0] == '+')
		i++;
	while (c[i])
	{
		if (c[i] < '0' || c[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_strslen_skip(char *args)
{
	int	i;

	i = 0;
	while (args[i] == ' ' || args[i] == '-' || args[i] == '+')
		i++;
	i = ft_strlen(args) - i;
	printf("i = %d\n", i);
	return (i);
}

int	check_args(char *args)
{
	size_t	arg_size;

	arg_size = ft_strslen_skip(args);
	if (arg_size > 19)
	{
		print_exit_message(": numeric argument required", args);
		return (1);
	}
	return (0);
}

void	ft_exit(t_data *data, t_shell *shell, char **args)
{
	if (count_args(data->args) > 1 && ft_isdigit_str(data->args[1]) == 1)
	{
		g_return_satus = 2;
		print_exit_message(": numeric argument required", data->args[1]);
		free_child(data, shell, g_return_satus);
		return ;
	}
	if (count_args(data->args) > 2)
	{
		g_return_satus = 1;
		print_exit_message(": too many arguments", NULL);
		return ;
	}
	if (args[1] && check_args(args[1]))
	{
		g_return_satus = ft_atol(args[1]);
		free_child(data, shell, g_return_satus);
		return ;
	}
	if (args[1] && ft_atol(args[1]) <= LONG_MAX)
	{
		g_return_satus = ft_atol(args[1]);
		free_child(data, shell, g_return_satus);
		return ;
	}
	else if (args[1] && ft_atol(args[1]) > LONG_MAX)
	{
		print_exit_message(": numeric argument required", args[1]);
		g_return_satus = ft_atol(args[1]);
		free_child(data, shell, g_return_satus);
		return ;
	}
	printf("exit\n");
	free_child(data, shell, g_return_satus);
}
