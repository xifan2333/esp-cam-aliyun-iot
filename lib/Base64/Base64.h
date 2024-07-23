/**
 * @file Base64.h
 * @author 稀饭
 * @brief 定义了 Base64 类，用于 Base64 编码和解码。
 */

#ifndef BASE64_H
#define BASE64_H

#include <Arduino.h>
#include <vector>
#include "Logger.h"

extern Logger logger;
/**
 * ### Base64 编码解码类
 * 
 * 该类提供了 Base64 编码和解码的功能。
 * 
 * #### 属性
 * 
 * - `b64Alphabet`：Base64 编码表
 * 
 * #### 方法
 * 
 * - `encode()`：对输入数据进行 Base64 编码
 * - `decode()`：对输入数据进行 Base64 解码
 * - `urlEncode()`：对输入字符串进行 URL 编码
 * - `bytes3To4()`：将 3 字节数据转换为 4 字节数据
 * - `bytes4To3()`：将 4 字节数据转换为 3 字节数据
 * - `lookupBase64()`：查找 Base64 编码表
 * 
 */
class Base64 {
public:
    static String encode(const uint8_t *input, size_t inputLength);
    static std::vector<uint8_t> decode(const String &input);
    static String urlEncode(const String &str);

private:
    static const char base64Alphabet[];

    static void bytes3To4(unsigned char *output, const unsigned char *input);
    static void bytes4To3(unsigned char *output, const unsigned char *input);
    static unsigned char lookupBase64(char character);
};

#endif // BASE64_H
