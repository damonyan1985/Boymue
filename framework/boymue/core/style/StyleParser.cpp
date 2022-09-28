#include "StyleParser.h"

namespace boymue {
StyleParser::StyleParser() {}
StyleParser::~StyleParser() {}  

StyleSheet* StyleParser::parse(const String& css) {
    // 去除注释
    String::size_type begin = 0;
    String::size_type open = 0;
    String::size_type close = 0;
    
    Vector<String> stringBuffer;
    while ((open = css.find("/*", begin)) != String::npos
           && (close = css.find("*/", begin)) != String::npos) {
        stringBuffer.push_back(std::move(css.substr(begin, open)));
        open = close + 2;
        begin = open;
    }

    // 提取出纯css样式文本，去除注释
    String cssText;
    StringUtil::join(stringBuffer, cssText, css);

    begin = 0;
    // 解析css规则
    while ((open = cssText.find("{", begin)) != String::npos
           && (close = cssText.find("}", begin) != String::npos)) {

        String selectorText = std::move(cssText.substr(begin, open));
        String declarationsText = std::move(cssText.substr(open + 1, close - 1));

        StringUtil::trim(selectorText);
        StringUtil::trim(declarationsText);

        Vector<String> filters =
            std::move(StringUtil::split(selectorText, " "));
        Vector<String> declarationText =
            std::move(StringUtil::split(selectorText, ";"));
        CSSRule* rule = new CSSRule();
    }
    return nullptr;
}
}
