#include <QCoreApplication>
#include <ftpclient.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FTPClient client;
    qDebug() << "Hello! Welcome to a primitive FTP client.";

    client.connectToServer();
    client.logIn("grtalca", "1234");
    client.listAll();
    client.download("tara.jpg", 0);
    client.upload("tara.jpg", 0);
    client.disconnect();

    return a.exec();
}
