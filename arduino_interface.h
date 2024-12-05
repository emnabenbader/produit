#ifndef ARDUINOINTERFACE_H
#define ARDUINOINTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
    class ArduinoInterface;
}

class ArduinoInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit ArduinoInterface(QWidget *parent = nullptr);
    ~ArduinoInterface();

private:
    Ui::ArduinoInterface *ui;
    QLabel *countLabel;
    QLabel *priceLabel;

    QSerialPort *serial;  // Port série pour la communication avec Arduino
    void readSerialData();
};

#endif // ARDUINOINTERFACE_H
