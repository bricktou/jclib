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

#include "SCCPNode.h"

namespace sccp {
int SCCPNode::captureStmt(IStmt *stmt) {
    stmt->serialization();
    
    int ret;
#if SELF_METHOD1 == METHODS
    funNode fnode = std::bind(&SCCPNode::captureNode, this,
        std::placeholders::_1);
    ret = stmt->for_node(fnode);
#else
    INode *node = stmt->getFirstNode();
    if (NULL == node) EXIT_INFO("First node is null!");
    do {
        ret = captureNode(node);
        if (ret) break;
    }while (!stmt->nodeMoveNext(node));
    /**
     * delete node. Must do!
     */
    stmt->freeNode(node);
#endif
    return ret;
}
int SCCPNode::captureNode(INode *node) {
    switch (node->getKind())
    {
    case libpackage:
        for (size_t i = 0; i < KeyWords.size(); i++) {
            if(!node->getCode().compare(KeyWords[i]))
                return 1;
        }
        break;
    case libfunc:
    case libtype:
    case libmember:
    case libvar:
    {
        if (matchKey(node->getPackageFullName())) return 1;
        break;
    }
    case usertype:
    {
        std::map<std::string, std::string>::iterator
            it = Classes.find(node->getCode());
        if (it != Classes.end()) return 1;
        break;
    }
    case isstring:
    {
        std::string code = node->getCode();
        if (matchKey(code.substr(1, code.length() - 2)))
            return 1;
        if (matchWord(code)) return 2;
        return PLI_OK;
    }
    default:
        break;
    }
    return PLI_OK;
}
int SCCPNode::matchKey(std::string &str) {
    for (size_t i = 0; i < KeyWords.size(); i++) {
        std::string::size_type pos = 0;
        while ((pos = str.find(KeyWords[i], pos)) != std::string::npos) {
            if (pos && ('.' != str[pos - 1])) {
                pos++;
                continue;
            }
            if ((pos + KeyWords[i].length() + 1 < str.length())
            && ('.' != str[pos + KeyWords[i].length()])) {
                pos++;
                continue;
            }
            return 1;
        }
    }
    return PLI_OK;
}
int SCCPNode::matchWord(std::string &str) {
    for (std::map<std::string, std::string>::iterator it = Words.begin();
        it != Words.end(); it++) {
        std::string::size_type pos = 0;
        while ((pos = str.find(it->first, pos)) != std::string::npos) {
            if (pos && (('.' == str[pos - 1])
            || ((str[pos - 1] >= 'a') && (str[pos - 1] <= 'z'))
            || ((str[pos - 1] >= 'A') && (str[pos - 1] <= 'Z'))
            || ('_' == str[pos - 1]))) {
                pos++;
                continue;
            }
            std::string::size_type epos = pos + it->first.length();
            if ((epos < str.length())
            && (('.' == str[epos])
            || ((str[epos] >= 'a')
            && (str[epos] <= 'z'))
            || ((str[epos] >= 'A')
            && (str[epos] <= 'Z'))
            || ('_' == str[epos]))) {
                pos++;
                continue;
            }
            return 2;
        }
    }
    return PLI_OK;
}
}
