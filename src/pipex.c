/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/15 20:56:46 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_parent(t_data *data, char **env);
void	create_children(t_data *data, char **env);
void	first_child(t_data *data, char **env);
void	second_child(t_data *data, char **env);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data, argc, argv, envp);
	execute_parent(&data, envp);
	exit_cleanup(&data, 0);
	return (0);
}

void	execute_parent(t_data *data, char **env)
{
	pipe(data->pipefd);
	create_children(data, env);
	waitpid(data->pid1, NULL, 0);
	waitpid(data->pid2, NULL, 0);
}

void	create_children(t_data *data, char **env)
{
	data->pid1 = fork();
	if (data->pid1 == -1)
		exit_cleanup(data, CREATE_FORK);
	else if (data->pid1 == 0)
		first_child(data, env);
	data->pid2 = fork();
	if (data->pid2 == -1)
		exit_cleanup(data, CREATE_FORK);
	else if (data->pid2 == 0)
		second_child(data, env);
	if (data->pid1 > 0 && data->pid2 > 0)
	{
		close(data->pipefd[0]);
		close(data->pipefd[1]);
	}
}

void	first_child(t_data *data, char **env)
{
	int	fd_infile;

	close(data->pipefd[0]);
	dup2(data->pipefd[1], 1);
	close(data->pipefd[1]);
	fd_infile = open(data->infile, O_RDONLY);
	if (fd_infile == -1)
		exit_cleanup(data, OPEN_FILE);
	dup2(fd_infile, 0);
	close(fd_infile);
	execve(data->path[0], data->cmd1, env);
}

void	second_child(t_data *data, char **env)
{
	int	fd_outfile;

	close(data->pipefd[1]);
	dup2(data->pipefd[0], 0);
	close(data->pipefd[0]);
	fd_outfile = open(data->outfile, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd_outfile == -1)
		exit_cleanup(data, OPEN_FILE);
	dup2(fd_outfile, 1);
	close(fd_outfile);
	execve(data->path[1], data->cmd2, env);
}
