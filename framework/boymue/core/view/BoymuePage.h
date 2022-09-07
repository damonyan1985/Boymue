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
    std::unique_ptr<Document> m_dom;
    // 根结构，用于页面切换使用，定义为一个UI栈
    std::unique_ptr<Layout> m_root;
    int m_pageId;
};
}

#endif /* BoyiaPage_h */
