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
class CSSSelector {
public:
    CSSSelector() {};
    ~CSSSelector() {};
    
    Vector<String> filters;
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

class CSSRule {
public:
    CSSRule() {};
    ~CSSRule() {};
    
    CSSSelector selector;
    CSSDeclarations declaration;
};

class StyleSheet
{
public:
    StyleSheet();
    ~StyleSheet();

};
    
}

#endif
