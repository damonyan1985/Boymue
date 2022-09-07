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
    while ((open = css.find("/*", begin)) != -1
           && (close = css.find("*/", begin)) != -1) {
        stringBuffer.push_back(std::move(css.substr(begin, open)));
        open = close + 2;
        begin = open;
    }

    String cssText;
    StringUtil::join(stringBuffer, cssText, css);

    begin = 0;
    while ((open = cssText.find("{", begin)) != -1
           && (close = cssText.find("}", begin) != -1)) {

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
