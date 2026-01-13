/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:35 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/13 20:20:02 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/include/libft.h"

typedef struct s_data
{
	int		fd_infile;
	int		fd_outfile;
	int		pipefds[2];
	char	**path;
	char	*cmd;
}	t_data;

#endif