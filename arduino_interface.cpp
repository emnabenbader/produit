#include "arduino_interface.h"
#include "arduinointerface.h"
#include "ui_arduinointerface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

ArduinoInterface::ArduinoInterface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ArduinoInterface)
{
    ui->setupUi(this);  // Initialisation de l'interface utilisateur

    // Configuration des labels
    countLabel = new QLabel("Products Counted: 0", this);
    priceLabel = new QLabel("Last Product Price: $0.00", this);

    countLabel->setAlignment(Qt::AlignCenter);
    priceLabel->setAlignment(Qt::AlignCenter);

    // Mise en page avec QVBoxLayout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(countLabel);
    layout->addWidget(priceLabel);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Initialisation du port série pour la communication avec Arduino
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &ArduinoInterface::readSerialData);
    serial->setPortName("COM3");  // Remplacez par le port correct de votre Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->open(QIODevice::ReadWrite);
}

ArduinoInterface::~ArduinoInterface()
{
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
}

void ArduinoInterface::readSerialData()
{
    // Lire les données envoyées par Arduino
    QByteArray data = serial->readAll();

    // Afficher les données reçues dans le terminal de débogage
    qDebug() << "Received data:" << data;

    // Supposons que Arduino envoie le nombre de produits et le prix sous forme de texte
    QString receivedString = QString::fromStdString(data.toStdString());

    // Traiter la chaîne reçue (par exemple, en utilisant une regex ou un séparateur)
    QStringList parts = receivedString.split(",");

    if (parts.size() == 2) {
        bool ok;
        int count = parts[0].toInt(&ok);
        double price = parts[1].toDouble(&ok);

        if (ok) {
            countLabel->setText("Products Counted: " + QString::number(count));
            priceLabel->setText("Last Product Price: $" + QString::number(price, 'f', 2));
        }
    }
}
