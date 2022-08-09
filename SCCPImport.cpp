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

#include "SCCPImport.h"

namespace sccp {
int SCCPImport::importStmt(IStmt *stmt) {
    int ret = captureStmt(stmt);
    if (!ret) return outStmtToken(stmt);
    else if (ret < 0) return PLI_EXIT;
    
    return convertImport(stmt);
}
int SCCPImport::convertImport(IStmt *stmt) {
    INode *node = stmt->getFirstNode();
    if (NULL == node) EXIT_INFO("First node is null!");
    std::string skey, simp;
    do {
        node_kind kind = node->getKind();
        if (simp.empty()) {
            if (lankey == kind) {
                if(skey.empty()) skey = node->getCode();
                //import status
                else skey += " " + node->getCode();
            }
            else if (libpackage == kind)
                simp = node->getCode();
            else {
                skey += " " + node->getCode();
                if (useLang.compare("Chinese"))
                    MessageBoxA(NULL, skey.c_str(), "Unknown statement", MB_OK);
                else MessageBoxA(NULL, skey.c_str(), "未知语句", MB_OK);
                return PLI_EXIT;
            }
        }
        else if ((isoper == kind)
        && !node->getCode().compare(";"))
            break;
        else simp += node->getCode();
    }while (!stmt->nodeMoveNext(node));
    
    chgNum++;
    return outImports(skey, simp);
}
int SCCPImport::outImports(std::string key, std::string src) {
    std::map<std::string, std::vector<std::string>>::iterator
        it = Imports.find(src);
    if ((it == Imports.end()) || it->second.empty()) {
        if (useLang.compare("Chinese"))
            MessageBoxA(NULL, src.c_str(), "No corresponding conversion item is configured!", MB_OK);
        else MessageBoxA(NULL, src.c_str(), "没有配置对应的转换项", MB_OK);
        return PLI_EXIT;
    }
    
    for (size_t i = 0; i < it->second.size(); i++) {
        size_t j = 0;
        for (; j < Imported.size(); j++) {
            if (Imported[j].compare(it->second[i])) continue;
            break;
        }
        if (j < Imported.size()) continue;
        
        if (NULL != hf)
            fprintf_s(hf, "%s %s;\n", key.c_str(), it->second[i].c_str());
        
        /**
         * Avoid repetition.
         */
        Imported.push_back(it->second[i]);
    }
    return PLI_OK;
}
}
