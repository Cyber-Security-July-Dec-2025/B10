#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(const std::string& myPriv, const std::string& peerPub,
                       quint16 listenPort, const QString& peerIP, quint16 peerPort)
    : peerIP(peerIP), peerPort(peerPort)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    btnConnect = new QPushButton("Connect");
    btnSend = new QPushButton("Send");
    messageDisplay = new QTextEdit();
    messageDisplay->setReadOnly(true);
    inputBox = new QLineEdit();

    layout->addWidget(btnConnect);
    layout->addWidget(messageDisplay);
    layout->addWidget(inputBox);
    layout->addWidget(btnSend);

    setCentralWidget(central);

    net = new NetworkManager(myPriv, peerPub, this);
    net->startServer(listenPort);

    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(net, &NetworkManager::messageReceived, this, &MainWindow::appendMessage);
}

void MainWindow::onConnectClicked() {
    net->connectToPeer(peerIP, peerPort);
}

void MainWindow::onSendClicked() {
    QString text = inputBox->text();
    net->sendPlaintext(text);
    messageDisplay->append("You: " + text);
    inputBox->clear();
}

void MainWindow::appendMessage(const QString& msg) {
    messageDisplay->append("Peer: " + msg);
}
