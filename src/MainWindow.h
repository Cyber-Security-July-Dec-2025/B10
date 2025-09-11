#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include "NetworkManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(const std::string& myPriv, const std::string& peerPub, quint16 listenPort, const QString& peerIP, quint16 peerPort);

private slots:
    void onConnectClicked();
    void onSendClicked();
    void appendMessage(const QString& msg);

private:
    QPushButton* btnConnect;
    QPushButton* btnSend;
    QTextEdit* messageDisplay;
    QLineEdit* inputBox;
    NetworkManager* net;
    QString peerIP;
    quint16 peerPort;
};
