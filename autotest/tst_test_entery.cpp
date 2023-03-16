#include <QtTest>
#include <QCoreApplication>

#include "testcase_filehandler_test.h"
// add necessary includes here


class test_entery : public QObject
{
    Q_OBJECT
public:
    test_entery();
    ~test_entery();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
private:
    TestCase_FileHandler_Test* mFileHandlerTest;
};

test_entery::test_entery()
           :mFileHandlerTest(new TestCase_FileHandler_Test())
{

}

test_entery::~test_entery()
{

}

void test_entery::initTestCase()
{

}

void test_entery::cleanupTestCase()
{

}

void test_entery::test_case1()
{
    mFileHandlerTest->testcase_test_fileLoad();
}

QTEST_MAIN(test_entery)

#include "tst_test_entery.moc"
