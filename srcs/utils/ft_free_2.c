/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:02:40 by jtaravel          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/07/19 17:13:12 by jsarda           ###   ########.fr       */
=======
/*   Updated: 2024/07/19 16:06:31 by jtaravel         ###   ########.fr       */
>>>>>>> afe0d436202726e0b3c753c84c547cabc99b3771
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_hd_file(t_data **data, int mode)
{
	t_data	*tmp_data;

	tmp_data = *data;
	while (tmp_data)
	{
		if (mode == 1 && tmp_data->is_hd)
		{
			if (tmp_data->tmpfile_hd)
				free(tmp_data->tmpfile_hd);
			tmp_data->tmpfile_hd = NULL;
		}
		else if (mode == 2 && tmp_data->is_hd)
			unlink(tmp_data->tmpfile_hd);
		tmp_data = tmp_data->next;
	}
}

void	free_child(t_data *data, t_shell *shell, int exit_status)
{
	ft_free_env_list(&(shell->envp));
	ft_free_env_list(&(shell->exp));
	free(data->path);
	data->path = NULL;
	free_hd_file(&data, 1);
	ft_clear_datas(&(shell->datas));
	free(shell);
	exit(exit_status);
}

void	ft_recup(t_shell *shell)
{
	static t_shell *tmp_shell;

	if (shell)
		tmp_shell = shell;
	else
	{
		ft_free_env_list(&(tmp_shell->envp));
		ft_free_env_list(&(tmp_shell->exp));
		ft_clear_datas(&(tmp_shell->datas));
		free(tmp_shell);
	}
}
