/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:51:28 by bmoreira          #+#    #+#             */
/*   Updated: 2026/02/08 00:35:20 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_data(t_data *data, int count, char **args, char **env);
void	exit_cleanup(t_data *data, t_error error);
char	**get_env_path(char **envp);
char	*build_cmd_path(char **path, char *cmd);

void	init_data(t_data *data, int count, char **args, char **env)
{
	if (count != 5)
		exit_cleanup(data, INVALID_ARGS);
	data->infile = args[1];
	data->outfile = args[4];
	data->env_path = get_env_path(env);
	data->cmd1 = ft_split(args[2], ' ');
	data->cmd2 = ft_split(args[3], ' ');
	if (!data->cmd1 || !data->cmd2)
		exit_cleanup(data, INVALID_CMD);
	data->path[0] = build_cmd_path(data->env_path, data->cmd1[0]);
	data->path[1] = build_cmd_path(data->env_path, data->cmd2[0]);
	if (!data->path[0] || !data->path[1])
		exit_cleanup(data, INVALID_CMD);
}

void	exit_cleanup(t_data *data, t_error error)
{
	if (error == INVALID_ARGS)
	{
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
		exit (0);
	}
	else if (error == OPEN_FILE)
		ft_printf("Error opening file.\n");
	else if (error == CREATE_FORK)
		ft_printf("Error to create child.\n");
	else if (error == INVALID_CMD)
		ft_printf("Invalid command provided.\n");
	ft_split_free(data->env_path);
	ft_split_free(data->cmd1);
	ft_split_free(data->cmd2);
	if (data->path[0])
		free(data->path[0]);
	if (data->path[1])
		free(data->path[1]);
	exit(0);
}

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
