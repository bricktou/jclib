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

#include "SCCPConfig.h"

namespace sccp {
int SCCPConfig::Initialize() {
    char dllpath[MAX_PATH] = { 0 };
    GetModuleFileName(ghModule, dllpath, MAX_PATH);
    
    if (!strlen(dllpath))
        EXIT_INFO("Module path is null!");
    
    for (size_t i = strlen(dllpath) - 1; i; i--) {
        if('\\' != dllpath[i]) continue;
        dllpath[++i] = '\0';
        break;
    }
    
    if (!strlen(dllpath))
        EXIT_INFO("Module path is null!");
    
    if ('\\' != dllpath[strlen(dllpath) - 1])
        EXIT_INFO("Module path is error!");
    
    strcat_s(dllpath, "config.xml");
    if (_access(dllpath, 0) == -1) {
        if(useLang.compare("Chinese"))
            MessageBoxA(NULL, dllpath, "File does not exist", MB_OK | MB_ICONWARNING);
        else MessageBoxA(NULL, dllpath, "文件不存在", MB_OK | MB_ICONWARNING);
        return PLI_EXIT;
    }
    
    tinyxml2::XMLDocument doc;
    int ret = doc.LoadFile(dllpath);
    if (ret) {
        if (XML_ERROR_MISMATCHED_ELEMENT == ret) {
            if (useLang.compare("Chinese"))
                MessageBoxA(NULL, dllpath, "File format error", MB_OK | MB_ICONWARNING);
            else MessageBoxA(NULL, dllpath, "文件格式错误", MB_OK | MB_ICONWARNING);
        }
        return PLI_EXIT;
    }
    
    for (XMLElement* root = doc.RootElement(); root;
        root = root->NextSiblingElement()) {
        if (!strcmp(root->Name(), "KeyWords")) {
            if (initKeyWords(root)) return PLI_EXIT;
            continue;
        }
        if (!strcmp(root->Name(), "Imports")) {
            if (initImports(root)) return PLI_EXIT;
            continue;
        }
        if (!strcmp(root->Name(), "Classes")) {
            if (initClasses(root)) return PLI_EXIT;
            continue;
        }
        if (!strcmp(root->Name(), "Members")) {
            if (initMembers(root)) return PLI_EXIT;
            continue;
        }
        if (!strcmp(root->Name(), "Methods")) {
            if (initMethods(root)) return PLI_EXIT;
            continue;
        }
        if (!strcmp(root->Name(), "Words")) {
            if (initWords(root)) return PLI_EXIT;
            continue;
        }
    }
    if (KeyWords.empty()) EXIT_INFO("Key words is null!");
    return PLI_OK;
}
int SCCPConfig::initKeyWords(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "Key")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string code;
        bool valid = false;
        while(ratt) {
            if (!strcmp(ratt->Name(), "kind")
            && !strcmp(ratt->Value(), "package"))
                valid = true;
            else if(!strcmp(ratt->Name(), "code"))
                code = ratt->Value();
            ratt = ratt->Next();
        }
        if (valid && !code.empty())
            KeyWords.push_back(code);
    }
    return PLI_OK;
}
int SCCPConfig::initImports(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "item")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string item;
        while (ratt) {
            if (!strcmp(ratt->Name(), "code"))
                item = ratt->Value();
            
            ratt = ratt->Next();
        }
        if (item.empty()) EXIT_INFO("Convert item is null!");
        
        std::vector<std::string> vchg;
        for (XMLElement* c = e->FirstChildElement(); c;
        c = c->NextSiblingElement()) {
            if (strcmp(c->Name(), "change")) continue;
            
            ratt = c->FirstAttribute();
            if (NULL == ratt) EXIT_INFO("Attribute is null!");
            
            while (ratt) {
                if (!strcmp(ratt->Name(), "code"))
                    vchg.push_back(ratt->Value());
                
                ratt = ratt->Next();
            }
        }
        if (vchg.empty()) EXIT_INFO("Convert items is null!");
        
        Imports.insert(std::pair<std::string, std::vector<std::string>>(item, vchg));
    }
    return PLI_OK;
}
int SCCPConfig::initClasses(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "class")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string src, to;
        while (ratt) {
            if (!strcmp(ratt->Name(), "src"))
                src = ratt->Value();
            else if (!strcmp(ratt->Name(), "to"))
                to = ratt->Value();
            ratt = ratt->Next();
        }
        
        if (!src.empty() && !to.empty())
            Classes.insert(std::pair<std::string, std::string>(src, to));
    }
    return PLI_OK;
}
int SCCPConfig::initMembers(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "member")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string src, to;
        while (ratt) {
            if (!strcmp(ratt->Name(), "src"))
                src = ratt->Value();
            else if (!strcmp(ratt->Name(), "to"))
                to = ratt->Value();
            ratt = ratt->Next();
        }
        if (!src.empty() && !to.empty())
            Members.insert(std::pair<std::string, std::string>(src, to));
    }
    return PLI_OK;
}
int SCCPConfig::initMethods(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "method")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string src;
        MethodChg mChg;
        mChg.flag = -1;
        while (ratt) {
            if (!strcmp(ratt->Name(), "src"))
                src = ratt->Value();
            else if (!strcmp(ratt->Name(), "to"))
                mChg.to = ratt->Value();
            else if (!strcmp(ratt->Name(), "params"))
                mChg.flag = atoi(ratt->Value());
            ratt = ratt->Next();
        }
        if (src.empty() || mChg.to.empty()) EXIT_INFO("Convert item is null!");
        
        if (-1 != mChg.flag) {
            for (XMLElement* c = e->FirstChildElement(); c;
            c = c->NextSiblingElement()) {
                if (strcmp(c->Name(), "param")) continue;
                
                ratt = c->FirstAttribute();
                if (NULL == ratt) EXIT_INFO("Attribute is null!");
                
                ParamChg chg;
                chg.old = -1;
                int num = -1;
                while (ratt) {
                    if (!strcmp(ratt->Name(), "new"))
                        num = atoi(ratt->Value());
                    else if (!strcmp(ratt->Name(), "old"))
                        chg.old = atoi(ratt->Value());
                    else if (!strcmp(ratt->Name(), "default"))
                        chg.val = ratt->Value();
                    else if (!strcmp(ratt->Name(), "type"))
                        chg.type = ratt->Value();
                    
                    ratt = ratt->Next();
                }
                if (num != mChg.lParams.size() + 1) {
                    if (useLang.compare("Chinese"))
                        MessageBoxA(NULL, "Parameter list error!", "error", MB_OK | MB_ICONWARNING);
                    else MessageBoxA(NULL, "参数列表错误!", "error", MB_OK | MB_ICONWARNING);
                    return PLI_EXIT;
                }
                if ((-1 == chg.old) && chg.val.empty()) {
                    if (useLang.compare("Chinese"))
                        MessageBoxA(NULL, "Default value is not set!", "error", MB_OK | MB_ICONWARNING);
                    else MessageBoxA(NULL, "默认值没有设置!", "error", MB_OK | MB_ICONWARNING);
                    return PLI_EXIT;
                }
                if ((-1 == chg.old) && !chg.val.empty()) {
                    if (chg.type.compare("string"))
                        chg.type = "code";
                }
                mChg.lParams.push_back(chg);
            }
        }
        Methods.insert(std::pair<std::string, MethodChg>(src, mChg));
    }
    return PLI_OK;
}
int SCCPConfig::initWords(XMLElement *root) {
    for (XMLElement* e = root->FirstChildElement(); e;
    e = e->NextSiblingElement()) {
        if (strcmp(e->Name(), "word")) continue;
        
        const XMLAttribute *ratt = e->FirstAttribute();
        if (NULL == ratt) EXIT_INFO("Attribute is null!");
        
        std::string src, to;
        while (ratt) {
            if (!strcmp(ratt->Name(), "src"))
                src = ratt->Value();
            else if (!strcmp(ratt->Name(), "to"))
                to = ratt->Value();
            ratt = ratt->Next();
        }
        if (!src.empty() && !to.empty())
            Words.insert(std::pair<std::string, std::string>(src, to));
    }
    return PLI_OK;
}
}
