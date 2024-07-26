/**
 * @file _Base64.h
 * @brief Defines the _Base64 class methods for Base64 encoding and decoding.
 */

#ifndef _BASE64_H_
#define _BASE64_H_

#include <Arduino.h>
#include <vector>

class _Base64
{
public:
    // Base64 encode methods
    static String encode(const uint8_t *input, size_t inputLength);
    static std::vector<uint8_t> encodeToVector(const uint8_t *input, size_t inputLength);
    static String encode(const String &inputString);
    static std::vector<uint8_t> encodeToVector(const String &inputString);

    // URL-safe Base64 encode methods
    static String urlSafeEncode(const uint8_t *input, size_t inputLength);
    static String urlSafeEncode(const String &inputString);
    static std::vector<uint8_t> urlSafeEncodeToVector(const uint8_t *input, size_t inputLength);
    static std::vector<uint8_t> urlSafeEncodeToVector(const String &inputString);

    // Base64 decode methods
    static std::vector<uint8_t> decode(const String &input);
    static String decodeToString(const String &input);

    // URL-safe Base64 decode methods
    static std::vector<uint8_t> urlSafeDecode(const String &inputString);
    static String urlSafeDecodeToString(const String &inputString);


private:
    static void bytes3To4(unsigned char *output, const unsigned char *input);
    static void bytes4To3(unsigned char *output, const unsigned char *input);
    static unsigned char lookupBase64(char character);

    static const char base64Alphabet[];
};

#endif // _BASE64_H_
