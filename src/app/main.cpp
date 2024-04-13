#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    const auto pathTemplate = QString("%1/%2").arg(a.applicationDirPath(), "%1");

    QSettings settings(pathTemplate.arg("Grafika.ini"), QSettings::Format::IniFormat);

    while (true)
    {
        auto lang = settings.value("lang").toString();
        if (lang.isEmpty())
            lang = "en";

        QTranslator translator;
        QString translationPath = pathTemplate.arg(QString("%1.qm").arg(lang));
        if (translator.load(translationPath))
            a.installTranslator(&translator);
        else
            assert(false && "Cant load translation file");

        MainWindow w(settings);
        w.show();
        const auto result = a.exec();
        if (result != 96)
            return result;
    }
}
