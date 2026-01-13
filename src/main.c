/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/13 20:55:06 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_args(int count)
{
	if (count != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
}

void	read_files(t_data *data, char *infile, char *outfile)
{
	data->fd_infile = open(infile, O_RDONLY);
	data->fd_outfile = open(outfile, O_WRONLY);
	if (data->fd_infile == -1 || data->fd_outfile == -1)
		ft_printf("Error opening file.\n");
}

void	get_path(t_data *data, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			data->path = ft_split(envp[i] + 5, ':');
			break ;
		}
		i++;
	}
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

void	fork_input(t_data *data, char *cmd_path, char *cmd)
{
	dup2(data->fd_infile, 0);
	dup2(data->pipefds[1], 1);
	close(data->pipefds[0]);
	close(data->pipefds[1]);
	close(data->fd_infile);
	execve(cmd_path, cmd, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	validate_args(argc);
	read_files(&data, argv[1], argv[4]);
	get_path(&data, envp);
	pipe(data.pipefds);
	fork_input(&data, build_cmd(data.path, argv[2]), argv[2]);
	fork_output(&data, build_cmd(data.path, argv[3]), argv[3]);
	ft_matrix_free(data.path);
	return (0);
}
