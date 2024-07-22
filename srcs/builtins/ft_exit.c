/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 10:54:23 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/22 17:01:44 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_exit(t_data *data, t_shell *shell, char **args)
{
	(void)args;
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
	if (args[1])
		g_return_satus = ft_atoi(args[1]);
	printf("exit\n");
	free_child(data, shell, g_return_satus);
}
