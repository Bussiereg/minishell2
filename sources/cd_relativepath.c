/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_relativepath.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbussier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:45:10 by gbussier          #+#    #+#             */
/*   Updated: 2023/07/12 17:45:14 by gbussier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cd_relpath1(char *cmd, t_minishell *mini)
{
	char	*temp1;
	char	*temp2;
	char	*temp3;

	temp1 = ft_strtrim(cmd, "./");
	temp2 = ft_strdup(getenv_mini("PWD", mini));
	temp3 = ft_strjoin(temp2, (char *)"/");
	temp2 = ft_strjoin(temp3, temp1);
	free(temp1);
	temp3 = ft_strtrim(temp2, " /");
	free(temp2);
	temp2 = ft_strjoin(ft_strdup("/"), temp3);
	free(temp3);
	return (temp2);
}

char	*cd_relpath2(char *cmd, t_minishell *mini)
{
	char	*temp1;
	char	*temp2;
	char	*temp3;
	int		count_trim;

	temp1 = ft_strtrim(cmd, "./");
	count_trim = ft_count_trim(cmd);
	temp2 = origine_path(count_trim, mini, 0, 0);
	temp3 = ft_strjoin(temp2, (char *)"/");
	temp2 = ft_strjoin(temp3, temp1);
	temp3 = ft_strtrim(temp2, " /");
	ft_free_success(temp1, temp2, NULL, NULL);
	temp1 = ft_strjoin(ft_strdup("/"), temp3);
	free(temp3);
	return (temp1);
}

char	*cd_relpath3(char *cmd, t_minishell *mini)
{
	char	*temp1;
	char	*temp2;
	char	*temp3;
	char	*relative_path;

	temp3 = NULL;
	temp2 = NULL;
	relative_path = NULL;
	temp1 = ft_strtrim(cmd, " /");
	if (ft_strlen(getenv_mini("PWD", mini)) == 1)
		relative_path = ft_strjoin(ft_strdup("/"), temp1);
	else
	{
		temp2 = origine_path(0, mini, 0, 0);
		temp3 = ft_strjoin(temp2, (char *)"/");
		relative_path = ft_strjoin(temp3, temp1);
	}
	free(temp1);
	return (relative_path);
}

char	*cd_relpath4(t_minishell *mini)
{
	char	*temp1;
	char	*relative_path;

	relative_path = origine_path(1, mini, 0, 0);
	temp1 = ft_strtrim(relative_path, " /");
	free(relative_path);
	relative_path = ft_strjoin(ft_strdup("/"), temp1);
	free(temp1);
	return (relative_path);
}

char	*ft_relative_path_cd(char *cmd, t_minishell *mini)
{
	char	*relative_path;

	relative_path = NULL;
	if (ft_strncmp(cmd, "./", 2) == 0)
		relative_path = cd_relpath1(cmd, mini);
	else if (ft_strncmp(cmd, "../", 3) == 0)
		relative_path = cd_relpath2(cmd, mini);
	else if (ft_isalnum(cmd[0]) != 0)
		relative_path = cd_relpath3(cmd, mini);
	else if ((ft_strncmp(cmd, "..", 2) == 0) && ft_strlen(cmd) == 2)
		relative_path = cd_relpath4(mini);
	else if ((cmd[0] == '.') && (ft_strlen(cmd) == 1))
		relative_path = ft_strdup(getenv_mini("PWD", mini));
	else if ((cmd[0] == '-' && ft_strlen(cmd) == 1))
		relative_path = ft_strdup(getenv_mini("OLDPWD", mini));
	else if ((cmd[0] == '~' && ft_strlen(cmd) == 1))
		relative_path = ft_strdup(getenv_mini("HOME", mini));
	else if ((cmd[0] == '.') && (ft_strlen(cmd) > 1))
		relative_path = cd_relpath3(cmd, mini);
	return (relative_path);
}
