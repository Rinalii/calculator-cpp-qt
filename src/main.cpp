#include "calculatorview.h"
#include "calculatormodel.h"
#include "calculatorcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalculatorView calc;
    CalculatorModel model;
    CalculatorController controller(&model, &calc);
    calc.show();

    return a.exec();
}
