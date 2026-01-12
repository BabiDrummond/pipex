/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/11 23:05:50 by bmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_args(int count, char **args)
{
	if (count != 5)
		ft_printf("Invalid arguments. "
			"Usage: ./pipex [file1] [cmd1] [cmd2] [file2]\n");
	(void) args;
}

char **get_path(char **envp)
{
	char	**path;
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i] + 5, ':');
			break ;
		}
	}
	i = -1;
	while (path[++i])
		if (ft_strncmp(path[i] + ft_strlen(path[i]) - 1, "/", 1) != 0)
			path[i] = ft_strjoin(path[i], "/");
	return (path);
}

int	main(int argc, char **argv, char **envp)
{
	char **result;
	int i;

	i = 0;
	validate_args(argc, argv);
	result = get_path(envp);
	while (result[i])
	{
		ft_printf("%s\n", result[i]);
		i++;
	}
	return (0);
}
