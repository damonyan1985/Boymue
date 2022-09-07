// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef DomTags_h
#define DomTags_h

#include "StringUtil.h"

namespace boymue {
class DomTags {
public:
    enum TagType {
        kView,
        kButton,
        kImage,
        kText,
        kRadio,
        kCheckBox,
        kInput,
        kIcon,
    };

    static DomTags* instance();
    void initDomTags();
    int getTag(const String& key);

private:
    DomTags();
    HashMap<String, int> m_map;
};
}
#endif
