/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:20:56 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/02 16:40:06 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define APPEND 1 // '>>'
#define OUT 2 // '>'
#define IN 3 // '<'
#define HD 4 // '<<'

typedef	struct	s_env
{
	char		*line;
	char		*name;
	char		*value;

	struct s_env	*next;
}	t_env;

typedef	struct	s_data
{
	char		*cmd;
	char		**args;
	
	int		redir_type;
	int		fdin;
	char		*namein;
	int		fdout;
	char		*nameout;

	int			is_hd;
	char		**limiter_hd;
	int			nb_hd;
	char		*tmpfile_hd;


	struct s_data	*next;
}	t_data;

typedef struct	s_shell
{
	t_env		*envp;
	t_env		*exp;
	t_data		*datas;
}	t_shell;
