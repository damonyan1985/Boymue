// StyleEngine 实现
#include "StyleEngine.h"

namespace boymue {namespace css {

StyleEngine::StyleEngine()
    : m_parser(this) {
    initTags();
}

void StyleEngine::initTags() {
    m_tags["width"] = kWidth;
    m_tags["height"] = kHeight;
    m_tags["color"] = kColor;
    m_tags["font"] = kFont;
    m_tags["background-color"] = kBackgroundColor;
    m_tags["margin"] = kMargin;
    m_tags["margin-top"] = kMarginTop;
    m_tags["margin-right"] = kMarginRight;
    m_tags["margin-bottom"] = kMarginBottom;
    m_tags["margin-left"] = kMarginLeft;
    m_tags["padding"] = kPadding;
    m_tags["padding-top"] = kPaddingTop;
    m_tags["padding-right"] = kPaddingRight;
    m_tags["padding-bottom"] = kPaddingBottom;
    m_tags["padding-left"] = kPaddingLeft;
    m_tags["font-size"] = kFontSize;
    m_tags["font-family"] = kFontFamily;
    m_tags["left"] = kLeft;
    m_tags["top"] = kTop;
    m_tags["right"] = kRight;
    m_tags["bottom"] = kBottom;
    m_tags["position"] = kPosition;
    m_tags["z-index"] = kZIndex;
    m_tags["min-width"] = kMinWidth;
    m_tags["min-height"] = kMinHeight;
    m_tags["max-width"] = kMaxWidth;
    m_tags["max-height"] = kMaxHeight;
    m_tags["opacity"] = kOpacity;
    m_tags["visibility"] = kVisibility;
    m_tags["display"] = kDisplay;
    m_tags["font-weight"] = kFontWeight;
    m_tags["font-style"] = kFontStyle;
    m_tags["line-height"] = kLineHeight;
    m_tags["text-align"] = kTextAlign;
    m_tags["text-decoration"] = kTextDecoration;
    m_tags["letter-spacing"] = kLetterSpacing;
    m_tags["word-spacing"] = kWordSpacing;
    m_tags["border-width"] = kBorderWidth;
    m_tags["border-top-width"] = kBorderTopWidth;
    m_tags["border-right-width"] = kBorderRightWidth;
    m_tags["border-bottom-width"] = kBorderBottomWidth;
    m_tags["border-left-width"] = kBorderLeftWidth;
    m_tags["border-color"] = kBorderColor;
    m_tags["border-radius"] = kBorderRadius;
    m_tags["border-top-left-radius"] = kBorderTopLeftRadius;
    m_tags["border-top-right-radius"] = kBorderTopRightRadius;
    m_tags["border-bottom-right-radius"] = kBorderBottomRightRadius;
    m_tags["border-bottom-left-radius"] = kBorderBottomLeftRadius;
    m_tags["overflow"] = kOverflow;
    m_tags["overflow-x"] = kOverflowX;
    m_tags["overflow-y"] = kOverflowY;
    m_tags["background-image"] = kBackgroundImage;
}

int StyleEngine::getType(const String& tag) const {
    auto it = m_tags.find(tag);
    return it == m_tags.end() ? -1 : it->second;
}

void StyleEngine::parseCSS(const String& sheetText) {
    SharedPtr<StyleSheet> sheet = m_parser.parse(sheetText);
    if (sheet && !sheet->empty()) {
        m_sheets.push_back(sheet);
    }
}

void StyleEngine::apply(dom::DocumentElement* root) {
    if (!root || m_sheets.empty()) {
        return;
    }
    StyleResolver::applyToSubtree(root, m_sheets);
}

}  // namespace css
}  // namespace boymue
