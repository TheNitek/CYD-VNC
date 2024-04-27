#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <VNC_BB_SPI.h>
#include <VNC.h>

const char * vnc_ip = "192.168.1.118";
const uint16_t vnc_port = 5900;
const char * vnc_pass = "blubblub";

BB_SPI_VNC tft = BB_SPI_VNC();
arduinoVNC vnc = arduinoVNC(&tft);

TOUCHINFO ti;

unsigned long lastUpdateP;
unsigned long lastUpdateR;
uint16_t lx, ly;

void setup(void) {
    Serial.begin(115200);
    tft.begin(DISPLAY_TYPE);

    tft.setRotation(LCD_ORIENTATION_270);
    tft.setFont(FONT_12x16);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);

    tft.rtInit(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);

    const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    char password[8 + 1] = {'\0'};
    for(uint8_t i = 0; i < 8; i++){
        int r = random(0, strlen(chars));
        password[i] = chars[r];
    }

    char ssid[] = "espvnc-000000";
    for(uint8_t i = 0; i < 6; i++){
        int r = random(0, strlen(chars));
        ssid[7+i] = chars[r];
    }

    WiFiManager wm;
    tft.printf("SSID: %s\n", ssid);
    tft.printf("Password: %s\n", password);


    if(!wm.autoConnect(ssid, password)) {
        tft.println("Wifi connection failed");
        while(true){
            sleep(10);
        }
    }

    tft.fillScreen(TFT_BLACK);
    tft.println("Starting VNC");

    vnc.begin(vnc_ip, vnc_port);
    vnc.setPassword(vnc_pass); // check for vnc server settings
}

void loop() {
    if(WiFi.status() != WL_CONNECTED) {
        vnc.reconnect();
        delay(100);
    } else {
        vnc.loop();
        if(!vnc.connected()) {
            delay(5000);
        }
        if(tft.rtReadTouch(&ti)) {
            uint16_t x = ti.x[0];
            uint16_t y = ti.y[0];
            if((millis() - lastUpdateP) > 20) {
                vnc.mouseEvent(x, y, 0b001);
                lx = x;
                ly = y;
                lastUpdateP = millis();
                Serial.printf("[Touch] press: 1 X: %d Y: %d Z: %d\n", x, y, ti.pressure[0]);
            }
            lastUpdateR = 0;
        } else {
            if(lastUpdateR == 0) {
                vnc.mouseEvent(lx, ly, 0b000);
                lastUpdateR = millis();
                Serial.printf("[Touch] press: 0 X: %d Y: %d\n", lx, ly);
            }
            lastUpdateP = 0;
        }
   }
}