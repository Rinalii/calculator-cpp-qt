#include "calculatormodel.h"

CalculatorModel::CalculatorModel() {}

void CalculatorModel::ProcessPercent() {
    double left = StringToDouble(prev_num_);
    double right = StringToDouble(active_num_);
    if(operator_==BOperator::Add || operator_==BOperator::Subtract) {
        double tmp = left/100. * right;
        active_num_ = DoubleToString(tmp);
    } else if(operator_==BOperator::Multiply || operator_==BOperator::Divide) {
        double tmp = right/100.;
        active_num_ = DoubleToString(tmp);
    } else {
        active_num_ = "";
    }
}

void CalculatorModel::ProcessSignChange() {
    if(active_num_.empty()) {
        if(prev_num_.empty()) return;
        active_num_ = prev_num_;
    }

    if(active_num_[0] == '-') {
        active_num_ = active_num_.substr(1);
    } else {
        active_num_ = '-' + active_num_;
    }
}

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
    return operator_ != BOperator::None && !prev_num_.empty() && !active_num_.empty();
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

std::string CalculatorModel::DoubleToString(double value) {
    std::string str = std::to_string(value);

    // Удаляем trailing zeros
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    // Удаляем точку, если она осталась последней
    if (str.back() == '.') {
        str.pop_back();
    }

    return str;
}

double CalculatorModel::StringToDouble(const std::string &str) {
    try {
        size_t pos;
        double result = std::stod(str, &pos);

        // Проверяем, что вся строка была преобразована
        if (pos == str.length()) {
            return result;
        }
        // Если после числа есть пробелы - считаем успехом
        while (pos < str.length() && std::isspace(str[pos])) {
            pos++;
        }
        if (pos == str.length()) {
            return result;
        }
        return 0; // Есть непробельные символы после числа
    }
    catch (const std::exception&) {
        return 0;
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
        result.expression = "";
        result.result = active_num_;
    }
    return result;
}

CalculatorModel::Display CalculatorModel::ProcessEquality(const std::string &input) {
    Display result;
    if(input != "=") {
        return ProcessUnknownOperator();
    }

    try{
        //calculate
        if(operator_ != BOperator::None) {
            double left = StringToDouble(prev_num_);
            double right = 0;
            if(active_num_.empty()) {
                right = left;
            } else {
                right = StringToDouble(active_num_);
            }
            double res = ApplyOperator(left, right, operator_);
            result.expression = prev_num_ + BOperatorToString() + active_num_ + input;
            result.result = DoubleToString(res);
        } else {
            result.expression = active_num_ + input;
            result.result = active_num_;
        }
    } catch(const std::runtime_error& err) {
        result.expression = err.what();
        result.result = "";
    }

    Clear();
    prev_num_ = result.result;

    return result;
}

CalculatorModel::Display CalculatorModel::ProcessUOperator(const std::string &input) {
    Display result;
    std::optional<UOperator> u_op = StringToUOperator(input);
    if(!u_op) {
        return ProcessUnknownOperator();
    }

    switch(*u_op) {
    case UOperator::ChangeSign:
        ProcessSignChange();
        break;
    case UOperator::Percent:
        ProcessPercent();
        break;
    default: throw std::runtime_error("Unknown operator");
    }

    if(IsSignInMiddle()) {
        result.expression = prev_num_ + BOperatorToString() + active_num_;
    } else {
        result.expression = active_num_;
    }
    result.result = active_num_;
    return result;
}

CalculatorModel::Display CalculatorModel::ProcessBOperator(const std::string &input) {
    std::optional<BOperator> b_op = StringToBOperator(input);
    if(!b_op) {
        return ProcessUnknownOperator();
    }

    Display result;

    try{
        if(IsSignInMiddle()) {
            //calculate
            double left = StringToDouble(prev_num_);
            double right = StringToDouble(active_num_);
            double res = ApplyOperator(left, right, operator_);
            result.expression = DoubleToString(res) + input;
            result.result = DoubleToString(res);
            Clear();
            operator_ = *b_op;
            prev_num_ = DoubleToString(res);
        } else {
            if(!active_num_.empty()) {
                ActiveNumberToPrev(*b_op);
            } else {
                operator_ = *b_op;
            }
            result.expression = prev_num_ + input;
            result.result = prev_num_;
        }
    } catch(const std::runtime_error& err) {
        result.expression = err.what();
        result.result = "";
    }
    return result;

}

CalculatorModel::Display CalculatorModel::ProcessUnknownOperator() const {
    Display result{"Unknown operator", ""};
    return result;
}
