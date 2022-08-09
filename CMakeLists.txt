MESSAGE(STATUS "Brick Co.,Ltd.")
MESSAGE(STATUS "plugin for Source code convert tool")
MESSAGE(STATUS cmake .. -G "Visual Studio 14 2015")

CMAKE_MINIMUM_REQUIRED(VERSION 3.7.0)

PROJECT(jclib)

SET(CMAKE_BUILE_TYPE RELEASE CACHE STRING "set build type to release")

SET(TARGET_CODE "jclib")

SET(ALL_SOURCE_FILE
    stdafx.cpp
    tinyxml2.cpp
    SCCPBase.cpp
    SCCPConfig.cpp
    SCCPFile.cpp
    SCCPNote.cpp
    SCCPNode.cpp
    SCCPImport.cpp
    SCCPStmt.cpp
    SCCPConvert.cpp
    dllmain.cpp
    )

ADD_LIBRARY(${TARGET_CODE} SHARED 
            ${ALL_SOURCE_FILE}
            )

SET_TARGET_PROPERTIES(${TARGET_CODE}
    PROPERTIES
    OUTPUT_NAME ${TARGET_CODE}
    SOVERSION ${major_version}
    VERSION ${version}
    )
