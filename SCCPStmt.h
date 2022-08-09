#pragma once

#include "SCCPImport.h"

namespace sccp {
class SCCPStmt : public SCCPImport
{
public:
    int methodStmt(IStmt *stmt);
    int outStmt(IStmt *stmt, std::string hCode = NULL);
private:
    int convertStmt(IStmt *stmt, std::string hCode);
    std::string getNodeCode(IStmt *stmt, INode *node);
    std::string nodeConvert(IStmt *stmt, INode *node, std::string lead);
    std::string keyReplace(IStmt *stmt, INode *node);
    std::string wordReplace(IStmt *stmt, INode *node);
    
    /* Processing line breaks */
    size_t nCount, nPos;
    
    /* First word of the previous line */
    std::string prevCode;
};//SCCPStmt
}
