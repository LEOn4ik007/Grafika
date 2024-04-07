#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    const auto pathTemplate = QString("%1/%2").arg(a.applicationDirPath(), "%1");

    QSettings settings(pathTemplate.arg("Grafika.ini"), QSettings::Format::IniFormat);
    auto lang = settings.value("lang").toString();
    if (lang.isEmpty())
        lang = "en";
    
    QTranslator translator;
    QString translationPath = pathTemplate.arg(QString("%1.qm").arg(lang));
    if (translator.load(translationPath))
        a.installTranslator(&translator);
    else
        assert(false && "Cant load translation file");
      
    MainWindow w;
    w.show();
    return a.exec();
}