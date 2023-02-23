#include "StyleSheet.h"

namespace boymue {
SimpleSelector::SimpleSelector(const String& text)
    : filter(text) {}

StyleSheet::StyleSheet() {
    
}

StyleSheet::~StyleSheet() {
    
}

void StyleSheet::addRule(SharedPtr<CSSRule> rule) {
    m_ruleList.push_back(rule);
}

bool StyleSheet::empty() {
    return m_ruleList.size() == 0;
}

void StyleSheet::apply() {

}
}
