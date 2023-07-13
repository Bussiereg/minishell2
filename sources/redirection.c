/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbussier <gbussier@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 19:06:50 by gbussier          #+#    #+#             */
/*   Updated: 2023/07/13 19:06:51 by gbussier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_convert_dollar(t_minishell mini, char *line)
{
	char	 **line_split;
	char	*temp;
	char	*result;
	int i;

	i = 0;
	temp = NULL;
	result = NULL;
	line_split = ft_split(line, ' ');
	while(line_split[i] != NULL)
	{
		if (ft_strchr(line_split[i], '$') != NULL)
		{
			temp = ft_strdup(getenv_mini(line_split[i] + 1, &mini));
			if (temp != NULL)
			{
				free(line_split[i]);
				line_split[i] = malloc(sizeof(char) * ft_strlen(temp));
				ft_strlcpy(line_split[i], temp, ft_strlen(temp));
			}
			else
			{
				free(line_split[i]);
				line_split[i] = malloc(sizeof(char) * 2);
				ft_strlcpy(line_split[i], (char *)"\n", 2);
			}
		}
		i++;
	}
	i = 0;
	result = NULL;
	while(line_split[i] != NULL)
	{
		result = ft_strjoin(result, line_split[i]);
		i++;
	}
	return (result);
}


void	here_doc_put_in(char *limiter, int *fds, t_minishell mini)
{
	char	*heredoc_line;
	char	*heredoc_line2;

	close(fds[0]);
	while (1)
	{
		heredoc_line = readline("heredoc> ");
		if (ft_strncmp(heredoc_line, limiter, ft_strlen(limiter)) == 0)
		{
			free(heredoc_line);
			exit(EXIT_SUCCESS);
		}
		if (ft_strrchr(heredoc_line2, '$') != NULL)
		{
			printf("test\n");
			heredoc_line2 = heredoc_convert_dollar(mini, heredoc_line);
		}
		else
			heredoc_line2 = ft_strdup(heredoc_line);
		ft_putendl_fd(heredoc_line2, fds[1]);
		ft_free_success(heredoc_line, heredoc_line2, NULL, NULL);
	}
}

void	here_doc(char *limiter, t_minishell mini)
{
	int		fds[2];
	pid_t	pid;

	if (pipe(fds) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == EXIT_SUCCESS)
		here_doc_put_in(limiter, fds, mini);
	else
	{
		wait(NULL);
		close(fds[1]);
		dup2(fds[0], 0);
	}
}

void	here_doc_insert(char *limiter, t_minishell mini)
{
	int		fds[2];
	pid_t	pid;

	if (pipe(fds) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == EXIT_SUCCESS)
		here_doc_put_in(limiter, fds, mini);
	else
	{
		wait(NULL);
		close(fds[1]);
	}
}

int	input_redirection(t_minishell mini)
{
	int		fd_infile;
	t_redir	*redir;

	redir = mini.redir_start;
	if (!redir)
		return (EXIT_SUCCESS);
	if (redir->type == 1)
	{
		fd_infile = open(redir->file, O_RDONLY, 0777);
		if (fd_infile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir->file);
			return (EXIT_FAILURE);
		}
		if (dup2(fd_infile, 0) == -1)
		{
			perror(NULL);
			return (EXIT_FAILURE);
		}
		close(fd_infile);
	}
	else if (redir->type == 2)
		here_doc(redir->file, mini);
	return (EXIT_SUCCESS);
}

int	output_redirection(t_minishell mini)
{
	int		fd_outfile ;
	t_redir	*redir;

	fd_outfile = 0;
	redir = mini.redir_end;
	if (!redir)
		return (EXIT_SUCCESS);
	if (redir->type)
	{
		if (redir->type == 1)
		{
			fd_outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd_outfile == -1)
			{
				perror(NULL);
				return (EXIT_FAILURE);
			}
		}
		else if (redir->type == 2)
		{
			fd_outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (fd_outfile == -1)
			{
				perror(NULL);
				return (EXIT_FAILURE);
			}
		}
		if (dup2(fd_outfile, 1) == -1)
		{
			perror(NULL);
			return (EXIT_FAILURE);
		}
		close(fd_outfile);
	}
	return (EXIT_SUCCESS);
}

int	infile_insert(t_minishell mini)
{
	int		fd_infile;
	t_redir	*redir;

	redir = mini.redir_start;
	if (!redir)
		return (EXIT_SUCCESS);
	if (redir->type == 1)
	{
		fd_infile = open(redir->file, O_RDONLY, 0777);
		if (fd_infile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir->file);
			return (EXIT_FAILURE);
		}
		close(fd_infile);
	}
	else if (redir->type == 2)
		here_doc(redir->file, mini);
	return (EXIT_SUCCESS);
}

int	outfile_insert(t_minishell mini)
{
	int		fd_outfile;
	t_redir	*redir;

	fd_outfile = 0;
	redir = mini.redir_end;
	if (!redir)
		return (EXIT_SUCCESS);
	if (redir->type)
	{
		if (redir->type == 1)
		{
			fd_outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd_outfile == -1)
			{
				perror(NULL);
				return (EXIT_FAILURE);
			}
		}
		else if (redir->type == 2)
		{
			fd_outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (fd_outfile == -1)
			{
				perror(NULL);
				return (EXIT_FAILURE);
			}
		}
		close(fd_outfile);
	}
	return (EXIT_SUCCESS);
}
