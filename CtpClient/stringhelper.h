#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>
#include <vector>
#include <iconv.h>

class StringHelper
{
public:
    template<typename ... Args>
    static int string_format(std::string& result, const std::string& format, Args ... args)
    {
        // 为'\0'留出额外的空间。
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        result.resize(size);
        int n = std::snprintf((char*)result.c_str(), size, format.c_str(), args ...);
        result.resize(size - 1);
        return n;
    }

    static void split(const std::string& src, const std::string& separator, std::vector<std::string>& result);

    static void maxSplit(const std::string& src, const std::string& separator, std::vector<std::string>& result);

    static bool endsWith(const std::string& src, const std::string& suffix);

    static bool startsWith(const std::string& src, const std::string& suffix);

    static bool contains(const std::string& src, char ch);

    static std::string convertCodec(const std::string& src,
                                    bool* ok = NULL,
                                    const std::string& srcCodec = std::string("gb18030"),
                                    const std::string& destCodec = std::string("utf-8"));

    static std::string toUpper(const std::string& source);
    static std::string toLower(const std::string& source);
};

#endif // STRINGHELPER_H
