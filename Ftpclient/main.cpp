#include <QCoreApplication>

#include <ftpclient.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString username, password, fileName;
    int command, fileType;
    FTPClient client;
    qDebug() << "Hello! Welcome to a primitive FTP client.";

    client.connectToServer();
    qDebug() << "Please enter your username: ";
    QTextStream user(stdin);
    username = user.readLine();
    qDebug() << "Please enter your password: ";
    QTextStream pass(stdin);
    password = pass.readLine();
    client.logIn(username, password);



    while(true)
    {
        qDebug() << "Please choose your command: ";
        qDebug() << "1. List all";
        qDebug() << "2. Download file";
        qDebug() << "3. Upload file";
        qDebug() << "4. Disconnect";
        QTextStream comm(stdin);
        command = comm.readLine().toInt();
        switch(command)
        {
            case 1:
                client.listAll();
                break;
            case 2:
            {
                qDebug() << "Please enter file name name.ext";

                QTextStream file(stdin);
                fileName = file.readLine();

                qDebug() << "Please enter file type: ";
                qDebug() << "0 for binar - jpg, mp4..";
                qDebug() << "1 for ASCII - txt..";

                QTextStream type(stdin);
                fileType = type.readLine().toInt();
                client.download(fileName, fileType);
                break;
            }
            case 3:
            {
                qDebug() << "Please enter file name name.ext";
                QTextStream file1(stdin);
                fileName = file1.readLine();

                qDebug() << "Please enter file type: ";
                qDebug() << "0 for binar - jpg, mp4..";
                qDebug() << "1 for ASCII - txt..";

                QTextStream type1(stdin);
                fileType = type1.readLine().toInt();
                client.upload(fileName, fileType);
                break;
             }
            case 4:
                client.disconnect();
                return a.exec();
            default:
                break;
        }
    }

}
