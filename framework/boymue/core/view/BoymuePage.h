//
//  BoyiaPage.h
//  core
//
//  Created by yanbo on 2022/9/6.
//

#ifndef BoyiaPage_h
#define BoyiaPage_h

#include "Layout.h"
#include "Document.h"

namespace boymue {
class BoymuePage {
public:
    BoymuePage(int pageId);
    
private:
    // 页面dom结构
    std::unique_ptr<dom::Document> m_dom;
    int m_pageId;
};
}

#endif /* BoyiaPage_h */
