/**
 * @file ioport_config.cpp
 * @author Dirk Broßwick (dirk.brosswick@googlemail.com)
 * @brief 
 * @version 1.0
 * @date 2022-10-03
 * 
 * @copyright Copyright (c) 2022
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */ 
#include "ioport_config.h"
#include "measure.h"

ioport_config_t::ioport_config_t() : BaseJsonConfig( IOPORT_JSON_CONFIG_FILE ) {
}

bool ioport_config_t::onSave(JsonDocument& doc) {
    doc["ioportcount"] = IOPORT_MAX;

    for( int i = 0 ; i < IOPORT_MAX ; i++ ) {
        doc["ioport"][ i ]["info"] = i;
        doc["ioport"][ i ]["name"] = ioport[ i ].name;
        doc["ioport"][ i ]["start_state"] = ioport[ i ].start_state;
        doc["ioport"][ i ]["active"] = ioport[ i ].active;
        doc["ioport"][ i ]["gpio_pin_num"] = ioport[ i ].gpio_pin_num;
        doc["ioport"][ i ]["invert"] = ioport[ i ].invert;
        doc["ioport"][ i ]["value_channel"] = ioport[ i ].value_channel;
        doc["ioport"][ i ]["trigger"] = ioport[ i ].trigger;
        doc["ioport"][ i ]["value"] = ioport[ i ].value;
    }

    return true;
}

bool ioport_config_t::onLoad(JsonDocument& doc) {

    ioport_count = doc["ioportcount"];;

    for( int i = 0 ; i < IOPORT_MAX ; i++ ) {

        int infonumber = doc["ioport"][ i ]["info"];
        if ( infonumber >= IOPORT_MAX )
            continue;

        strncpy( ioport[ infonumber ].name, doc["ioport"][ i ]["name"], IOPORT_MAX_INFO_TEXT_SIZE );
        ioport[ infonumber ].start_state = doc["ioport"][ i ]["start_state"];
        ioport[ infonumber ].active = doc["ioport"][ i ]["active"];
        ioport[ infonumber ].gpio_pin_num = doc["ioport"][ i ]["gpio_pin_num"];
        ioport[ infonumber ].invert = doc["ioport"][ i ]["invert"];
        ioport[ infonumber ].value_channel = doc["ioport"][ i ]["value_channel"];
        ioport[ infonumber ].trigger = doc["ioport"][ i ]["trigger"];
        ioport[ infonumber ].value = doc["ioport"][ i ]["value"];
    }

    return true;
}

bool ioport_config_t::onDefault( void ) {
    // snprintf( ioport[ 0 ].name, sizeof( ioport[ 0 ].name ), "Relay 0");
    // snprintf( ioport[ 0 ].start_state, sizeof( ioport[ 0 ].start_state ), true);
    // snprintf( ioport[ 0 ].active, sizeof( ioport[ 0 ].active ), true);
    // snprintf( ioport[ 0 ].gpio_pin_num, sizeof( ioport[ 0 ].gpio_pin_num ), 32);
    // snprintf( ioport[ 0 ].value_channel, sizeof(ioport[ 0 ].value_channel, 1 ));
    // snprintf( ioport[ 0 ].trigger, sizeof(ioport[ 0 ].trigger), IOPORT_TRIGGER_HIGHER);

    return true;
}
