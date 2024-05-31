#include "components.h"
#include <QInputDialog>
#include <QMenu>

Components::Components(QWidget* parent) : QWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
}

void Components::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		bool ok;
		QString newText = QInputDialog::getText(nullptr,
			"修改属性", "请输入新的属性内容，以空格分隔l T b:", QLineEdit::Normal, getDescribe(), &ok);
		if (ok && !newText.isEmpty()) {
			QStringList list = newText.split(" ");
			if (list.size() == 3) {
				setText(list[0]);
				prop_T = list[1];
				prop_b = list[2];
				//prop_v = list[3];
			}
		}
	}
}

void Components::edit_L()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
					"修改标签", "请输入新的标签文字", QLineEdit::Normal, getText(), &ok);
	if (ok && !newText.isEmpty()) {
		setText(newText);
	}
}

void Components::edit_T()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
					"修改数据类型", "请输入新的数据类型", QLineEdit::Normal, prop_T, &ok);
	if (ok && !newText.isEmpty()) {
		prop_T = newText;
	}
}

void Components::edit_b()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
					"修改表名称", "请输入新的表名称", QLineEdit::Normal, prop_b, &ok);
	if (ok && !newText.isEmpty()) {
		prop_b = newText;
	}
}

void Components::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else if (event->button() == Qt::RightButton) {
		QMenu* pMenu = new QMenu(parentWidget());

		QAction* editLabel = new QAction(tr("编辑标签文字"), this);
		QAction* editType = new QAction(tr("编辑数据类型"), this);
		QAction* editTable = new QAction(tr("编辑表名称"), this);
		//QAction* delComp = new QAction(tr("删除组件"), this);

		pMenu->addAction(editLabel);
		pMenu->addAction(editType);
		pMenu->addAction(editTable);
		//pMenu->addAction(delComp);

		connect(editLabel, &QAction::triggered, this, &Components::edit_L);
		connect(editType, &QAction::triggered, this, &Components::edit_T);
		connect(editTable, &QAction::triggered, this, &Components::edit_b);
		//connect(delComp, &QAction::triggered, this, &[this]() { 
			//emit deleteComp;
		//});

		/* 在鼠标右键处显示菜单 */
		pMenu->exec(cursor().pos());

		/* 释放内存 */
		QList<QAction*> list = pMenu->actions();
		foreach(QAction * pAction, list) delete pAction;
		delete pMenu;
	}
}

void Components::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint newPos = event->globalPos() - offset;
		move(newPos);
	}
}

TextForm::TextForm(QWidget* parent) : Components(parent)
{
	setStyleSheet("background-color:white");

	label = new QLabel("标签文字", this);
	label->setFont(QFont("Microsoft YaHei", 10));

	lineEdit = new QLineEdit(this);
	lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	lineEdit->move(label->width(), 0);
}

void TextForm::setText(const QString& text)
{
	label->setText(text);
}

QString TextForm::getText() const
{
	return label->text();
}

QString TextForm::getDescribe() const
{
	return label->text() + " " + prop_T + " " + prop_b;
}

AreaForm::AreaForm(QWidget* parent) : Components(parent)
{
	setStyleSheet("background-color:white");
	
	label = new QLabel("标签文字", this);
	label->setFont(QFont("Microsoft YaHei", 10));

	textEdit = new QTextEdit(this);
	textEdit->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	textEdit->move(label->width(), 0);
}

void AreaForm::setText(const QString& text)
{
	label->setText(text);
}

QString AreaForm::getText() const
{
	return label->text();
}

QString AreaForm::getDescribe() const
{
	return label->text() + " " + prop_T + " " + prop_b;
}

ComboBoxForm::ComboBoxForm(QWidget* parent) : Components(parent)
{	
	label = new QLabel("标签文字", this);
	label->setFont(QFont("Microsoft YaHei", 10));
	label->setStyleSheet("background-color:white");

	comboBox = new QComboBox(this);
	//comboBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	comboBox->move(label->width(), 0);
}

QString ComboBoxForm::getItems()
{
	//to string
	QString items_str;
	for (int i = 0; i < items.size(); i++) {
		items_str += items[i];
		if (i != items.size() - 1) items_str += " ";
	}
	return items_str;
}

void ComboBoxForm::setItems()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
					"修改菜单项", "请输入菜单项目，空格分隔多个项目", QLineEdit::Normal, getItems(), &ok);
	if (ok && !newText.isEmpty()) {
		items = newText.split(" ");
	}
	
	comboBox->clear();
	comboBox->addItems(items);
}

void ComboBoxForm::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else if (event->button() == Qt::RightButton) {
		QMenu* pMenu = new QMenu(parentWidget());

		QAction* editLabel = new QAction(tr("编辑标签文字"), this);
		QAction* editType = new QAction(tr("编辑数据类型"), this);
		QAction* editTable = new QAction(tr("编辑表名称"), this);
		QAction* editOption = new QAction(tr("编辑选项"), this);
		//QAction* delComp = new QAction(tr("删除组件"), this);

		pMenu->addAction(editLabel);
		pMenu->addAction(editType);
		pMenu->addAction(editTable);
		pMenu->addAction(editOption);
		//pMenu->addAction(delComp);

		connect(editLabel, &QAction::triggered, this, &Components::edit_L);
		connect(editType, &QAction::triggered, this, &Components::edit_T);
		connect(editTable, &QAction::triggered, this, &Components::edit_b);
		connect(editOption, &QAction::triggered, this, &ComboBoxForm::setItems);
		//connect(delComp, &QAction::triggered, this, &[this]() { 
			//emit deleteComp;
		//});

		/* 在鼠标右键处显示菜单 */
		pMenu->exec(cursor().pos());

		/* 释放内存 */
		QList<QAction*> list = pMenu->actions();
		foreach(QAction * pAction, list) delete pAction;
		delete pMenu;
	}
}

void ComboBoxForm::setText(const QString& text)
{
	label->setText(text);
}

QString ComboBoxForm::getText() const
{
	return label->text();
}

QString ComboBoxForm::getDescribe() const
{
	return label->text() + " " + prop_T + " " + prop_b;
}

CheckBoxForm::CheckBoxForm(QWidget* parent) : Components(parent)
{
	setStyleSheet("background-color:white");
	
	label = new QLabel("标签文字", this);
	label->setFont(QFont("Microsoft YaHei", 10));

	checkBox = new QCheckBox(this);
	checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	checkBox->move(label->width(), 0);
	checkBox->setFont(QFont("Microsoft YaHei", 10));
	checkBox->setText("选项");
}

void CheckBoxForm::setItems()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
							"修改选项", "请输入选项，空格分隔多个选项", QLineEdit::Normal, checkBox->text(), &ok);
	if (ok && !newText.isEmpty()) {
		checkBox->setText(newText);
	}
}

void CheckBoxForm::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else if (event->button() == Qt::RightButton) {
		QMenu* pMenu = new QMenu(parentWidget());

		QAction* editLabel = new QAction(tr("编辑标签文字"), this);
		QAction* editType = new QAction(tr("编辑数据类型"), this);
		QAction* editTable = new QAction(tr("编辑表名称"), this);
		QAction* editOption = new QAction(tr("编辑选项"), this);
		//QAction* delComp = new QAction(tr("删除组件"), this);

		pMenu->addAction(editLabel);
		pMenu->addAction(editType);
		pMenu->addAction(editTable);
		pMenu->addAction(editOption);
		//pMenu->addAction(delComp);

		connect(editLabel, &QAction::triggered, this, &Components::edit_L);
		connect(editType, &QAction::triggered, this, &Components::edit_T);
		connect(editTable, &QAction::triggered, this, &Components::edit_b);
		connect(editOption, &QAction::triggered, this, &CheckBoxForm::setItems);
		//connect(delComp, &QAction::triggered, this, &[this]() { 
			//emit deleteComp;
		//});

		/* 在鼠标右键处显示菜单 */
		pMenu->exec(cursor().pos());

		/* 释放内存 */
		QList<QAction*> list = pMenu->actions();
		foreach(QAction * pAction, list) delete pAction;
		delete pMenu;
	}
}

void CheckBoxForm::setText(const QString& text)
{
	label->setText(text);
}

QString CheckBoxForm::getText() const
{
	return label->text();
}

QString CheckBoxForm::getDescribe() const
{
	return label->text() + " " + prop_T + " " + prop_b;
}

RadioForm::RadioForm(QWidget* parent) : Components(parent)
{
	setStyleSheet("background-color:white");
	
	label = new QLabel("标签文字", this);
	label->setFont(QFont("Microsoft YaHei", 10));

	radioButton = new QRadioButton(this);
	radioButton->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	radioButton->move(label->width(), 0);
	radioButton->setFont(QFont("Microsoft YaHei", 10));
	radioButton->setText("选项");
}

void RadioForm::setItems()
{
	bool ok;
	QString newText = QInputDialog::getText(nullptr,
							"修改选项", "请输入选项，空格分隔多个选项", QLineEdit::Normal, radioButton->text(), &ok);
	if (ok && !newText.isEmpty()) {
		radioButton->setText(newText);
	}
}

void RadioForm::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else if (event->button() == Qt::RightButton) {
		QMenu* pMenu = new QMenu(parentWidget());

		QAction* editLabel = new QAction(tr("编辑标签文字"), this);
		QAction* editType = new QAction(tr("编辑数据类型"), this);
		QAction* editTable = new QAction(tr("编辑表名称"), this);
		QAction* editOption = new QAction(tr("编辑选项"), this);
		//QAction* delComp = new QAction(tr("删除组件"), this);

		pMenu->addAction(editLabel);
		pMenu->addAction(editType);
		pMenu->addAction(editTable);
		pMenu->addAction(editOption);
		//pMenu->addAction(delComp);

		connect(editLabel, &QAction::triggered, this, &Components::edit_L);
		connect(editType, &QAction::triggered, this, &Components::edit_T);
		connect(editTable, &QAction::triggered, this, &Components::edit_b);
		connect(editOption, &QAction::triggered, this, &RadioForm::setItems);
		//connect(delComp, &QAction::triggered, this, &[this]() { 
			//emit deleteComp;
		//});

		/* 在鼠标右键处显示菜单 */
		pMenu->exec(cursor().pos());

		/* 释放内存 */
		QList<QAction*> list = pMenu->actions();
		foreach(QAction * pAction, list) delete pAction;
		delete pMenu;
	}
}

void RadioForm::setText(const QString& text)
{
	label->setText(text);
}

QString RadioForm::getText() const
{
	return label->text();
}

QString RadioForm::getDescribe() const
{
	return label->text() + " " + prop_T + " " + prop_b;
}

ButtonForm::ButtonForm(QWidget* parent) : Components(parent)
{
	button = new QPushButton("Button", this);
	button->setFont(QFont("Microsoft YaHei", 10));
	button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	prop_T = "11";
}

void ButtonForm::setText(const QString& text)
{
	button->setText(text);
}

QString ButtonForm::getText() const
{
	return button->text();
}

QString ButtonForm::getDescribe() const
{
	return button->text();
}

void ButtonForm::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		bool ok;
		QString newText = QInputDialog::getText(parentWidget(),
			"修改描述文字", "请输入新的描述文字:", QLineEdit::Normal, getText(), &ok);
		if (ok && !newText.isEmpty())
			setText(newText);
	}
}
