// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef DocumentElement_h
#define DocumentElement_h

namespace boymue {
class Document;
class Layout;
class DocumentElement {
public:
    DocumentElement(Document* dom);

    virtual void parseAttribute(const char** atts) {}
    void addChild(DocumentElement* element);
    Layout* layout();
    
private:
    Document* m_dom;
    int m_elemId;
};
}  // namespace boymue
#endif  // !DocumentElement_h
