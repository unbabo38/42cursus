#include "libft.h"
#include <string.h>
#include <stdio.h>

void ft_lstdelone(t_list *lst, void (*del)(void*))
{
    if (lst && del)
    {
        del(lst->content);
        free(lst);
    }
}
