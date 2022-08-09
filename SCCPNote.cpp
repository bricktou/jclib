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

#include "SCCPNote.h"

namespace sccp {
void SCCPNote::addNote(IStmt *stmt) {
    /**
     * The main program will filter the comments in the method.
     * This logic will add comments according to the rules.
     * Refer to the report plug-in.
     */
    stmt->build();
    /* Compilation reduces the logic of extracting comments */
    if (1 != stmt->getNodeCount()) return;
    
    INode *node = stmt->getFirstNode();
    if (NULL == node) return;
    
    std::string note = getNote(node);
    
    /**
     * delete node. Must do!
     */
    stmt->freeNode(node);
    
    if (note.empty()) return;
    
    putIndent();
    fprintf_s(hf, "/* %s */\n", note.c_str());
}
std::string SCCPNote::getNote(INode *node, bool conditional) {
    std::string note;
    switch (node->getKind()) {
    case isoper:
        if (!node->getCode().compare("=")) {
            INode *val = node->getRightValue();
            if (NULL != val) {
                /* Assignment statement */
                if (userfunc == val->getKind())
                    note = val->getName();
                /**
                 * delete node. Must do!
                 */
                node->freeNode(val);
            }
        }
        else if (conditional
        && (!node->getCode().compare("&&")
        || !node->getCode().compare("||"))) {
            INode *val = node->getLeftValue();
            if (NULL != val) {
                note = getNote(val, conditional);
                /**
                 * delete node. Must do!
                 */
                node->freeNode(val);
            }
            val = node->getRightValue();
            if (NULL != val) {
                note += getNote(val, conditional);
                /**
                 * delete node. Must do!
                 */
                node->freeNode(val);
            }
        }
    case userfunc:
        /* User function */
        note = node->getName();
        break;
    case lankey:
        if (keyif == node->getKey()) {
            node->build();
            INode *con = node->getFirstNode();
            note = getNote(con, true);
            /**
             * delete node. Must do!
             */
            node->freeNode(con);
        }
    }
    return note;
}
}
