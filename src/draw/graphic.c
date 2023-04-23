/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 05:46:33 by tumolabs          #+#    #+#             */
/*   Updated: 2023/04/23 06:01:15 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int     rgb(char *txt)
{
    int     i;
    char    *copy;
    char    **split;
    
    copy = ft_strdup(txt);
    i = -1;
    while (copy[++i])
        if (!ft_isdigit(copy[i]))
            copy[i] = 2;
    split = ft_split(copy, 2);
    i = (ft_atoi(split[0]) << 16) | (ft_atoi(split[1]) << 8) | ft_atoi(split[2]);
    free_char_pp(&split);
    free(copy);
    return (i);
}

void	mpp(t_data *data, int x, int y, int color)
{
	char	*dst;
	
	dst = data->addr + (y * data->len + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

void	draw_textured_walls(int x, t_game *game)
{
	int		y;
	int		clr;
	double	angle;
	double	wall_x;
	t_texture	data;

	y = game->tsc.drawStart - 1 ;
	angle = atan2(game->tsc.rayDirY, game->tsc.rayDirX);
	if (angle > 0 && angle < PI) // to the north
		data = game->texture[0];
	else if (angle > PI_HALF && angle < PI_3) // to the west
		data = game->texture[2];
	else if (angle < 0 || angle > PI_2) // to the south
		data = game->texture[1];
	else // to the east
		data = game->texture[3];
		
	if (game->tsc.side == 0)
		wall_x = game->player->pos.y + game->tsc.perpWallDist * game->tsc.rayDirY;
	else
		wall_x = game->player->pos.x + game->tsc.perpWallDist * game->tsc.rayDirX;
	wall_x -= floor(wall_x);
	while (++y < game->tsc.drawEnd)
	{
		double texture_coord = (y - game->tsc.drawStart) / (double)(game->tsc.drawEnd - game->tsc.drawStart);
		int	texture_pixel = (int)(texture_coord * data.h);
		int color = *(data.img + (texture_pixel * data.size) + (int)(wall_x  * data.w) * (data.bpp / 8));
		// game->img.data[y * WIDTH + x] = color;
		// game->img.addr[y * WIDTH + x] = color;
		mpp(&game->img, x, y, color);
	}
}

void	draw_sky(int x, t_game *game)
{
	int	s;
	
	s = -1;
	while (++s < game->tsc.drawStart)
		mpp(&game->img, x, s, game->floor_color);	
}

void	draw_floor(int x, t_game *game)
{
	int	c;
	
	c = game->tsc.drawEnd - 1;
	while (++c < HEIGHT)
		mpp(&game->img, x, c, game->ceiling_color);
}