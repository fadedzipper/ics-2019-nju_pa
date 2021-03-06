#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
	  uint32_t in = *(volatile uint32_t *)SCREEN_ADDR;
      info->width = in >> 16;
      info->height = in & 0xffff;

	  /* printf("width = %d, height = %d\n", info->width, info->height); */

      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
		_DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;

		int i, j;
		for(i = 0; i < ctl->w; i++){
			for(j = 0; j < ctl->h; j++){

				/* printf("reach here\n"); */
				/* x, y, i, j */

				outl(FB_ADDR + 4 * ( ctl->x + 400*ctl->y + i + 400*j ), *ctl->pixels);
			}
		}

      if (ctl->sync) {
		  /* printf("1\n"); */
        outl(SYNC_ADDR, 1);

		  /* printf("2\n"); */
      }
	  else{
		  /* printf("11\n"); */
		outl(SYNC_ADDR, 0);
		  /* printf("22\n"); */
	  }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
	int i;
  	int size = screen_width() * screen_height();
  	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  	for (i = 0; i < size; i ++) fb[i] = i;
  	draw_sync();
}
