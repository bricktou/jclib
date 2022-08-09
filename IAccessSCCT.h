/********************************************************
 * Copyright (c) 2004-2022, Brick Technologies Co., Ltd
 * All rights reserved.
 * Home page:http://www.bricktou.com
********************************************************/
/**
 * Interface to access SCCT
 */
#ifndef _I_ACCESS_SCCT_H_
#define _I_ACCESS_SCCT_H_

#include <functional>
#include "UniqueId.h"

namespace sccp {
struct ISourceLine {
    virtual size_t fileLine() = 0;
    virtual std::string getLine() = 0;
};
typedef std::function<int(ISourceLine *)> funLine;

struct IToken {
    virtual std::string getCode() = 0;
    
    virtual bool isSymbol() = 0;
    virtual bool isOperator() = 0;
    virtual bool isString() = 0;
    virtual bool isNumber() = 0;
    
    virtual bool isHeadWord() = 0;
    virtual bool isBlank() = 0;
    
    virtual int getSerial() = 0;
};
typedef std::function<int(IToken *)> funToken;

struct IParam {
    virtual std::string getCode() = 0;
    virtual std::string getName() = 0;
    virtual std::string getType() = 0;
    virtual std::string getInOut() = 0;
};
typedef std::function<int(IParam *)> funParam;

struct INode {
    virtual std::string getCode() = 0;
    virtual std::string getName() = 0;
    virtual std::string getType() = 0;
    virtual std::string getProto() = 0;
    virtual std::string getBrief() = 0;
    
    virtual std::string getFilePath() = 0;
    virtual std::string getFileName() = 0;
    virtual std::string getFileBaseName() = 0;
    virtual std::string getRelativePath() = 0;
    
    virtual std::string getDBCode() = 0;
    virtual std::string getDBName() = 0;
    
    virtual node_kind getKind() = 0;
    virtual key_word getKey() = 0;
    
    virtual INode *getParent() = 0;
    virtual std::string getRelation() = 0;
    virtual INode *getAncestry() = 0;
    
    virtual INode *getNamespace() = 0;
    virtual INode *getPackage() = 0;
    virtual INode *getModule() = 0;
    virtual INode *getClass() = 0;
    
    virtual std::string getNamespaceFullName() = 0;
    virtual std::string getPackageFullName() = 0;
    virtual std::string getModuleFullName() = 0;
    virtual std::string getClassFullName() = 0;
    
    virtual INode *getFirstTag() = 0;
    virtual int tagMoveNext(INode *node) = 0;
    virtual int tagMoveFirst(INode *node) = 0;
    
    virtual INode *getLeftValue() = 0;
    virtual INode *getRightValue() = 0;
    
    virtual std::string getCheckName(INode *node) = 0;
    
    virtual void serialization() = 0;
    virtual int build() = 0;
    virtual INode *getFirstNode() = 0;
    virtual int nodeMoveNext(INode *node) = 0;
    virtual int nodeMoveFirst(INode *node) = 0;
    
    virtual void freeNode(INode *node) = 0;
    // userfunc|libfunc == getKind()
    virtual size_t argsCount() = 0;
    virtual INode *getArgs(int argN) = 0;
    virtual bool funcOverload() = 0;
    virtual std::string getOverload() = 0;
    // Sql statement
    virtual INode *getFirstTable() = 0;
    virtual int tableMoveNext(INode *node) = 0;
    virtual INode *getWhereItem() = 0;
    virtual int itemWhereMoveNext(INode *node) = 0;
    virtual INode *getFirstSelectItem() = 0;
    virtual int itemSelectMoveNext(INode *node) = 0;
    virtual INode *getFirstIntoItem() = 0;
    virtual int itemIntoMoveNext(INode *node) = 0;
    virtual INode *getFirstUpdateItem() = 0;
    virtual int itemUpdateMoveNext(INode *node) = 0;
    
    virtual std::string getTableOP() = 0;
    
    virtual bool isAnnotation() = 0;
    virtual bool isArray() = 0;
    virtual bool isConst() = 0;
    virtual bool isDeclare() = 0;
    virtual bool isExtern() = 0;
    virtual bool isFinal() = 0;
    virtual bool isFriend() = 0;
    virtual bool isPointer() = 0;
    virtual bool isPrivate() = 0;
    virtual bool isProtected() = 0;
    virtual bool isPublic() = 0;
    virtual bool isReference() = 0;
    virtual bool isSigned() = 0;
    virtual bool isStatic() = 0;
    virtual bool isTemplate() = 0;
    virtual bool isUnsigned() = 0;
    virtual bool isVirtual() = 0;
};
typedef std::function<int(INode *)> funNode;

struct IColumn {
    virtual std::string getCode() = 0;
    virtual std::string getName() = 0;
    
    virtual std::string getType() = 0;
    virtual type_kind typeKind() = 0;
    virtual int getPrecision() = 0;
    virtual int getScale() = 0;
    
    virtual bool isPrimaryKey() = 0;
    virtual bool isAuto() = 0;
    virtual bool isTimeStamp() = 0;
    virtual bool isIndex() = 0;
    virtual bool isUnsigned() = 0;
};
typedef std::function<int(IColumn *)> funColumn;

struct ITable {
    virtual std::string getCode() = 0;
    virtual std::string getName() = 0;
    
    virtual std::string getDBCode() = 0;
    virtual std::string getDBName() = 0;
    
    virtual int for_column(funColumn column) = 0;
    
    virtual size_t getOPCount() = 0;
    virtual int for_tabop(funNode node) = 0;
};
typedef std::function<int(ITable *)> funTable;

struct IInherit {
    virtual INode *getBaseClass() = 0;
    
    virtual bool isPrivate() = 0;
    virtual bool isProtected() = 0;
    virtual bool isPublic() = 0;
    
    virtual bool isExtends() = 0;
    virtual bool isImplements() = 0;
};
typedef std::function<int(IInherit *)> funInherit;

struct IStmt {
    virtual int fileLine() = 0;
    virtual int prevLine() = 0;
    virtual int nextLine() = 0;
    
    virtual size_t getRow() = 0;
    virtual int moveNext() = 0;
    virtual int movePrev() = 0;
    virtual int moveRow(size_t row) = 0;
    
    virtual void serialization() = 0;
    
    virtual void build() = 0;
    
    virtual int for_node(funNode node) = 0;
    virtual INode *getFirstNode() = 0;
    virtual int nodeMoveNext(INode *node) = 0;
    virtual int nodeMovePrev(INode *node) = 0;
    virtual int nodeMoveFirst(INode *node) = 0;
    virtual int nodeMoveLast(INode *node) = 0;
    virtual void freeNode(INode *node) = 0;
    virtual size_t getNodeCount() = 0;
    virtual size_t getNodeSerial(INode *node) = 0;
    
    virtual int for_token(funToken token) = 0;
    virtual IToken *getFirseToken() = 0;
    virtual IToken *getLastToken() = 0;
    virtual int tokenMoveNext(IToken *token) = 0;
    virtual int tokenMoveFirst(IToken *token) = 0;
    virtual void freeToken(IToken *token) = 0;
    /**
     * Indent at the beginning of a line
     */
    virtual int getIndention() = 0;
    virtual int getPrevIndention() = 0;
    virtual int getNextIndention() = 0;
    
    virtual std::string getWebText() = 0;
    
    /**
     * Definition statement
     */
    virtual stmt_kind getStmtKind() = 0;
    
    virtual type_kind typeKind() = 0;
    
    virtual bool isAnnotation() = 0;
    virtual bool isArray() = 0;
    virtual bool isConst() = 0;
    virtual bool isDeclare() = 0;
    virtual bool isExtern() = 0;
    virtual bool isFinal() = 0;
    virtual bool isFriend() = 0;
    virtual bool isInline() = 0;
    virtual bool isNative() = 0;
    virtual bool isPointer() = 0;
    virtual bool isPrivate() = 0;
    virtual bool isProtected() = 0;
    virtual bool isPublic() = 0;
    virtual bool isSigned() = 0;
    virtual bool isStatic() = 0;
    virtual bool isSynchronized() = 0;
    virtual bool isTemplate() = 0;
    virtual bool isUnsigned() = 0;
    virtual bool isVirtual() = 0;
    
    virtual size_t inheritCount() = 0;
    virtual int for_inherit(funInherit inherit) = 0;
    
    virtual bool throwsEmpty() = 0;
    virtual int for_throws(funNode node) = 0;
};
typedef std::function<int(IStmt *)> funStmt;

struct IAccessSCCT
{
    virtual size_t getFirstLine() = 0;
    virtual int for_line(funLine line) = 0;
    
    virtual size_t stmtCount() = 0;
    virtual int for_stmt(funStmt stmt) = 0;
    virtual IStmt *getFirstStmt() = 0;
    virtual IStmt *getSqlStmt(INode *argNode, int endRow) = 0;
    virtual IStmt *getStringSqlStmt(char *sql) = 0;
    virtual void freeStmt(IStmt *stmt) = 0;
    
    virtual std::string getFuncCode() = 0;
    virtual std::string getFuncName() = 0;
    virtual std::string getFuncProto() = 0;
    virtual std::string getFuncType() = 0;
    virtual std::string getFuncBrief() = 0;
    // sql procedure | function
    virtual std::string getProcCode() = 0;
    virtual std::string getProcName() = 0;
    virtual std::string getProcType() = 0;
    virtual std::string getProcReturns() = 0;
    virtual std::string getProcBrief() = 0;
    
    virtual bool paramEmpty() = 0;
    virtual int for_param(funParam param) = 0;
    virtual bool funcOverload() = 0;
    virtual std::string getOverload() = 0;
    
    virtual size_t getCallCount() = 0;
    virtual int for_call(funNode node) = 0;
    
    virtual std::string getSrcRoot() = 0;
    virtual std::string getOutRoot() = 0;
    virtual std::string getSrcPath() = 0;
    virtual std::string getSrcFileName() = 0;
    virtual std::string getSrcFileBaseName() = 0;
    virtual std::string getSrcFileExtName() = 0;
    virtual time_t getSrcFileTime() = 0;
    
    virtual std::string getReportBaseName() = 0;
    
    virtual std::string getDBCode() = 0;
    virtual std::string getDBName() = 0;
    
    virtual int for_table(funTable table) = 0;
    virtual int for_procedure(funNode node) = 0;
    virtual int for_function(funNode node) = 0;
    
    virtual std::string getProgName() = 0;
    virtual std::string getProgShort() = 0;
    virtual std::string getProgCode() = 0;
    virtual std::string getProgVersion() = 0;
    virtual std::string getProgEncoding() = 0;
    
    virtual std::string getRegFullName() = 0;
    virtual std::string getRegName() = 0;
    virtual std::string getRegLanguage() = 0;
    
    virtual std::string getWebCode(std::string &code) = 0;
    virtual std::string getWebString(std::string &text) = 0;
    
    virtual void copyCDN() = 0;
    
    virtual void sendInfo(char *major, char *minor, char *file, size_t line) = 0;
};
}//sccp
#endif//_I_ACCESS_SCCT_H_
