#include "pulsegenerator.h"
#include "ui_pulsegenerator.h"
#include <QDebug>
#include <wiringPi.h>
#include <wiringPiSPI.h>

PulseGenerator::PulseGenerator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PulseGenerator)
{

    ui->setupUi(this);
    currentMode = WIDTH;
    //QPalette *p = new QPalette(QPalette::Background, Qt::blue);
    QPalette PalBlack(palette());
    QPalette PalBlue(palette());
    PalBlack.setColor(QPalette::WindowText, Qt::black);
    PalBlue.setColor(QPalette::WindowText, Qt::blue);
    ui->lcdAmp->setPalette(PalBlack);
    ui->lcdPeriod->setPalette(PalBlack);
    ui->lcdWidth->setPalette(PalBlue);
    timer = new QTimer(this);
    timer->setInterval(100);
    timer->setSingleShot(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_expired()));
    timer->start(100);
    //Export gpio pins 17 and 23 as inputs with pulldown
    system("gpio export 23 in");
    system("gpio -g mode 23 in");
    system("gpio -g mode 23 down");
    system("gpio export 17 in");
    system("gpio -g mode 17 in");
    system("gpio -g mode 17 down");

    // Export GPIO pin 18 as pwm output with a period of about 10 kHz
    system("gpio export 18 out");
    system("gpio -g mode 18 pwm");
    system("gpio pwm-ms");
    system("gpio pwmc 2");

    wiringPiSetupSys();
    wiringPiSPISetup (0, 500000) ;
    opledOn=false;
    ocledOn = false;
    showFullScreen();
}

PulseGenerator::~PulseGenerator()
{
    delete ui;
}


void PulseGenerator::on_timer_expired()
{
    static int i=0;

    if (digitalRead(17)==1){
        opledOn=true;
    }
    if(digitalRead(23)==1){
        ocledOn=true;
    }
    opLed(opledOn);
    ocLed(ocledOn);
    qDebug()<<"Timer expired: "<<i++;
}

void PulseGenerator::ocLed(bool on)
{
    if(on){
        ui->OCLed->setPixmap(QPixmap(QString::fromUtf8(":/img/img/led.png")));
    } else {
        ui->OCLed->setPixmap(QPixmap(QString::fromUtf8(":/img/img/ledgrey.png")));
    }
}

void PulseGenerator::opLed(bool on)
{
    if(on){
        ui->OPLed->setPixmap(QPixmap(QString::fromUtf8(":/img/img/led.png")));
    } else {
        ui->OPLed->setPixmap(QPixmap(QString::fromUtf8(":/img/img/ledgrey.png")));
    }
}

void PulseGenerator::setPWMVal(int val)
{
    char gpiostr[256];
    sprintf(gpiostr, "gpio -g pwm 18 %i", val);
    system(gpiostr);
}

void PulseGenerator::onNumPad(int num)
{
    QLCDNumber* lcd;
    int maxVal;
    switch(currentMode)
    {
        case WIDTH:
        lcd = ui->lcdWidth;
        maxVal = 999;
        break;
        case PERIOD:
        lcd = ui->lcdPeriod;
        maxVal = 999;
        break;
        case AMPLITUDE:
        lcd = ui->lcdAmp;
        maxVal = 99;
        break;
    }
    int val = lcd->intValue();
    val = val * 10;
    if (num==-1){
        val = 0;
    } else {
        val = val + num;
    }
    if(val<=maxVal){
        lcd->display(val);
    } else {
        qDebug()<<"Maximum value reached...";
    }

    unsigned char spiData[2];
    spiData[0]=(unsigned char)(val>>8);
    spiData[1]=(unsigned char)(val);
    wiringPiSPIDataRW (0, spiData, 2) ;
    setPWMVal(val);
    

}

void PulseGenerator::on_buttonWidth_clicked()
{
    QPalette PalBlack(palette());
    QPalette PalBlue(palette());
    PalBlack.setColor(QPalette::WindowText, Qt::black);
    PalBlue.setColor(QPalette::WindowText, Qt::blue);
    ui->lcdAmp->setPalette(PalBlack);
    ui->lcdPeriod->setPalette(PalBlack);
    ui->lcdWidth->setPalette(PalBlue);

    currentMode = WIDTH;
}

void PulseGenerator::on_buttonPeriod_clicked()
{
    QPalette PalBlack(palette());
    QPalette PalBlue(palette());
    PalBlack.setColor(QPalette::WindowText, Qt::black);
    PalBlue.setColor(QPalette::WindowText, Qt::blue);
    ui->lcdAmp->setPalette(PalBlack);
    ui->lcdPeriod->setPalette(PalBlue);
    ui->lcdWidth->setPalette(PalBlack);

    currentMode = PERIOD;
}

void PulseGenerator::on_buttonAmp_clicked()
{
    QPalette PalBlack(palette());
    QPalette PalBlue(palette());
    PalBlack.setColor(QPalette::WindowText, Qt::black);
    PalBlue.setColor(QPalette::WindowText, Qt::blue);
    ui->lcdAmp->setPalette(PalBlue);
    ui->lcdPeriod->setPalette(PalBlack);
    ui->lcdWidth->setPalette(PalBlack);

    currentMode = AMPLITUDE;
}

void PulseGenerator::on_numPad1_clicked()
{
    onNumPad(1);
}

void PulseGenerator::on_numPad2_clicked()
{
    onNumPad(2);
}

void PulseGenerator::on_numPad3_clicked()
{
    onNumPad(3);
}

void PulseGenerator::on_numPad4_clicked()
{
    onNumPad(4);
}


void PulseGenerator::on_numPad5_clicked()
{
    onNumPad(5);
}

void PulseGenerator::on_numPad6_clicked()
{
    onNumPad(6);
}

void PulseGenerator::on_numPad7_clicked()
{
    onNumPad(7);
}

void PulseGenerator::on_numPad8_clicked()
{
    onNumPad(8);
}

void PulseGenerator::on_numPad9_clicked()
{
    onNumPad(9);
}

void PulseGenerator::on_gnumPad0_clicked()
{
    onNumPad(0);
}

void PulseGenerator::on_numPadAC_clicked()
{
     onNumPad(-1);
}

void PulseGenerator::on_numPadus_clicked()
{
    switch(currentMode)
    {
    case WIDTH:
        ui->labelWidth->setText("us");
        break;
    case PERIOD:
        ui->labelPeriod->setText("us");
        break;
    case AMPLITUDE:

        break;

    }
}

void PulseGenerator::on_numPadns_clicked()
{
    switch(currentMode)
    {
    case WIDTH:
        ui->labelWidth->setText("ns");
        break;
    case PERIOD:
        ui->labelPeriod->setText("ns");
        break;
    case AMPLITUDE:
        break;
    }
}

void PulseGenerator::on_Resetled_clicked()
{
    ocledOn=false;
    opledOn=false;
}
