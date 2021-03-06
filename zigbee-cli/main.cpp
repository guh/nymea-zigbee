#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLoggingCategory>
#include "zigbeenetworkkey.h"
#include "zigbeenetworkmanager.h"

//static QHash<QString, bool> s_loggingFilters;

//static bool s_aboutToShutdown = false;

//static void catchUnixSignals(const std::vector<int>& quitSignals, const std::vector<int>& ignoreSignals = std::vector<int>()) {
//    auto handler = [](int sig) -> void {
//        switch (sig) {
//        case SIGQUIT:
//            qDebug() << "Cought SIGQUIT quit signal...";
//            break;
//        case SIGINT:
//            qDebug() << "Cought SIGINT quit signal...";
//            break;
//        case SIGTERM:
//            qDebug() << "Cought SIGTERM quit signal...";
//            break;
//        case SIGHUP:
//            qDebug() << "Cought SIGHUP quit signal...";
//            break;
//        case SIGSEGV: {
//            qDebug() << "Cought SIGSEGV signal. Segmentation fault!";
//            exit(1);
//        }
//        default:
//            break;
//        }

//        if (s_aboutToShutdown) {
//            return;
//        }

//        s_aboutToShutdown = true;
//        TerminalCommander::instance()->destroy();
//        TerminalCommander::instance()->quit();
//    };



//    // all these signals will be ignored.
//    for (int sig : ignoreSignals)
//        signal(sig, SIG_IGN);

//    for (int sig : quitSignals)
//        signal(sig, handler);
//}

//static void loggingCategoryFilter(QLoggingCategory *category)
//{
//    // If this is a known category
//    if (s_loggingFilters.contains(category->categoryName())) {
//        category->setEnabled(QtDebugMsg, s_loggingFilters.value(category->categoryName()));
//        category->setEnabled(QtWarningMsg, true);
//        category->setEnabled(QtCriticalMsg, true);
//        category->setEnabled(QtFatalMsg, true);
//    } else {
//        //Disable default debug messages, print only >= warnings
//        category->setEnabled(QtDebugMsg, false);
//        category->setEnabled(QtWarningMsg, true);
//        category->setEnabled(QtCriticalMsg, true);
//        category->setEnabled(QtFatalMsg, true);
//    }
//}

//static void consoleLogHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
//{
//    switch (type) {
//    case QtInfoMsg:
//        TerminalCommander::instance()->printToTerminal(QString("%1: %2\n").arg(context.category).arg(message.toUtf8().data()));
//        break;
//    case QtDebugMsg:
//        TerminalCommander::instance()->printToTerminal(QString("%1: %2\n").arg(context.category).arg(message.toUtf8().data()));
//        break;
//    case QtWarningMsg:
//        TerminalCommander::instance()->printToTerminal(QString("%1%2: %3%4\n").arg(terminalColorYellow).arg(context.category).arg(message.toUtf8().data()).arg(terminalColorNormal));
//        break;
//    case QtCriticalMsg:
//        TerminalCommander::instance()->printToTerminal(QString("%1%2: %3%4\n").arg(terminalColorRed).arg(context.category).arg(message.toUtf8().data()).arg(terminalColorNormal));
//        break;
//    case QtFatalMsg:
//        TerminalCommander::instance()->printToTerminal(QString("%1%2: %3%4\n").arg(terminalColorRed).arg(context.category).arg(message.toUtf8().data()).arg(terminalColorNormal));
//        break;
//    }
//}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(consoleLogHandler);

    QCoreApplication application(argc, argv);

    //catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP, SIGSEGV});

    application.setOrganizationName("nymea");
    application.setApplicationName("zigbee-cli");

    // Command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QString("\nCommand line tool for the zigbee NXP uart control bridge.\n\nCopyright %1 2018 Simon Stürz <simon.stuerz@guh.io>\nAll rights reserved.").arg(QChar(0xA9)));

    // Debug level
    QCommandLineOption debugLevelOption(QStringList() << "d" << "debug-level", "Set debug level [1-4]. Default 1.");
    debugLevelOption.setDefaultValue("1");
    debugLevelOption.setValueName("level");
    parser.addOption(debugLevelOption);

    // Debug level
    QCommandLineOption serialOption(QStringList() << "s" << "serial-port", "Set the serial port for the NXP controller. Default '/dev/ttyUSB0'.");
    serialOption.setDefaultValue("/dev/ttyUSB0");
    serialOption.setValueName("port");
    parser.addOption(serialOption);

    QCommandLineOption baudOption(QStringList() << "b" << "baudrate", "Set the serial port baud rate for the NXP controller. Default '115200'.");
    baudOption.setDefaultValue("115200");
    baudOption.setValueName("baudrate");
    parser.addOption(baudOption);

    // Channel
    QCommandLineOption channelOption(QStringList() << "c" << "channel", "Set channel for the zigbee network. Channel between [11-26] are allowed. If not specified, the quitest channel will be choosen automatically.");
    channelOption.setDefaultValue("0");
    channelOption.setValueName("channel");
    parser.addOption(channelOption);

    parser.process(application);

    // Check debug level
    bool debugLevelValueOk = false;
    int debugLevel = parser.value(debugLevelOption).toInt(&debugLevelValueOk);
    if (debugLevel < 1 || debugLevel > 4 || !debugLevelValueOk) {
        qCritical() << "Invalid debug level passed:" << parser.value(debugLevelOption) << "Reset to default debug level 1.";
        debugLevel = 1;
    }

    //    s_loggingFilters.insert("Application", true);
    //    s_loggingFilters.insert("Zigbee", true);
    //    s_loggingFilters.insert("ZigbeeController", (debugLevel > 1));
    //    s_loggingFilters.insert("ZigbeeInterface", (debugLevel > 2));
    //    s_loggingFilters.insert("ZigbeeInterfaceTraffic", (debugLevel > 3));

    //QLoggingCategory::installFilter(loggingCategoryFilter);

    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "Zigbee.debug=true\n"
                                     "ZigbeeController.debug=true\n"
                                     "ZigbeeInterface.debug=false\n"
                                     "ZigbeeInterfaceTraffic.debug=false\n"
                                     );

    // Check channel
    bool channelValueOk = false;
    int channel = parser.value(channelOption).toInt(&channelValueOk);
    if (channel != 0) {
        if (channel < 11 || channel > 26 || !channelValueOk) {
            qCritical() << "Invalid channel value passed:" << parser.value(channelOption) << "Selecting automatically quitest channel.";
            channel = 0;
        }
    }

    // Check baud rate value
    bool baudrateValueOk = false;
    int baudrate = parser.value(baudOption).toInt(&baudrateValueOk);
    if (!debugLevelValueOk) {
        qCritical() << "Invalid baud rate passed:" << parser.value(baudOption) << "Reset to default baudrate 115200.";
        baudrate = 115200;
    }

    ZigbeeNetwork *network = ZigbeeNetworkManager::createZigbeeNetwork(ZigbeeNetworkManager::BackendTypeNxp);
    network->setSerialPortName(parser.value(serialOption));
    network->setSerialBaudrate(baudrate);
    network->setSettingsFileName("/tmp/zigbee.conf");
    network->setExtendedPanId(5);
    ZigbeeChannelMask mask;
    mask.setChannel(Zigbee::ZigbeeChannel13);
    network->setChannelMask(mask);

    ZigbeeSecurityConfiguration securityConfiguration;
    securityConfiguration.setNetworkKey(ZigbeeNetworkKey(QString("2a:59:f4:11:75:bb:64:48:55:c5:23:62:b0:33:ea:33")));
    network->setSecurityConfiguration(securityConfiguration);

    network->startNetwork();

    return application.exec();
}
