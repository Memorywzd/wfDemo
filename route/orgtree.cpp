#include "orgtree.h"
using namespace std;

OrgTree::OrgTree() {


}

void OrgTree::generate(Ui::Widget *ui) {
    ui->treeWidget->setHeaderLabels(QStringList() << "单位/部门");

    static int upperLevel = 1;
    stack<QTreeWidgetItem *> stk;

    function<int(QString)> getLevel = [&](QString line)->int {
        int n = line.length(), level = 0;
        for(; level < n && line.at(level) == '-'; level ++);
        return level;
    };

    QFile file(":/res/orgnization/orgnization.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Qfile open failed";
    }
    QTextStream in(&file);
    QString line;

    while(!in.atEnd()) {
        in >> line;
        int curLevel = getLevel(line);
        QString nodeName = line.right(line.length() - curLevel);

        if( curLevel == upperLevel ) {
            if(!stk.empty()) stk.pop();

        } else if( curLevel < upperLevel ) {

            while( !(upperLevel < curLevel) ) {
                stk.pop();
                upperLevel --;
            }
        }

        QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << nodeName);

        if(stk.empty()) ui->treeWidget->addTopLevelItem(item);
        else stk.top()->addChild(item);

        stk.push(item);
        upperLevel = curLevel;
    }
}


/*
 * TODO：设置右键之后可以在UI区域添加结点
 * 目前暂时用单击取代做简化处理
 */
void OrgTree::onCustomContextMenuRequested(const QPoint &pos) {
    qDebug() << "菜单栏弹出";
}

