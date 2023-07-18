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

int	token_yield_word(char *token, t_minishell *mini)
{
	int		len;
	char	*str;

	len = 0;
	while (token[len] != ' ' && token[len] != 0 && token[len] != '|'
		&& token[len] != 34 && token[len] != 39)
		len++;
	if (token[len] == 34 || token[len] == 39)
	{
		len++;
		while (token[len] != 34 && token[len] != 39 && token[len] != 0)
			len++;
		if (token[len] != 0)
			len++;
	}
	MALLOC_OR_ZERO(str, sizeof (char) * (len + 1));
	ft_strlcpy(str, token, len + 1);
	if (!add_to_list(&mini->lexer_table, str))
		return (0);
	return (len);
}

int	token_yield_quote(char *token, t_minishell *mini)
{
	int		i;
	int		len;
	char	*str;

	i = 1;
	while (token[i] != token[0] && token[i] != 0)
		i++;
	len = i;
	if (token[i] == token[0])
		len++;
	MALLOC_OR_ZERO(str, sizeof (char) * (len + 1));
	ft_strlcpy(str, token, len + 1);
	if (!add_to_list(&mini->lexer_table, str))
		return (0);
	return (len);
}

int	token_yield_var(char *token, t_minishell *mini)
{
	char	*str;
	int		i;
	int		len;

	len = 0;
	while (token[len] != ' ' && token[len] != 0)
		len++;
	MALLOC_OR_ZERO(str, sizeof (char) * (len + 1));
	i = 0;
	ft_strlcpy(str, token, len + 1);
	if (!add_to_list(&mini->lexer_table, str))
		return (0);
	return (len);
}

int	token_yield_pipe(char *token, t_minishell *mini)
{
	char	*str;

	mini->pipe += 1;
	MALLOC_OR_ZERO(str, sizeof (char) * (2));
	ft_strlcpy(str, token, 2);
	if (!add_to_list(&mini->lexer_table, str))
		return (0);
	return (1);
}

int	token_yield_redir(char *redir, t_minishell *mini)
{
	char	*str;

	if (redir[0] != redir[1])
	{
		MALLOC_OR_ZERO(str, sizeof (char) * (2));
		ft_strlcpy(str, redir, 2);
		if (!add_to_list(&mini->lexer_table, str))
			return (0);
		return (1);
	}
	if (redir[0] == redir[1])
	{
		MALLOC_OR_ZERO(str, sizeof (char) * (3));
		ft_strlcpy(str, redir, 3);
		if (!add_to_list(&mini->lexer_table, str))
			return (0);
		return (2);
	}
	return (0);
}
