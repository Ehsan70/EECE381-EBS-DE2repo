#include "myvga.h"
int x_0 = 1;
int x_1 = 19;
int y_0 = 1;
int y_1 = 1;
int up_once = 0;
int left_once = 0;
int right_once = 0;
int down_once = 0;
int space_once = 0;

int* x_pos;
void draw_scroll_bar(int* x_pos) {

	int i;
	//If scroll bar draws over a column, redraw it. If audio is present play audio
	if (*x_pos == 1) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, *x_pos, SCREEN_HEIGHT
				- 4, *x_pos + 18, SCREEN_HEIGHT - 1, 0xD940, 0); //draw tracker box
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 301, SCREEN_HEIGHT - 4,
				318, SCREEN_HEIGHT - 1, 0x0000, 0); //erase tracker box

	}

	//column 2,3, ...
	if (*x_pos == 21 || *x_pos == 41 || *x_pos == 61 || *x_pos == 81 || *x_pos
			== 101 || *x_pos == 121 || *x_pos == 141 || *x_pos == 161 || *x_pos
			== 181 || *x_pos == 201 || *x_pos == 221 || *x_pos == 241 || *x_pos
			== 261 || *x_pos == 281 || *x_pos == 301) {

		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, *x_pos, SCREEN_HEIGHT
				- 4, *x_pos + 18, SCREEN_HEIGHT - 1, 0xD940, 0); //draw tracker box
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, (*x_pos - 20),
				SCREEN_HEIGHT - 4, (*x_pos - 20) + 18, SCREEN_HEIGHT - 1,
				0x0000, 0); //draw tracker box

	}

	//Redraw the rows
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, *x_pos - 1, 0);
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, *x_pos - 1, 60);
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, *x_pos - 1, 120);
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, *x_pos - 1, 180);
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, *x_pos - 1, 239);

}
void draw_loop_grid(alt_up_pixel_buffer_dma_dev* pixel_buffer) {

	int j;

	//Columns
	for (j = 0; j < 16; j++) {
		alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, (j * (SCREEN_WIDTH
				/ 16)), 239, 0, 0xFFFF, 0);
		printf("Column Position %d : %d\n", j, (j * (SCREEN_WIDTH / 16)));

	}

	//Last Column isn't drawing so manually adding it in
	alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, SCREEN_WIDTH - 1, 239, 0,
			0xFFFF, 0);
	printf("Last column pos : %d\n", SCREEN_WIDTH - 1);

	//Rows
	for (j = 0; j < 4; j++) {
		//draw the row bars
		alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, 0, 319, (SCREEN_HEIGHT
				/ 4) * j, 0xFFFF, 0);
		printf("Row position %d : %d\n", j, (j * (SCREEN_HEIGHT / 4)));

	}
	//Last row isn't drawing so manually adding it in
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer, 0, 319, SCREEN_HEIGHT - 1,
			0xFFFF, 0);
	printf("Last row pos: %d\n", SCREEN_HEIGHT - 1);

}

void fill_loop_grid_green(int row_select, int column_select) {

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, ((SCREEN_HEIGHT / 4) * row_select + 1),
			((SCREEN_WIDTH / 16) * (column_select + 1) - 1), ((SCREEN_HEIGHT
					/ 4) * (row_select + 1) - 1), 0xA700, 0);

}
void fill_loop_grid_black(int row_select, int column_select) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, ((SCREEN_HEIGHT / 4) * row_select + 1),
			((SCREEN_WIDTH / 16) * (column_select + 1) - 1), ((SCREEN_HEIGHT
					/ 4) * (row_select + 1) - 1), 0x0000, 0);
}
void fill_top_loop_grid_green(int row_select, int column_select) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, ((SCREEN_HEIGHT / 4) * row_select + 1),
			((SCREEN_WIDTH / 16) * (column_select + 1) - 1), ((SCREEN_HEIGHT
					/ 8) * (2 * row_select + 1) - 1), 0xA700, 0);
}
void fill_top_loop_grid_black(int row_select, int column_select) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, ((SCREEN_HEIGHT / 4) * row_select + 1),
			((SCREEN_WIDTH / 16) * (column_select + 1) - 1), ((SCREEN_HEIGHT
					/ 8) * (2 * row_select + 1) - 1), 0x0000, 0);
}
void fill_bottom_loop_grid_orange(int row_select, int column_select) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, (((SCREEN_HEIGHT / 8) * (2 * row_select + 1))
			- 5), ((SCREEN_WIDTH / 16) * (column_select + 1) - 1),
			((SCREEN_HEIGHT / 4) * (row_select + 1) - 1), 0xA2FF, 0);
}
void fill_bottom_loop_grid_black(int row_select, int column_select) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, ((SCREEN_WIDTH / 16)
			* column_select) + 1, (((SCREEN_HEIGHT / 8) * (2 * row_select + 1))
			- 5), ((SCREEN_WIDTH / 16) * (column_select + 1) - 1),
			((SCREEN_HEIGHT / 4) * (row_select + 1) - 1), 0x0000, 0);
}
