/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/16 09:27:28 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_shell *shell)
{
	t_data	*datas;

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
