#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class Calculator: public QWidget
{
    Q_OBJECT
public:
    Calculator();
    void UpdateDisplay(const QString& text);
    void AddToDisplay(const QString& text);
    void ClearDisplay();

    void UpdateActiveNumber(const QString& text);
    void AddToActiveNumber(const QString& text);
    void ClearActiveNumber();

private:
    QLabel* display;
    QLabel* active_number;

    void SetupUI();

    void SetupDisplay(QVBoxLayout* v_layout);
    void SetupActiveNumber(QVBoxLayout* v_layout);
    void SetupButtons(QGridLayout* layout);

    QPushButton* CreateButton(const QString& str);

signals:
    void buttonPressed(const QString &text); // Сигнал для контроллера

public slots:

    void onButtonClicked() {
        // Определяем, какая кнопка была нажата
        QPushButton *button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString text = button->text();
            // Излучаем сигнал для контроллера
            emit buttonPressed(text);
        }
    }
};

#endif // CALCULATOR_H
