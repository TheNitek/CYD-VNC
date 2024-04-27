#ifndef VNC_BB_SPI_H_
#define VNC_BB_SPI_H_

#include <bb_spi_lcd.h>
#include "VNC_config.h"
#include "VNC.h"

class BB_SPI_VNC : public VNCdisplay, public BB_SPI_LCD {
  public:
    BB_SPI_VNC();

    bool hasCopyRect(void);

    uint32_t getHeight(void);
    uint32_t getWidth(void);

    void draw_area(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t * data);

    void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color);

    void copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h);

    void area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    void area_update_data(char * data, uint32_t pixel);
    void area_update_end(void);

    void vnc_options_override(dfb_vnc_options * opt);

  private:
    uint32_t offset_x, offset_y;
};

#endif