//
//  BoymueAppInfo.h
//  core
//
//  Created by yanbo on 2022/9/5.
//

#ifndef BoymueAppInfo_h
#define BoymueAppInfo_h

#include "StringUtil.h"
#include <stdint.h>

namespace boymue {
class BoymueAppInfo {
public:
    BoymueAppInfo();
    
    void parseConfig();
    
    int64_t appId;
    String appName;
};
}


#endif /* BoymueAppInfo_h */
