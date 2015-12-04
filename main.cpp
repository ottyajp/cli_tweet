#include <QCoreApplication>
#include <QTimer>
#include "twit_func.h"

using namespace std;

bool input = false;
bool single = false;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    a.setApplicationName("cli_tweet");
    QCommandLineParser parser;
    parser.setApplicationDescription("cli_tweet");
    parser.addHelpOption();
    QCommandLineOption input_auth_info("a","input authentication info. CK/CS,Token/TokenSecret.");
    parser.addOption(input_auth_info);
    QCommandLineOption single_mode("s","single tweet mode.");
    parser.addOption(single_mode);
    parser.process(a);

    if(parser.isSet(input_auth_info)){
        input = true;
    }
    if(parser.isSet(single_mode)){
        single = true;
    }

    twit_func m(0, &a);
    QTimer::singleShot(0, &m, SLOT(run()));
    return a.exec();
}

