
void stripSetup() {
  // Fill the pixel data with zeros
  memset(pixels, 0 , NUMPIXELS);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));

  // Start with all on dimmly.
  stripSetAllPixels(2);
  stripShow();
}

// Send the pixel data to the led strip.
void stripShow(void) {
  uint8_t i;
  uint16_t n = NUMPIXELS; 
  // Junk from the spi response; 
  uint8_t spi_return;
  
  // 4 bytes of 0 as the start frame
  for (i = 0; i < 4; i++) {
    spi_return = SPI.transfer(0x00);
  }

  // Pixel frame
  for (n = 0; n < NUMPIXELS; n++) {
    // For each pixel...
    // Pixel start, 3 bits of 1, then 5 bits of brightness.
    spi_return = SPI.transfer(0xFF);
    spi_return = SPI.transfer(pixels[n][0]);
    spi_return = SPI.transfer(pixels[n][1]);
    spi_return = SPI.transfer(pixels[n][2]);
  }

  // End frame
  for(i = 0; i < ((NUMPIXELS + 15) / 16); i++) {
    spi_return = SPI.transfer(0xFF);
  }
}

void stripSetPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if (n < NUMPIXELS) {
    pixels[n][0] = r;
    pixels[n][1] = g;
    pixels[n][2] = b;
  }
}

/**
 * Temporary
 */
void stripSetAllPixels(int val) {
  brightness = constrain(val, 0, STRIP_MAX_VALUE);
  int r, g, b; // all white but still 3 leds per pixel

  // Distribute the bulk of the brightness across all three leds.
  int x = brightness / 3;
  r = x;
  g = x;
  b = x;

  // Spread the rest of the brightness across the leds.
  int mod = brightness % 3;
  if (mod > 0) {
    r++;
  }
  if (mod == 2) {
    g++;
  }

  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    stripSetPixel(i, r, g, b);
  }
}

