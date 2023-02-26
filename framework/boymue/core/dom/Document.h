// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef Document_h
#define Document_h

#include <stack>
#include <string>

#include "DocumentElement.h"
#include "TextElement.h"
#include "StringUtil.h"

namespace boymue {
namespace dom {
// Create and run in ui thread
class Document {
public:
    enum DOMCmdType {
        kAddNode = 1,
        kSetProp,
        kRemoveNode,
        kReplaceNode
    };
    
    Document();
    // 利用XML内容来初始化document
    void parseFromXML(const String& content);
    Stack<DocumentElement*>* getParseStack();
    DocumentElement* createElement(int tag, const char** atts,
                                 DocumentElement* parent);
    
    void createElement(int tag, int uid, int pid);
    void removeElement(int pid, int uid);
    
    void addUniqueElement(DocumentElement* elem);
    void addStyleElement(DocumentElement* elem);
    
    void setElementProperty(int uid, const String& key, const String& value);

private:
    Stack<DocumentElement*> m_parseStack;
    DocumentElement* m_root;
    // 使用elem id查找DocumentElement
    HashMap<int, DocumentElement*> m_uniqueElems;
    // 使用style id查找DocumentElement
    HashMap<String, DocumentElement*> m_styleElems;
};
}
}  // namespace boymue
#endif  // !Document_h
