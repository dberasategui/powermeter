#include <FreeRTOS.h>
#include <driver/i2s.h>
// #include <arduinoFFT.h>
// #include <math.h>

// #include "esp_wifi.h"

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
float under_voltage = 180;
bool f_red_ok = false;
bool f_gen_ok = false;
int cont1 = 0;
int cont2 = 0;
int cont3 = 0;
int cont4 = 0;

void detect_Task( void * pvParameters ) {
    log_i("Start detect Task on Core: %d", xPortGetCoreID() );

    detect_init();

    while( true ) {
        detect_mes();
        // vTaskDelay( 10000 );
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
    vTaskDelay(100);
    digitalWrite(33, LOW);
    vTaskDelay(100);
    digitalWrite(25, LOW);
    vTaskDelay(100);
    digitalWrite(26, LOW);
    vTaskDelay(100);
    vTaskDelay(1000);
    digitalWrite(32, HIGH);
    vTaskDelay(100);
    digitalWrite(33, HIGH);
    vTaskDelay(100);
    digitalWrite(25, HIGH);
    vTaskDelay(100);
    digitalWrite(26, HIGH);
    vTaskDelay(100);
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
}

void detect_mes( void ) {
    float tension_red = measure_get_channel_rms( CHANNEL_1 );
    float tension_gen = measure_get_channel_rms( CHANNEL_5 );   

    //  CHECK RED OK
    if (under_voltage < tension_red){
        log_e("VRED=%.3f ", measure_get_channel_rms( CHANNEL_1 ) );
        cont1++;
          log_e("count1=%d ",cont1 );
        if(cont1>=5){        
        f_red_ok = true;
        cont1 = 0;}        
    }
    //  CHECK RED NO OK
    if (under_voltage > tension_red){ 
        log_e("VRED=%.3f ", measure_get_channel_rms( CHANNEL_1 ) );
         log_e("count2=%d ",cont2 );
        cont2++;
        if(cont2>=5){        
        f_red_ok = false;
        cont2 = 0;}
    }
    //  CHECK GEN OK
    if (under_voltage < tension_gen){
        log_e("VGEN=%.3f ", measure_get_channel_rms( CHANNEL_5 ) );
        cont3++;
        log_e("count3=%d ",cont3 );
        if(cont3>=5){        
        f_gen_ok = true;
        cont3 = 0;}        
    }
    //  CHECK GEN NO OK
    if (under_voltage > tension_gen){ 
        log_e("VGEN=%.3f ", measure_get_channel_rms( CHANNEL_5 ) );
        cont4++;
        if(cont4>=5){        
        f_red_ok = false;
        cont4 = 0;}
    } 
    //ACTION

    if (f_red_ok==true){
        vTaskDelay(1000);
        digitalWrite(32, LOW);
        log_i("Red conectada!");
        log_e("VRED=%.3f ", measure_get_channel_rms( CHANNEL_1 ) );
    }

    if (f_red_ok==false){
        digitalWrite(32, HIGH);
        vTaskDelay(1000);
        //arrancar generador
        if(f_gen_ok==true){            
            digitalWrite(33, LOW);
            log_i("GRUPO ELECTROGENO conectado!");
            log_e("VGEN=%.3f ", measure_get_channel_rms( CHANNEL_5 ) );
        }
    }
    return;
}