// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef DocumentElement_h
#define DocumentElement_h

#include "StringUtil.h"

namespace boymue {
class Document;
class Layout;
class DocumentElement {
public:
    DocumentElement(Document* dom);

    virtual void parseAttribute(const char** atts) {}
    void addChild(DocumentElement* element);
    virtual void removeChild(DocumentElement* element) {};
    // layout与element一一对应
    Layout* layout();
    
    int elementId() const;
    const String& styleId() const;
    
    void setElementId(int elemId);
    void setStyleId(const String& styleId);
    void setProperty(const String& key, const String& value);
    
private:
    Document* m_dom;
    // 0是无效id, elemId为js端分配的唯一id
    int m_elemId;
    // 样式id, 通过styleId可以匹配样式
    String m_styleId;
    // 属性
    HashMap<String, String> m_properties;
};
}  // namespace boymue
#endif  // !DocumentElement_h
