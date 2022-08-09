#pragma once

#define Brick "Brick Technologies Co., Ltd"

// Output report succeeded
#define PLI_OK      0

// Stop the program running
#define PLI_EXIT    -1

/**
 * Choose the method of information processing.
 */
#define METHODS         1

/**
 * Custom methods: deep processing through 
 * the information given by SCCT.
 */
#define SELF_METHOD1    1
#define SELF_METHOD2    2

/* Characters displayed on one line */
#define LINE_MAX 70

#define S_INDENT "    "

#define SEND_INFO(major, minor) \
    aScct->sendInfo(major, minor, __FILE__, __LINE__)

#define EXIT_INFO(major) do { \
    aScct->sendInfo(major, "", __FILE__, __LINE__); \
    return PLI_EXIT; \
}while (0)

#include "stdafx.h"

#include "IAccessSCCT.h"
#include "ISCCConvert.h"

#include "tinyxml2.h"
using namespace tinyxml2;

/**
 * handle of dll. 
 */
extern HMODULE ghModule;

namespace sccp {
class SCCPBase {
public:
    SCCPBase(void);
    
    /**
     * Handle of access SCCT
     */
    IAccessSCCT *aScct;
    
    bool existFile(const std::string &name);
    
    /**
     * Handle of convert output file
     */
    FILE *hf;
    int createFile();
    int createPath(const std::string &path);
    int copyFile();
    
    std::string getOutRoot();
    /**
     * language. Chinese|English
     */
    std::string useLang;
    
    /**
     * Pretreatment flag
     * 0: Direct conversion without preprocessing.
     * 1: preprocessing
     *    Other situations.
     */
    int preFlag;
    
    /**
     * Copy file.
     */
    bool copyFlag;
    
    /**
     * Record the number of changes.
     */
    size_t chgNum;
    
    /**
     * Capture keywords
     */
    std::vector<std::string> KeyWords;
    
    /**
     * Change package
     */
    std::map<std::string, std::vector<std::string>> Imports;
    
    /**
     * Mark import conversion results
     */
    std::vector<std::string> Imported;
    
    std::map<std::string, std::string> Classes;
    
    std::map<std::string, std::string> Members;
    
    typedef struct {
        /* Original sequence. 1..n */
        /* -1:Replace with default */
        int old;
        /* default value */
        std::string val;
        /* default type */
        std::string type;
    }ParamChg;
    typedef struct {
        std::string to;
        /**
         * Check flag.
         * -1: Do not check.
         */
        int flag;
        /* Parameter conversion list. */
        std::vector<ParamChg> lParams;
    }MethodChg;
    std::map<std::string, MethodChg> Methods;
    
    /* String substitution */
    std::map<std::string, std::string> Words;
private:
};
}//sccp
