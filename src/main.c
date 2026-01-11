/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoreira <bmoreira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:58:07 by bmoreira          #+#    #+#             */
/*   Updated: 2026/01/11 17:08:30 by bmoreira         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	validate_args(argc, argv);
	return (0);
}
