/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clouaint <clouaint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:18:57 by clouaint          #+#    #+#             */
/*   Updated: 2024/11/16 14:15:49 by clouaint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_args(t_token *token)
{
	t_token	*tmp;
	char	**args;
	int		i;
	int		j;

	tmp = token;
	i = 0;
	while (tmp && (tmp->index == CMD || tmp->index == ARG))
	{
		i++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	j = 0;
	tmp = token;
	while (tmp && j < i)
	{
		args[j] = ft_strdup(tmp->str);
		tmp = tmp->next;
		j++;
	}
	args[i] = NULL;
	return (args);
}

char	*get_command_path(char *command, t_env *env)
{
	char	**paths;
	char	*tmp_path;
	char	*full_path;
	int		i;
	char	*env_path;

	env_path = get_env_value(env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	i = 0;
	while (paths[++i])
	{
		tmp_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp_path, command);
		free(tmp_path);
		if (access(full_path, X_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free(paths);
	return (NULL);
}

int	count_pipes(t_token *token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->index == PIPE)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

void	child_process(t_token *token, t_exec *execp, int i, t_env **envp)
{
	char	**args;

	handle_redirections(token, execp);
	while (token && token->index != CMD)
		token = token->next;
	if (i > 0)
		dup2(execp->pipe_fd[i - 1][0], STDIN_FILENO);
	if (i < execp->pipe_num)
		dup2(execp->pipe_fd[i][1], STDOUT_FILENO);
	close_pipes(execp->pipe_fd, execp->pipe_num, i);
	args = init_args(token);
	if (!is_builtin(token) && token->index == CMD)
		exec(args, envp);
	else
	{
		exec_builtin(token, envp, STDOUT_FILENO, execp);
		exit(EXIT_SUCCESS);
	}
}

char	**lst_to_array(t_env **env)
{
	t_env	*tmp;
	char	**array;
	int		i;

	tmp = *env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	array = malloc(sizeof(char *) * (i + 1));
	if (!array)
		return (NULL);
	i = 0;
	tmp = *env;
	while (tmp)
	{
		array[i] = ft_strjoin(tmp->name, "=");
		array[i] = ft_strjoin(array[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
