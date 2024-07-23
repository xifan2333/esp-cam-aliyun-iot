/*
 * Copyright (c) 2013 Adam Rudd.
 * See LICENSE for more information
 * https://github.com/adamvr/arduino-base64 
 */
#ifndef _BASE64_H
#define _BASE64_H

#include <Arduino.h>
#include <vector>
#include "Logger.h"
extern Logger logger;
class Base64 {
public:
    // Encodes a byte array as base64
    static String encode(const uint8_t *input, size_t inputLen);

    // Decodes a base64 encoded string into bytes
    static std::vector<uint8_t> decode(const String &input);

    // URL encodes a given string
    static String urlencode(const String &str);

private:
    // Base64 alphabet table
    static const char b64_alphabet[];

    // Helper functions for internal use
    static void a3_to_a4(unsigned char *a4, const unsigned char *a3);
    static void a4_to_a3(unsigned char *a3, const unsigned char *a4);
    static unsigned char b64_lookup(char c);
};

#endif // _BASE64_H
