#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

int main() {
  stdio_init_all();
  float v1, v2;
  adc_init();
  adc_gpio_init(28);
  adc_gpio_init(27);
 
  gpio_set_function(9, GPIO_FUNC_PWM);
  gpio_set_function(8, GPIO_FUNC_PWM);

  uint8_t slice_num = pwm_gpio_to_slice_num(9);
  pwm_config config = pwm_get_default_config();
  pwm_init(slice_num, &config, true);
  
  while(1) {
    adc_select_input(0);
    uint16_t pote = adc_read();
    adc_select_input(1);
    uint16_t lm = adc_read();
    v1= (float) pote * 3.3/4095;
    int PoteTemp = v1 * 35/3.3; 
    printf("Temp de POTE %d\n", PoteTemp);
    v2= (float) lm * 3.3 / 4095;
    int LMTemp = v2*100;
    printf("Temp de LM %d\n", LMTemp);
    sleep_ms(500);
    int DT = (PoteTemp - LMTemp);
    if (DT > 10){
      DT = 10;
    }
    else if (DT < -10) {
      DT = 10;
    }
    else if (DT < 0) {
      DT = DT * -1;
    }

    DT = DT * 0xffff / 10;
    if (PoteTemp > LMTemp){
      pwm_set_gpio_level(9, DT);
      pwm_set_gpio_level(8, 0);
    }
    if (LMTemp > PoteTemp){
      pwm_set_gpio_level(8, DT);
      pwm_set_gpio_level(9, 0);
    }
    printf("TempDif %d\n", DT);
  }
}
