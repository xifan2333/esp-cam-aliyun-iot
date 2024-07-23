/**
 * @file Base64.cpp
 * @author 稀饭
 * @brief 实现了 Base64 类的方法，用于 Base64 编码和解码。
 */

#include "Base64.h"

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

// Base64 字符表

const char PROGMEM Base64::base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "abcdefghijklmnopqrstuvwxyz"
                                           "0123456789+/";
/**
 * ### Base64 编码
 * 
 * 对输入数据进行 Base64 编码。
 * 
 * #### 参数
 * 
 * - `input`：输入数据
 * - `inputLength`：输入数据长度
 * 
 * #### 返回
 * 
 * - String：Base64 编码后的字符串
 */
String Base64::encode(const uint8_t *input, size_t inputLength)
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
    String result = String(output.data());
    logger.info(result, "Base64");
    return result;
}
/**
 * ### Base64 编码
 * 
 * 对输入数据进行 Base64 编码。
 * 
 * #### 参数
 * 
 * - `input`：输入数据
 * 
 * #### 返回
 * 
 * - String：Base64 编码后的字符串
 */
std::vector<uint8_t> Base64::decode(const String &input)
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

void Base64::bytes3To4(unsigned char *output, const unsigned char *input)
{
    output[0] = (input[0] & 0xfc) >> 2;
    output[1] = ((input[0] & 0x03) << 4) + ((input[1] & 0xf0) >> 4);
    output[2] = ((input[1] & 0x0f) << 2) + ((input[2] & 0xc0) >> 6);
    output[3] = (input[2] & 0x3f);
}

void Base64::bytes4To3(unsigned char *output, const unsigned char *input)
{
    output[0] = (input[0] << 2) + ((input[1] & 0x30) >> 4);
    output[1] = ((input[1] & 0xf) << 4) + ((input[2] & 0x3c) >> 2);
    output[2] = ((input[2] & 0x3) << 6) + input[3];
}

unsigned char Base64::lookupBase64(char character)
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

String Base64::urlEncode(const String &str)
{
    const char *inputString = str.c_str();
    const char *hexCharacters = "0123456789ABCDEF";
    String encodedString = "";
    while (*inputString != '\0')
    {
        if (('a' <= *inputString && *inputString <= 'z') || ('A' <= *inputString && *inputString <= 'Z') || ('0' <= *inputString && *inputString <= '9') || *inputString == '-' || *inputString == '_' || *inputString == '.' || *inputString == '~')
        {
            encodedString += *inputString;
        }
        else
        {
            encodedString += '%';
            encodedString += hexCharacters[(unsigned char)*inputString >> 4];
            encodedString += hexCharacters[*inputString & 0xf];
        }
        inputString++;
    }
    return encodedString;
}
