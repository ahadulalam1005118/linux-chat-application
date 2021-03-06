#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>

#include <QIODevice>
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>

#include "../util/Packaging.h"


class Client : public QObject {
    Q_OBJECT
friend class MainWindow;

public:
	explicit Client(QObject* parent = 0);
	Client(QString serverAddr = "127.0.0.1", QString portNr = "10013", QString username = "test user");

	QString getUsername() { return username; }

	void connectToServer();
	void sendPackage(QString package);

private:
	QString serverAddr;
    QString portNr;
	QString username;
	QSharedPointer<QTcpSocket> tcpSocket;
	QSharedPointer<QDataStream> dataStream;

	Packaging packaging;

signals:
    void readyRead();
    void error();
	void login();
	void receivedMessage(QString message);
	void receivedUsersList(QString usersList);
	void receivedPrivateMessage(QString receiver, QString message, QString sender);

public slots:
	void onLoginRequest();
	void onReadMsg();
	void onLogoutRequest();
	void onGlobalPackage(QString message);
	void onSendPrivateMessage(QString receiver, QString message, QString sender);
	void onSendFile(QByteArray blob, int fileSize, QString receiver, QString sender);
};

#endif // CLIENT_H
