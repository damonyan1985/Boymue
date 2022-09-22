// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef ViewElement_h
#define ViewElement_h

#include "DocumentElement.h"
#include "StringUtil.h"

namespace boymue {
class ViewElement : public DocumentElement {
public:
    ViewElement(Document* dom);
    virtual void parseAttribute(const char** atts);
    virtual void addChild(DocumentElement* element);
    virtual void removeChild(DocumentElement* element);
    
private:
    List<SharedPtr<DocumentElement>> m_children;
};
}  // namespace boymue
#endif  // !ViewElement
