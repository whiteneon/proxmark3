//-----------------------------------------------------------------------------
// Copyright (C) 2019 iceman
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Routines to support a mangeled ISO 14443 type A for Thinfilm tags by Kovio
//-----------------------------------------------------------------------------

#include "thinfilm.h"

/**
  * ref
  *   https://www.thinfilmnfc.com/wp-content/uploads/2017/09/Thinfilm-Kovio-NFC-Barcode-Protocol-Tag-Functional-Specification-v3.4-2017-05-26.pdf
  *   https://developer.android.com/reference/android/nfc/tech/NfcBarcode
  *  
  */

void ReadThinFilm(void) {
    
    clear_trace();

    set_tracing(true);

    iso14443a_setup(FPGA_HF_ISO14443A_READER_LISTEN);

    uint8_t len = 0;
    uint8_t buf[36] = {0x00};

    // power on and listen for answer.
    bool status = GetIso14443aAnswerFromTag_Thinfilm(buf, &len);

    // lsb -> msb
    for (uint8_t i = 0; i < len; i++) {
        uint8_t b = buf[i];
        buf[i] = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16; 
    }
    // Startbit is always set and used in byte
    buf[0] |= 0x80;
        
    reply_ng(CMD_THINFILM_READ, status ? PM3_SUCCESS : PM3_ENODATA, buf, len);

    iso14443a_off();
    set_tracing(false);
}

