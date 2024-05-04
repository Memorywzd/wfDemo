#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>
//#include <QDomDocument> //deprecated xml
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>
#include <QString>

class Tools
{

public:
    explicit Tools();

    inline QString GetUuid() const
    {
        QUuid uuid = QUuid::createUuid();
        QString uuidString = uuid.toString(); // 转换成字符串
        return uuidString;
    }

    inline qint64 GetTimeMs() const
    {
        return QDateTime::currentMSecsSinceEpoch();
    }

    inline QString GetTimeMsStr() const
    {
        return QString::number(GetTimeMs());
    }

    inline QString GetDateTime() const
    {
        QDateTime dateTime = QDateTime::currentDateTimeUtc(); // 获取当前UTC时间
        QDateTime localDateTime = dateTime.toLocalTime(); // 转换为本地时间
        QString dateTimeStr = localDateTime.toString("yyyy-MM-dd hh:mm:ss"); // 格式化日期时间字符串
        return dateTimeStr;
    }

    /*inline QByteArray GetXmlData(const QDomDocument& doc) const
    {
        return doc.toByteArray();
    }*/


};

#endif // TOOLS_H
