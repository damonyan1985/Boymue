//
//  StringUtil.c
//  core
//
//  Created by yanbo on 2022/8/24.
//

#include "StringUtil.h"

namespace boymue {
Vector<String> StringUtil::split(const String& str, const String& pattern) {
    Vector<String> result;
    String::size_type begin = 0;
    String::size_type i = 0;
    while ((i = str.find(pattern, begin)) != String::npos) {
        result.push_back(std::move(str.substr(begin, i - begin)));
        begin = i + pattern.length();
    }
    
    if (i == -1 && begin < str.length()) {
        result.push_back(std::move(str.substr(begin, str.length() - begin)));
    }
    
    return result;
}
    
void StringUtil::join(const Vector<String>& buffer,
                      String& dest,
                      const String& defStr) {
    if (buffer.size() == 0) {
        dest = defStr;
        return;
    }
    
    for (int i = 0; i < buffer.size(); i++) {
        dest.append(buffer[i]);
    }
}
    
String& StringUtil::trim(String& str) {
    if (str.empty()) {
        return str;
    }
    
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}

bool StringUtil::isspace(const char* str, int len) {
    for (int i = 0; i < len; i++) {
        if (!::isspace(*(str + i))) {
            return false;
        }
    }
    
    return true;
}

bool StringUtil::startWidth(const String& str, const String& head) {
    return str.compare(0, head.size(), head) == 0;
}

bool StringUtil::endWidth(const String& str, const String& tail) {
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}
    
}
