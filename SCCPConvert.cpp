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

#include "SCCPConvert.h"

namespace sccp {
int SCCPConvert::startConvert(convert_kind convert) {
    if ((coFunction != convert)
    && (coBlock != convert)) return PLI_OK;
    
    neIndent = 0;
    dIndent = false;
    
    if (!aScct->stmtCount()) {
        /* Null function */
        if (NULL == hf) return PLI_OK;
        if (coFunction != convert) return PLI_OK;
        
        if (Indent) Indent--;
        else {
            if (NULL != hf) {
                fclose(hf);
                hf = NULL;
            }
            EXIT_INFO("File matching error!");
        }
        
        putIndent();
        fputs("}\n", hf);
        
        return PLI_OK;
    }
    
#if SELF_METHOD1 == METHODS
    funStmt fstmt = std::bind(&SCCPStmt::methodStmt, this,
        std::placeholders::_1);
    if (aScct->for_stmt(fstmt))
        return PLI_EXIT;
#else
    IStmt *stmt = aScct->getFirstStmt();
    if (NULL == stmt) {
        if (NULL != hf) {
            fclose(hf);
            hf = NULL;
        }
        EXIT_INFO("First stmt is null!");
    }
    
    do {
        if (methodStmt(stmt)) {
            /**
             * delete stmt. Must do!
             */
            aScct->freeStmt(stmt);
            SEND_INFO("fail", "");
            return PLI_EXIT;
        }
    }while (!stmt->moveNext());
    /**
     * delete stmt. Must do!
     */
    aScct->freeStmt(stmt);
#endif
    if ((NULL == hf) || copyFlag) return PLI_OK;
    if (coFunction != convert) return PLI_OK;
    
    if (Indent) Indent--;
    else {
        if (NULL != hf) {
            fclose(hf);
            hf = NULL;
        }
        EXIT_INFO("File matching error!");
    }
    
    putIndent();
    fputs("}\n", hf);
    
    return PLI_OK;
}
int SCCPConvert::defStmt(IStmt *stmt) {
    switch (stmt->getStmtKind())
    {
    case defpackage:
        if (NULL == hf) return PLI_OK;
        return outStmtToken(stmt);
        break;
    case importstmt:
        return importStmt(stmt);
        break;
    case defmodule:
        copyFlag = true;
        break;
    default:
    {
        IToken *tok = stmt->getFirseToken();
        if (NULL == tok) EXIT_INFO("getFirseToken fail!");
        std::string hCode = tok->getCode();
        /**
         * delete token. Must do!
         */
        stmt->freeToken(tok);
        return outStmt(stmt, hCode);
        break;
    }
    }
    return PLI_OK;
}
int SCCPConvert::putNote(std::string &note) {
    if ((NULL == hf) || copyFlag) return PLI_OK;
    /**
     * Comments for the output source file.
     * If you do not output comments, return PLI_OK;
     */
    fputs(note.c_str(), hf);
    if (!note.empty() && ('\n' != note[note.length() - 1]))
        fputs("\n", hf);
    return PLI_OK;
}
int SCCPConvert::openFile() {
    if (KeyWords.empty()) {
        if (useLang.compare("Chinese"))
            MessageBox(NULL, TEXT("Configuration not initialized or initialization error!"), TEXT("Error"), MB_OK | MB_ICONWARNING);
        else MessageBox(NULL, TEXT("没有初始化配置或初始化错误！"), TEXT("错误"), MB_OK | MB_ICONWARNING);
        return PLI_EXIT;
    }
    
    Indent = 0;
    copyFlag = false;
    chgNum = 0;
    Imported.clear();
    
    /**
     * Preprocessing, do not create file temporarily.
     */
    if (preFlag) return PLI_OK;
    
    if (createFile()) return PLI_EXIT;
    
    return PLI_OK;
}
int SCCPConvert::inclOpenFile() {
    /**
     * The Java language has no include statement.
     */
    return PLI_EXIT;
}
int SCCPConvert::reOpenFile() {
    /**
     * The Java language has no include statement.
     */
    return PLI_EXIT;
}
int SCCPConvert::inclFile(file_kind kind, std::string &name) {
    /**
     * The Java language has no include statement.
     */
    return PLI_EXIT;
}
int SCCPConvert::eoFile() {
    if (NULL == hf) {
        if (copyFlag || !chgNum)
            return copyFile();
        
        Indent = 0;
        copyFlag = false;
        chgNum = 0;
        Imported.clear();
        
        if (createFile()) return PLI_EXIT;
        return REREAD_FILE;
    }
    fclose(hf);
    hf = NULL;
    return PLI_OK;
}
int SCCPConvert::blockStart() {
    if ((NULL == hf) || copyFlag) return PLI_OK;
    
    putIndent();
    fputs("{\n", hf);
    Indent++;
    return PLI_OK;
}
int SCCPConvert::blockOver() {
    if ((NULL == hf) || copyFlag) return PLI_OK;
    
    if (Indent) Indent--;
    else {
        if (NULL != hf) {
            fclose(hf);
            hf = NULL;
        }
        EXIT_INFO("File matching error!");
    }
    putIndent();
    fputs("}\n", hf);
    return PLI_OK;
}
SCCPConvert::SCCPConvert(IAccessSCCT *accessScct) {
    aScct = accessScct;
    useLang = aScct->getRegLanguage();
    Initialize();
}
}//sccp
