#include "uicontent.h"


QPoint UiContent::dragStartCursorPos(0,0);
bool UiContent::isDrawing = false;

UiContent::UiContent(QWidget *parent) :
    QWidget(parent), map(new NodeMap(this))
{
    setAcceptDrops(true);
    SetupNodesMapAction();
}

UiContent::~UiContent() {
    //delete(painter);
}


/* 求箭头两翼的坐标 */
void UiContent::calcVertexes( const QPointF &startPos,
                              const QPointF &endPos,
                              QPointF &wingPos1,
                              QPointF &wingPos2)
{
    static qfloat16 arrowLength = 20.0;     // 箭头长度，一般固定
    static qfloat16 arrowDegrees = 0.48;      // 箭头长度，一般固定

    // 求 y / x 的反正切值
    qfloat16 angle = qAtan2(endPos.y() - startPos.y(), endPos.x() - startPos.x()) + 3.1415926;

    // 求得箭头点 1 的坐标
    wingPos1.setX( endPos.x() - arrowLength * qCos(angle - arrowDegrees) );
    wingPos1.setY( endPos.y() - arrowLength * qSin(angle - arrowDegrees) );

    // 求得箭头点 2 的坐标
    wingPos2.setX( endPos.x() - arrowLength * qCos(angle + arrowDegrees) );
    wingPos2.setY( endPos.y() - arrowLength * qSin(angle + arrowDegrees) );
}

QPointF UiContent::calArrowPos(FlowNode* startNode, FlowNode* endNode, float scale) {
    QPoint start = startNode->centerPos;
    QPoint end = endNode->centerPos;

    if(startNode == nullptr || endNode == nullptr)
    {
        qDebug() << "nullptr detectived!!";
    }

    if(this->arrowMode == UiContent::ArrowMode::middle)
    {
        QPoint pos = start + (end - start) * 0.5;
        return pos;
    }

    else if(this->arrowMode == UiContent::ArrowMode::hover)
    {
        QPoint pos = start + (end - start) * scale; // 计算出距离起点scale比例处的点
        while(!this->childAt(pos)) {
            scale += 0.035;
            pos = start + (end - start) * scale; // 不断逼近终点
        }

        return pos;
    }
}

// 绘图事件
void UiContent::paintEvent(QPaintEvent *ev)
{
    qDebug() << __func__ << "begin";
    QPen pen(QColor(182, 125, 184));
    QPainter painter(this);

    pen.setWidth(3);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawLine(tmpLineStartPos, tmpLineEndPos);

    auto &routeMaps = map->queryAllRoutes();
    for( auto it = routeMaps.begin(); it != routeMaps.end(); it ++ )
    {
        FlowNode* startNode = (*it)->startNode;
        FlowNode* endNode   = (*it)->endNode;

        qDebug() << "painting: " << startNode->getName()
                 << " ---> " << endNode->getName()
                 << " --- " << (*it)->funcName << " : " << (*it)->funcId;

        QPointF wingPos1(0,0), wingPos2(0,0);

        QPointF arrowPos = calArrowPos(startNode, endNode);

        calcVertexes(endNode->centerPos, arrowPos, wingPos1, wingPos2);

        pen.setStyle(Qt::DotLine);
        painter.setPen(pen);
        painter.drawLine(startNode->centerPos, endNode->centerPos);

        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(arrowPos, wingPos1);
        painter.drawLine(arrowPos, wingPos2);
    }
    qDebug() << __func__ << "end";
}

void UiContent::mousePressEvent(QMouseEvent *ev) { 
    if(isDrawing){
        isDrawing = false;
        setMouseTracking(false);
        resetTmpLine(QPoint(0,0), QPoint(0,0));
        repaint();
        return;
    }

    UiContent::dragStartCursorPos = ev->globalPos();
}

void UiContent::mouseMoveEvent(QMouseEvent *ev) {
    if(!isDrawing) {
        if ((ev->pos() - UiContent::dragStartCursorPos).manhattanLength()
                < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText("TODO");
        drag->setMimeData(mimeData);
        drag->exec();
    }
    else {
        resetTmpLine(tmpLineStartPos, QPoint(ev->x(), ev->y()));
        this->repaint();
    }

}

void UiContent::dragEnterEvent(QDragEnterEvent *ev) {
    ev->accept();
}

void UiContent::dragMoveEvent(QDragMoveEvent *ev) {

}

void UiContent::dragLeaveEvent(QDragLeaveEvent *ev) {

}

void UiContent::dropEvent(QDropEvent *ev) {

    if(FlowNode::dragingNode) {
        FlowNode::dragingNode->move(ev->pos() - FlowNode::dragStartCursorPos);
        FlowNode::dragingNode->centerPos.setX(FlowNode::dragingNode->x()
                                               + FlowNode::dragingNode->width()/2 );
        FlowNode::dragingNode->centerPos.setY(FlowNode::dragingNode->y()
                                               + FlowNode::dragingNode->height()/2 );

    }

    FlowNode::dragingNode = nullptr;
    repaint();
}

void UiContent::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint clickPos = event->pos();       // 获取鼠标点击位置的全局坐标
    auto it = map->ClickAtWfLine(clickPos);
    if(it == map->nops())
    {
        qDebug() << "map->nops()";
        return;
    }

    WfLine *line = (*it);
    qDebug() << __func__ << ": line [" << line->startNode->getName() << " --- " << line->endNode->getName() << "]";

    QMenu *menu = new QMenu(this);
    QAction *functionAdd = menu->addAction("添加投递流程");
    QAction *functionCheck = menu->addAction("查看投递流程");
    QAction *functionUpdate = menu->addAction("更换投递流程");
    QAction *deleteLine = menu->addAction("删除线段");
    QAction *more = menu->addAction("...");

    QAction *selectedAction = menu->exec(cursor().pos());  // 在鼠标点击位置弹出菜单，并获取用户选择的动作
    if (selectedAction == functionAdd) {
        qDebug() << "用户选择了 添加投递流程";
        auto func = SelectProcess();
        qDebug() << __func__ << ": func Str = " << func;
        line->SetFunc(func);
    }
    else if (selectedAction == functionCheck) {
        qDebug() << "用户选择了 查看投递流程";
        QString info = line->startNode->getName() + " ---> " \
                + line->endNode->getName()                   \
                + " 的投递流程： " + (line->funcName.isEmpty()?"未设置":line->funcName);           \
        QMessageBox::information(this, "来自FlowMaster: \"查看投递流程\"", info);
    }
    else if (selectedAction == functionUpdate) {
        qDebug() << "用户选择了 更换投递流程";
        auto func = SelectProcess();
        line->SetFunc(func);
    }
    else if (selectedAction == deleteLine) {
        qDebug() << "用户选择了 删除线段";
        map->deleteRoute(line->startNode, line->endNode);
        update();
    }
    else if (selectedAction == functionUpdate) {
        qDebug() << "用户选择了 ...";
    }

}

void UiContent::resetTmpLine(QPoint startPos, QPoint endPos) {
    tmpLineStartPos = startPos;
    tmpLineEndPos = endPos;
}


void UiContent::onLineStart(FlowNode *starNode) {
    curStartNode = starNode;
    resetTmpLine(starNode->centerPos,
                 starNode->centerPos);
    this->isDrawing = true;
    setMouseTracking(true);

}

void UiContent::onLineEnd(FlowNode *endNode) {
    if(curStartNode) {
        if( this->map->insertRoute(curStartNode, endNode) == 1 ) {
            curStartNode->outDgr ++;
            endNode->inDgr ++;
        }
    }

    curStartNode = nullptr;
    this->map->showRoute__debug();
    resetTmpLine(QPoint(0,0), QPoint(0,0));

    repaint();
    this->isDrawing = false;
    setMouseTracking(false);
}

void UiContent::onRequestForDeleteNode(FlowNode *node) {

    function<void()> eraseFromBoard = [this, node](){
        for( auto it = this->nodesOnBoard.begin(); it != this->nodesOnBoard.end(); it ++ ) {
            if( (*it) == node ) {
                this->nodesOnBoard.erase(it);
                return;
            }
        }
    };

    function<void()> showBoard = [this](){
        qDebug() << " ------------ 出入度 -----------";
        for( auto it = this->nodesOnBoard.begin(); it != this->nodesOnBoard.end(); it ++ ) {
            qDebug() << (*it)->getName() <<
                        ": 出度 -- " << (*it)->outDgr <<
                        ": 入度 -- " << (*it)->inDgr;
        }
    };

    /* 查询它指向哪些节点 */
    QList<FlowNode *> endClt;
    map->queryEndOfStartNode(node, endClt);
    for( auto endNode : endClt ) endNode->inDgr --;

    /* 查询哪些节点指向它 */
    QList<FlowNode *>startClt;
    map->queryStartOfEndNode(node, startClt);
    for( auto startNode : startClt ) startNode->outDgr --;

    /* 删除该节点 */
    this->map->deleteRoute(node);
    eraseFromBoard();
    showBoard();

    repaint();
    /* 允许释放对象 */
    emit permitDeleteNode(node);
}

/* 新增结点 */
void UiContent::onAddNodeToUiContent() {
    QTreeWidgetItem * newItem = OrganizationTree::activeItem;
    FlowNode * newNode = new FlowNode(newItem->text(0), this);
    newNode->setItem(OrganizationTree::activeItem);
    this->SetUpNewNodeAction(newNode);
}

void UiContent::OnDeleteAllNodesOnBoard()
{
    int size = nodesOnBoard.size();
    qDebug() << "nodesOnBoard size = " << size;

    for( auto node : nodesOnBoard ) qDebug() << " 删除节点： "<< node->getName();

    for( auto node : nodesOnBoard )
    {
        delete(node);
    }

    nodesOnBoard.clear();
}

//void UiContent::OnRightButtonClick(QPointF pos)
//{

//}

void UiContent::SetupNodesMapAction()
{
    connect(this->map, &NodeMap::DeleteAllNodes, this, &UiContent::OnDeleteAllNodesOnBoard);
}

/* 注册绘板上的 FlowNode 相关信号槽 */
void UiContent::SetUpNewNodeAction(FlowNode *newNode)
{
    this->nodesOnBoard.append(newNode);
    connect(newNode, &FlowNode::lineStart, this, &UiContent::onLineStart);
    connect(newNode, &FlowNode::lineEnd, this, &UiContent::onLineEnd);
    connect(newNode, &FlowNode::requestForDeleteNode, this, &UiContent::onRequestForDeleteNode);
    connect(this, &UiContent::permitDeleteNode, newNode, &FlowNode::onPermitDeleteNode);
    update();
}


/* 为新节点注册目录树上的实例 -- 预留接口 */
void UiContent::RegisterNewNodeByOrgTreeItem(FlowNode *newNode)
{
    /* TODO */
}


QString UiContent::ChooseXML()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "TEMPLATE/example.xml", tr("XML Files (*.xml);;All Files (*)"));
    if (!fileName.isEmpty()) {
        qDebug() << "选择文件：" << fileName;
    }

    return fileName;
}

QString UiContent::SelectProcess(){
    QString selectedText;      // 返回值

    // 创建5个单选框和对应的下拉框
    QRadioButton *radioButton1 = new QRadioButton("跟进");
    QComboBox *comboBox1 = new QComboBox;
    comboBox1->addItem("跟进流程1-1");
    comboBox1->addItem("跟进流程1-2");
    QRadioButton *radioButton2 = new QRadioButton("申请");
    QComboBox *comboBox2 = new QComboBox;
    comboBox2->addItem("申请流程2-1");
    comboBox2->addItem("申请流程2-2");
    QRadioButton *radioButton3 = new QRadioButton("审批");
    QComboBox *comboBox3 = new QComboBox;
    comboBox3->addItem("审批流程3-1");
    comboBox3->addItem("审批流程3-2");
    QRadioButton *radioButton4 = new QRadioButton("申报");
    QComboBox *comboBox4 = new QComboBox;
    comboBox4->addItem("申报流程4-1");
    comboBox4->addItem("申报流程4-2");
    QRadioButton *radioButton5 = new QRadioButton("盖章");
    QComboBox *comboBox5 = new QComboBox;
    comboBox5->addItem("盖章流程5-1");
    comboBox5->addItem("盖章流程5-2");

    // 使用QButtonGroup管理单选框

    QButtonGroup *buttonGroup = new QButtonGroup;
    buttonGroup->addButton(radioButton1, 1);
    buttonGroup->addButton(radioButton2, 2);
    buttonGroup->addButton(radioButton3, 3);
    buttonGroup->addButton(radioButton4, 4);
    buttonGroup->addButton(radioButton5, 5);

    // 创建选择和取消按钮

    QPushButton *selectButton = new QPushButton("选择");
    QPushButton *cancelButton = new QPushButton("取消");

    // 创建垂直布局器

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(radioButton1);
    layout->addWidget(comboBox1); // 添加对应的下拉框
    comboBox1->setVisible(true); // 设置为不可见

    layout->addWidget(radioButton2);
    layout->addWidget(comboBox2);
    comboBox2->setVisible(true);

    layout->addWidget(radioButton3);
    layout->addWidget(comboBox3);
    comboBox3->setVisible(true);

    layout->addWidget(radioButton4);
    layout->addWidget(comboBox4);
    comboBox4->setVisible(true);

    layout->addWidget(radioButton5);
    layout->addWidget(comboBox5);
    comboBox5->setVisible(true);

    layout->addWidget(selectButton);
    layout->addWidget(cancelButton);

    // 创建QDialog并设置布局

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("来自FLowMaster: \"请选择投递流程\"");
    dialog->setMinimumSize(350,230);
    dialog->setLayout(layout);

    // 连接选择按钮的槽函数

    connect(selectButton, &QPushButton::clicked, [&]() {
        int checkedId = buttonGroup->checkedId();
        if (checkedId != -1) {
            QString checkedText = buttonGroup->checkedButton()->text();
            qDebug() << "用户选择了：" << checkedText;
            QComboBox *comboBox = nullptr;
            if (checkedId == 1) {
                comboBox = comboBox1;
            } else if (checkedId == 2) {
                comboBox = comboBox2;
            } else if (checkedId == 3) {
                comboBox = comboBox3;
            } else if (checkedId == 4) {
                comboBox = comboBox4;
            } else if (checkedId == 5) {
                comboBox = comboBox5;
            }
            if (comboBox != nullptr) {
                selectedText = comboBox->currentText();
                qDebug() << "下拉框中选择了：" << selectedText;
                delete(dialog);
            }
        } else {
            delete(dialog);
            qDebug() << "用户未选择任何单选框";
        }
    });

    // 显示QDialog
    dialog->exec();
    return selectedText;
}



