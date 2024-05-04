#ifndef OUTPUTTER_H
#define OUTPUTTER_H

#include "tools.h"
#include "QObject"

class Outputter : public QObject,
                         Tools
{

    static const bool SAVE_TO_DATABASE {false};
    static const bool SAVE_TO_FILE {true};

public:
    Outputter(QObject *parent = nullptr);

public:

    /*bool SaveXML(const QDomDocument& doc, const QString& wid)
    {
        bool ret {false};
        if(SAVE_TO_DATABASE && SaveXMLToBase(GetXmlData(doc), wid))
        {
            ret |= true;
        }

        if(SAVE_TO_FILE && SaveXMLToFile(doc, wid))
        {
            ret |= true;
        }

        return ret;
    }

    bool SaveXMLToFile(const QDomDocument &doc, const QString &wid)
    {
        qDebug() << "saving file" << endl;
        QString savePath = "XML_OUTPUT/" + wid + ".xml";
        QFile file(savePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open file for writing.";
            return false;
        }

        else
        {
            QTextStream out(&file);
            doc.save(out, 4);
            file.close();
            qDebug() << "File saved successfully.";
            return true;
        }
    }

    bool SaveXMLToBase(const QByteArray &xmlStr, const QString &wid)
    {

    }*/
};

#endif // OUTPUTTER_H
