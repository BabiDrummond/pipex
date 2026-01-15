/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:35 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/15 20:54:58 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "../libft/include/libft.h"

typedef enum e_error
{
	INVALID_ARGS = 1,
	OPEN_FILE = 2,
	CREATE_FORK = 3,
	INVALID_CMD = 4
}	t_error;

typedef struct s_data
{
	char	*infile;
	char	*outfile;
	char	**env_path;
	char	**cmd1;
	char	**cmd2;
	char	*path[2];
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
}	t_data;

// Pipex
void	execute_parent(t_data *data, char **env);
void	create_children(t_data *data, char **env);
void	first_child(t_data *data, char **env);
void	second_child(t_data *data, char **env);

// Utils
void	init_data(t_data *data, int count, char **args, char **env);
void	exit_cleanup(t_data *data, t_error error);
char	**get_env_path(char **envp);
char	*build_cmd_path(char **path, char *cmd);

#endif