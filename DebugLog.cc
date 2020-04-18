#include "DebugLog.h"

// Default initialization
DebugLog *DebugLog::debugLogPtr = 0;

DebugLog::DebugLog()
    : isFileExits(false), logfilePtr(NULL)
{};

DebugLog::~DebugLog(){
    if(logfilePtr.is_open())
        logfilePtr.close();
}

void DebugLog::setDebugLogPower(bool _b_mode)
{
    this->b_debugLogMode = _b_mode;
}

void DebugLog::createLogFile(std::string strfileName)
{
    logfilename = strfileName;
    std::string strfilePath = 	DIR_PATH + strfileName;
    DIR* isDirExits = opendir(strfilePath.c_str());
    if (isDirExits)
    {
        std::cout << "INFO : LOG DIR EXITS - "<< strfilePath << std::endl;
        /* Directory exists. */
        closedir(isDirExits);
        isFileExits = true;
    }  else {
        std::cout << "INFO : DIR Not Exits, Creating DIR Structure .. .." << std::endl;
        std::string path = std::string(DIR_MAIN) + std::string(DIR_LOGS) + strfileName;
        if(!this->makedir(0777, DIR_VAR_OPT, path))
        {
            /*TODO:: Close File Support.*/
            isFileExits = false;
        } else {
            isFileExits = true;
        }
    }
    if(isFileExits) {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        char buffer2[80];
        char buffer3[80];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        sprintf(buffer, "%.2d_%.2d_%d", timeinfo->tm_mday, (timeinfo->tm_mon + 1) , (timeinfo->tm_year + 1900));
        std::string strDate(buffer);
        strftime(buffer3,sizeof(buffer3),"%H:%M:%S",timeinfo);
        std::string strTime(buffer3);

        /*Remove old file */
        sprintf(buffer2, "%.2d_%.2d_%d", timeinfo->tm_mday, (timeinfo->tm_mon) , (timeinfo->tm_year + 1900));
        std::string strOldDate(buffer2);
        std::string strOldfilename = strfilePath + "/" +strOldDate + strfileName + FILE_EXTENSION;
        std::cout << "INFO : Remove Old File if Exits - "<< strOldfilename << std::endl;
        if( remove( strOldfilename.c_str() ) != 0 )
            std::cerr << "EROR : Fail to remove old file." << std::endl;
        else
            std::cout << "INFO : Old File Remove successfully." << std::endl;

        std::string strfilename = strfilePath + "/" +strDate + strfileName + FILE_EXTENSION;
        std::cout << "INFO : Log File Name - " << strfilename << std::endl;
        std::ifstream infile(strfilename.c_str());
        if(infile.good())
        {
            std::cout << "INFO : FILE EXITS !!" << std::endl;
            if(logfilePtr.is_open())
                logfilePtr.close();

            logfilePtr.open(strfilename.c_str(), std::ios_base::app);
            if(logfilePtr.is_open())
            {
                logfilePtr << "\n\n";
                logfilePtr << "**************************************************************************\n";
                logfilePtr << "*                      \"APP LOG FILE\"\n";
                logfilePtr << "* App Name  - " << strfileName << "\n";
                logfilePtr << "* Date      - " << strDate << "\n";
                logfilePtr << "* Time      - " << strTime << "\n";
                logfilePtr << "* File Name - " << strDate << strfileName << FILE_EXTENSION << "\n";
                logfilePtr << "* File Path - " << strfilename << "\n";
                logfilePtr << "**************************************************************************\n";
                logfilePtr << "\n\n";
            }
            logfilePtr.flush();
        }
        else
        {
            std::cout << "INFO : FILE NOT EXITS, Creating File !!" << std::endl;
            mode_t perms = S_IRWXU; // Pick appropriate permissions for the new file.
            int fd = open(strfilename.c_str(), O_CREAT|O_EXCL, perms);
            if (fd >= 0)
            {
                isFileExits = true;
                std::cout << "INFO : File Created SUCCESSFULLY." << std::endl;
                //This creates it.
                if(logfilePtr.is_open())
                    logfilePtr.close();

                logfilePtr.open(strfilename.c_str(), std::ios_base::app);
                if(logfilePtr.is_open())
                {
                    logfilePtr << "**************************************************************************\n";
                    logfilePtr << "*                      \"APP LOG FILE\"\n";
                    logfilePtr << "* App Name  - " << strfileName << "\n";
                    logfilePtr << "* Date      - " << strDate << "\n";
                    logfilePtr << "* Time      - " << strTime << "\n";
                    logfilePtr << "* File Name - " << strDate << strfileName << FILE_EXTENSION << "\n";
                    logfilePtr << "* File Path - " << strfilename << "\n";
                    logfilePtr << "**************************************************************************\n";
                    logfilePtr << "\n\n";
                }
                logfilePtr.flush();
            } else {
                /*TODO:: Close File Support.*/
                /* Error occurred. Examine errno to find the reason. */
                std::cerr << "EROR : Unable to Create File." << std::endl;
                isFileExits = false;
            }
        }
        std::cout << "INFO : File Path - " <<  strfilename << std::endl;
    }
}

bool DebugLog::makedir(mode_t mode, const std::string& rootPath, std::string& path)
{
    bool ret(false);
    struct stat st;
    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
        std::string::iterator newIter = std::find( iter, path.end(), '/' );
        std::string newPath = rootPath + "/" + std::string( path.begin(), newIter);
        //std::cout << "DBUG : Created Path - " << newPath << std::endl;
        if( stat( newPath.c_str(), &st) != 0)
        {
            if( mkdir( newPath.c_str(), mode/*S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH*/) != 0 && errno != EEXIST )
            {
                std::cerr << "EROR : Cannot create folder - " << newPath << ". ErroNo -" << strerror(errno) << std::endl;
                ret = false;
            }
            else
            {
                std::cout << "INFO : SUCCESS. Created Folder - '" << newPath  << std::endl;
                ret = true;
            }
        }
        else
            if( !S_ISDIR(st.st_mode) )
            {
                errno = ENOTDIR;
                std:: cerr << "EROR : Dir Path '" << newPath << "' is not a dir " << std::endl;
                ret = false;
            }
            else
            {
                std::cout << "INFO : Dir Path '" << newPath << "' id already exists " << std::endl;
                ret = true;
            }

        iter = newIter;
        if( newIter != path.end() )
            ++ iter;
    }
    return ret;
}

void DebugLog::closeLogFile()
{
    if (logfilePtr.is_open())
    {
        if(logfilePtr.is_open())
        {
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[80];
            char buffer2[80];
            char buffer3[80];
            time (&rawtime);
            timeinfo = localtime(&rawtime);
            sprintf(buffer, "%.2d_%.2d_%d", timeinfo->tm_mday, (timeinfo->tm_mon + 1) , (timeinfo->tm_year + 1900));
            std::string strDate(buffer);
            strftime(buffer3,sizeof(buffer3),"%H:%M:%S",timeinfo);
            std::string strTime(buffer3);

            logfilePtr << "\n\n";
            logfilePtr << "**************************************************************************\n";
            logfilePtr << "*                      \"APP LOG FILE - ShutDown\"\n";
            logfilePtr << "* App Name  - " << logfilename << "\n";
            logfilePtr << "* Date      - " << strDate << "\n";
            logfilePtr << "* Time      - " << strTime << "\n";
            logfilePtr << "* File Name - " << strDate << logfilename << FILE_EXTENSION << "\n";
            logfilePtr << "* File Path - " << logfilename << "\n";
            logfilePtr << "**************************************************************************\n";
        }
        logfilePtr.flush();
        logfilePtr.close();
    }
}

void DebugLog::checklogfileOverflow()
{
    linecount++;
    if(linecount == 20000) {
        //this->createLogFile(logfilename);
        linecount = 0;
    }
}

void DebugLog::processDateAndTime(std::string &date, std::string &time_)
{
    time_t rawtime_;
    struct tm * timeinfo;
    char bufferDate[80];
    char bufferTime[80];
    time (&rawtime_);
    timeinfo = localtime(&rawtime_);
    sprintf(bufferDate, "%.2d/%.2d/%d", timeinfo->tm_mday, (timeinfo->tm_mon + 1) , (timeinfo->tm_year + 1900));
    date = bufferDate;
    strftime(bufferTime,sizeof(bufferTime),"%H:%M:%S",timeinfo);
    time_ = bufferTime;
    return ;
}

void DebugLog::processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            logfilePtr << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            logfilePtr << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFileInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            logfilePtr << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFile(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            logfilePtr << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            std::string date;
            std::string time;
            this->processDateAndTime(date, time);
            logfilePtr << date<< " : " << time << " : " << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFileWithTDInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2)
{
    if((msgType == "EROR") || (msgType == "WARN")) {
        if(logfilePtr.is_open() && isFileExits) {
            std::string date;
            std::string time;
            this->processDateAndTime(date, time);
            logfilePtr << date<< " : " << time << " : " << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 <<std::endl;
            logfilePtr.flush();
        }
        this->checklogfileOverflow();
    }
}

void DebugLog::processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2)
{
    if(logfilePtr.is_open() && isFileExits) {
        std::string date;
        std::string time;
        this->processDateAndTime(date, time);
        logfilePtr << date<< " : " << time << " : " << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 <<std::endl;
        logfilePtr.flush();
    }
    this->checklogfileOverflow();
}

void DebugLog::processDumpFileWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3)
{
    if(logfilePtr.is_open() && isFileExits) {
        std::string date;
        std::string time;
        this->processDateAndTime(date, time);
        logfilePtr << date<< " : " << time << " : " << msgType << " : " << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 <<std::endl;
        logfilePtr.flush();
    }
    this->checklogfileOverflow();
}

void DebugLog::processConsolOut(std::string msgType,int i_lnum, std::string functionname, std::string module, std::string msg)
{
    if(this->b_debugLogMode) {
        if(!msgType.compare("EROR"))
            std::cerr << KRED << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOut(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2)
{
    if(this->b_debugLogMode) {
        if(!msgType.compare("EROR"))
            std::cerr << KRED << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOutInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2)
{
    if(this->b_debugLogMode) {
        if(!msgType.compare("EROR"))
            std::cerr << KRED << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOut(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1,std::string msg2, std::string msg3)
{
    if(this->b_debugLogMode) {
        if(!msgType.compare("EROR"))
            std::cerr << KRED << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg)
{
    if(this->b_debugLogMode) {
        std::string date;
        std::string time;
        this->processDateAndTime(date,time);
        if(!msgType.compare("EROR"))
            std::cerr << KRED << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2)
{
    if(this->b_debugLogMode) {
        std::string date;
        std::string time;
        this->processDateAndTime(date,time);
        if(!msgType.compare("EROR"))
            std::cerr << KRED << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOutWithTDInt(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, double msg2)
{
    if(this->b_debugLogMode) {
        std::string date;
        std::string time;
        this->processDateAndTime(date,time);
        if(!msgType.compare("EROR"))
            std::cerr << KRED << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << RST << std::endl << std::flush;
    }
}

void DebugLog::processConsolOutWithTD(std::string msgType, int i_lnum, std::string functionname, std::string module, std::string msg1, std::string msg2, std::string msg3)
{
    if(this->b_debugLogMode) {
        std::string date;
        std::string time;
        this->processDateAndTime(date,time);
        if(!msgType.compare("EROR"))
            std::cerr << KRED << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("DBUG"))
            std::cout << KWHT << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("INFO"))
            std::cout << KGRN << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
        else if(!msgType.compare("WARN"))
            std::cout << KYEL << date << " : " << time << " : " << msgType << " : "
                      << module << " : " << i_lnum << " : ("<< functionname <<") " << msg1 << " : " << msg2 << " : " << msg3 << RST << std::endl << std::flush;
    }
}
