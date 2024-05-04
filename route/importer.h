#ifndef IMPORTER_H
#define IMPORTER_H

#include <QObject>
#include "tools.h"

class Importer : public QObject,
                        Tools
{
    //Q_OBJECT

    const QString IMPORT_PATH {"TEMPLATE/example.xml"};

public:
    explicit Importer(QObject *parent = nullptr);
    //QDomDocument importXML(const QString& fileName);

signals:

public slots:
};

#endif // IMPORTER_H
