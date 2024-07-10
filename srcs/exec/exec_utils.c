/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:19:26 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/10 12:09:06 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_tmp_file(t_data *datas)
{
	size_t	i;
	int tmp;
	char	rand_char;
	char	filename[36] = "/tmp/.minishell-XXXXXX";

	i = 11;
	tmp = open("/dev/urandom", O_RDONLY);
	if (tmp == -1)
	{
		perror("Error opening /dev/urandom");
		return ;
	}
	while (i < 22)
	{
		if (read(tmp, &rand_char, 1) != 1)
		{
			perror("Error reading /dev/urandom");
			close(tmp);
			return ;
		}
		filename[i] = 'a' + (rand_char % 26);
		i++;
	}
	filename[i] = '\0';
	if (datas->tmpfile_hd)
		free(datas->tmpfile_hd);
	datas->tmpfile_hd = ft_strdup(filename);
}

int	is_built_in(t_data *data)
{
	int		i;
	char	*built_in[NUM_OF_BUILT_INS];
	if (!data->cmd)
		return (-1);
	built_in[0] = "pwd";
	built_in[1] = "env";
	built_in[2] = "export";
	built_in[3] = "echo";
	built_in[4] = "exit";
	// built_in[3] = "cd";
	// built_in[5] = "unset";
	i = 0;
	while (i < NUM_OF_BUILT_INS)
	{
		if (ft_strncmp(data->cmd, built_in[i], ft_strlen(built_in[i])) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	check_if_redir(t_data *datas)
{
	t_data	*current;

	current = datas;
	while (current)
	{
		if (current->redir_type_out >= 1 && current->redir_type_out <= 4)
			return (0);
		if (current->redir_type_in >= 1 && current->redir_type_in <= 4)
			return (0);
		current = current->next;
	}
	return (1);
}

void	exec_built_in(t_data *datas, t_shell *shell)
{
	int		index;
	void	(*built_in_funcs[NUM_OF_BUILT_INS])(t_data *, t_shell *);

	built_in_funcs[0] = &ft_pwd;
	built_in_funcs[1] = &ft_env;
	built_in_funcs[2] = &ft_export;
	built_in_funcs[3] = &ft_echo;
	built_in_funcs[4] = &ft_exit;
	// built_in_funcs[3] = &ft_cd;
	// built_in_funcs[5] = &ft_unset;
	index = is_built_in(datas);
	if (index == -1)
		return ;
	built_in_funcs[index](datas, shell);
}

void	ft_dup(t_data *data)
{
	dup2(data->fdin, STDIN_FILENO);
	dup2(data->fdout, STDOUT_FILENO);
}
