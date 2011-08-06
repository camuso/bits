#include <QtGui/QApplication>
#include "bits.h"

int	main(int argc, char	*argv[])
{
	QApplication a(argc, argv);
	bits w;
	w.show();

	return a.exec();
}
