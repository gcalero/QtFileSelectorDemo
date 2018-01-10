#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>
#include <QFileSelector>
#include <QDir>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

// HERE THE DEMO STARTS

    QFile assetFile("assets:/Skybox.ktx");

    QFileInfo checkAssetFile(assetFile);

    qDebug() << "Skybox exists: " << (checkAssetFile.exists() && checkAssetFile.isFile());
    qDebug() << "Copying assets to cache ";

    QString cacheFilePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/Skybox.ktx";
    assetFile.copy(cacheFilePath);

    QFile cacheFile(cacheFilePath);
    QFileInfo checkCacheFile(cacheFile);
    qDebug() << "Cache file exists: " <<  (checkCacheFile.exists() && checkCacheFile.isFile());

    QFileSelector selector;
    qDebug() << "selector.select(assets:/Skybox.ktx) = " << selector.select("assets:/Skybox.ktx");
    Q_ASSERT(selector.select("assets:/Skybox.ktx") == "assets:/+android/Skybox.ktx");

    // ensure +android/Skybox.ktx does not exist yet
    if (QFileInfo(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/+android/Skybox.ktx").exists()) {
        QFile::remove(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/+android/Skybox.ktx");
    }

    qDebug() << "selector.select(" << cacheFilePath << ") = " << selector.select(cacheFilePath);
    qDebug() << "Creating +android version of file cache";

    QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/+android");
    cacheFile.copy(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/+android/Skybox.ktx");
    qDebug() << "selector.select(" << cacheFilePath << ") = " << selector.select(cacheFilePath);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
