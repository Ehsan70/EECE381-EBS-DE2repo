#ifndef MYVGA_H_
#define MYVGA_H_
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include <stdio.h>
#include <assert.h>
#include <system.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
extern int x_0 ;
extern int x_1 ;
extern int y_0 ;
extern int y_1 ;
extern int up_once ;
extern int left_once ;
extern int right_once ;
extern int down_once ;
extern int space_once ;

extern int* x_pos;


alt_up_pixel_buffer_dma_dev* pixel_buffer;


 void draw_scroll_bar(int* x_pos);
 void draw_loop_grid(alt_up_pixel_buffer_dma_dev* pixel_buffer);
 void fill_loop_grid_green(int row_select,int column_select);
 void fill_loop_grid_black(int row_select,int column_select);

void fill_top_loop_grid_green(int row_select,int column_select);
void fill_bottom_loop_grid_orange(int row_select,int column_select);
void fill_top_loop_grid_black(int row_select,int column_select);
void fill_bottom_loop_grid_black(int row_select,int column_select);
#endif /* MYVGA_H_ */
