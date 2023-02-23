#include "StyleParser.h"
#include "StyleEngine.h"

namespace boymue {
StyleParser::StyleParser(StyleEngine* engine) : m_engine(engine) {}
StyleParser::~StyleParser() {}  

SharedPtr<StyleSheet> StyleParser::parse(const String& css) {
    if (css.empty()) {
        return nullptr;
    }

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


    SharedPtr<StyleSheet> sheet(new StyleSheet());
    begin = 0;
    // 解析css规则
    while ((open = cssText.find("{", begin)) != String::npos
           && (close = cssText.find("}", begin) != String::npos)) {
        // 创建CSS规则
        SharedPtr<CSSRule> rule(new CSSRule());
        sheet->addRule(rule);
        // 提取selector字符串
        String selectorText = std::move(cssText.substr(begin, open));
        // 提取declaration字符串
        String declarationsText = std::move(cssText.substr(open + 1, close - 1));

        // 去除左右空白字符
        //StringUtil::trim(selectorText);
        //StringUtil::trim(declarationsText);

        // selector group类似 div .mycls, #p .mycls1
        // 通过分割逗号解出selector列表
        Vector<String> selectors =
            std::move(StringUtil::split(selectorText, ","));
        // 将selector拆分成simpleselector
        if (selectors.size() > 0) {
            SharedPtr<CSSSelector> selector(new CSSSelector());
            for (int i = 0; i < selectors.size(); i++) {
                Vector<String> filters = std::move(StringUtil::split(selectors[i], " "));
                for (int f = 0; f < filters.size(); f++) {
                    StringUtil::trim(filters[i]);
                    selector->filters.push_back(SharedPtr<SimpleSelector>(new SimpleSelector(filters[i])));
                }
            } 
            rule->selectors.push_back(selector);
        }

        // 每条delaration以分号结尾
        Vector<String> declarationTexts =
            std::move(StringUtil::split(declarationsText, ";"));
        for (int i = 0; i < declarationTexts.size(); i++) {
            Vector<String> kv =
                std::move(StringUtil::split(declarationTexts[i], ":"));
            // CSS语法错误，返回null
            if (kv.size() != 2) {
                return nullptr;
            }

            addDeclaration(rule.get(), kv);
        }
         

        begin = close + 1;
    }
    return sheet->empty() ? nullptr : sheet;
}

void StyleParser::addDeclaration(CSSRule* rule, Vector<String>& kv) {
    CSSDeclarations::CSSProperty prop;
    int type = m_engine->getType(kv[0]);
    switch (type)
    {
    case StyleEngine::kWidth: {
        if (StringUtil::endWith(kv[1], "px")) {
            prop.intVal = StringUtil::stringToInt(kv[1].substr(0, kv[1].size() - 2));
        }
    } break;
    default:
        return;
    }

    rule->declarations.propertyMap[type] = prop;
}
}
