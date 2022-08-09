#pragma once

#include "SCCPNote.h"

namespace sccp {
class SCCPNode : public SCCPNote
{
public:
    /* Capture keywords */
    int captureStmt(IStmt *stmt);
    int captureNode(INode *node);
    int matchKey(std::string &str);
    int matchWord(std::string &str);
private:
};//SCCPNode
}
