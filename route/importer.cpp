#include "importer.h"

Importer::Importer(QObject *parent) : QObject(parent)
{

}


/*QDomDocument Importer::importXML(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QDomDocument();
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return QDomDocument();
    }

    file.close();
    return doc;
}*/
