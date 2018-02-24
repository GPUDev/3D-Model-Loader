//
//  Utility.hpp
//
//  Created by Gaurav Sharma on 15/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp
#include <string>
#include <unordered_map>
#include "DataType.h"
using namespace std;
class Utility
{
    static string lineIdentifierTags[];
    unordered_map<string, int> lineIdentifierTagMap;
    unordered_map<const char*, CurveSurfTypeEnum> curveSurfTypeMap;
    public:
        Utility();
        int getTagValue(string &key);
        CurveSurfTypeEnum getCStype(const char*);
    
};

#endif /* Utility_hpp */
