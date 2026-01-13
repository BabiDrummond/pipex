/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/13 20:25:53 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_args(int count)
{
	if (count != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
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

void	build_cmd(t_data *data, char **path, char *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[i])
	{
		if (ft_strncmp(path[i] + ft_strlen(path[i]) - 1, "/", 1) != 0)
		{
			tmp = ft_strjoin(path[i], "/");
			data->cmd = ft_strjoin(tmp, cmd);
			free(tmp);
			if (!access(data->cmd, F_OK) && ft_printf("found path: %s\n", data->cmd))
				break ;
		}
		free(data->cmd);
		data->cmd = NULL;
		i++;
	}
}

void	read_files(t_data *data, char *infile, char *outfile)
{
	data->fd_infile = open(infile, O_RDONLY);
	data->fd_outfile = open(outfile, O_WRONLY);
	if (data->fd_infile == -1 || data->fd_outfile == -1)
		ft_printf("Error opening file.\n");
}

void	cleanup_program(t_data *data)
{
	ft_matrix_free(data->path);
	if (data->cmd)
		free(data->cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	validate_args(argc);
	read_files(&data, argv[1], argv[4]);
	get_path(&data, envp);
	build_cmd(&data, data.path, argv[2]);
	cleanup_program(&data);
	return (0);
}
