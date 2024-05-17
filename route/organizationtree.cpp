#include "organizationtree.h"

QTreeWidgetItem * OrganizationTree::activeItem = nullptr;

OrganizationTree::OrganizationTree(QWidget *parent) :
    QTreeWidget(parent) {

    setUpConfig();
    setUpTree();
}

/* 鼠标按下事件 */
void OrganizationTree::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        QTreeWidget::mousePressEvent(ev);
        this->activeItem = itemAt(ev->pos());
        emit leftButtonClicked(ev->pos());
    }

    if (ev->button() == Qt::RightButton) {
        this->activeItem = itemAt(ev->pos());
        //emit rightButtonClicked(ev->pos());
		onCustomContextMenuRequested(ev->pos());
    }
}

/* 鼠标释放事件 */
void OrganizationTree::mouseReleaseEvent(QMouseEvent *ev) {
    this->activeItem = nullptr;
    QTreeWidget::mouseReleaseEvent(ev);
}

/* 设置信号槽和配置 */
void OrganizationTree::setUpConfig() {
    connect(this, &OrganizationTree::leftButtonClicked, this, [this](){
        qDebug() << "Tree内按下左键-- " << (activeItem?activeItem->text(0):"null");
    });

    connect(this, &OrganizationTree::rightButtonClicked, this, [this](){
        qDebug() << "Tree内按下右键-- " << (activeItem?activeItem->text(0):"null");
    });

    // 设置上下文菜单选项 --- 用于实现右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, &OrganizationTree::rightButtonClicked,
    //        this, &OrganizationTree::onCustomContextMenuRequested);
}

/* 建立组织机构目录树 */
void OrganizationTree::setUpTree() {
    clear();

    this->setHeaderLabels(QStringList("单位/部门"));

    static int upperLevel = 1;
    stack<QTreeWidgetItem *> stk;

    function<int(QString)> getLevel = [&](QString line)->int {
        int n = line.length(), level = 0;
        for(; level < n && line.at(level) == '-'; level ++);
        return level;
    };

    QFile file("orgnization.txt");
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

        if(stk.empty()) addTopLevelItem(item);
        else stk.top()->addChild(item);

        stk.push(item);
        upperLevel = curLevel;
    }
}

/* 右键弹出菜单 */
void OrganizationTree::onCustomContextMenuRequested(const QPoint &pos) {
    if(!activeItem) return;

    QMenu *pMenu = new QMenu(this);
    QAction *moreMsg = new QAction(tr("详细信息"), this);
    QAction *addNode = new QAction(tr("添加结点"), this);

    /* 添加菜单项 */
    pMenu->addAction(moreMsg);
    pMenu->addAction(addNode);

    /* 连接菜单信号槽 */
    connect(moreMsg, &QAction::triggered, this, &OrganizationTree::onMoreMsg);
    connect(addNode, &QAction::triggered, this, &OrganizationTree::onAddNode);


    /* 在鼠标右键处显示菜单 */
    pMenu->exec(cursor().pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list) delete pAction;
    delete pMenu;
}

/* 详细信息 -- 槽函数 */
void OrganizationTree::onMoreMsg() {

    QMessageBox::information(this, "detail", (activeItem?activeItem->text(0):"null"));
}

/* 添加新结点 -- 槽函数 */
void OrganizationTree::onAddNode() {
    qDebug() << __func__;
    emit addNodeToUiArea();
}
