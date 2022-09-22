//
//  StringUtil.h
//  core
//
//  Created by yanbo on 2022/8/24.
//

#ifndef StringUtil_h
#define StringUtil_h

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <stack>
#include <memory>

namespace boymue {

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using List = std::list<T>;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename T>
using Stack = std::stack<T>;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using OwnerPtr = std::unique_ptr<T>;

const String kAppPrefix = "boymue";
const String kAttrSplitFlag = ":";

class StringUtil {
public:
    // 分割字符串
    static Vector<String> split(const String& str, const String& pattern);
    // 合并字符串，如果字符串合集为空，则使用默认字符串
    static void join(const Vector<String>& buffer, String& dest, const String& defStr);
    
    // 去除字符串两边的空格
    static String& trim(String& str);
    
    // 判断整个字符串是否是空白串
    static bool isspace(const char* str, int len);
    
    // 判断字符串开头是否包含
    static bool startWidth(const String& str, const String& head);
    
    // 判断字符串结尾是否包含
    static bool endWidth(const String& str, const String& tail);
};
}

#endif /* StringUtil_h */
