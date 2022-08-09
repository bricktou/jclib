#pragma once

#include "SCCPStmt.h"

namespace sccp {
class SCCPConvert : public SCCPStmt,
                    public IConvert
{
public:
    int startConvert(convert_kind convert);
    
    int defStmt(IStmt *stmt);
    
    int putNote(std::string &note);
    
    int openFile();
    int inclOpenFile();
    int reOpenFile();
    int inclFile(file_kind kind, std::string &name);
    int eoFile();
    
    int blockStart();
    int blockOver();
    
    SCCPConvert(IAccessSCCT *accessScct);
    ~SCCPConvert(void){};
private:
};//SCCPConvert
}
