// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef StyleEngine_h
#define StyleEngine_h

#include "StyleParser.h"
#include <list>

namespace boymue {
class StyleEngine {
private:
    StyleParser m_parser;
    std::list<StyleSheet> m_sheets;
};
}  // namespace boymue
#endif  // !StyleEngine_h