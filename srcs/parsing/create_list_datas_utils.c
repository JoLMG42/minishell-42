/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list_datas_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:17:27 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/17 11:09:50 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*pre_init_block(void)
{
	t_data	*tmp;

	tmp = malloc(sizeof(struct s_data));
	tmp->cmd = NULL;
	tmp->args = NULL;
	tmp->redir_type = 0;
	tmp->fdin = -1;
	tmp->fdout = -1;
	tmp->namein = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nameout = NULL;
	tmp->is_hd = 0;
	tmp->limiter_hd = NULL;
	tmp->tmpfile_hd = NULL;
	tmp->nb_hd = 0;
	tmp->pid = -42;
	tmp->next = NULL;
	return (tmp);
}

t_data	*ft_lstlast_block(t_data *lst)
{
	t_data	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}
