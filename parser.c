/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:23:55 by vvarussa          #+#    #+#             */
/*   Updated: 2022/02/28 15:00:37 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_parse_data	parse_in(t_parse_data aux);
t_parse_data	parse_out(t_parse_data aux, t_node *other_pipes);

t_parse_data	parse_assigment(t_parse_data data)
{
	t_node	*node;

	node = *data.token_list;
	if (is_char_in_set('=', node->data))
	{
		if (data.assigment != NULL)
		{
			free(data.assigment->data);
			free(data.assigment);
		}
		data.assigment = node;
		*data.token_list = node->next;
	}
	return (data);
}

void	process_cmd(t_parse_data data)
{
	data.envp = make_envp_from_dict(data.dict);
	if (check_builtin(data) == 1)
		exec_builtin(data);
	else
	{
		data.bin_path = check_command_path(data);
		if (data.bin_path != NULL)
			exec_command(data);
		free(data.bin_path);
	}
	free_str_array(data.args);
	free_str_array(data.envp);
}

t_parse_data	init_parse(t_parse_data aux, t_node **dict)
{
	aux.last_was_pipe = 0;
	aux.assigment = NULL;
	aux.dict = dict;
	return (aux);
}

t_parse_data	pipe_parse(t_parse_data aux)
{
	aux.pipe2[0] = aux.pipe1[0];
	aux.pipe2[1] = aux.pipe1[1];
	pipe(aux.pipe1);
	return (aux);
}

void	parse(t_node *token_list, t_node **dict)
{
	t_node			*sub_token_list;
	t_parse_data	data;

	data = init_parse(data, dict);
	while (token_list != NULL)
	{
		data = pipe_parse(data);
		sub_token_list = split_by_pipe(&token_list);
		data.token_list = &sub_token_list;
		data = parse_in(data);
		data = parse_out(data, token_list);
		// verify_errno(errno, 502, "Sintax error\n");
		// if (verify_errno(errno, 502, "Sintax error\n"))
		// 	return ;
		if (*data.token_list == NULL)
			continue ;
		data = parse_assigment(data);
		data = parse_cmd_and_args(data);
		if (token_list == NULL && data.assigment != NULL && !data.last_was_pipe)
			assign_var(data);
		else if (*data.args != NULL)
			process_cmd(data);
		data.last_was_pipe = 1;
	}
}
