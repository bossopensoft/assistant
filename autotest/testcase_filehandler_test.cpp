#include "testcase_filehandler_test.h"
#include "inifilehandler.h"

#include <QTest>
#include <QByteArray>
#include <QTextStream>
#include <QDebug>

#define CONFIG_FILE_DIR (QString)"../assistant/conf"

#define NAME_OF_CA_CONF_FILE    (QString)"ca.ini"
#define NAME_OF_DLL_CONF_FILE   (QString)"dll.ini"
#define NAME_OF_SYS_CONF_FILE   (QString)"sys.ini"

#define __VERIFY(EXPRESSION)    QVERIFY2((EXPRESSION), (#EXPRESSION))

TestCase_FileHandler_Test::TestCase_FileHandler_Test(QObject *parent)
                         : QObject(parent)
                         , mFileHandler(new FileHandler(CONFIG_FILE_DIR+"/"+NAME_OF_CA_CONF_FILE))
{

}

void TestCase_FileHandler_Test::testcase_test_fileLoad()
{
   QByteArray _text;
    __VERIFY(mFileHandler->load(_text));
}
