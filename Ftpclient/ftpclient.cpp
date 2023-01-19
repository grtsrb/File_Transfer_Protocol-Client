#include "ftpclient.h"


FTPClient::FTPClient(QObject *parent)
    : QObject{parent}
{
    ftpSocket = new QTcpSocket(this);

    // connectionInit() to be called whenever the connection is estabished
    connect(ftpSocket, SIGNAL(connected()), this, SLOT(connectionInit()));

    // dataArrived(); is called when the data has arrived from the server
    connect(ftpSocket, SIGNAL(readyRead()), this, SLOT(dataArrived()));

    // handleError( ); is called if an error occurs when connecting to the server
    connect(ftpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
}

void FTPClient::connectToServer()
{
    // ftpserver3lite port -> 21
    ftpSocket->connectToHost(QHostAddress::LocalHost, 21);
    qDebug() << "Connected to server";
}


void FTPClient::logIn(const QString &myusername, const QString &password)
{
    // FTP command for username USER + myusername
    QByteArray username = "USER " + myusername.toLocal8Bit() + "\r\n";
    ftpSocket->write(username);
    wait();

    QByteArray userpassword = "PASS " + password.toLocal8Bit() + "\r\n";
    ftpSocket->write(userpassword);
    wait();
}
void FTPClient::download(const QString &file, int filetype)
{
    switch (filetype) {
    case 0:
        ftpSocket->write("TYPE I\r\n");
        wait();
        break;
    case 1:
        ftpSocket->write("TYPE A\r\n");
        wait();
        break;
    default:
        break;
    }
    // Set up new socket for data
    ftpSocket->write("PASV\r\n");
    wait();
    dataSocket = new QTcpSocket(this);
    dataSocket->connectToHost(QHostAddress::LocalHost, 1024);
    if(!dataSocket->waitForConnected())
    {
        qDebug() << "Error. Failed to connect to data port.";
        return;
    }

    QByteArray retr = "RETR " + file.toLocal8Bit() + "\r\n";
    ftpSocket->write(retr);
    wait();

    QString file_name = "C:/Users/milic/Desktop/Client/" + file;
    QFile fileDownload(file_name);
    if(!fileDownload.open(QIODevice::ReadWrite))
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    while(dataSocket->bytesAvailable() == 0)
    {
        if(!dataSocket->waitForBytesWritten())
        {
            qDebug() << "Error: Failed to download file.";
            return;
        }
    }

    fileDownload.write(dataSocket->readAll());
    fileDownload.close();

    dataSocket->disconnectFromHost();
    wait();

    qDebug() << "File downloaded successfully!";
}

void FTPClient::upload(const QString &file, int filetype)
{
    switch (filetype) {
    case 0:
        ftpSocket->write("TYPE I\r\n");
        wait();
        break;
    case 1:
        ftpSocket->write("TYPE A\r\n");
        wait();
        break;
    default:
        break;
    }
    // Set up new socket for data and send PASV
    ftpSocket->write("PASV\r\n");
    wait();
    dataSocket = new QTcpSocket(this);
    dataSocket->connectToHost(QHostAddress::LocalHost, 1024);
    if(!dataSocket->waitForConnected())
    {
        qDebug() << "Error. Failed to connect to data port.";
        return;
    }
    QString file_name = "C:/Users/milic/Desktop/Client/" + file;
    QByteArray stor = "STOR " + file.toLocal8Bit() + "\r\n";
    ftpSocket->write(stor);
    wait();

    QFile fileUpload(file_name);
    if(!fileUpload.open(QIODevice::ReadWrite))
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    dataSocket->write(fileUpload.readAll());
    wait();

    fileUpload.close();

    dataSocket->disconnectFromHost();
    wait();

    qDebug() << "File uploaded successfully!";
}
void FTPClient::listAll()
{
    // Set up new socket for data
    ftpSocket->write("PASV\r\n");
    wait();
    dataSocket = new QTcpSocket(this);
    dataSocket->connectToHost(QHostAddress::LocalHost, 1024);
    if(!dataSocket->waitForConnected())
    {
        qDebug() << "Error. Failed to connect to data port.";
        return;
    }
    QByteArray list = "LIST\r\n";
    ftpSocket->write(list);
    wait();

   QByteArray fileList = dataSocket->readAll();
   qDebug() << "File list: " << fileList;

    dataSocket->disconnectFromHost();
    wait();

    qDebug() << "List updated successfully!";
}

void FTPClient::disconnect()
{
    QByteArray dis = "QUIT\r\n";
    ftpSocket->write(dis);
    wait();
}


void FTPClient::wait()
{
    QEventLoop eventLoop;
    QTimer::singleShot(1000, &eventLoop, SLOT(quit()));
    eventLoop.exec();
}


// -------------------------------------------------------------
void FTPClient::connectionInit()
{
    qDebug() << "Successfully connected";
}

void FTPClient::handleError(QAbstractSocket::SocketError socketError)
{
    qCritical() << "Error connecting to the server: " + ftpSocket->errorString();
    ftpSocket->close();
}

void FTPClient::dataArrived()
{
    QByteArray buffer;
    buffer.append(ftpSocket->readAll());
    qDebug() << buffer;
}
