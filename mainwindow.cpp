#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qdebug>
#include <QString>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString hexInput = ui->plainTextEdit->toPlainText();
    QStringList hexStrings = hexInput.split(" ", QString::SkipEmptyParts);

    QList<quint8> hexValues;
    bool hasInvalidValue = false;

    for (const QString& hexString : hexStrings) {
        bool ok;
        quint8 value = hexString.toUInt(&ok, 16);
        if (ok) {
            hexValues.append(value);
        } else {
            qDebug() << "Invalid hex value: " << hexString;
            hasInvalidValue = true;
            break;
        }
    }

    if (!hasInvalidValue) {
        quint32 sum = 0;

        for (const quint8 value : hexValues) {
            sum += value;
        }

        quint32 checksum = sum & 0xFFFFFFFF;

        qDebug() << "Checksum in decimal:" << checksum;
        qDebug("Checksum in hexadecimal: 0x%08X", checksum);

        QString hexChecksum = QString("0x%1").arg(checksum, 8, 16, QChar('0'));
        ui->Chcek_sum_lable->setText("Checksum in hexadecimal: " + hexChecksum);
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select a Text File", "D:/", "Text Files (*.txt)");

    if (!filePath.isEmpty()) {
        QFile inputFile(filePath);

        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&inputFile);
            int lineCount = 0;

            while (!in.atEnd()) {
                QString line = in.readLine();
                lineCount++;

                if (line.length() >= 41) {
                    QString extractedValue = line.mid(7, 34);
                    QString formattedOutput;

                    for (int i = 0; i < extractedValue.length(); i += 2) {
                        QString byte = extractedValue.mid(i, 2);
                        bool ok;
                        int decimalValue = byte.toInt(&ok, 16);
                        if (ok) {
                            formattedOutput += "0x";
                            if (decimalValue < 16) {
                                formattedOutput += "0";
                            }
                            formattedOutput += QString::number(decimalValue, 16) + " ";
                        }
                    }

                    qDebug() << "Formatted output for value " << lineCount << ": " << formattedOutput;
                    ui->plainTextEdit->appendPlainText(formattedOutput);
                }
            }

            qDebug() << "Number of lines in the file: " << lineCount;

            inputFile.close();
        } else {
            qDebug() << "Failed to open the file.";
        }
    }
}



















