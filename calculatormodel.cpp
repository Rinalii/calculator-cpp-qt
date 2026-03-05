#include "calculatormodel.h"

CalculatorModel::CalculatorModel() {}

void CalculatorModel::ActiveNumberToPrev(BOperator b_op) {
    prev_num_=active_num_;
    active_num_.clear();
    operator_ = b_op;
    is_dot = false;
}

void CalculatorModel::Clear() {
    prev_num_.clear();
    active_num_.clear();
    operator_ = BOperator::None;
    is_dot = false;
}

void CalculatorModel::SetPrevNum(const std::string &str) {
    prev_num_ = str;
}

bool CalculatorModel::IsSignInMiddle() const {
    return !prev_num_.empty() && !active_num_.empty();
}

bool CalculatorModel::IsActiveNumberEmpty() const {
    return active_num_.empty();
}

std::string CalculatorModel::BOperatorToString() const {
    switch(operator_) {
    case BOperator::Add: return "+";
    case BOperator::Divide: return "/";
    case BOperator::Multiply: return "*";
    case BOperator::Subtract: return "-";
    default: return "Unknown";
    }
}

double CalculatorModel::ApplyOperator(double left, double right, BOperator op) const {
    switch(op) {
    case BOperator::Add:
        return left + right;
    case BOperator::Subtract:
        return left - right;
    case BOperator::Multiply:
        return left * right;
    case BOperator::Divide:
        if (right == 0) throw std::runtime_error("Division by zero");
        return left / right;
    default: throw std::runtime_error("Unknown operator");
    }
}

double CalculatorModel::ApplyOperator(double number, UOperator op) const {
    switch(op) {
    case UOperator::ChangeSign:
        return -number;
    case UOperator::Percent:
        return 123; ///////////////
    default: throw std::runtime_error("Unknown operator");
    }
}

std::optional<CalculatorModel::BOperator> CalculatorModel::StringToBOperator(const std::string &input) const {
    if(input == "+") return BOperator::Add;
    if(input == "-") return BOperator::Subtract;
    if(input == "*") return BOperator::Multiply;
    if(input == "/") return BOperator::Divide;
    return std::nullopt;
}

std::optional<CalculatorModel::UOperator> CalculatorModel::StringToUOperator(const std::string &input) const {
    if(input == "+/-")  return UOperator::ChangeSign;
    if(input == "%")    return UOperator::Percent;
    return std::nullopt;
}

CalculatorModel::Display CalculatorModel::ProcessAC(const std::string &input) {
    if(input != "AC") {
        return ProcessUnknownOperator();
    }
    Clear();
    return Display{"", "0"};
}

CalculatorModel::Display CalculatorModel::ProcessNumberOrDot(const std::string &input) {
    Display result;

    if(input == ".") {
        if(!is_dot) {
            is_dot = true;
            if(active_num_.empty()) {
                Clear();
                active_num_ = "0.";
            } else {
                active_num_ += input;
            }
        }
    } else {
        active_num_ += input;
    }
    if(IsSignInMiddle()) {
        result.expression = prev_num_ + BOperatorToString();
        result.result = active_num_;
    } else {
        result.expression = active_num_;
        result.result = active_num_;
    }
    return result;
}

CalculatorModel::Display CalculatorModel::ProcessEquality(const std::string &input) {
    Display result;
    if(input != "=") {
        return ProcessUnknownOperator();
    }

    //calculate
    double left = std::stod(prev_num_);
    double right = std::stod(active_num_);
    double res = ApplyOperator(left, right, operator_);
    result.expression = prev_num_ + BOperatorToString() + active_num_ + input;
    result.result = std::to_string(res);
    Clear();
    prev_num_ = std::to_string(res);
    //active_num_ = std::to_string(res);
    return result;
}

CalculatorModel::Display CalculatorModel::ProcessUOperator(const std::string &input) {
    Display result;
    std::optional<UOperator> u_op = StringToUOperator(input);
    if(!u_op) {
        return ProcessUnknownOperator();
    }

    if(IsActiveNumberEmpty()) {
        active_num_ = prev_num_;
    }
    double right = std::stod(active_num_);
    double res = ApplyOperator(right, *u_op);
    active_num_ = std::to_string(res);
    if(IsSignInMiddle()) {
        result.expression = prev_num_ + BOperatorToString() + active_num_;
        result.result = std::to_string(res);
    } else {
        result.expression = active_num_;
        result.result = std::to_string(res);
    }
    return result;

}

CalculatorModel::Display CalculatorModel::ProcessBOperator(const std::string &input) {
    std::optional<BOperator> b_op = StringToBOperator(input);
    if(!b_op) {
        return ProcessUnknownOperator();
    }

    Display result;

    if(IsSignInMiddle()) {
        //calculate
        double left = std::stod(prev_num_);
        double right = std::stod(active_num_);
        double res = ApplyOperator(left, right, operator_);
        result.expression = std::to_string(res) + input;
        result.result = std::to_string(res);
        Clear();
        operator_ = *b_op;
        prev_num_ = std::to_string(res);
    } else {
        if(prev_num_.empty()) {
            ActiveNumberToPrev(*b_op);
        } else {
            operator_ = *b_op;
        }
        result.expression = prev_num_ + input;
        result.result = prev_num_;
    }
    return result;

}

CalculatorModel::Display CalculatorModel::ProcessInput(const std::string &input) {
    if(input.find_first_of("+-/*") != std::string::npos) {
        return ProcessBOperator(input);
    }
    if(input == "+/-" || input == "%") {
        return ProcessUOperator(input);
    }
    if(input == "=") {
        return ProcessEquality(input);
    }
    if(input == "AC") {
        return ProcessAC(input);
    }
    if(input.find_first_of("0123456789.") != std::string::npos) {
        return ProcessNumberOrDot(input);
    }
    return ProcessUnknownOperator();
}

CalculatorModel::Display CalculatorModel::ProcessUnknownOperator() const {
    Display result{"Unknown operator", ""};
    return result;
}
