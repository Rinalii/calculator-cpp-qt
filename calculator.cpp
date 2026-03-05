#include "calculator.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#define NUMROWS 4
#define NUMCOLS 4

Calculator::Calculator() {
    SetupUI();
}

void Calculator::UpdateDisplay(const QString &text) {
    display->setText(text);
}

void Calculator::AddToDisplay(const QString &text) {
    QString curr_text = display->text();
    UpdateDisplay(curr_text+text);
}

void Calculator::ClearDisplay() {
    display->setText("0");
}

void Calculator::UpdateActiveNumber(const QString& text) {
    active_number->setText(text);
}
void Calculator::AddToActiveNumber(const QString& text) {
    QString curr_text = active_number->text();
    UpdateActiveNumber(curr_text+text);
}
void Calculator::ClearActiveNumber() {
    active_number->setText("0");
}

void Calculator::SetupUI()
{
    QGridLayout* layout = new QGridLayout();

    SetupDisplay(layout);
    SetupActiveNumber(layout);
    SetupButtons(layout);

    setLayout(layout);
}

void Calculator::SetupDisplay(QGridLayout* layout) {
    // Create display
    display = new QLabel("");
    display->setMinimumSize(241, 31);
    display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QFont display_font("Arial", 12, QFont::Bold);
    display->setFont(display_font);
    display->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   background-color: #262626;"
        "   border-bottom: 1px solid gray;"
        "}"
        );
    display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(display, 0, 0, 1, 4);
}
void Calculator::SetupActiveNumber(QGridLayout* layout) {
    // Create display
    active_number = new QLabel("0");
    active_number->setMinimumSize(241, 61);
    active_number->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QFont display_font("Arial", 24, QFont::Bold);
    active_number->setFont(display_font);
    active_number->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   background-color: #262626;"
        "   border-bottom: 1px solid gray;"
        "}"
        );
    active_number->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(active_number, 1, 0, 1, 4);
}

void Calculator::SetupButtons(QGridLayout* layout) {
    QString buttons_str[NUMROWS][NUMCOLS] = {
        { "AC", "+/-", "%", "/" },
        { "7", "8", "9", "*" },
        { "4", "5", "6", "-" },
        { "1", "2", "3", "+" }
    };

    // Create digit and operator buttons
    for (int i = 0; i < NUMROWS; ++i) {
        for (int j = 0; j < NUMCOLS; ++j) {
            QPushButton* button = CreateButton(buttons_str[i][j]);
            layout->addWidget(button, i + 2, j);
        }
    }

    // Create bottom row buttons
    QStringList bottomButtons = { "0", ".", "=" };
    layout->addWidget(CreateButton(bottomButtons[0]), 6, 0, 1, 2);
    layout->addWidget(CreateButton(bottomButtons[1]), 6, 2);
    layout->addWidget(CreateButton(bottomButtons[2]), 6, 3);
}

QPushButton* Calculator::CreateButton(const QString& str) {
    QPushButton* button = new QPushButton(str);
    button->setMinimumSize(60, 60);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    button->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #262626;"
        "   background-color: #363636;"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #454545;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #00b899;"
        "}"
        );
    connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    //connect(button, &QPushButton::clicked, this, [this, button]() {
    //    onButtonClicked(button->text());
    //});
    return button;
}



