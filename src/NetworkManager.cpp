

#include "NetworkManager.h"
#include <QHostAddress>

NetworkManager::NetworkManager(const std::string& myPriv, const std::string& peerPub, QObject* parent)
    : QObject(parent), crypto(myPriv, peerPub), server(nullptr),
      incomingSocket(nullptr), outgoingSocket(nullptr) {}

bool NetworkManager::startServer(quint16 port) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);
    return server->listen(QHostAddress::Any, port);
}

void NetworkManager::connectToPeer(const QString& ip, quint16 port) {
    outgoingSocket = new QTcpSocket(this);
    connect(outgoingSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    outgoingSocket->connectToHost(ip, port);
}

void NetworkManager::onNewConnection() {
    incomingSocket = server->nextPendingConnection();
    connect(incomingSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
}

void NetworkManager::onReadyRead() {
    QTcpSocket* activeSocket = qobject_cast<QTcpSocket*>(sender());
    if (!activeSocket) return;

    recvBuffer.append(activeSocket->readAll());
    if (recvBuffer.size() > 0) {
        std::vector<uint8_t> v(recvBuffer.begin(), recvBuffer.end());
        try {
            emit messageReceived(QString::fromStdString(crypto.decryptPacket(v)));
        } catch (...) {
            emit messageReceived("!! Decryption failed !!");
        }
        recvBuffer.clear();
    }
}

void NetworkManager::sendPlaintext(const QString& text) {
    std::vector<uint8_t> packet = crypto.encryptMessageToPeer(text.toStdString());

    if (outgoingSocket && outgoingSocket->state() == QAbstractSocket::ConnectedState) {
        outgoingSocket->write(reinterpret_cast<const char*>(packet.data()), packet.size());
    }
    if (incomingSocket && incomingSocket->state() == QAbstractSocket::ConnectedState) {
        incomingSocket->write(reinterpret_cast<const char*>(packet.data()), packet.size());
    }
}


