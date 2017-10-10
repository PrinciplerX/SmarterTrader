#include "stringhelper.h"

using std::string;

void StringHelper::split(const std::string &src, const std::string &separator, std::vector<std::string> &result){
    string::size_type index = 0;
    string::size_type nextTokenStart = 0;

    while ((nextTokenStart = src.find_first_not_of(separator, index)) != string::npos){
        index = src.find_first_of(separator, nextTokenStart);
        if (index == string::npos){
            result.push_back(src.substr(nextTokenStart));
            break;
        }
        else{
            result.push_back(src.substr(nextTokenStart, index - nextTokenStart));
        }
    }
}

void StringHelper::maxSplit(const std::string &src, const std::string &separator, std::vector<std::string> &result)
{
    string::size_type index = 0;
    string::size_type nextTokenStart = src.find_first_of(separator, index);

    if (nextTokenStart == string::npos) {
        result.push_back(src);
        return;
    }

    if (nextTokenStart != 0) {
        index = nextTokenStart;
        result.push_back(src.substr(0, nextTokenStart));
    } else {
        result.push_back(string(""));
    }

    while ((nextTokenStart = src.find_first_of(separator, index)) != string::npos){
        // tail
        if (nextTokenStart + separator.length() == src.size()) {
            result.push_back(string(""));
            break;
        }

        index = src.find_first_of(separator, nextTokenStart + separator.length());
        if (index == string::npos){
            result.push_back(src.substr(nextTokenStart + separator.length()));
            break;
        } else {
            if (index == nextTokenStart + separator.length()) {
                result.push_back(string(""));
            } else {
                result.push_back(src.substr(nextTokenStart + separator.length(),
                                            index - nextTokenStart - separator.length()));
            }
        }
    }
}

bool StringHelper::endsWith(const std::string &src, const std::string &suffix){
    if (src.length() < suffix.length())
        return false;
    string srcEnds = src.substr(src.length() - suffix.length(), suffix.length());
    return srcEnds == suffix;
}

bool StringHelper::startsWith(const std::string &src, const std::string &suffix){
    if (src.length() < suffix.length())
        return false;
    string srcEnds = src.substr(0, suffix.length());
    return srcEnds == suffix;
}

bool StringHelper::contains(const std::string &src, char ch)
{
    return src.find(ch) != string::npos;
}

std::string StringHelper::convertCodec(const std::string &src, bool *ok, const std::string &srcCodec, const std::string &destCodec)
{
    bool result = true;
    size_t maxLength = src.size() * 4;
    size_t outputLength = maxLength;
    string dest;
    dest.assign(maxLength, 0);

    iconv_t codec = iconv_open(destCodec.c_str(), srcCodec.c_str());
    if(codec == (iconv_t) -1){
        result = false;
    }else{
        size_t srcLength = src.length();

        char* srcStr = (char*)src.data();
        char* destStr = (char*)dest.data();

        if (iconv(codec, (const char**)&srcStr, &srcLength, &destStr, &outputLength) == (size_t) -1){
            result = false;
        }
    }

    dest.resize(maxLength - outputLength);
    if(ok) *ok = result;
    return dest;
}

std::string StringHelper::toUpper(const std::string &source)
{
    string result = source;
    for (int i = 0; i < source.size(); i++) {
        if (isalpha(result[i])) {
            result[i] = toupper(result[i]);
        }
    }
    return result;
}

std::string StringHelper::toLower(const std::string &source)
{
    string result = source;
    for (int i = 0; i < source.size(); i++) {
        if (isalpha(result[i])) {
            result[i] = tolower(result[i]);
        }
    }
    return result;
}
