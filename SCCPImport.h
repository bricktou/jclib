#pragma once

#include "SCCPNode.h"

namespace sccp {
class SCCPImport : public SCCPNode
{
public:
    int importStmt(IStmt *stmt);
private:
    int convertImport(IStmt *stmt);
    
    int outImports(std::string key, std::string src);
};//SCCPImport
}
