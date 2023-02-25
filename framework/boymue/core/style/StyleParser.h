//
//  StyleParser.h
//  core
//
//  Created by yanbo on 2022/8/19.
//

#ifndef StyleParser_h
#define StyleParser_h

#include "StyleSheet.h"

namespace boymue {
namespace css {
class StyleEngine;
class StyleParser
{
public:
    StyleParser(StyleEngine* engine);
    ~StyleParser();

    SharedPtr<StyleSheet> parse(const String& cssText);
private:
    void addDeclaration(CSSRule* rule, Vector<String>& kv);
    StyleEngine* m_engine;
};
}    
}

#endif
