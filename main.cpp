#include "wfDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	wfDemo w;
	w.show();
	return a.exec();
}
