#ifndef CALCULATORMODEL_H
#define CALCULATORMODEL_H

#include <QDebug>
#include <optional>

class CalculatorModel
{
private:
    enum class BOperator { Add, Subtract, Multiply, Divide, None};
    enum class UOperator { ChangeSign, Percent};

    std::string prev_num_;
    std::string active_num_;
    BOperator operator_ = BOperator::None;
    bool is_dot = false;

public:
    CalculatorModel();

    struct Display{
        std::string expression;
        std::string result;
    };

    Display ProcessInput(const std::string& input);

private:

    Display ProcessUnknownOperator() const;
    Display ProcessBOperator(const std::string& input);
    Display ProcessUOperator(const std::string& input);
    Display ProcessEquality(const std::string& input);
    Display ProcessNumberOrDot(const std::string& input);
    Display ProcessAC(const std::string& input);

    void ActiveNumberToPrev(BOperator b_op = BOperator::None);
    void Clear();
    void SetPrevNum(const std::string& str);
    bool IsSignInMiddle() const;
    bool IsActiveNumberEmpty() const;

    double ApplyOperator(double left, double right, BOperator op) const;
    double ApplyOperator(double number, UOperator op) const;

    std::optional<BOperator> StringToBOperator(const std::string& input) const;
    std::optional<UOperator> StringToUOperator(const std::string& input) const;
    std::string BOperatorToString() const;

    static std::string DoubleToString(double value);
    static double StringToDouble(const std::string& str);

};

#endif // CALCULATORMODEL_H
