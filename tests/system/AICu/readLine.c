#include "main.h"

char	*ft_strchr(char *str, int c)
{
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	if (c == '\0')
		return ((char *)&str[ft_strlen(str)]);
	while (str[i] != c)
	{
		if (str[i] == '\0')
			return (NULL);
		i++;
	}
	return ((char *)str + i);
}

char	*readLine(int fd, char *stat)
{
	char	*buff;
	int		byteread;

	byteread = 1;
	while (!ft_strchr(stat, '\n') && byteread != 0)
	{
		buff = malloc (sizeof(char) * (BUFFER_SIZE_READ_LINE + 1));
		if (!buff)
			return (NULL);
		byteread = read (fd, buff, BUFFER_SIZE_READ_LINE);
		if (byteread == -1)
		{
			free (buff);
			buff = NULL;
			return (NULL);
		}
		buff[byteread] = '\0';
		stat = ft_strjoin(stat, buff);
		free (buff);
		buff = NULL;
	}
	return (stat);
}