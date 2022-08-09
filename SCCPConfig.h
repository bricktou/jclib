#pragma once

#include "SCCPBase.h"

namespace sccp {
class SCCPConfig : public SCCPBase
{
public:
    int Initialize();
private:
    int initKeyWords(XMLElement *root);
    int initImports(XMLElement *root);
    int initClasses(XMLElement *root);
    int initMembers(XMLElement *root);
    int initMethods(XMLElement *root);
    int initWords(XMLElement *root);
};//SCCPConfig
}
