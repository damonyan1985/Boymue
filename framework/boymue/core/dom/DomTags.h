// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef DomTags_h
#define DomTags_h

#include "StringUtil.h"

namespace boymue {
class DomTags {
public:
    // 基础组件Tag
    enum TagType {
        kView,
        kButton,
        kImage,
        kText,
        kTextField,
    };

    static DomTags* instance();
    void initDomTags();
    int getTag(const String& key);
    
    // 支持自定义tag
    void addExtensionTag(const String& key, int tag);

private:
    DomTags();
    HashMap<String, int> m_map;
};
}
#endif
