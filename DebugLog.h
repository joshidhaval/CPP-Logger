#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <string.h>
#include <cerrno>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


/*IF Unable Any Debug, Project should be recompile*/
//#define DEBUG_CONSOLOUT						1
//#define DEBUG_FILEOUT						1
//#define DEBUG_CONSOLOUT_WITH_TD				1
//#define DEBUG_FILEOUT_WITH_TD				1
#define DEBUG_CONSOLOUT_FILEOUT				1
//#define DEBUG_CONSOLOUT_FILEOUT_WITH_TD		1

//#define edu_MODULE			"eI"
#define DIR_MODE			0777
#define DIR_PATH			"/var/opt/logs/"  /* Log file will dump in /opt/log/<AppName>/logfile.txt */
#define DIR_VAR_OPT         "/var/"
#define DIR_EDUTREE         "opt/logs/"
#define DIR_LOGS            "logs/"
#define FILE_EXTENSION 		".txt"

/**
 * Macro for printing output messagages in color format.
 */
#define RST   "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST
#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define AA_RED(r,s,t,u,v,w,x,y,z) FRED(r s t u v w x y z)
#define AA_GRN(r,s,t,u,v,w,x,y,z) FGRN(r s t u v w x y z)
#define AA_YEL(r,s,t,u,v,w,x,y,z) FYEL(r s t u v w x y z)
#define AA_BLU(r,s,t,u,v,w,x,y,z) FBLU(r s t u v w x y z)
#define AA_MAG(r,s,t,u,v,w,x,y,z) FMAG(r s t u v w x y z)
#define AA_CYN(r,s,t,u,v,w,x,y,z) FCYN(r s t u v w x y z)
#define AA_WHT(r,s,t,u,v,w,x,y,z) FWHT(r s t u v w x y z)

#define A_RED(v,w,x,y,z) FRED(v w x y z)
#define A_GRN(v,w,x,y,z) FGRN(v w x y z)
#define A_YEL(v,w,x,y,z) FYEL(v w x y z)
#define A_BLU(v,w,x,y,z) FBLU(v w x y z)
#define A_MAG(v,w,x,y,z) FMAG(v w x y z)
#define A_CYN(v,w,x,y,z) FCYN(v w x y z)
#define A_WHT(v,w,x,y,z) FWHT(v w x y z)

class DebugLog;

/**
 * Different Level log Macros.
 */
//std::string strLog = __DATE__ + __TIME__ + msgType_ + module_ + msg_;
//DebugLog::instance()->processConsolPrint(strLog);
#if DEBUG_CONSOLOUT == 1
#define DBUG_PRINT(msgType_, linenumber, functionname, module_, msg_)						  \
    do {																					  \
    DebugLog::instance()->processConsolOut(msgType_, linenumber, functionname, module_, msg_);\
    } while (0)
#elif DEBUG_FILEOUT == 1
#define DBUG_PRINT(msgType_,linenumber,functionname,module_, msg_)							 \
    do {																					 \
    DebugLog::instance()->processDumpFile(msgType_, linenumber, functionname, module_, msg_);\
    } while (0)
#elif DEBUG_CONSOLOUT_WITH_TD == 1
#define DBUG_PRINT(msgType_,linenumber,functionname,module_, msg_)									\
    do {																							\
    DebugLog::instance()->processConsolOutWithTD(msgType_, linenumber, functionname, module_, msg_);\
    } while (0)
#elif DEBUG_FILEOUT_WITH_TD == 1
#define DBUG_PRINT(msgType_,linenumber,functionname,module_,msg_)									\
    do {																							\
    DebugLog::instance()->processDumpFileWithTD(msgType_, linenumber, functionname, module_, msg_);	\
    } while (0)
#elif DEBUG_CONSOLOUT_FILEOUT == 1
#define DBUG_PRINT(msgType_,linenumber,functionname,module_,msg_)							  \
    do {																					   \
    DebugLog::instance()->processConsolOut(msgType_, linenumber, functionname, module_, msg_); \
    DebugLog::instance()->processDumpFile(msgType_, linenumber, functionname, module_, msg_);  \
    } while (0)
#define DBUG_PRINT2(msgType_,linenumber,functionname,module_,msg_1,msg_2)							   \
    do {																							   \
    DebugLog::instance()->processConsolOut(msgType_, linenumber, functionname, module_, msg_1, msg_2); \
    DebugLog::instance()->processDumpFile(msgType_, linenumber, functionname, module_, msg_1, msg_2);  \
    } while (0)
#define DBUG_PRINT2I(msgType_,linenumber,functionname,module_,msg_1,msg_2)      						  \
    do {                                                                                                  \
    DebugLog::instance()->processConsolOutInt(msgType_, linenumber, functionname, module_, msg_1, msg_2); \
    DebugLog::instance()->processDumpFileInt(msgType_, linenumber, functionname, module_, msg_1, msg_2);  \
    } while (0)
#define DBUG_PRINT3(msgType_,linenumber,functionname,module_, msg_1, msg_2,msg_3)							 \
    do {																							         \
    DebugLog::instance()->processConsolOut(msgType_, linenumber, functionname, module_, msg_1, msg_2,msg_3); \
    DebugLog::instance()->processDumpFile(msgType_, linenumber, functionname, module_, msg_1, msg_2,msg_3);	 \
    } while (0)
#elif DEBUG_CONSOLOUT_FILEOUT_WITH_TD == 1
#define DBUG_PRINT(msgType_,linenumber,functionname,module_,msg_)	                                \
    do {																							\
    DebugLog::instance()->processConsolOutWithTD(msgType_, linenumber, functionname, module_, msg_);\
    DebugLog::instance()->processDumpFileWithTD(msgType_, linenumber, functionname, module_, msg_);	\
    } while (0)
#define DBUG_PRINT2(msgType_,linenumber,functionname,module_,msg_1,msg_2)									\
    do {																							        \
    DebugLog::instance()->processConsolOutWithTD(msgType_, linenumber, functionname, module_, msg_1, msg_2);\
    DebugLog::instance()->processDumpFileWithTD(msgType_, linenumber, functionname, module_, msg_1, msg_2);	\
    } while (0)
#define DBUG_PRINT2I(msgType_,linenumber,functionname,module_,msg_1,msg_2)      							   \
    do {																							           \
    DebugLog::instance()->processConsolOutWithTDInt(msgType_, linenumber, functionname, module_, msg_1, msg_2);\
    DebugLog::instance()->processDumpFileWithTDInt(msgType_, linenumber, functionname, module_, msg_1, msg_2); \
    } while (0)
#define DBUG_PRINT3(msgType_,linenumber,functionname,module_, msg_1, msg_2,msg_3)								  \
    do {																							              \
    DebugLog::instance()->processConsolOutWithTD(msgType_, linenumber, functionname, module_, msg_1, msg_2,msg_3);\
    DebugLog::instance()->processDumpFileWithTD(msgType_, linenumber, functionname, module_, msg_1, msg_2,msg_3); \
    } while (0)
#endif

//#if DEBUG_CONSOLOUT || DEBUG_FILEOUT || DEBUG_CONSOLOUT_WITH_TD ||DEBUG_FILEOUT_WITH_TD 			\
//    || DEBUG_CONSOLOUT_FILEOUT || DEBUG_CONSOLOUT_FILEOUT_WITH_TD
#define LOGEROR(x)			DBUG_PRINT("EROR",__LINE__,__func__,edu_MODULE,x)
#define LOGWARN(x)			DBUG_PRINT("WARN",__LINE__,__func__,edu_MODULE,x)
#define LOGINFO(x)			DBUG_PRINT("INFO",__LINE__,__func__,edu_MODULE,x)
#define LOGDBUG(x)			DBUG_PRINT("DBUG",__LINE__,__func__,edu_MODULE,x)

#define LOGEROR2(x,y)		DBUG_PRINT2("EROR",__LINE__,__func__,edu_MODULE,x,y)
#define LOGWARN2(x,y)		DBUG_PRINT2("WARN",__LINE__,__func__,edu_MODULE,x,y)
#define LOGINFO2(x,y)		DBUG_PRINT2("INFO",__LINE__,__func__,edu_MODULE,x,y)
#define LOGDBUG2(x,y)		DBUG_PRINT2("DBUG",__LINE__,__func__,edu_MODULE,x,y)

#define LOGEROR2I(x,y)		DBUG_PRINT2I("EROR",__LINE__,__func__,edu_MODULE,x,y)
#define LOGWARN2I(x,y)		DBUG_PRINT2I("WARN",__LINE__,__func__,edu_MODULE,x,y)
#define LOGINFO2I(x,y)		DBUG_PRINT2I("INFO",__LINE__,__func__,edu_MODULE,x,y)
#define LOGDBUG2I(x,y)		DBUG_PRINT2I("DBUG",__LINE__,__func__,edu_MODULE,x,y)

#define LOGEROR3(x,y,z)		DBUG_PRINT3("EROR",__LINE__,__func__,edu_MODULE,x,y,z)
#define LOGWARN3(x,y,z)		DBUG_PRINT3("WARN",__LINE__,__func__,edu_MODULE,x,y,z)
#define LOGINFO3(x,y,z)		DBUG_PRINT3("INFO",__LINE__,__func__,edu_MODULE,x,y,z)
#define LOGDBUG3(x,y,z)		DBUG_PRINT3("DBUG",__LINE__,__func__,edu_MODULE,x,y,z)

//#else
//#define LOGEROR(x)
//#define LOGWARN(x)
//#define LOGINFO(x)
//#define LOGDBUG(x)

//#define LOGEROR2(x,y)
//#define LOGWARN2(x,y)
//#define LOGINFO2(x,y)
//#define LOGDBUG2(x,y)

//#define LOGEROR2I(x,y)
//#define LOGWARN2I(x,y)
//#define LOGINFO2I(x,y)
//#define LOGDBUG2I(x,y)

//#define LOGEROR3(x,y,z)
//#define LOGWARN3(x,y,z)
//#define LOGINFO3(x,y,z)
//#define LOGDBUG3(x,y,z)
//#endif

/**
 * DebugLog responsible for implementing the debug log methods
 * for messages controlling debug output.
 */
class DebugLog
{
    static DebugLog *debugLogPtr;

public:
    DebugLog();

    ~DebugLog();

    void setDebugLogPower(bool _b_mode);

    void createLogFile(std::string strfileName);

    bool makedir(mode_t mode, const std::string& rootPath, std::string& path);

    void appendLogIntoFile();

    void processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg);

    void processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2);

    void processDumpFileInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2);

    void processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3);

    void processConsolOut(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg);

    void processConsolOut(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2);

    void processConsolOutInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2);

    void processConsolOut(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3);

    void processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg);

    void processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2);

    void processDumpFileWithTDInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2);

    void processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3);

    void processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg);

    void processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2);

    void processConsolOutWithTDInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2);

    void processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3);

    void processDateAndTime(std::string &date, std::string &time);

    void closeLogFile();

    void checklogfileOverflow();

    static DebugLog *instance()
    {
        if (!debugLogPtr)
            debugLogPtr = new DebugLog;
        return debugLogPtr;
    }

    static void deleteInstance()
    {
        if(debugLogPtr) {
            delete debugLogPtr;
            debugLogPtr = NULL;
        }
    }

private:
    bool isFileExits;

    bool b_debugLogMode = false;

    std::ofstream logfilePtr;

    std::string logfilename;

    int linecount = 0;
};

#endif // DEBUGLOG
