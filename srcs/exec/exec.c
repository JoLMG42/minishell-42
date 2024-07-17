/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/17 14:17:16 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_shell *shell)
{
	t_data	*datas;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	datas = shell->datas;
	handle_heredoc(shell, datas);
	open_files(&datas);
	datas = shell->datas;
	if (datas->next != NULL)
		exec_pipe(shell);
	else
		exec_simple_cmd(datas, shell);
	return (0);
}
