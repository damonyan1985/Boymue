// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef StyleEngine_h
#define StyleEngine_h

#include "StyleParser.h"
#include "DocumentElement.h"

namespace boymue {
namespace css {
class StyleEngine {
public:
    enum StyleType {
        kWidth = 0,
        kHeight = 1,
        kStyleInherit = 2,
        kColor = 3,
        kFont = 4,
        kBackgroundColor = 5,
        kMargin = 6,
        kMarginTop = 7,
        kMarginRight = 8,
        kMarginBottom = 9,
        kMarginLeft = 10,
        kPadding = 11,
        kPaddingTop = 12,
        kPaddingRight = 13,
        kPaddingBottom = 14,
        kPaddingLeft = 15,
        kFontSize = 16,
        kFontFamily = 17,
        ///  WebCore CSSPropertyNames
        kLeft = 18,
        kTop = 19,
        kRight = 20,
        kBottom = 21,
        kPosition = 22,
        kZIndex = 23,
        kMinWidth = 24,
        kMinHeight = 25,
        kMaxWidth = 26,
        kMaxHeight = 27,
        kOpacity = 28,
        kVisibility = 29,
        kDisplay = 30,
        kFontWeight = 31,
        kFontStyle = 32,
        kLineHeight = 33,
        kTextAlign = 34,
        kTextDecoration = 35,
        kLetterSpacing = 36,
        kWordSpacing = 37,
        kBorderWidth = 38,
        kBorderTopWidth = 39,
        kBorderRightWidth = 40,
        kBorderBottomWidth = 41,
        kBorderLeftWidth = 42,
        /// ?? kColor??kBackgroundColor ???????/??��??
        kBorderColor = 43,
        kBorderRadius = 44,
        kOverflow = 45,
        kOverflowX = 46,
        kOverflowY = 47,
        kBackgroundImage = 48,
        kBorderTopLeftRadius = 49,
        kBorderTopRightRadius = 50,
        kBorderBottomRightRadius = 51,
        kBorderBottomLeftRadius = 52,
    };

    StyleEngine();

    void parseCSS(const String& sheetText);
    int getType(const String& tag) const;
    void apply(dom::DocumentElement* root);

private:
    void initTags();

    StyleParser m_parser;
    Vector<SharedPtr<StyleSheet>> m_sheets;
    HashMap<String, int> m_tags;
};
}  // namespace css
}  // namespace boymue
#endif  // !StyleEngine_h
