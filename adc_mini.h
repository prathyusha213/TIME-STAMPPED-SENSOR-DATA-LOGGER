// Custom type alias header for fixed width readability
#include "types.h"

// Initialize ADC hardware for the selected analog input channel
void Init_ADC(u32 chNo);

// Perform ADC conversion and return voltage + 10-bit digital value
void Read_ADC(u32 chNo,f32 *eAR,u32 *adcDVal);
