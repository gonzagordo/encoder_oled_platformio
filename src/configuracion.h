#if !defined(configuracion_h)
#define configuracion_h

//configuracion del driver oled 

// variables de configuracion 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET     4    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)

//configuracion de los pines del encoder 
#define CLK  4      // CLK a pin digital 3 (CLK en modulo)
#define DT  3       // DT a pin digital 2 (DT en modulo) Pin de la interrupcion 
#define btn 2       // Swiche del encoder al pin 4
#define LED 13      // LED, interno se ve por el agujero 

int tabla_de_potencias []= {(0,0,0),(10,10,1),(5,5,1),(4,3,1),(3,2,1),(2,2,1),(2,3,0),(3,4,0),(5,5,0),(10,10,0),(0,0,1)};

#endif // configuracion_h
