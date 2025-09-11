#include <QApplication>
#include "MainWindow.h"
#include <nlohmann/json.hpp>
#include <fstream>

int main(int argc, char* argv[]) {
    if(argc < 2) return 1; // pass config file

    std::ifstream f(argv[1]);
    nlohmann::json j; f >> j;

    QApplication app(argc, argv);
    MainWindow w(j["rsa_private_der"], j["peer_public_der"],
                 j["listen_port"], QString::fromStdString(j["peer_ip"]), j["peer_port"]);
    w.show();
    return app.exec();
}
