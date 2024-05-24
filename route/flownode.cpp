#include "flownode.h"
#include "uicontent.h"
#include <QInputDialog>

QPoint FlowNode::dragStartCursorPos(0,0);
QPoint FlowNode::dragStartWidgetPos(0,0);
FlowNode * FlowNode::dragingNode = nullptr;

FlowNode::FlowNode(QWidget *parent) :
	QWidget(parent), ui(new Ui::FlowNode) {

	ui->setupUi(this);
	setUpConfig();


}

FlowNode::FlowNode(QString name, QWidget *parent, bool format, int lid, int lseq) :
	FlowNode(parent)
{
	this->setName(name);
	int x = 50, y = 50;
	// 默认生成位置
		if(!format){
			while(this->parentWidget()->childAt(QPoint(x, y)))
			{
				x += this->width()/5;
				//y += this->height()/5;
			}
			/* TODO: 默认生成的位置可以有更好的控制 */
			move(x, y);
		}

		// 导出工作流模板时的格式化位置
		else
		{
			int gapX = 80;
			int gapY = 50;
			int formatX = x + lid * (this->width() + gapX);
			int formatY = y + lseq * (this->height() + gapY);
			move(formatX, formatY);
		}

		show();

		centerPos.setX(this->x()+ this->width()/2 );
		centerPos.setY(this->y() + this->height()/2);
}


FlowNode::FlowNode(FlowNode &&node): ui(node.ui) {
	node.ui = nullptr;
	qDebug() << "移动构造调用";
}

FlowNode::~FlowNode()
{
	delete ui;
}

void FlowNode::setUpConfig() {
	// 设置接收拖放事件
	setAcceptDrops(true);
	// 设置上下文菜单选项 --- 用于实现右键菜单
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &FlowNode::rightButtonClicked,
			this, &FlowNode::onRightButtonClicked);

	setAttribute(Qt::WA_TranslucentBackground,false);
}



void FlowNode::setDraingNode(FlowNode *node) {
	FlowNode::dragingNode = node;
}

void FlowNode::setLdap(QString ldap) {
	this->ldap = ldap;
}

QString FlowNode::getLdap() {
	return this->ldap;
}


void FlowNode::setName(QString str) {
	ui->label->setText(str);
}

QString FlowNode::getName() {
	return ui->label->text();
}

void FlowNode::setItem(QTreeWidgetItem *item) {
	this->selfItem = item;
}

QTreeWidgetItem * FlowNode::getItem() {
	return this->selfItem;
}

void FlowNode::onEditNode() {
	QString text = QInputDialog::getText(this->parentWidget(), 
		"修改节点", "请输入节点ldap", QLineEdit::Normal, getLdap());
	if (!text.isEmpty())
		setLdap(text);
	else
		QMessageBox::information(this->parentWidget(), 
		 "警告", "节点ldap不能为空");
}

void FlowNode::onDrawLine() {
	qDebug() << "结点" << ui->label->text() << "绘线";
	emit lineStart(this);
}

void FlowNode::onMoreMsg(){
	QMessageBox::information(parentWidget(), "detail", this->getName());
}

void FlowNode::onDelNode(){
	emit requestForDeleteNode(this);
}

/* 防止野指针，等待UiContent处理完本指针相关事务后，才允许删除 */
void FlowNode::onPermitDeleteNode(FlowNode *node) {
	if(this == node)
		delete(this);
}

/* 删除连线 */
void FlowNode::ondelLine() {
	QDialog *dlg = new QDialog();
	dlg->setWindowTitle(tr("删除连线"));
	dlg->resize(300,400);
	QLabel *lab = new QLabel("勾选节点以删除连线(待开发)", dlg);
	lab->resize(300,400);

	dlg->exec();
}

void FlowNode::onRightButtonClicked() {
	QMenu *pMenu = new QMenu(parentWidget());

	QAction* editNode = new QAction(tr("编辑结点"), this);
	QAction* regNode = new QAction(tr("注册结点"), this);
	QAction *drawLine = new QAction(tr("添加连线"), this);
	//QAction *moreMsg = new QAction(tr("详细信息"), this);
	QAction *delNode = new QAction(tr("删除本结点"), this);
	//QAction *delLine = new QAction(tr("删除连线"), this);

	pMenu->addAction(editNode);
	pMenu->addAction(regNode);
	pMenu->addAction(drawLine);
	//pMenu->addAction(moreMsg);
	pMenu->addAction(delNode);
	//pMenu->addAction(delLine);

	connect(editNode, &QAction::triggered, this, &FlowNode::onEditNode);
	connect(regNode, &QAction::triggered, this, [this]() {
		emit registerNode(this);
	});
	connect(drawLine, &QAction::triggered, this, &FlowNode::onDrawLine);
	//connect(moreMsg, &QAction::triggered, this, &FlowNode::onMoreMsg);
	connect(delNode, &QAction::triggered, this, &FlowNode::onDelNode);
	//connect(delLine, &QAction::triggered, this, &FlowNode::ondelLine);


	/* 在鼠标右键处显示菜单 */
	pMenu->exec(cursor().pos());

	/* 释放内存 */
	QList<QAction*> list = pMenu->actions();
	foreach (QAction* pAction, list) delete pAction;
	delete pMenu;
}

/**
  * @brief mousePressEvent 鼠标按下事件
  * @param ev 鼠标事件对象
  */
void FlowNode::mousePressEvent(QMouseEvent *ev) {

	// 左键拖动功能
	if (ev->button() == Qt::LeftButton) {

		if(UiContent::isDrawing){
		   emit lineEnd(this);

		   return;
		}


		dragStartCursorPos = ev->pos();
		dragStartWidgetPos = this->pos();

		FlowNode::setDraingNode(this);
	}

	// 右键选择绘制线段功能
	else if (ev->button() == Qt::RightButton) {
		emit rightButtonClicked();
	}
}

/**
  * @brief mouseMoveEvent 鼠标移动事件
  * @param ev 鼠标事件对象
  */
void FlowNode::mouseMoveEvent(QMouseEvent *ev) {
	//FlowNode::setDraingNode(this);
	if ((ev->pos() - dragStartCursorPos).manhattanLength()
			< QApplication::startDragDistance())
		return;

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setText("Child Wieget 拖拽操作");
	drag->setMimeData(mimeData);

	qDebug() << "拖拽操作开始";
	drag->exec();
	qDebug() << "拖拽操作开结束";
}

/**
  * @brief dragEnterEvent 拖拽进入事件
  * @param ev 拖拽事件对象
  */
void FlowNode::dragEnterEvent(QDragEnterEvent *ev) {
	static int i1 = 0;
	ev->accept();

	qDebug() << __func__ << i1++;
}

/**
  * @brief dragMoveEvent 拖拽移动事件
  * @param ev 拖拽事件对象
  */
void FlowNode::dragMoveEvent(QDragMoveEvent *ev) {
	static int i2 = 0;
	qDebug()  << "FlowNodeArea ---- " << __func__ << i2++;
}

/**
 * @brief dragLeaveEvent 拖拽离开事件
 * @param ev 拖拽事件对象
 */
void FlowNode::dragLeaveEvent(QDragLeaveEvent *ev){
	static int i3 = 0;
	qDebug() << __func__ << i3++;
}

/**
  * @brief dropEvent 放下事件
  * @param ev 放下事件对象
  */
void FlowNode::dropEvent(QDropEvent *ev) {
	QString text = ev->mimeData()->text();

	if(FlowNode::dragingNode) {
		FlowNode::dragingNode->move( ev->pos() +
					( FlowNode::dragStartWidgetPos -
					  FlowNode::dragStartCursorPos )
					);
		FlowNode::dragingNode->centerPos.setX(FlowNode::dragingNode->x()
											   + FlowNode::dragingNode->width()/2 );
		FlowNode::dragingNode->centerPos.setY(FlowNode::dragingNode->y()
											   + FlowNode::dragingNode->height()/2 );

	}

	FlowNode::dragingNode = nullptr;

	this->parentWidget()->repaint();
	this->repaint();
}
