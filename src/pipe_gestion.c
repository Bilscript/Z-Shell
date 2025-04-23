/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_gestion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:49:19 by slebik            #+#    #+#             */
/*   Updated: 2025/04/23 15:25:30 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
	exit(1);
}

void	errorcmd(const char *cmd, int exit_code)
{
	perror(cmd);
	exit(exit_code);
}

void	ft_free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char *get_path_from_list(t_envp *env_list)
{
    t_envp *current;
    
	current = env_list;
    while (current)
    {
        if (ft_strcmp(current->key, "PATH") == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

char	*find_executable(char **chemins, char *cmd)
{
	int		i;
	char	*part_chem;
	char	*final_chem;

	if (!chemins)
		return (NULL);
	i = 0;
	while (chemins[i])
	{
		part_chem = ft_strjoin(chemins[i], "/");
		if (!part_chem)
			return (ft_free_split(chemins), NULL);
		final_chem = ft_strjoin(part_chem, cmd);
		free(part_chem);
		if (!final_chem)
			return (ft_free_split(chemins), NULL);
		if (access(final_chem, F_OK | X_OK) == 0)
			return (ft_free_split(chemins), final_chem);
		free(final_chem);
		i++;
	}
	return (ft_free_split(chemins), NULL);
}

char *parsing(t_envp *env_list, char *cmd)
{
	char	*path_val;
	char	**chemins;

	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	if (!cmd || !cmd[0])
		return (NULL);
	path_val = get_path_from_list(env_list);
	if (!path_val)
		return (NULL);
	chemins = ft_split(path_val, ':');
	if (!chemins)
		return (NULL);
	return (find_executable(chemins, cmd));
}

void	run_command(t_command *cmd, t_envp *env_list)
{
	pid_t	pid;
	int		status;
	char	**envp_array;
	char	*path;

	if (!cmd || !cmd->cmd)
		return;
	envp_array = env_list_to_array(env_list);
	if (!envp_array)
		error("env conversion failed");
	path = parsing(env_list, cmd->cmd);
	if (!path)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_split(envp_array);
		return;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		ft_free_split(envp_array);
		return;
	}
	if (pid == 0)
	{
		execve(path, cmd->args, envp_array);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(path);
		ft_free_split(envp_array);
	}
}

void	handle_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == TOKEN_REDIR_OUT)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				error("open > failed");
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == TOKEN_APPEND)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				error("open >> failed");
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == TOKEN_REDIR_IN)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				error("open < failed");
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		/* else if (redir->type == TOKEN_HEREDOC)
		{
		
		} */
		redir = redir->next;
	}
}

void	exec_piped_commands(t_command *cmd, t_envp *env)
{
	int		fd[2];
	int		in_fd = 0;
	pid_t	pid;
	int		status;
	char	**envp_array;
	char	*path;

	while (cmd)
	{
		envp_array = env_list_to_array(env);
		if (!envp_array)
			error("env conversion failed");
		if (cmd->next && pipe(fd) == -1)
			error("pipe failed");
		pid = fork();
		if (pid == 0)
		{
			// Duplication entrée (si pipe précédent)
			if (in_fd != 0)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			// Duplication sortie (si pipe suivant)
			if (cmd->next)
			{
				close(fd[0]); // Ne lit pas
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			handle_redirections(cmd->redirs); // ca marche mais je gere pas les erreur
			if (is_builtin(cmd->cmd))
			{
				exec_builtin(cmd, env);
				exit(EXIT_SUCCESS);
			}
			// si c un built in ca l'exec puis on stop avec un exit manuel
			// Si c pas un built in
			path = parsing(env, cmd->cmd);
			if (!path)
			{
				ft_putstr_fd(cmd->cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				exit(127);
			}
			execve(path, cmd->args, envp_array);
			perror("execve failed");
			exit(1);
		}
		else if (pid < 0)
			error("fork failed");

		// dans lprocess parent : gestion des fd
		if (in_fd != 0)
			close(in_fd);
		if (cmd->next)
		{
			close(fd[1]); // On n'écrit plus dans le pipe
			in_fd = fd[0]; // Et on lit avec ça la prochaine fois
		}
		else
		{
			// on ferme tt pr la last cmd
			if (fd[0])
				close(fd[0]);
		}

		ft_free_split(envp_array);
		cmd = cmd->next;
	}
	while (wait(&status) > 0) ;
}

static char **free_array_and_return_null(char **array, int count)
{
    int i;
    
    i = 0;
    while (i < count)
    {
        free(array[i]);
        i++;
    }
    free(array);
    return (NULL);
}

char **env_list_to_array(t_envp *env)
{
    int count;
    char **array;
    t_envp *cur;
    char *tmp;
    
    count = 0;
    cur = env;
    while (cur)
    {
        if (cur->export)
            count++;
        cur = cur->next;
    }
    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return (NULL);
    cur = env;
    count = 0;
    while (cur)
    {
        if (cur->export)
        {
            if (!cur->key)
                return (free_array_and_return_null(array, count));
                
            tmp = ft_strjoin(cur->key, "=");
            if (!tmp)
                return (free_array_and_return_null(array, count));
            
            array[count] = ft_strjoin(tmp, cur->value ? cur->value : "");
            free(tmp);
            
            if (!array[count])
                return (free_array_and_return_null(array, count));
            
            count++;
        }
        cur = cur->next;
    }
    array[count] = NULL;
    return (array);
}

void	parse_and_execute(char *input, char **env_origin, t_envp *env)
{
	t_token		*token;
	t_command	*command;

	token = tokenizer(input, env_origin);
	print_tokens(token); //debug
	command = lexer(token);
	print_commands(command); ///debug
	exec(command, env, token);
	free_tokens(token);
	free_command(command);
}

