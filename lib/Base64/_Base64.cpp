/**
 * @file _Base64.cpp
 * @brief Implements the _Base64 class methods for Base64 encoding and decoding.
 */

#include "_Base64.h"

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

// Base64 character table
const char PROGMEM _Base64::base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                               "abcdefghijklmnopqrstuvwxyz"
                                               "0123456789+/";

// Base64 encode methods
String _Base64::encode(const uint8_t *input, size_t inputLength)
{
    std::vector<char> output((inputLength + 2 - ((inputLength + 2) % 3)) / 3 * 4 + 1);
    int i = 0, j = 0;
    int encLen = 0;
    unsigned char bytes3[3];
    unsigned char bytes4[4];

    while (inputLength--)
    {
        bytes3[i++] = *(input++);
        if (i == 3)
        {
            bytes3To4(bytes4, bytes3);
            for (i = 0; i < 4; i++)
            {
                output[encLen++] = pgm_read_byte(&base64Alphabet[bytes4[i]]);
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            bytes3[j] = '\0';
        }
        bytes3To4(bytes4, bytes3);
        for (j = 0; j < i + 1; j++)
        {
            output[encLen++] = pgm_read_byte(&base64Alphabet[bytes4[j]]);
        }
        while ((i++ < 3))
        {
            output[encLen++] = '=';
        }
    }
    output[encLen] = '\0';
    return String(output.data());
}

std::vector<uint8_t> _Base64::encodeToVector(const uint8_t *input, size_t inputLength)
{
    std::vector<uint8_t> output((inputLength + 2 - ((inputLength + 2) % 3)) / 3 * 4);
    int i = 0, j = 0;
    int encLen = 0;
    unsigned char bytes3[3];
    unsigned char bytes4[4];

    while (inputLength--)
    {
        bytes3[i++] = *(input++);
        if (i == 3)
        {
            bytes3To4(bytes4, bytes3);
            for (i = 0; i < 4; i++)
            {
                output[encLen++] = pgm_read_byte(&base64Alphabet[bytes4[i]]);
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            bytes3[j] = '\0';
        }
        bytes3To4(bytes4, bytes3);
        for (j = 0; j < i + 1; j++)
        {
            output[encLen++] = pgm_read_byte(&base64Alphabet[bytes4[j]]);
        }
        while ((i++ < 3))
        {
            output[encLen++] = '=';
        }
    }
    return output;
}

String _Base64::encode(const String &inputString)
{
    return encode((const uint8_t *)inputString.c_str(), inputString.length());
}

std::vector<uint8_t> _Base64::encodeToVector(const String &inputString)
{
    return encodeToVector((const uint8_t *)inputString.c_str(), inputString.length());
}

String _Base64::urlSafeEncode(const uint8_t *input, size_t inputLength)
{
    String encodedString = encode(input, inputLength);
    encodedString.replace('+', '-');
    encodedString.replace('/', '_');
    return encodedString;
}

String _Base64::urlSafeEncode(const String &inputString)
{
    return urlSafeEncode((const uint8_t *)inputString.c_str(), inputString.length());
}

std::vector<uint8_t> _Base64::urlSafeEncodeToVector(const uint8_t *input, size_t inputLength)
{
    std::vector<uint8_t> encodedVector = encodeToVector(input, inputLength);
    for (auto &ch : encodedVector)
    {
        if (ch == '+')
        {
            ch = '-';
        }
        else if (ch == '/')
        {
            ch = '_';
        }
    }
    return encodedVector;
}

std::vector<uint8_t> _Base64::urlSafeEncodeToVector(const String &inputString)
{
    return urlSafeEncodeToVector((const uint8_t *)inputString.c_str(), inputString.length());
}

// Base64 decode methods
std::vector<uint8_t> _Base64::decode(const String &input)
{
    int inputLength = input.length();
    std::vector<uint8_t> output((6 * inputLength) / 8 + 1);
    int i = 0, j = 0;
    int decLen = 0;
    unsigned char bytes3[3];
    unsigned char bytes4[4];

    const char *inputPtr = input.c_str();
    while (inputLength--)
    {
        if (*inputPtr == '=')
        {
            break;
        }
        bytes4[i++] = *(inputPtr++);
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                bytes4[i] = lookupBase64(bytes4[i]);
            }
            bytes4To3(bytes3, bytes4);
            for (i = 0; i < 3; i++)
            {
                output[decLen++] = bytes3[i];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            bytes4[j] = '\0';
        }
        for (j = 0; j < 4; j++)
        {
            bytes4[j] = lookupBase64(bytes4[j]);
        }
        bytes4To3(bytes3, bytes4);
        for (j = 0; j < i - 1; j++)
        {
            output[decLen++] = bytes3[j];
        }
    }
    output.resize(decLen);
    return output;
}

String _Base64::decodeToString(const String &inputString)
{
    std::vector<uint8_t> decodedVector = decode(inputString);
    String result;
    for (auto byte : decodedVector)
    {
        result += (char)byte;
    }
    return result;
}

std::vector<uint8_t> _Base64::urlSafeDecode(const String &inputString)
{
    String tempString = inputString;
    tempString.replace('-', '+');
    tempString.replace('_', '/');
    return decode(tempString);
}

String _Base64::urlSafeDecodeToString(const String &inputString)
{
    std::vector<uint8_t> decodedVector = decode(inputString);
    String result;
    for (auto byte : decodedVector)
    {
        result += (char)byte;
    }
    return result;
}

// Helper methods
void _Base64::bytes3To4(unsigned char *output, const unsigned char *input)
{
    output[0] = (input[0] & 0xfc) >> 2;
    output[1] = ((input[0] & 0x03) << 4) + ((input[1] & 0xf0) >> 4);
    output[2] = ((input[1] & 0x0f) << 2) + ((input[2] & 0xc0) >> 6);
    output[3] = (input[2] & 0x3f);
}

void _Base64::bytes4To3(unsigned char *output, const unsigned char *input)
{
    output[0] = (input[0] << 2) + ((input[1] & 0x30) >> 4);
    output[1] = ((input[1] & 0xf) << 4) + ((input[2] & 0x3c) >> 2);
    output[2] = ((input[2] & 0x3) << 6) + input[3];
}

unsigned char _Base64::lookupBase64(char character)
{
    if (character >= 'A' && character <= 'Z')
        return character - 'A';
    if (character >= 'a' && character <= 'z')
        return character - 71;
    if (character >= '0' && character <= '9')
        return character + 4;
    if (character == '+')
        return 62;
    if (character == '/')
        return 63;
    return -1;
}

