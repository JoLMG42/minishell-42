/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:11:03 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/11 21:21:55 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_shell *shell)
{
	t_data	*datas;

	datas = shell->datas;
	if (open_files(&datas))
		return (free(datas->tmpfile_hd), 0);
	handle_heredoc(shell, datas);
	datas = shell->datas;
	if (datas->next != NULL)
		exec_pipe(shell);
	else
		exec_simple_cmd(datas, shell);
	return (0);
}
