/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/15 20:17:20 by bmoreira         ###   ########.fr       */
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
			tmp = ft_strjoin(path[i], "/");
			cmd_path = ft_strjoin(tmp, cmd);
			free(tmp);
			if (!access(cmd_path, F_OK))
				return (cmd_path);
		}
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}

void	init_data(t_data *data, int count, char **args, char **env)
{
	if (count != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
	data->infile = args[1];
	data->outfile = args[4];
	data->env_path = get_env_path(env);
	data->cmd1 = ft_split(args[2], ' ');
	data->cmd2 = ft_split(args[3], ' ');
	data->path[0] = build_cmd_path(data->env_path, data->cmd1[0]);
	data->path[1] = build_cmd_path(data->env_path, data->cmd2[0]);
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

void	first_child(t_data *data, char **env)
{
	int	fd_infile;
	
	close(data->pipefds[0]);
	dup2(data->pipefds[1], 1);
	close(data->pipefds[1]);
	fd_infile = open(data->infile, O_RDONLY);
	if (fd_infile == -1)
		ft_printf("Error opening infile.\n");
	dup2(fd_infile, 0);
	close(fd_infile);
	execve(data->path[0], data->cmd1, env);
}

void	second_child(t_data *data, char **env)
{
	int	fd_outfile;
	
	close(data->pipefds[1]);
	dup2(data->pipefds[0], 0);
	close(data->pipefds[0]);
	fd_outfile = open(data->outfile, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd_outfile == -1)
		ft_printf("Error opening outfile.\n");
	dup2(fd_outfile, 1);
	close(fd_outfile);
	execve(data->path[1], data->cmd2, env);
}

void	parent_process(t_data *data, char **env)
{
	pipe(data->pipefds);
	data->pid1 = fork();
	if (data->pid1 == -1)
		ft_printf("Error to create first child.\n");
	else if (data->pid1 == 0)
		first_child(data, env);
	data->pid2 = fork();
	if (data->pid2 == -1)
		ft_printf("Error to create second child.\n");
	else if (data->pid2 == 0)
		second_child(data, env);
	if (data->pid1 > 0 && data->pid2 > 0)
	{
		close(data->pipefds[0]);
		close(data->pipefds[1]);
	}
	waitpid(data->pid1, NULL, 0);
	waitpid(data->pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data, argc, argv, envp);
	parent_process(&data, envp);
	cleanup_program(&data);
	return (0);
}
