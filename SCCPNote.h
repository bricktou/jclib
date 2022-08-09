#pragma once

#include "SCCPFile.h"

namespace sccp {
class SCCPNote : public SCCPFile
{
public:
    void addNote(IStmt *stmt);
private:
    std::string getNote(INode *node, bool conditional = false);
};//SCCPNote
}
