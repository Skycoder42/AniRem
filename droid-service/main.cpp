#include <QAndroidService>

int main(int argc, char *argv[])
{
	QAndroidService service(argc, argv);
	return service.exec();
}
