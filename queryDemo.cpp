#include "queryDemo.h"

#include <QtSql>
#include <QMessageBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QLabel>

#include <QDebug>

void queryDemo::initializeModel(QSqlTableModel* model)
{
    //model->setQuery("select * from zuoyestatics");
	model->setTable("zuoyestatics");
	model->select();
}

QTableView* queryDemo::createView(QSqlTableModel* model, const QString& title)
{
    QTableView* view = new QTableView;
    view->setModel(model);

	view->setSortingEnabled(true);
	view->sortByColumn(0, Qt::AscendingOrder); //按第0列升序排列

    view->setWindowTitle(title);
    view->move(100, 100);

    //view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽

    return view;
}
//* 计数\统计、排序 实现于模板列表中
queryDemo::queryDemo(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    /*qDebug() << "available drivers:";
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
        qDebug() << driver;*/
    db.setHostName("211.71.149.249");
    db.setUserName("wf5");
    db.setPassword("3edc4rfv");
    db.setDatabaseName("workflow");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                "This example needs MySql support. Please read "
                "the Qt SQL driver documentation for information how "
                "to build it.\n\n"
                "Click Cancel to exit."), QMessageBox::Cancel);
    }

	QSqlTableModel* model = new QSqlTableModel(this, db);
	initializeModel(model);

    QTableView* view = createView(model, "查询结果");

	//输入表名
	QLineEdit* tableName = new QLineEdit(this);
	tableName->setPlaceholderText("请输入表名");
	tableName->setFixedWidth(200);
	tableName->setFixedHeight(30);
	tableName->move(100, 100);

	//统计行数并显示于输入框中
	QLabel* label = new QLabel(this);
	label->setText("统计总行数：");
	QLineEdit* lineEdit = new QLineEdit(this);
	lineEdit->setText(QString::number(model->rowCount()));
	lineEdit->setReadOnly(true);

	QPushButton* query = new QPushButton("查询", this);
	connect(query, &QPushButton::clicked, this, [this, model, view, tableName, lineEdit]() {
		model->setTable(tableName->text());
		model->select();
		view->setModel(model);
		lineEdit->setText(QString::number(model->rowCount()));
	});

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(view);
	QHBoxLayout* hlay = new QHBoxLayout(this);
	hlay->addWidget(tableName);
	hlay->addWidget(query);
	hlay->addWidget(label);
	hlay->addWidget(lineEdit);
	lay->addLayout(hlay);
	setLayout(lay);
    resize(1000, 400);
	
    show();
}

queryDemo::~queryDemo()
{}
