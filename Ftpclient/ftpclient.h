#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QFile>


class FTPClient : public QObject
{
    Q_OBJECT
public:
    explicit FTPClient(QObject *parent = nullptr);

    void logIn(const QString &username, const QString &password);
    void connectToServer();
    void disconnect();
    void download(const QString &file, int filetype);
    void upload(const QString &file, int filetype);
    void wait();
    void listAll();
private slots:
    // This function is called when the connection has been established successfully with the server stated.
    void connectionInit();

    // This function is called if there is an error (or disruption) in the connection
    void handleError(QAbstractSocket::SocketError socketError);

    // This function is called when the data is fully arrived from the server to the client
    void dataArrived();

signals:
    void connected();

private:
    QTcpSocket *ftpSocket;
    QTcpSocket *dataSocket;
};

#endif // FTPCLIENT_H
