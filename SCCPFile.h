#pragma once

#include "SCCPConfig.h"

namespace sccp {
class SCCPFile : public SCCPConfig
{
public:
    /* Directly output statements without change. */
    int outStmtToken(IStmt *stmt);
    
    void putIndent();
    
    /* Record file hierarchy */
    size_t Indent;
    /* neutralize indent */
    int neIndent;
    /* case|default */
    bool dIndent;
private:
    int outToken(IToken *token);
};//SCCPFile
}
