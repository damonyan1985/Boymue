//
//  StyleParser.h
//  core
//
//  Created by yanbo on 2022/8/19.
//

#ifndef StyleParser_h
#define StyleParser_h

#include "StyleSheet.h"
#include <string>

namespace boymue {
class StyleParser
{
public:
    StyleParser();
    ~StyleParser();

    StyleSheet* parse(const std::string& cssText);    
};
    
}

#endif