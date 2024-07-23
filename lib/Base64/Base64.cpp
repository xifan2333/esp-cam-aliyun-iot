/*
 * Copyright (c) 2013 Adam Rudd.
 * See LICENSE for more information
 * https://github.com/adamvr/arduino-base64 
 */
#include "Base64.h"
#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

const char PROGMEM Base64::b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789+/";

String Base64::encode(const uint8_t *input, size_t inputLen) {
    std::vector<char> output((inputLen + 2 - ((inputLen + 2) % 3)) / 3 * 4 + 1);
    int i = 0, j = 0;
    int encLen = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    while(inputLen--) {
        a3[i++] = *(input++);
        if(i == 3) {
            a3_to_a4(a4, a3);
            for(i = 0; i < 4; i++) {
                output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
            }
            i = 0;
        }
    }

    if(i) {
        for(j = i; j < 3; j++) {
            a3[j] = '\0';
        }
        a3_to_a4(a4, a3);
        for(j = 0; j < i + 1; j++) {
            output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
        }
        while((i++ < 3)) {
            output[encLen++] = '=';
        }
    }
    output[encLen] = '\0';
	String result = String(output.data());
	logger.info(result,"Base64");
    return result;
}

std::vector<uint8_t> Base64::decode(const String &input) {
    int inputLen = input.length();
    std::vector<uint8_t> output((6 * inputLen) / 8 + 1);
    int i = 0, j = 0;
    int decLen = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    const char *inputPtr = input.c_str();
    while(inputLen--) {
        if(*inputPtr == '=') {
            break;
        }
        a4[i++] = *(inputPtr++);
        if(i == 4) {
            for(i = 0; i < 4; i++) {
                a4[i] = b64_lookup(a4[i]);
            }
            a4_to_a3(a3, a4);
            for(i = 0; i < 3; i++) {
                output[decLen++] = a3[i];
            }
            i = 0;
        }
    }

    if(i) {
        for(j = i; j < 4; j++) {
            a4[j] = '\0';
        }
        for(j = 0; j < 4; j++) {
            a4[j] = b64_lookup(a4[j]);
        }
        a4_to_a3(a3, a4);
        for(j = 0; j < i - 1; j++) {
            output[decLen++] = a3[j];
        }
    }
    output.resize(decLen);
    return output;
}

void Base64::a3_to_a4(unsigned char *a4, const unsigned char *a3) {
    a4[0] = (a3[0] & 0xfc) >> 2;
    a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
    a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
    a4[3] = (a3[2] & 0x3f);
}

void Base64::a4_to_a3(unsigned char *a3, const unsigned char *a4) {
    a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
    a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
    a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

unsigned char Base64::b64_lookup(char c) {
    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 71;
    if(c >= '0' && c <= '9') return c + 4;
    if(c == '+') return 62;
    if(c == '/') return 63;
    return -1;
}

String Base64::urlencode(const String &str) {
    const char *msg = str.c_str();
    const char *hex = "0123456789ABCDEF";
    String encodedMsg = "";
    while(*msg != '\0') {
        if(('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[(unsigned char)*msg >> 4];
            encodedMsg += hex[*msg & 0xf];
        }
        msg++;
    }
    return encodedMsg;
}
