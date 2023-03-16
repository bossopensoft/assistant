#ifndef TESTCASE_FILEHANDLER_TEST_H
#define TESTCASE_FILEHANDLER_TEST_H

#include <QObject>

class FileHandler;
class TestCase_FileHandler_Test : public QObject
{
    Q_OBJECT
public:
    explicit TestCase_FileHandler_Test(QObject *parent = nullptr);

public slots:
    void testcase_test_fileLoad();

private:
    FileHandler* mFileHandler;
};

#endif // TESTCASE_FILEHANDLER_TEST_H
