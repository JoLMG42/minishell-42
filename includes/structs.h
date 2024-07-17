/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarda <jsarda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:20:56 by jtaravel          #+#    #+#             */
/*   Updated: 2024/07/17 14:52:45 by jsarda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define APPEND 1 // '>>'
# define OUT 2    // '>'
# define IN 3     // '<'
# define HD 4     // '<<'

# define NUM_OF_BUILT_INS 7

typedef struct s_syntax
{
	int		sq_opened;
	int		dq_opened;
}			t_s;

typedef struct s_env
{
	char			*line;
	char			*name;
	char			*value;

	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	char			*cmd;
	char			*path;
	char			**args;

	int				redir_type_in;
	int				redir_type_out;
	int				fdin;
	char			**namein;
	int				nb_in;
	int				fdout;
	char			**nameout;
	int				nb_out;
	int				pid;
	int				status;

	int				is_hd;
	char			**limiter_hd;
	int				nb_hd;
	char			*tmpfile_hd;
	struct s_data	*next;
}					t_data;

typedef struct s_shell
{
	t_env			*envp;
	t_env			*exp;
	t_data			*datas;
	int				pipes[2];
}					t_shell;

#endif
