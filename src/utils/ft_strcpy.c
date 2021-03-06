/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strcpy.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: bperez <bperez@student.le-101.fr>          +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/20 20:34:55 by bperez       #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 09:41:54 by bperez      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

char	*ft_strcpy(char *dst, const char *src)
{
	if (dst && src)
	{
		while (*src)
			*dst++ = *src++;
		*dst = *src;
	}
	return (dst);
}
