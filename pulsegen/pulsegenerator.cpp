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
    //showFullScreen();
    QPalette p = ui->textCommand->palette();
    p.setColor(QPalette::Text, Qt::red);
    ui->textCommand->setPalette(p);


}

PulseGenerator::~PulseGenerator()
{
    delete ui;
}

void PulseGenerator::setPWMVal(int val)
{
    char gpiostr[256];
    sprintf(gpiostr, "gpio -g pwm 18 %i", val);
    system(gpiostr);
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

void PulseGenerator::on_numPadns_clicked()
{
    int id;
    int unit;
    int startByte = 102; //hex 66
    int endByte = 153; //hex99
    unsigned char spiData[4];
    spiData[0]=(unsigned char)(startByte);
    spiData[3]=(unsigned char)(endByte);

    switch(currentMode)
    {
    case WIDTH:
        ui->labelWidth->setText("ns");
        id = 163; //hex A3
        unit = 1; //hex 01
        spiData[1]=(unsigned char)(id);
        spiData[2]=(unsigned char)(unit);
        wiringPiSPIDataRW (0, spiData, 4) ;
        break;
    case PERIOD:
        ui->labelPeriod->setText("ns");
        id = 164; //hex A4
        unit = 1; //hex 01
        spiData[1]=(unsigned char)(id);
        spiData[2]=(unsigned char)(unit);
        wiringPiSPIDataRW (0, spiData, 4) ;
        break;
    case AMPLITUDE:
        break;
    }
}

void PulseGenerator::on_numPadus_clicked()
{
    int id;
    int unit;
    int startByte = 102; //hex 66
    int endByte = 153; //hex99
    unsigned char spiData[4];
    spiData[0]=(unsigned char)(startByte);
    spiData[3]=(unsigned char)(endByte);

    switch(currentMode)
    {
    case WIDTH:
        ui->labelWidth->setText("us");
        id = 163; //hex A3
        unit = 2; //hex 02
        spiData[1]=(unsigned char)(id);
        spiData[2]=(unsigned char)(unit);
        wiringPiSPIDataRW (0, spiData, 4) ;
        break;
    case PERIOD:
        ui->labelPeriod->setText("us");
        id = 164; //hex A4
        unit = 2; //hex 02
        spiData[1]=(unsigned char)(id);
        spiData[2]=(unsigned char)(unit);
        wiringPiSPIDataRW (0, spiData, 4) ;
        break;
    case AMPLITUDE:
        break;
    }
}

void PulseGenerator::on_numPadms_clicked()
{
    int id;
    int unit;
    int startByte = 102; //hex 66
    int endByte = 153; //hex99
    unsigned char spiData[4];
    spiData[0]=(unsigned char)(startByte);
    spiData[3]=(unsigned char)(endByte);

    switch(currentMode)
    {
    case WIDTH:
        break;
    case PERIOD:
        ui->labelPeriod->setText("ms");
        id = 164; //hex A4
        unit = 3; //hex 03
        spiData[1]=(unsigned char)(id);
        spiData[2]=(unsigned char)(unit);
        wiringPiSPIDataRW (0, spiData, 4) ;
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

void PulseGenerator::on_timer_expired()
{
    //static int i=0;

    if (digitalRead(17)==1){
        opledOn=true;
    }
    if(digitalRead(23)==1){
        ocledOn=true;
    }
    opLed(opledOn);
    ocLed(ocledOn);
    //qDebug()<<"Timer expired: "<<i++;
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

void PulseGenerator::onNumPad(int num)
{
    QLCDNumber* lcd;
//    int maxVal;
    int id;
    int startByte = 102; //hex 66
    int endByte = 153; //hex99
    unsigned char spiData[5];
    spiData[0]=(unsigned char)(startByte);
    spiData[4]=(unsigned char)(endByte);

    switch(currentMode)
    {
        case WIDTH:
        lcd = ui->lcdWidth;
        //maxVal = 999;
        break;
        case PERIOD:
        lcd = ui->lcdPeriod;
        //maxVal = 999;
        break;
        case AMPLITUDE:
        lcd = ui->lcdAmp;
        //maxVal = 99;
        break;
    }

    int val = lcd->intValue();
//    int labelWidth = lcd->intValue();
    QString currentLabel;

    switch(currentMode)
    {
        case WIDTH:
        //lcd = ui->lcdWidth;
        //int val = lcd->intValue();
//        int labelWidth;
        currentLabel = ui->labelWidth->text();
        if (num == -1 || val <= 99){
            if (num == -1){
                val = 0;
            } else {
                val = val * 10;
			    val = val + num;
            }
            if (currentLabel == "ns"){
                ui->textCommand->setText("Minimum width 20ns");
            }
            lcd->display(val);
			id = 161; //hex A1
			spiData[1]=(unsigned char)(id);
			spiData[2]=(unsigned char)(val >> 8);
			spiData[3]=(unsigned char)(val);
			wiringPiSPIDataRW (0, spiData, 5) ;
            break;
        }
        break;
        case PERIOD:
        //lcd = ui->lcdPeriod;
        //int val = lcd->intValue();
        if (num == -1 || val <= 99){
            if (num == -1){
                val = 0;
            } else {
                val = val * 10;
			    val = val + num;
            }		    
            lcd->display(val);
			id = 162; //hex A2
			spiData[1]=(unsigned char)(id);
			spiData[2]=(unsigned char)(val >> 8);
			spiData[3]=(unsigned char)(val);
			wiringPiSPIDataRW (0, spiData, 5) ;
			break;
        }
		break;
        case AMPLITUDE:
        int valPWM;
        //lcd = ui->lcdAmp;
        //int val = lcd->intValue();
        if (num == -1 || val <= 9){
            if (num == -1){
                val = 0;
            } else {
                val = val * 10;
			    val = val + num;
            }
            if (val > 70) {
                ui->textCommand->setText("Maximum amplitude 70V");
            } else if (val < 10 && val > 0) {
                ui->textCommand->setText("Minimum amplitude 10V");
            } else {
                ui->textCommand->setText("");
            }
            lcd->display(val);
            if (val == 0 || (val >= 10 && val <= 70)){
                valPWM = (1023 * val) / 70;
                setPWMVal(valPWM);
            }
			break;
		}
        break;
    }
}

