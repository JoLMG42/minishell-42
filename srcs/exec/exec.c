/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/18 11:23:08 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_shell *shell)
{
	t_data	*datas;
	int		num_cmd;

	num_cmd = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	datas = shell->datas;
	handle_heredoc(shell, datas);
	open_files(&datas);
	datas = shell->datas;
	if (datas->next != NULL)
	{
		num_cmd = ft_lstsize_cmd(shell->datas);
		exec_pipe(shell, num_cmd);
	}
	else
		exec_simple_cmd(datas, shell);
	return (0);
}
