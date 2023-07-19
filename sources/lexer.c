/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 13:05:00 by jubernar          #+#    #+#             */
/*   Updated: 2023/06/22 13:05:02 by jubernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer_iswordstart(char c)
{
	if (c == '-' || c == '.' || c == '=' || c == '/'
		|| c == '~' || c == '(')
		return (1);
	else
		return (0);
}

int	lexer(t_minishell *mini)
{
	int	i;
	int a;

	i = 0;
	a = 0;
	if (mini->error == 1)
		return (1);
	while (mini->input[i] != 0)
	{
		if (mini->input[i] == '<' || mini->input[i] == '>')
		{
			a = token_yield_redir(&(mini->input[i]), mini);
			if (a == -1)
				return 0;
			i = i + a;
		}
		else if (ft_isalnum(mini->input[i]) != 0 || lexer_iswordstart(mini->input[i]) == 1)
		{
			a = token_yield_word(&(mini->input[i]), mini);
			if (a == -1)
				return 0;
			i = i + a;
		}
		else if (mini->input[i] == 39 || mini->input[i] == 34)
		{
			a = token_yield_quote(&(mini->input[i]), mini);
			if (a == -1)
				return 0;
			i = i + a;
		}
		else if (mini->input[i] == '$')
		{
			a = token_yield_var(&(mini->input[i]), mini);
			if (a == -1)
				return 0;
			i = i + a;
		}
		else if (mini->input[i] == '|')
		{
			a = token_yield_pipe(&(mini->input[i]), mini);
			if (a == -1)
				return 0;
			i = i + a;
		}
		else
			i++;
	}
	if (!mini->lexer_table)
		mini->error = 1;
	mini->nb_cmd = mini->pipe + 1;
	return (1);
}
