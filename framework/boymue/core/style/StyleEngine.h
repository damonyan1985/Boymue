// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef StyleEngine_h
#define StyleEngine_h

#include "StyleParser.h"
#include "DocumentElement.h"

namespace boymue {
class StyleEngine {
public:
    enum StyleType {
        kWidth,
        kHeight,
        kStyleInherit, // ���¶��ǿ��Ա��̳е�����
        kColor,
        kFont,
    };

    StyleEngine();

    // ����CSS�ı�
    void parseCSS(const String& sheetText);
    int getType(const String& tag);
    void apply(DocumentElement* elem);

private:
    void initTags();

    StyleParser m_parser;
    Vector<SharedPtr<StyleSheet>> m_sheets;
    HashMap<String, int> m_tags;
};
}  // namespace boymue
#endif  // !StyleEngine_h