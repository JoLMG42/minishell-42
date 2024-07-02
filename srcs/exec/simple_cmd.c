/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:24:02 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/02 14:56:17 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_simple_cmd(t_data *datas)
{
	// pid_t	pid;
	t_data	*current;

	current = datas;
	if (is_built_in(current) != -1)
	{
		if (check_if_redir(current) == 0 || current->is_hd == 1)
		{
			while (current)
			{
				// // handle_redir(current);
				// current = current->next;
			}
		}
		exec_built_in(datas);
		return ;
	}
	// pid = fork();
	// if (pid < 0)
	// 	perror("fork");
	// else if (pid == 0)
	// 	exec_child_process(data, list);
	// else
	// 	exec_parent_process(pid);
}
