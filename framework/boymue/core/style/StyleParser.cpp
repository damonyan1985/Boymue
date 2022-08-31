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
        stringBuffer.push_back(css.substr(begin, open));
        open = close + 2;
        begin = open;
    }

    String cssText;
    StringUtil::join(stringBuffer, cssText, css);

    begin = 0;
    while ((open = cssText.find("{", begin)) != -1
           && (close = cssText.find("}", begin) != -1)) {

        String selectorText = cssText.substr(begin, open);
        String declarationsText = cssText.substr(open + 1, close - 1);

        StringUtil::trim(selectorText);
        StringUtil::trim(declarationsText);

        Vector<String> filters = StringUtil::split(selectorText, " ");
        Vector<String> declarationText = StringUtil::split(selectorText, ";");
        CSSRule* rule = new CSSRule();
    }
    return nullptr;
}
}
