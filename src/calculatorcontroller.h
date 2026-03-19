#ifndef CALCULATORCONTROLLER_H
#define CALCULATORCONTROLLER_H

#include <QObject>

#include "calculatorview.h"
#include "calculatormodel.h"

class CalculatorController : public QObject {
    Q_OBJECT
public:
    CalculatorController(CalculatorModel *model, CalculatorView *view, QObject *parent = nullptr)
        : QObject(parent), m_model(model), m_view(view)
    {
        connect(m_view, &CalculatorView::buttonPressed,
                this, &CalculatorController::handleButtonPress);
    }
private slots:

    void handleButtonPress(const QString &text) {
        CalculatorModel::Display result;
        std::string text_str = text.toStdString();
        if(text == "+/-" || text == "%") {
            result = m_model->ProcessUOperator(text_str);
        } else if(text_str.find_first_of("+-/*") != std::string::npos) {
            result = m_model->ProcessBOperator(text_str);
        } else if(text_str == "=") {
            result = m_model->ProcessEquality(text_str);
        } else if(text_str == "AC") {
            result = m_model->ProcessAC(text_str);
        } else if(text_str.find_first_of("0123456789.") != std::string::npos) {
            result = m_model->ProcessNumberOrDot(text_str);
        } else {
            result = m_model->ProcessUnknownOperator();
        }

        m_view->UpdateDisplay(QString::fromStdString(result.expression));
        m_view->UpdateActiveNumber(QString::fromStdString(result.result));
    }
private:
    CalculatorModel *m_model;
    CalculatorView *m_view;
};

#endif // CALCULATORCONTROLLER_H
