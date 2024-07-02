/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 09:19:26 by jsarda            #+#    #+#             */
/*   Updated: 2024/07/02 09:44:59 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_tmp_file(t_data *datas)
{
	int		random_fd;
	size_t	i;
	char	rand_char;
	char	filename[36] = "/tmp/.minishell-XXXXXX";

	i = 11;
	random_fd = open("/dev/urandom", O_RDONLY);
	if (random_fd == -1)
	{
		perror("Error opening /dev/urandom");
		exit(EXIT_FAILURE);
	}
	while (i < 22)
	{
		if (read(random_fd, &rand_char, 1) != 1)
		{
			perror("Error reading /dev/urandom");
			close(random_fd);
			exit(EXIT_FAILURE);
		}
		filename[i] = 'a' + (rand_char % 26);
		i++;
	}
	filename[i] = '\0';
	ft_strlcpy(datas->tmpfile_hd, filename, ft_strlen(datas->tmpfile_hd));
	close(random_fd);
	datas->tmpfile_hd[sizeof(datas->tmpfile_hd)] = '\0';
}
