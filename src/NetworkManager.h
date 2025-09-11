

#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "CryptoManager.h"

class NetworkManager : public QObject {
    Q_OBJECT
public:
    NetworkManager(const std::string& myPriv, const std::string& peerPub, QObject* parent=nullptr);
    bool startServer(quint16 port);
    void connectToPeer(const QString& ip, quint16 port);
    void sendPlaintext(const QString& text);

signals:
    void messageReceived(const QString& msg);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* server;
    QTcpSocket* incomingSocket;   // for accepted connections
    QTcpSocket* outgoingSocket;   // for initiated connections
    QByteArray recvBuffer;
    CryptoManager crypto;
};

