/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juliensarda <juliensarda@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:15:58 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/21 16:28:34 by juliensarda      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first(t_data *head, t_shell *shell)
{
	head->path = get_cmd_path(head, shell);
	if (!head->path && head->cmd && is_built_in(head) == -1)
		manage_no_path(head, shell, 1);
	else
		first_exec(shell, head, head->path);
}

void	mid(t_data *head, t_shell *shell)
{
	head->path = get_cmd_path(head, shell);
	if (!head->path && head->cmd && is_built_in(head) == -1)
		manage_no_path(head, shell, 1);
	else
		middle_exec(shell, head, shell->pipes[0]);
}

void	last(t_data *head, t_shell *shell)
{
	head->path = get_cmd_path(head, shell);
	if (!head->path && head->cmd && is_built_in(head) == -1)
		manage_no_path(head, shell, 1);
	else
		last_exec(shell, head);
}

void	exec_pipe(t_shell *shell, int num_cmd)
{
	int		i;
	t_data	*head;

	head = shell->datas;
	i = -1;
	pipe(shell->pipes);
	if (num_cmd > 1)
		first(head, shell);
	else
		close(shell->pipes[1]);
	head = head->next;
	while (++i < (num_cmd - 2))
	{
		mid(head, shell);
		head = head->next;
	}
	last(head, shell);
	head = shell->datas;
	ft_wait(head);
}
