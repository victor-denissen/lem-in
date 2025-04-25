/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdenisse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 10:56:59 by vdenisse          #+#    #+#             */
/*   Updated: 2023/10/14 14:38:43 by vdenisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

size_t	increase_buffer(char **saved, int fd)
{
	char	*new_buffer;
	char	*new_read;
	size_t	new_len;
	int		bytes_read;

	bytes_read = 0;
	bytes_read = ft_read(&new_read, fd);
	if (bytes_read <= 0 || bytes_read > BUFFER_SIZE)
		return (bytes_read);
	new_len = ft_strlen_gnl(new_read) + ft_strlen_gnl(*saved);
	new_buffer = (char *)malloc((new_len + 1) * sizeof(char));
	if (!new_buffer)
	{
		free(new_read);
		return (0);
	}
	new_buffer[0] = '\0';
	ft_strcpy(new_buffer, *saved, ft_strlen_gnl(*saved));
	ft_strcat(new_buffer, new_read);
	if (*saved)
		free(*saved);
	*saved = new_buffer;
	free(new_read);
	return (bytes_read);
}

int	ft_extract_line(char **next_line,
					char **saved,
					size_t saved_size,
					int bytes_read)
{
	size_t	index;

	index = (size_t)ft_strchr_gnl(*saved, '\n');
	if (index == (size_t)-1)
		index = (size_t)ft_strchr_gnl(*saved, '\0');
	if (!ft_substr_gnl((char const *)*saved, &(*next_line), 0, index + 1))
	{
		free(*next_line);
		return (-1);
	}
	if (!reduce_buffer(saved, saved_size, bytes_read))
	{
		free(*next_line);
		return (-1);
	}
	return (1);
}

int	reduce_buffer(char **saved, size_t saved_size, int bytes_read)
{
	char	*new_buffer;
	size_t	index;

	if (bytes_read == 0)
	{
		if (*saved)
			free(*saved);
		*saved = NULL;
		return (1);
	}
	index = 0;
	if (*saved[0] == '\0')
		return (1);
	index = ft_strchr_gnl(*saved, '\n');
	if (index == (size_t)-1)
		return (1);
	if (!ft_substr_gnl(*saved, &new_buffer, index + 1, saved_size))
	{
		free(new_buffer);
		return (0);
	}
	if (*saved)
		free(*saved);
	*saved = new_buffer;
	return (1);
}

int	ft_read(char **new_read, int fd)
{
	int	bytes_read;

	*new_read = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!*new_read)
		return (0);
	bytes_read = read(fd, *new_read, BUFFER_SIZE);
	if (bytes_read != -1)
		(*new_read)[bytes_read] = '\0';
	if (bytes_read <= 0 || bytes_read > BUFFER_SIZE)
		free(*new_read);
	return (bytes_read);
}

char	*get_next_line(int fd, int free_saved)
{
	static char	*saved;
	char		*next_line;
	size_t		saved_size;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	saved_size = ft_strlen_gnl(saved);
	bytes_read = -1;
	while (ft_strchr_gnl((const char *)saved, '\n') == -1 && bytes_read != 0)
	{
		bytes_read = increase_buffer(&saved, fd);
		saved_size += bytes_read;
		if (saved_size == (size_t)0 || bytes_read == -1)
		{
			free(saved);
			saved = NULL;
			return ((void *)0);
		}
	}
	ft_extract_line(&next_line, &saved, saved_size, bytes_read);
	if (free_saved)
		free(saved);
	return (next_line);
}
