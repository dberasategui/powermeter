#include <FreeRTOS.h>
#include <driver/i2s.h>
#include <arduinoFFT.h>
#include <math.h>

#include "esp_wifi.h"

#include "config/measure_config.h"
#include "measure.h"
#include "config.h"
#include "detect.h"

extern "C" {
    #include "soc/syscon_reg.h"
    #include "soc/syscon_struct.h"
    }

/* taske handle */
TaskHandle_t _DETECT_Task;


void detect_Task( void * pvParameters ) {
    log_i("Start detect Task on Core: %d", xPortGetCoreID() );

    detect_init();

    while( true ) {
        detect_mes();
        vTaskDelay( 10000 );
    }
}
/**
 * 
 */

void detect_StartTask( void ) {
    xTaskCreatePinnedToCore(
                    detect_Task,               /* Function to implement the task */
                    "detect Task", /* Name of the task */
                    10000,                      /* Stack size in words */
                    NULL,                       /* Task input parameter */
                    2,                          /* Priority of the task */
                    &_DETECT_Task,             /* Task handle. */
                    _DETECT_TASKCORE );        /* Core where the task should run */  
}
/*
 * 
 */
void detect_init( void ) {
    // iniciar GPIOs
    log_i("Start Detect Task on Core: %d", xPortGetCoreID() );
    vTaskDelay(1000);
    pinMode(32, OUTPUT );
    pinMode(33, OUTPUT );
    pinMode(25, OUTPUT );
    pinMode(26, OUTPUT );
    // rutina de startup de relays
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    vTaskDelay(1000);
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    vTaskDelay(1000);
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    vTaskDelay(1000);
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    vTaskDelay(1000);
    digitalWrite(32, LOW);
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    
}

void detect_mes( void ) {
    float tension = measure_get_channel_rms( CHANNEL_1 );
    float under_voltage = 180;
    float over_voltage = 245;

    if (under_voltage < tension){
        digitalWrite(32, LOW);
        digitalWrite(33, HIGH);
        digitalWrite(25, HIGH);
        digitalWrite(26, HIGH);
        log_i("Red activa y conectada!");
    }        

    else if (tension < under_voltage ){
        digitalWrite(32, HIGH);
        vTaskDelay(1000);
        digitalWrite(33, LOW);
        log_i("GRUPO ELECTROGENO activo y conectado!");
    }
    else return;
}

