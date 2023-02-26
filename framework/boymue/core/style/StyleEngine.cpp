// StyleEngine µœ÷
#include "StyleEngine.h"

namespace boymue {
namespace css {
StyleEngine::StyleEngine()
    : m_parser(this){
    initTags();
}

void StyleEngine::initTags() {
    m_tags["width"] = kWidth;
    m_tags["height"] = kHeight;
    m_tags["color"] = kColor;
    m_tags["font"] = kFont;
}

int StyleEngine::getType(const String& tag) {
    return m_tags[tag];
}

void StyleEngine::parseCSS(const String& sheetText) {
    m_sheets.push_back(m_parser.parse(sheetText));
}

void StyleEngine::apply(dom::DocumentElement* elem) {
}
}

}