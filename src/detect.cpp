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
    pinMode(33, OUTPUT );   //32 33
    pinMode(25, OUTPUT );   //33 25
    pinMode(26, OUTPUT );   //25 26
    pinMode(27, OUTPUT );   //26 27
                            // GPIO 14  
                            // GPIO12  
                            // GPIO 13
                            // GPIO15
    // rutina de startup de relays -- 33 25 27 26
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    vTaskDelay(1000);
    digitalWrite(33, LOW);
    vTaskDelay(100);
    digitalWrite(25, LOW);
    vTaskDelay(100);
    digitalWrite(26, LOW);
    vTaskDelay(100);
    digitalWrite(27, LOW);
    vTaskDelay(100);
    vTaskDelay(1000);
    digitalWrite(33, HIGH);
    vTaskDelay(100);
    digitalWrite(25, HIGH);
    vTaskDelay(100);
    digitalWrite(26, HIGH);
    vTaskDelay(100);
    digitalWrite(27, HIGH);
    vTaskDelay(100);
    vTaskDelay(1000);
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    vTaskDelay(1000);
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
}

void detect_mes( void ) {
    float tension_red = measure_get_channel_rms( CHANNEL_1 );
    float tension_gen = measure_get_channel_rms( CHANNEL_5 );   

    //  CHECK RED OK
    if (under_voltage < tension_red){
        cont1++;
        if(cont1>=5){        
            f_red_ok = true;
            cont1 = 0;
        }  
    }else{ 
    //  CHECK RED NO OK   
        cont2++;     
        if(cont2>=5){        
            f_red_ok = false;        
            cont2 = 0;
        }

    }
    //  CHECK GEN OK
    if (under_voltage < tension_gen){
        cont3++;        
        if(cont3>=5){        
            f_gen_ok = true;
            cont3 = 0;
        }        
    }else{ 
    //  CHECK GEN NO OK
        cont4++;
        if(cont4>=5){        
            f_gen_ok = false;
            cont4 = 0;
        }
    }
    //ACTION
    if (f_red_ok==true){
        digitalWrite(25, HIGH);
        vTaskDelay(1000);
        digitalWrite(33, LOW);
        log_i("Red conectada!");
        log_e("VRED=%.3f ", measure_get_channel_rms( CHANNEL_1 ) );
    } else digitalWrite(33, HIGH);

    if (f_gen_ok==true){
        if(f_red_ok==false){
            digitalWrite(33, HIGH);
            vTaskDelay(1000);
            digitalWrite(25, LOW);
            log_i("GRUPO ELECTROGENO conectado!");
            log_e("VGEN=%.3f ", measure_get_channel_rms( CHANNEL_5 ) );
        }
    } else digitalWrite(25, HIGH);

    return;
}