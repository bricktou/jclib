/********************************************************
 * Copyright (c) 2004-2022, Brick Technologies Co., Ltd
 * All rights reserved.
 * Home page:http://www.bricktou.com
********************************************************/
/**
 * Code conversion interface
 */
#ifndef _I_PLUGIN_CONVERT_H_
#define _I_PLUGIN_CONVERT_H_

// Output report succeeded
#ifndef PLI_OK
#define PLI_OK      0
#endif

// Stop the program running
#ifndef PLI_EXIT
#define PLI_EXIT    -1
#endif

/*
 * Reread file.
 * If the file can be converted directly,
 * you need to re read the file.
 * Note that this is only valid at the end of the file!
 */
#ifndef REREAD_FILE
#define REREAD_FILE 1
#endif

namespace sccp {
enum convert_kind {
    coFunction = 0,
    coProcedure,
    coBlock,
};
enum file_kind {
    userfile = 0,
    libfile,
    sqlfile,
};
struct IConvert
{
    virtual int startConvert(convert_kind convert) = 0;
    
    virtual int defStmt(IStmt *stmt) = 0;
    
    virtual int putNote(std::string &note) = 0;
    
    virtual int openFile() = 0;
    virtual int inclOpenFile() = 0;
    virtual int reOpenFile() = 0;
    virtual int inclFile(file_kind kind, std::string &name) = 0;
    virtual int eoFile() = 0;
    
    virtual int blockStart() = 0;
    virtual int blockOver() = 0;
};
}//sccp
#endif//_I_PLUGIN_CONVERT_H_
