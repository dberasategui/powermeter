#ifndef _DETECT_H
    #define _DETECT_H

#define DELAY   1000

//float rms;

void detect_init( void );
void detect_mes( void );
void detect_StartTask( void );
float measure_get_channel_rms( int channel );

#endif // _DETECT_H