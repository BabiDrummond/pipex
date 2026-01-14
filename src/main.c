/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/13 22:16:29 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_path(char **envp)
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

char	*build_cmd(char **path, char *cmd)
{
	char	*tmp;
	char	*cmd_path;
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
				break ;
		}
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (cmd_path);
}

void	input_cmd(t_data *data)
{
	dup2(data->fd_infile, 0);
	dup2(data->pipefds[1], 1);
	close(data->pipefds[0]);
	close(data->pipefds[1]);
	close(data->fd_infile);
	execve(data->path[0], &data->cmd[0], NULL);
}

void	output_cmd(t_data *data)
{
	dup2(data->fd_outfile, 1);
	dup2(data->pipefds[0], 0);
	close(data->pipefds[0]);
	close(data->pipefds[1]);
	close(data->fd_outfile);
	execve(data->path[1], &data->cmd[1], NULL);
}
void	init_data(t_data *data, char **args, char **env)
{
	data->fd_infile = open(args[1], O_RDONLY);
	data->fd_outfile = open(args[4], O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->fd_infile == -1 || data->fd_outfile == -1)
		ft_printf("Error opening file.\n");
	data->env_path = get_path(env);
	data->cmd[0] = args[2];
	data->cmd[1] = args[3];
	data->path[0] = build_cmd(data->env_path, data->cmd[0]);
	data->path[1] = build_cmd(data->env_path, data->cmd[1]);
}

void	cleanup_program(t_data *data)
{
	ft_matrix_free(data->env_path);
	if (data->path[0])
		free(data->path[0]);
	if (data->path[1])
		free(data->path[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
	init_data(&data, argv, envp);
	pipe(data.pipefds);
	//input_cmd(&data);
	//output_cmd(&data);
	cleanup_program(&data);
	return (0);
}
