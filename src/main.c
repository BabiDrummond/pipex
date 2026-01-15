/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/14 22:05:42 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_env_path(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

char	*build_cmd_path(char **path, char *cmd)
{
	char	*cmd_path;
	char	*tmp;
	int		i;

	i = 0;
	while (path[i])
	{
		if (ft_strncmp(path[i] + ft_strlen(path[i]) - 1, "/", 1) != 0)
		{
			cmd_path = ft_strjoin(path[i], "/");
			tmp = ft_strjoin(cmd_path, cmd);
			if (!access(tmp, F_OK))
				break ;
		}
		free(cmd_path);
		free(tmp);
		cmd_path = NULL;
		tmp = NULL;
		i++;
	}
	free(tmp);
	tmp = NULL;
	return (cmd_path);
}

void	init_data(t_data *data, char **args, char **env)
{
	data->fd_infile = open(args[1], O_RDONLY);
	data->fd_outfile = open(args[4], O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->fd_infile == -1 || data->fd_outfile == -1)
		ft_printf("Error opening file.\n");
	data->env_path = get_env_path(env);
	data->cmd1 = ft_split(args[2], ' ');
	data->cmd2 = ft_split(args[3], ' ');
	data->path[0] = build_cmd_path(data->env_path, data->cmd1[0]);
	data->path[1] = build_cmd_path(data->env_path, data->cmd2[0]);
	ft_printf("path 1: %s\n", data->path[0]);
	ft_printf("path 2: %s\n", data->path[1]);
}

void	cleanup_program(t_data *data)
{
	ft_matrix_free(data->env_path);
	ft_matrix_free(data->cmd1);
	ft_matrix_free(data->cmd2);
	if (data->path[0])
		free(data->path[0]);
	if (data->path[1])
		free(data->path[1]);
}

void	create_fork(t_data *data)
{
	data->pid1 = fork();
	if (data->pid1 == - 1)
		ft_printf("Error to create fork.\n");
	if (data->pid1 != 0)
	{
		data->pid2 = fork();
		if (data->pid2 == - 1)
			ft_printf("Error to create fork.\n");
	}
	if (data->pid1 > 0 && data->pid2 > 0)
		if (close(data->pipefds[0]) == -1
			|| close(data->pipefds[1]) == -1)
			ft_printf("Error to close pipefds.\n");
}

void	execute_cmd(t_data *data)
{
	if (data->pid1 == 0)
	{
		dup2(data->fd_infile, 0);
		dup2(data->pipefds[1], 1);
		close(data->fd_infile);
		execve(data->path[0], data->cmd1, NULL);
	}
	else if (data->pid2 == 0)
	{
		dup2(data->fd_outfile, 1);
		dup2(data->pipefds[0], 0);
		close(data->fd_outfile);
		execve(data->path[1], data->cmd2, NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
	init_data(&data, argv, envp);
	pipe(data.pipefds);
	create_fork(&data);
	execute_cmd(&data);
	waitpid(data.pid1, 0, 0);
	waitpid(data.pid2, 0, 0);
	cleanup_program(&data);
	return (0);
}
