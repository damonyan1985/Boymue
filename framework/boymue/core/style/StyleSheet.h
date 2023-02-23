//
//  StyleSheet.h
//  core
//
//  Created by yanbo on 2022/8/19.
//

#ifndef StyleSheet_h
#define StyleSheet_h

#include "StringUtil.h"

namespace boymue {
class SimpleSelector {
public:
    SimpleSelector(const String& text);
    ~SimpleSelector() {};

    String filter;
};

class CSSSelector {
public:
    CSSSelector() {};
    ~CSSSelector() {};

    Vector<SharedPtr<SimpleSelector>> filters;
};

class CSSDeclarations {
public:
    CSSDeclarations() {};
    ~CSSDeclarations() {};
    
    struct CSSProperty {
        int intVal;
        String strVal;
    };
    
    HashMap<int, CSSProperty> propertyMap;
};

using CSSSelectorGroup = Vector<SharedPtr<CSSSelector>>;
class CSSRule {
public:
    CSSRule() {};
    ~CSSRule() {};
    
    CSSSelectorGroup selectors;
    CSSDeclarations declarations;
};

class StyleSheet
{
public:
    StyleSheet();
    ~StyleSheet();

    bool empty();
    void addRule(SharedPtr<CSSRule> rule);
    void apply();

private:
    List<SharedPtr<CSSRule>> m_ruleList;
};
    
}

#endif
