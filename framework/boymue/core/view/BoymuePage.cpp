//
//  BoyiaPage.cpp
//  core
//
//  Created by yanbo on 2022/9/6.
//

#include "BoymuePage.h"

namespace boymue {
BoymuePage::BoymuePage(int pageId)
  : m_pageId(pageId)
  , m_dom(std::make_unique<dom::Document>()) {
}

}
