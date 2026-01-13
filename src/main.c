/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/13 20:08:28 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_args(int count)
{
	if (count != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
}

void	get_path(t_pipex *pipex, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			pipex->path = ft_split(envp[i] + 5, ':');
			break ;
		}
		i++;
	}
}

void	get_cmd(t_pipex *pipex, char **path, char *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[i])
	{
		if (ft_strncmp(path[i] + ft_strlen(path[i]) - 1, "/", 1) != 0)
		{
			tmp = ft_strjoin(path[i], "/");
			pipex->cmd = ft_strjoin(tmp, cmd);
			free(tmp);
			if (!access(pipex->cmd, F_OK) && ft_printf("found path: %s\n", pipex->cmd))
				break ;
		}
		free(pipex->cmd);
		pipex->cmd = NULL;
		i++;
	}
}

void open_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		ft_printf("Error opening file %s.\n", file_name);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	validate_args(argc);
	get_path(&pipex, envp);
	get_cmd(&pipex, pipex.path, argv[2]);
	ft_matrix_free(pipex.path);
	free(pipex.cmd);
	return (0);
}
