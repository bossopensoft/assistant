#include <chrono>
#include <algorithm>
#include <thread>
#include <map>

#include "tinylogger.h"
#include "iloggerfilehandler.h"
#include "linuxloggerfile.h"
#include "blockingqueue.hpp"
#include "tracer.h"
#include "config.h"

using namespace std;


using namespace TINY_LOGGER_PRIVATE;

std::mutex Tracer::printMutex_;
char Tracer::vprintBuffer[MAX_BUFFER_SIZE]={0};


#define _DEBUG_LOGGER

#ifdef _DEBUG_LOGGER
#define LOG_LOGGER(...)    TRACE(__VA_ARGS__)
#define LOG_LOGGER_VA(...) TRACE_VA(__VA_ARGS__)
#define LOG_LOGGER_E(...)  TRACE_E(__VA_ARGS__)
#else
#define LOG_LOGGER(...)
#define LOG_LOGGER_VA(...)
#define LOG_LOGGER_E(...)
#endif


struct LoggerFileNode
{
    std::string    fileName;
    size_t         size;

    static TinyLogger::LoggerFileNode_Ptr make_file_node(const std::string& filename, size_t size)
    {
        return TinyLogger::LoggerFileNode_Ptr(new LoggerFileNode {filename, size});
    }
};


static void _dump_files(std::vector<TinyLogger::LoggerFileNode_Ptr>&  file_names)
{
#ifdef _DEBUG_LOGGER
        for_each(file_names.begin(),
                 file_names.end(),
                 [](const TinyLogger::LoggerFileNode_Ptr _fileNode)
                 {
                     LOG_LOGGER_VA("{file=%s, size=%d}", _fileNode->fileName.c_str(), _fileNode->size);
                 });
#endif
}

#define DUMP_LOGGER_FILES(_FILES)   _dump_files(_FILES)
#define _IS_GREATE(expression) ((expression) > 0?true:false)
#define _IS_LOWER(expression) (!_IS_GREATE(expression))

static map<string, function<ILoggerFileHandler*()>>  _factorys=
{
    {   "LINUX",
         []()->ILoggerFileHandler*
         {
            return (new LinuxLoggerFile);
         }
    },
};

void set_Debug_FileHandlerFactory(const string platform_name, function<ILoggerFileHandler*()> _factory)
{
    _factorys.insert({platform_name, _factory});
}

static ILoggerFileHandler* FileHandlerFactory(const string platform_name=PLATFORM_NAME)
{
    return  _factorys.at(platform_name)();
}

struct LoggerFileWrapper
{
    static LoggerFileWrapper* mInstance;

    ILoggerFileHandler* mImp = nullptr;

    std::string base_filename_;
    std::string filename_extension_;

    std::string currentFile;

    LoggerFileWrapper():mImp(FileHandlerFactory())
    {

    }

    static LoggerFileWrapper* Instance()
    {
       if(mInstance) return mInstance;

       mInstance = new LoggerFileWrapper();
       return mInstance;
    }

    virtual ~LoggerFileWrapper(){}

    static inline int _compare_to(int _a, int _b)
    {
        int _r = (_a > _b)?1:((_a < _b)?-1:0);
        return _r;
    }

    static inline int _compare_to(long _a, long _b)
    {
        int _r = (_a > _b)?1:((_a < _b)?-1:0);
        return _r;
    }

    static inline std::string LocalTimeInString()
    {
        return TinyLogger_StringUtility::getLocalTimeInString();
    }


    static inline std::string  make_logger_filename(
                                            const std::string&  workspace,
                                            const std::string&  baseFileName,
                                            const std::string&  filename,
                                            const std::string&  fileExtenstion
                                            )
    {
        if(fileExtenstion[0] != '.')
            return (workspace + "/" + baseFileName+"_"+filename+'.'+fileExtenstion);
        else
            return (workspace + "/" + baseFileName+"_"+filename+fileExtenstion);
    }

    bool OpenLogfile(const std::string& fileName)
    {
        currentFile = fileName;
        return mImp->OpenLogfile(currentFile);
    }

    int WriteLog(const std::string logText)
    {
        if(logText.size() == 0) return 0;
        return mImp->WriteLog(logText);
    }

    void CloseLogFile()
    {
        mImp->CloseLogFile();
    }

    bool RemoveFile(const std::string& fileName)
    {
        return mImp->RemoveFile(fileName);
    }

    bool MakeLoggerWorkSpace(const std::string& dirName)
    {
        return mImp->MakeLoggerWorkSpace(dirName);
    }

    bool FileExist(const std::string& file)
    {
        return mImp->FileExist(file);
    }

    bool IsVisiable(const std::string& file)
    {
        return mImp->IsVisiable(file);
    }


    bool TravelDirectory(FileList& files,
                         const std::string& dirname,
                         const std::string& fileExtension,
                         int deepth)
    {
      static auto _comparer =[](const std::string& _first,
                                const std::string& _second)->bool
                            {
                              struct tm _fstTime;
                              struct tm _scdTime;
                              long _fstTick;
                              long _secTick;

                              if(!TinyLogger_StringUtility::getDateStructFromFileName(&_fstTime,&_fstTick,_first))  return false;

                              if(!TinyLogger_StringUtility::getDateStructFromFileName(&_scdTime,&_secTick,_second)) return false;

                              int _r= _compare_to(_fstTime.tm_year, _scdTime.tm_year);
                              if(_r != 0) return _IS_LOWER(_r);

                              _r= _compare_to(_fstTime.tm_mon, _scdTime.tm_mon);
                              if(_r != 0)return _IS_LOWER(_r);

                              _r= _compare_to(_fstTime.tm_mday, _scdTime.tm_mday);
                              if(_r != 0) return _IS_LOWER(_r);

                              _r= _compare_to(_fstTime.tm_hour, _scdTime.tm_hour);
                              if(_r != 0) return _IS_LOWER(_r);

                              _r= _compare_to(_fstTime.tm_min, _scdTime.tm_min);
                              if(_r != 0)return _IS_LOWER(_r);

                              _r= _compare_to(_fstTime.tm_sec, _scdTime.tm_sec);
                              if(_r != 0) return _IS_LOWER(_r);

                              _r= _compare_to(_fstTick, _secTick);
                              if(_r != 0) return _IS_LOWER(_r);

                              return  false;
                            };

        return mImp->TravelDirectory(files, dirname, fileExtension, _comparer, deepth);
    }

    size_t GetFileSize(const std::string& filename)
    {
        return mImp->GetFileSize(filename);
    }

};

LoggerFileWrapper* LoggerFileWrapper::mInstance=nullptr;

struct LoggerFileLoader
{
    FileList    _LoggerFiles;
    bool        _status;
};

static LoggerFileLoader loadAvaliableLoggerFile(const string& workspace, const string& fileExtension)
{
    LOG_LOGGER_VA("workspace:%s, file extension=%s",workspace.c_str(), fileExtension.c_str());

    if(!LoggerFileWrapper::Instance()->FileExist(workspace))
        LoggerFileWrapper::Instance()->MakeLoggerWorkSpace(workspace);

    if(!LoggerFileWrapper::Instance()->IsVisiable(workspace)){
        LOG_LOGGER_E("directory was not visiable");
        return {{""}, false};
    }


    FileList    LoggerFiles;
    if(!LoggerFileWrapper::Instance()->TravelDirectory(LoggerFiles, workspace, fileExtension, 1)){
        LOG_LOGGER_E("travel directory was failure!");
        return {{""}, false};
    }

    return {LoggerFiles, true};
}


struct BlockingQueueWrapper : public BlockingQueue<string>
{
   inline BlockingQueueWrapper()
   {
        setMaxSize(10);
        setBlocking(true);
   }
   ~BlockingQueueWrapper()
   {
        if(isBlocking()) setBlocking(false);
   }
};

#define IS_SET(BITMAP, FLAGE) ((BITMAP)&(FLAGE))

//TinyLogger* TinyLogger::mINSTANCE =nullptr;

//TinyLogger *TinyLogger::getInstance()
//{
//    if(mINSTANCE == nullptr) mINSTANCE = new TinyLogger();

//    return mINSTANCE;
//}

TinyLogger::TinyLogger()
          : mLevel(LV_DEBUG|LV_INFO|LV_FATAL|LV_WARNING|LV_CRITICAL)
          , mQueue(new BlockingQueueWrapper)
          , mBackendThread(nullptr)
          , mIsBackendThreadRunning(false)
{

}

void TinyLogger::startHandler()
{
    if(mIsBackendThreadRunning) return;

    mIsBackendThreadRunning = true;
    mBackendThread= new  thread([this]()->int
                              {
                                logger_handler();
                                mIsBackendThreadRunning = false;
                                return 0;
                              });
}

int TinyLogger::logger_handler()
{
    LoggerFileNode_Ptr _file = mLoggerFileManager.loggerFiles.front();

    if(!LoggerFileWrapper::Instance()->OpenLogfile(_file->fileName)) return -1;

    while(mIsBackendThreadRunning)
    {
        LOG_LOGGER_VA("mIsHandlerThreadRunning =%s", mIsBackendThreadRunning?"true":"false");
        string _msg     =  mQueue->pop();
        int _writed_size= LoggerFileWrapper::Instance()->WriteLog(_msg);

        if(_writed_size <= 0) continue;

        /// if the size of the file excessed the capacity of a singal file.
        _file->size += _writed_size;
        if(_file->size > mLoggerFileManager.file_size)
        {
            LoggerFileWrapper::Instance()->CloseLogFile();

            if(!createLoggerFile())
            {
                LOG_LOGGER_E("create file failure!");
                break; // create new one.
            }

            _file = mLoggerFileManager.loggerFiles.front();

            LoggerFileWrapper::Instance()->OpenLogfile(_file->fileName);
        }

        /// if the file queue excessed  the capacity of queue.
        LOG_LOGGER_VA("before logger file size= %ld, capacity=%ld", mLoggerFileManager.loggerFiles.size(),  mLoggerFileManager.capacity);
        if(mLoggerFileManager.loggerFiles.size() > mLoggerFileManager.capacity)
        {
           LoggerFileNode_Ptr _removedFile =   mLoggerFileManager.loggerFiles.back();
           LoggerFileWrapper::Instance()->RemoveFile(_removedFile->fileName);
           mLoggerFileManager.loggerFiles.pop_back();
           delete _removedFile;

           DUMP_LOGGER_FILES(mLoggerFileManager.loggerFiles);
        }
        LOG_LOGGER_VA("after logger file size= %ld, capacity=%ld", mLoggerFileManager.loggerFiles.size(),  mLoggerFileManager.capacity);

        FileList    LoggerFiles;
        if(!LoggerFileWrapper::Instance()->TravelDirectory(LoggerFiles, mConfiguration.Workspace, mConfiguration.FileNameExtension, 1))
        {
            LOG_LOGGER_E("travel directory was failure!");
            break;
        }

        if(LoggerFiles.empty())
        {
            LOG_LOGGER_E("NO LOGGER FILE EXIST!");
            break;
        }
    }

    LoggerFileWrapper::Instance()->CloseLogFile();
    return 0;
}

TinyLogger::~TinyLogger()
{
    if(mIsBackendThreadRunning) mIsBackendThreadRunning = false;

    if(mQueue && mQueue->isBlocking()) mQueue->setBlocking(false);

//    LOG_LOGGER_VA("~TinyLogger()::mIsHandlerThreadRunning=%s", mIsBackendThreadRunning?"true":"false");

    if(mBackendThread) { mBackendThread->join(); delete mBackendThread; }

    if(mQueue) delete mQueue;

    if(!mLoggerFileManager.loggerFiles.empty())
    {
       for(auto _loggerFile : mLoggerFileManager.loggerFiles)
       {
           delete _loggerFile;
       }

       mLoggerFileManager.loggerFiles.clear();
    }
}

void TinyLogger::setLoggerConfiguration(const Configuration &config)
{
     mConfiguration = config;
}

bool TinyLogger::initialization()
{
    LoggerFileLoader _loader= loadAvaliableLoggerFile(mConfiguration.Workspace, mConfiguration.FileNameExtension);
    if(!_loader._status) return false;

    //// file name format as: XXXX_DATETIME.log
    if(_loader._LoggerFiles.empty())
    {
       /// no exist logger file.
       /// make a first one.
       if(!createLoggerFile()) return false;
    }
    else
    {
        /// remove the last files.
        if(_loader._LoggerFiles.size() > mConfiguration.MaxFileNum)
        {
            auto _it = _loader._LoggerFiles.begin();
            std::advance(_it, mConfiguration.MaxFileNum -1);

            for_each(_it,
                     _loader._LoggerFiles.end(),
                     [&](const string &_fileName)
                     {
                         LoggerFileWrapper::Instance()->RemoveFile(_fileName);
                     });

            /// remove the resets.
            int _counter = _loader._LoggerFiles.size() - mConfiguration.MaxFileNum;
            while(_counter--)
            {
                _loader._LoggerFiles.pop_back();
            }
        }

        //// initilize the file manager block.
        for (auto _loggerFile : _loader._LoggerFiles)
        {
            mLoggerFileManager.loggerFiles.push_back(
                                LoggerFileNode::make_file_node(_loggerFile, LoggerFileWrapper::Instance()->GetFileSize(_loggerFile))
                                );
        }
    }

    mLoggerFileManager.capacity  = mConfiguration.MaxFileNum;
    mLoggerFileManager.file_size = mConfiguration.MaxFileSize;
    LOG_LOGGER_VA("mLoggerFileManager.capacity=%ld file size=%ld",mLoggerFileManager.capacity, mLoggerFileManager.file_size);
    DUMP_LOGGER_FILES(mLoggerFileManager.loggerFiles);

    startHandler();
    LOG_LOGGER("init in suceess !");
    return true;
}

int TinyLogger::writeLogger(TinyLogger::loggerlv_t level, string message)
{
    if(!IS_SET(mLevel,level)) return 0;

    if(!mBackendThread) return -1;

//    LOG_LOGGER_VA("write logger=%s", message.c_str());

    int _write_size = message.size();
    mQueue->push(message);
    return _write_size;
}


bool TinyLogger::createLoggerFile()
{
    std::string _loggerFile;

    if(!getFileName(_loggerFile)) return false;

    /// create a logger file.
    if(!LoggerFileWrapper::Instance()->OpenLogfile(_loggerFile)) return  false;

    /// close current file.
    LoggerFileWrapper::Instance()->CloseLogFile();
    auto _first =mLoggerFileManager.loggerFiles.begin();
    mLoggerFileManager.loggerFiles.emplace(_first, LoggerFileNode::make_file_node(_loggerFile, 0));

//    mLoggerFileManager.loggerFiles.push_front(LoggerFileNode::make_file_node(_loggerFile, 0));
    return true;
}

bool TinyLogger::getFileName(string &fileNameOut)
{
    int counter  = 3;
    bool isFound = false;
    std::string _loggerFile;

    while(counter--)
    {

        std::string _loggerFile=  LoggerFileWrapper::make_logger_filename(mConfiguration.Workspace,
                                                                          mConfiguration.BaseFileName,
                                                                          LoggerFileWrapper::LocalTimeInString(),
                                                                          mConfiguration.FileNameExtension);
        for(auto _fileInfo : mLoggerFileManager.loggerFiles)
        {
            if(_fileInfo->fileName == _loggerFile) {isFound = true; break;}
        }

        if(isFound){sleep(1); isFound =false; continue;}

        fileNameOut = _loggerFile;
        break;
    }

    if(isFound)
    {
        LOG_LOGGER_VA("file name collison! new:%s", _loggerFile.c_str());
        DUMP_LOGGER_FILES(mLoggerFileManager.loggerFiles);
    }
    return !isFound;
}
