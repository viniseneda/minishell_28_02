/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 12:50:48 by aprotoce          #+#    #+#             */
/*   Updated: 2022/02/28 13:07:30 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printpwd(t_parse_data data)
{
	char	cwd[4096];

	ft_putstr_fd(getcwd(cwd, sizeof(cwd)), data.fd_out);
	ft_putchar_fd('\n', data.fd_out);
	// printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void	changeDir(t_parse_data data)
{
	char	cwd[4096];
	int		status;

	status = chdir(data.args[1]);
	if (ft_strncmp(data.args[1], " ", 1) || !data.args[1])
	{
		getcwd(cwd, 4096);
		change_or_add_value(data.dict, "PWD", getcwd(cwd, sizeof(cwd)));
		return ;
	}
	if (status == 0)
	{
		getcwd(cwd, 4096);
		chdir(cwd);
	}
	else
	{
		printf("%s\n", "Error, you need a correct parameter");
		return ;
	}
	printf("(%s)\n", getcwd(cwd, sizeof(cwd)));
	change_or_add_value(data.dict, "PWD", getcwd(cwd, sizeof(cwd)));
}
