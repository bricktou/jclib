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

#include "SCCPFile.h"

namespace sccp {
int SCCPFile::outStmtToken(IStmt *stmt) {
    if ((NULL == hf) || copyFlag) return PLI_OK;
    //fprintf_s(hf, "%d-%d ", Indent, stmt->getIndention());
#if SELF_METHOD1 == METHODS
    funToken ftok = std::bind(&SCCPFile::outToken, this,
        std::placeholders::_1);
    if (stmt->for_token(ftok))
        EXIT_INFO("Output token fail!");
#else
    IToken *tok = stmt->getFirseToken();
    if (NULL == tok) EXIT_INFO("First token is null!");
    do {
        if (outToken(tok)) {
            /**
             * delete token. Must do!
             */
            stmt->freeToken(tok);
            EXIT_INFO("Output token fail!");
        }
    }while (!stmt->tokenMoveNext(tok));
    /**
     * delete token. Must do!
     */
    stmt->freeToken(tok);
#endif

#if SELF_METHOD1 == METHODS
    /* Two statements on the same line. */
    if ((stmt->nextLine() != stmt->fileLine())
    || (stmt->fileLine() < 0))
        fputs("\n", hf);
    else fputs(" ", hf);
#else
    fputs("\n", hf);
#endif
    return PLI_OK;
}
int SCCPFile::outToken(IToken *token) {
    if (token->getSerial()) {
        if (token->isHeadWord()) {
            fputs("\n", hf);
            putIndent();
        }
        else if (token->isBlank())
            fputs(" ", hf);
        
    }
        
    fputs(token->getCode().c_str(), hf);
    if (token->isOperator()) {
        if (!token->getCode().compare("{"))
            Indent++, neIndent++;
        else if (!token->getCode().compare("}")
        && token->getSerial()) {
            if (Indent) Indent--, neIndent--;
            else {
                fclose(hf);
                hf = NULL;
                EXIT_INFO("fail!");
            }
        }
    }
    return PLI_OK;
}
void SCCPFile::putIndent() {
    for (size_t i = 0; i < Indent; i++)
        fputs(S_INDENT, hf);
}
}
