#ifndef CALCULATORCONTROLLER_H
#define CALCULATORCONTROLLER_H

#include <QObject>

#include "calculator.h"
#include "calculatormodel.h"

class CalculatorController : public QObject {
    Q_OBJECT
public:
    CalculatorController(CalculatorModel *model, Calculator *view, QObject *parent = nullptr)
        : QObject(parent), m_model(model), m_view(view)
    {
        connect(m_view, &Calculator::buttonPressed,
                this, &CalculatorController::handleButtonPress);
    }
private slots:
    void handleButtonPress(const QString &text)
    {
        CalculatorModel::Display result = m_model->ProcessInput(text.toStdString());
        m_view->UpdateDisplay(QString::fromStdString(result.expression));
        m_view->UpdateActiveNumber(QString::fromStdString(result.result));
    }
private:
    CalculatorModel *m_model;
    Calculator *m_view;
};

#endif // CALCULATORCONTROLLER_H
