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

#include "SCCPStmt.h"

namespace sccp {
int SCCPStmt::methodStmt(IStmt *stmt) {
    std::string hCode;
    if ((NULL != hf) && !copyFlag) {
        IToken *tok = stmt->getFirseToken();
        if (NULL == tok) EXIT_INFO("getFirseToken fail!");
        hCode = tok->getCode();
        /**
         * delete token. Must do!
         */
        stmt->freeToken(tok);
        
        if (!hCode.compare("case")
        || !hCode.compare("default")) {
            if (!Indent) EXIT_INFO("File matching error!");
            Indent--;
        }
        
        if (stmt->getPrevIndention() > stmt->getIndention()) {
            size_t n = stmt->getPrevIndention() - stmt->getIndention();
            if (!hCode.compare("case")
            || !hCode.compare("default")
            || !hCode.compare("}")) n--;
            
            if (dIndent && n && !hCode.compare("}")) {
                n--;
                Indent--;
            }
            
            for (size_t i = 0; i < n; i++) {
                if (!Indent) EXIT_INFO("File matching error!");
                Indent--;
                putIndent();
                if ((i + 1 == n)
                && !hCode.compare("while"))
                    fputs("}", hf);
                else fputs("}\n", hf);
            }
        }
        else if ((stmt->getPrevIndention() == stmt->getIndention())
        && (!prevCode.compare("if")
        || !prevCode.compare("for")
        || !prevCode.compare("else")
        || !prevCode.compare("else if")))
            Indent++;
    }
    
    if ((NULL != hf) && !copyFlag)
        addNote(stmt);
    
    if (outStmt(stmt, hCode)) return PLI_EXIT;
    
    if ((NULL != hf) && !copyFlag) {
        if ((stmt->getPrevIndention() == stmt->getIndention())
        && (!prevCode.compare("if")
        || !prevCode.compare("for")
        || !prevCode.compare("else")
        || !prevCode.compare("else if")))
            Indent--;
            
        if (NOTEXISTS == stmt->getNextIndention()) {
            /* last line */
            for (size_t i = stmt->getIndention() - 1; i; i--) {
                if (!Indent) EXIT_INFO("File matching error!");
                Indent--;
                putIndent();
                fputs("}\n", hf);
            }
            prevCode = "";
        }
        else if (!hCode.compare("switch")) {
#if SELF_METHOD1 == METHODS
            /* Two statements on the same line. */
            if ((stmt->nextLine() != stmt->fileLine())
            || (stmt->fileLine() < 0)) {
                putIndent();
                fputs("{\n", hf);
            }
            else fputs("{", hf);
#else
            putIndent();
            fputs("{\n", hf);
#endif
            Indent++;
            prevCode = hCode;
        }
        else if (stmt->getNextIndention() > stmt->getIndention()) {
            if (!neIndent) {
                size_t n = stmt->getNextIndention() - stmt->getIndention();
                if (!hCode.compare("case")
                || !hCode.compare("default"))
                    n--;
                if (n > 1) EXIT_INFO("File matching error!");
                if (n) {
#if SELF_METHOD1 == METHODS
                    /* Two statements on the same line. */
                    if ((stmt->nextLine() != stmt->fileLine())
                    || (stmt->fileLine() < 0)) {
                        putIndent();
                        fputs("{\n", hf);
                    }
                    else fputs("{", hf);
#else
                    putIndent();
                    fputs("{\n", hf);
#endif
                    Indent++;
                }
            }
            else if (((stmt->getNextIndention() - stmt->getIndention()) > 1)
            && (!hCode.compare("case")
            || !hCode.compare("default"))) {
                Indent++;
                dIndent = true;
            }
            
            prevCode = hCode;
        }
        else prevCode = hCode;
        
        if (!hCode.compare("case")
        || !hCode.compare("default"))
            Indent++;
    }
    return PLI_OK;
}
int SCCPStmt::outStmt(IStmt *stmt, std::string hCode) {
    neIndent = 0;
    
    if ((NULL != hf) && !copyFlag) {
        if (!hCode.compare("}")) {
            if (Indent) Indent--;
            else {
                fclose(hf);
                hf = NULL;
                EXIT_INFO("File matching error!");
            }
        }
        if (hCode.compare("while")
        || (stmt->getPrevIndention() <= stmt->getIndention())) {
#if SELF_METHOD1 == METHODS
            /* Two statements on the same line. */
            if ((stmt->prevLine() != stmt->fileLine())
            || (stmt->fileLine() < 0))
                putIndent();
#else
            putIndent();
#endif
        }
    }
    
    int ret = captureStmt(stmt);
    if(ret < 0) return PLI_EXIT;
    else if(!ret) return outStmtToken(stmt);
    
    if (convertStmt(stmt, hCode)) return PLI_EXIT;
    
    if ((NULL != hf) && !copyFlag) {
#if SELF_METHOD1 == METHODS
        /* Two statements on the same line. */
        if ((stmt->nextLine() != stmt->fileLine())
        || (stmt->fileLine() < 0))
            fputs("\n", hf);
        else fputs(" ", hf);
#else
        fputs("\n", hf);
#endif
    }
    
    return PLI_OK;
}
int SCCPStmt::convertStmt(IStmt *stmt, std::string hCode) {
    INode *node = stmt->getFirstNode();
    if (NULL == node) EXIT_INFO("First node is null!");
    
    node_kind prevkind = isunknow;
    std::string line, prevcode;
    int ig = 0;
    
    do {
        std::string code = getNodeCode(stmt, node);
        if(code.empty()) {
            /**
             * delete node. Must do!
             */
            SEND_INFO("getNodeCode is null!", "");
            stmt->freeNode(node);
            return PLI_EXIT;
        }
        
        if((isoper == node->getKind())
        && !code.compare(node->getCode())) {
            if (!code.compare("<")
            && (ig
            || (libtype == prevkind)
            || (usertype == prevkind))) {
                line += code;
                ig++;
            }
            else if (ig && !code.compare(">")) {
                line += code;
                ig--;
            }
            else if(!code.compare("+")
            || !code.compare("-")
            || !code.compare("*")
            || !code.compare("/")
            || !code.compare("=")
            || !code.compare("?")
            || !code.compare(":")
            || !code.compare("|")
            || !code.compare("&")
            || !code.compare(">")
            || !code.compare("<")
            || !code.compare("==")
            || !code.compare("!=")
            || !code.compare("+=")
            || !code.compare("-=")
            || !code.compare("/=")
            || !code.compare("%=")
            || !code.compare("|=")
            || !code.compare("&=")
            || !code.compare("^=")
            || !code.compare(">=")
            || !code.compare("<=")
            || !code.compare("<<")
            || !code.compare(">>")
            || !code.compare("<<=")
            || !code.compare(">>=")
            || !code.compare(">>>")) {
                if(line.empty()) line = code;
                else {
                    line += " " + code;
                    line += " ";
                }
            }
            else if(!code.compare(",")
            || !code.compare(";"))
                line += code + " ";
            else if (!code.compare("{")) {
                if ((NULL != hf) && !copyFlag)
                    Indent++, neIndent++;
                line += " " + code;
            }
            else if (!code.compare("}")) {
                if ((NULL != hf) && !copyFlag
                && stmt->getNodeSerial(node)) {
                    if (Indent) Indent--, neIndent--;
                    else {
                        fclose(hf);
                        hf = NULL;
                        EXIT_INFO("fail!");
                    }
                }
                line += code;
            }
            else line += code;
        }
        else if(line.empty()) line = code;
        else if(isoper != prevkind)
            line += " " + code;
        else line += code;
        
        prevkind = node->getKind();
        prevcode = node->getCode();
    }while (!stmt->nodeMoveNext(node));
    
    chgNum++;
    
    if ((NULL != hf) && !copyFlag) {
        fputs(line.c_str(), hf);
        
        if (node->getCode().compare(";")
        && node->getCode().compare("{")) {
            /* Ignore last ;|: in method */
            IToken *tok = stmt->getLastToken();
            if (NULL != tok) {
                if (!tok->getCode().compare(";")
                || !tok->getCode().compare(":"))
                    fputs(tok->getCode().c_str(), hf);
                /**
                 * delete token. Must do!
                 */
                stmt->freeToken(tok);
            }
        }
    }
    /**
     * delete node. Must do!
     */
    stmt->freeNode(node);
    return PLI_OK;
}
std::string SCCPStmt::getNodeCode(IStmt *stmt, INode *node) {
    std::string res;
    
    node_kind kind = node->getKind();
    while ((libpackage == kind)
    || (libtype == kind)) {
        res += node->getCode();
        if(stmt->nodeMoveNext(node)) {
            res = res.substr(0, res.length() - node->getCode().length());
            break;
        }
        
        if((isoper == node->getKind())
        && !node->getCode().compare(".")) {
            res += node->getCode();
            if(stmt->nodeMoveNext(node)) return "";
            kind = node->getKind();
        }
        else {
            if(stmt->nodeMovePrev(node)) return "";
            res = res.substr(0, res.length() - node->getCode().length());
            break;
        }
    }
    
    int ret = captureNode(node);
    if (1 == ret) {
        res = nodeConvert(stmt, node, res);
        if (res.empty()) {
            if (isstring == kind)
                return wordReplace(stmt, node);
        }
        return res;
    }
    else if (2 == ret) return wordReplace(stmt, node);
    
    return res + node->getCode();
}
std::string SCCPStmt::nodeConvert(IStmt *stmt, INode *node, std::string lead) {
    std::string code = lead + node->getCode();
    switch(node->getKind())
    {
    case libtype:
    {
        std::map<std::string, std::string>::iterator
            it = Classes.find(code);
        if ((it != Classes.end())
        && !it->second.empty())
            return it->second;
        if (!lead.empty()) {
            it = Classes.find(node->getCode());
            if((it != Classes.end())
            && !it->second.empty())
                return it->second;
        }
        if (useLang.compare("Chinese"))
            MessageBoxA(NULL, code.c_str(), "No corresponding conversion item is configured!", MB_OK);
        else MessageBoxA(NULL, code.c_str(), "没有配置对应的转换项", MB_OK);
        break;
    }
    case usertype:
    {
        std::map<std::string, std::string>::iterator
            it = Classes.find(node->getCode());
        if (it != Classes.end()) {
            if (!it->second.empty())
                return it->second;
            if (useLang.compare("Chinese"))
                MessageBoxA(NULL, code.c_str(), "No corresponding conversion item is configured!", MB_OK);
            else MessageBoxA(NULL, code.c_str(), "没有配置对应的转换项", MB_OK);
        }
        return node->getCode();
        break;
    }
    case libmember:
    case libvar:
    {
        std::map<std::string, std::string>::iterator
            it = Members.find(code);
        if ((it != Members.end())
        && !it->second.empty())
            return it->second;
        if (!lead.empty()) {
            it = Members.find(node->getCode());
            if ((it != Members.end())
            && !it->second.empty())
                return it->second;
        }
        if (useLang.compare("Chinese"))
            MessageBoxA(NULL, code.c_str(), "No corresponding conversion item is configured!", MB_OK);
        else MessageBoxA(NULL, code.c_str(), "没有配置对应的转换项", MB_OK);
        break;
    }
    case libfunc:
    {
        std::map<std::string, MethodChg>::iterator
            it = Methods.find(code);
        if (it == Methods.end()) {
            if (!lead.empty())
                it = Methods.find(node->getCode());
            if (it == Methods.end()) {
                if (useLang.compare("Chinese"))
                    MessageBoxA(NULL, code.c_str(), "No corresponding conversion item is configured!", MB_OK);
                else MessageBoxA(NULL, code.c_str(), "没有配置对应的转换项", MB_OK);
                break;
            }
        }
        if (-1 == it->second.flag) {
            if (it->second.to.empty()) {
                if (useLang.compare("Chinese"))
                    MessageBoxA(NULL, code.c_str(), "No corresponding conversion item is configured!", MB_OK);
                else MessageBoxA(NULL, code.c_str(), "没有配置对应的转换项", MB_OK);
                break;
            }
            return it->second.to;
        }
        
        if (stmt->nodeMoveNext(node)) return "";
        
        if (node->getCode().compare("(")) return "";
        
        int ip = 1;
        std::vector<std::string> params;
        std::string param;
        for (;;) {
            if (stmt->nodeMoveNext(node)) return "";
            std::string item = getNodeCode(stmt, node);
            if (item.empty()) return item;
            
            if ((isoper == node->getKind())
            && !item.compare(node->getCode())) {
                if (!item.compare("(")) ip++;
                else if (!item.compare(")")) {
                    ip--;
                    if (!ip) {
                        if (param.empty()) {
                            if(!params.empty()) return "";
                            break;
                        }
                        params.push_back(param);
                        break;
                    }
                }
                else if ((1 == ip) && !item.compare(",")) {
                    if (param.empty()) return "";
                    params.push_back(param);
                    param = "";
                    continue;
                }
                else if (!item.compare("+")
                || !item.compare("-")
                || !item.compare("*")
                || !item.compare("/")
                || !item.compare("=")
                || !item.compare("?")
                || !item.compare(":")
                || !item.compare("|")
                || !item.compare("&")
                || !item.compare(">")
                || !item.compare("<")
                || !item.compare("==")
                || !item.compare("!=")
                || !item.compare("+=")
                || !item.compare("-=")
                || !item.compare("/=")
                || !item.compare("%=")
                || !item.compare("|=")
                || !item.compare("&=")
                || !item.compare("^=")
                || !item.compare(">=")
                || !item.compare("<=")
                || !item.compare("<<")
                || !item.compare(">>")
                || !item.compare("<<=")
                || !item.compare(">>=")
                || !item.compare(">>>")) {
                    param += " " + item;
                    param += " ";
                }
                else if (!item.compare(",")
                || !item.compare(";"))
                    param += item + " ";
                else param += item;
                continue;
            }
            param += item;
        }
        
        if (params.size() != it->second.flag) break;
        
        if (it->second.lParams.empty()) {
            code = it->second.to + "(";
            for (size_t i = 0; i < params.size(); i++) {
                if (i) code += ", ";
                code += params[i];
            }
            code += ")";
            return code;
        }
        code = it->second.to + "(";
        for (size_t i = 0; i < it->second.lParams.size(); i++) {
            if (i) code += ", ";
            if (-1 == it->second.lParams[i].old) {
                if (it->second.lParams[i].val.empty()) {
                    if (useLang.compare("Chinese"))
                        MessageBoxA(NULL, "Default value is empty!", "error", MB_OK);
                    else MessageBoxA(NULL, "默认值为空!", "error", MB_OK);
                    return "";
                }
                if (it->second.lParams[i].type.compare("code")) {
                    code += "\"" + it->second.lParams[i].val;
                    code += "\"";
                }
                else code += it->second.lParams[i].val;
                continue;
            }
            if ((it->second.lParams[i].old > (int)params.size())
            || (it->second.lParams[i].old < 1)) {
                if (useLang.compare("Chinese"))
                    MessageBoxA(NULL, "Parameter replacement configuration error!", "error", MB_OK);
                else MessageBoxA(NULL, "参数替换配置错误!", "error", MB_OK);
                return "";
            }
            code += params[it->second.lParams[i].old - 1];
        }
        code += ")";
        return code;
    }
    case isstring:
        return keyReplace(stmt, node);
        break;
    default: break;
    }
    return "";
}
std::string SCCPStmt::keyReplace(IStmt *stmt, INode *node) {
    std::string code = node->getCode();
    std::string::size_type bpos, epos;
    for (size_t i = 0; i < KeyWords.size(); i++) {
        bpos = 0, epos = 0;
        while ((bpos = code.find(KeyWords[i], bpos)) != std::string::npos) {
            if (bpos && ('.' != code[bpos - 1])) {
                bpos++;
                continue;
            }
            if ((bpos + KeyWords[i].length() + 1 < code.length())
            && ('.' != code[bpos + KeyWords[i].length()])) {
                bpos++;
                continue;
            }
            epos = bpos + KeyWords[i].length();
            for (++epos; epos < code.length(); epos++) {
                if (('.' == code[epos])
                || ((code[epos] >= 'a') && (code[epos] <= 'z'))
                || ((code[epos] >= 'A') && (code[epos] <= 'Z'))
                || ('_' == code[epos])) continue;
                break;
            }
            for (; bpos >= 0; bpos--) {
                if (('.' == code[bpos])
                || ((code[bpos] >= 'a') && (code[bpos] <= 'z'))
                || ((code[bpos] >= 'A') && (code[bpos] <= 'Z'))
                || ('_' == code[bpos])) continue;
                bpos++;
                break;
            }
            break;
        }
        if (epos > bpos) break;
    }
    if (epos <= bpos) return "";
    
    std::string key = code.substr(bpos, epos - bpos);
    
    std::map<std::string, std::string>::iterator
        it = Classes.find(key);
    if ((it != Classes.end())
    && !it->second.empty()) {
        code.replace(bpos, epos - bpos, it->second);
        return code;
    }
    
    if (stmt->nodeMovePrev(node)) return code;
    if (node->getCode().compare("(")) {
        if (stmt->nodeMoveNext(node)) return "";
        return code;
    }
    if (stmt->nodeMovePrev(node)) {
        if (stmt->nodeMoveNext(node)) return "";
        return code;
    }
    
    if ((libfunc == node->getKind())
    && (!node->getCode().compare("forName")
    || !node->getCode().compare("getConnection"))) {
        std::string str;
        if (useLang.compare("Chinese")) {
            str = "There is no replacement configured for '" + key;
            str += "' in the " + code;
            str += "\nContinue conversion?";
        }
        else {
            str = "在 " + code;
            str += " 中，没有配置 " + key;
            str += " 对应的转换项！";
            str += "\n是否继续转换?";
        }
        if (IDNO == MessageBoxA(NULL, str.c_str(), "Message", MB_YESNO | MB_ICONQUESTION))
            return "";
    }
    if (stmt->nodeMoveNext(node)) return "";
    if (stmt->nodeMoveNext(node)) return "";
    return code;
}
std::string SCCPStmt::wordReplace(IStmt *stmt, INode *node) {
    std::string code = node->getCode();
    std::string::size_type bpos, epos;
    for (std::map<std::string, std::string>::iterator it = Words.begin();
        it != Words.end(); it++) {
        bpos = 0, epos = 0;
        while ((bpos = code.find(it->first, bpos)) != std::string::npos) {
            if (bpos && (('.' == code[bpos - 1])
            || ((code[bpos - 1] >= 'a') && (code[bpos - 1] <= 'z'))
            || ((code[bpos - 1] >= 'A') && (code[bpos - 1] <= 'Z'))
            || ('_' == code[bpos - 1]))) {
                bpos++;
                continue;
            }
            epos = bpos + it->first.length();
            if ((epos < code.length()) && (('.' == code[epos])
            || ((code[epos] >= 'a') && (code[epos] <= 'z'))
            || ((code[epos] >= 'A') && (code[epos] <= 'Z'))
            || ('_' == code[epos]))) {
                bpos = epos;
                continue;
            }
            code.replace(bpos, epos - bpos, it->second);
            bpos += it->second.length();
        }
    }
    return code;
}
}
