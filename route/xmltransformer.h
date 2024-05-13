#ifndef XMLTRANSFORMER_H
#define XMLTRANSFORMER_H

#include <QObject>
#include <QList>
//#include "nodemap.h"
//#include "uicontent.h"
//#include "tools.h"

class XMLTransformer : public QObject//,public Tools

{
    Q_OBJECT

public:
    class Listener {
    public :
        //virtual QPair<QString, int> OnXMLTransformerGetFunc(FlowNode *start, FlowNode *end)   = 0;
    };


public:
    explicit XMLTransformer(Listener* listener, QObject *parent = nullptr);
    //QDomDocument TransformUiIntoXML(NodeMap *map, QVector<FlowNode *>& sortedNodes, QString &widStr);
    //bool TransformXMLIntoUi(NodeMap *map, UiContent *uiContent, const QDomDocument &doc);
    //QPair<QString, int> GetFunc(FlowNode *start, FlowNode *end);

private:
    Listener* callBackListener {nullptr};

};

#endif // XMLTRANSFORMER_H
