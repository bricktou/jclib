/********************************************************
 * Copyright (c) 2004-2022, Brick Technologies Co., Ltd
 * All rights reserved.
 * Home page:http://www.bricktou.com
********************************************************/
/**
 * Java conversion library-jclib
 * Application scope of plug-in:
 * 1. Change Library.
 * 2. Upgrade Library.
 * 3. Add comments.
 * 4. Specification document.
 */
 
#include "SCCPBase.h"

HMODULE ghModule;

namespace sccp {
SCCPBase::SCCPBase(void) {
    /* Set Pretreatment flag */
    preFlag = 1;
    hf = NULL;
}
bool SCCPBase::existFile(const std::string &name) {
    return (_access(name.c_str(), 0) != -1);
}
int SCCPBase::createFile() {
    std::string file = getOutRoot();
    if(file.empty())
        EXIT_INFO("Output root directory is empty!");
    
    file += aScct->getSrcPath();
    if(_access(file.c_str(), 0)) {
        if(createPath(file)) return PLI_EXIT;
    }
    
    if(NULL != hf) fclose(hf);
    
    file += aScct->getSrcFileName();
    fopen_s(&hf, file.c_str(), "wt+");
    if(NULL == hf) {
        char buf[300] = { 0 };
        sprintf_s(buf, "Create %s fail!", file.c_str());
        MessageBoxA(NULL, buf, "Message", MB_OK | MB_ICONWARNING);
        return PLI_EXIT;
    }
    return PLI_OK;
}
int SCCPBase::createPath(const std::string &path) {
    for(size_t i = 0; i < path.length(); i++) {
        if(('\\' != path[i]) && ('/' != path[i])) continue;
        if(!_access(path.substr(0, i).c_str(), 0)) continue;
        if(_mkdir(path.substr(0, i).c_str()))
            EXIT_INFO((char*)path.substr(0, i).c_str());
    }
    return PLI_OK;
}
int SCCPBase::copyFile() {
    std::string src = aScct->getSrcRoot();
    if(src.empty()) return PLI_EXIT;
    
    std::string des = getOutRoot();
    if(des.empty()) return PLI_EXIT;
    
    src += aScct->getSrcPath();
    des += aScct->getSrcPath();
    
    if(_access(des.c_str(), 0)) {
        if(createPath(des)) return PLI_EXIT;
    }
    
    src += aScct->getSrcFileName();
    des += aScct->getSrcFileName();
    
    CopyFile(src.c_str(), des.c_str(), false);
    return PLI_OK;
}
std::string SCCPBase::getOutRoot() {
    std::string root = aScct->getOutRoot();
    if(root.empty()) root = aScct->getSrcRoot();
    
    if(!root.compare(aScct->getSrcRoot()))
        /* Avoid overwriting source files. */
        root += "Convert\\";
    
    return root;
}
}//sccp
