

#include "fillit.h"

/*
** This function is designed as a fuse prior to a gnl() call in a parsing
** situation.
** It returns the number of occurences of the 'trigger' char within the
** first 'lenght' bytes of the file.
** This way we can then avoid using gnl on /dev/zero or /dev/random
** for example.
*/

int	ft_basicfilecheck(char *filename, char trigger, int lenght)
{
	int		i;
	int		fd;
	int		readret;
	int		ret;
	char	*buf;

	if (!(buf = ft_strnew(lenght)))
		return (-1);
	if ((fd = open(filename, O_RDONLY)) < 0)
		return (-2);
	if ((readret = read(fd, buf, lenght)) == -1)
		ret = -3;
	else
	{
		ret = 0;
		i = -1;
		while (buf[++i])
			if (buf[i] == trigger)
				ret++;
	}
	ft_strdel(&buf);
	if (close(fd) == -1)
		return (-4);
	return (ret);
}
