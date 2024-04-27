#include "VNC_BB_SPI.h"

BB_SPI_VNC::BB_SPI_VNC() {
}

bool BB_SPI_VNC::hasCopyRect(void) {
    return false;
}

uint32_t BB_SPI_VNC::getHeight(void) {
    return BB_SPI_LCD::height();
}

uint32_t BB_SPI_VNC::getWidth(void) {
    return BB_SPI_LCD::width();
}

void BB_SPI_VNC::draw_area(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t * data) {
	if(x > BB_SPI_LCD::width() || y > BB_SPI_LCD::height()) {
		return;
	}
	Serial.printf("d: x %d, %d %dx%d\n", x, y, w, h);
	BB_SPI_LCD::pushImage(x, y, w, h, (uint16_t *)data);
}

void BB_SPI_VNC::draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color) {
    BB_SPI_LCD::fillRect(x, y, w, h, color);
}

void BB_SPI_VNC::copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h) {
}

void BB_SPI_VNC::area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    BB_SPI_LCD::setAddrWindow(x, y, w, h);
}

void BB_SPI_VNC::area_update_data(char * data, uint32_t pixel) {
    BB_SPI_LCD::pushPixels((uint16_t *)data, pixel);
}

void BB_SPI_VNC::area_update_end(void) {
}

void BB_SPI_VNC::vnc_options_override(dfb_vnc_options * opt) {
    opt->client.bigendian = 1;
}