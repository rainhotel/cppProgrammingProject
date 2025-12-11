#ifndef PARTTIMESALESPERSON_H
#define PARTTIMESALESPERSON_H

#include "Employee.h"

/**
 * 兼职推销员类 (PartTimeSalesperson)
 * - 继承自 Employee
 * - 特有属性：提成比例 (commissionRate_)、本月销售额 (salesAmount_)
 * - 薪资计算：销售额 × 提成比例
 */
class PartTimeSalesperson : public Employee {
private:
    double commissionRate_;  // 提成比例 (0.0 ~ 1.0)
    double salesAmount_;     // 本月销售额

public:
    PartTimeSalesperson() : Employee(), commissionRate_(0.0), salesAmount_(0.0) {}
    
    PartTimeSalesperson(int id, const std::string& name, const std::string& gender,
                        int level, double commissionRate, double salesAmount)
        : Employee(id, name, gender, level),
          commissionRate_(commissionRate), salesAmount_(salesAmount) {}

    // ========== 访问器 ==========
    double getCommissionRate() const { return commissionRate_; }
    void setCommissionRate(double rate) { commissionRate_ = rate; }
    
    double getSalesAmount() const { return salesAmount_; }
    void setSalesAmount(double amount) { salesAmount_ = amount; }

    // ========== 实现纯虚函数 ==========
    
    std::string getRoleName() const override {
        return "PartTimeSales";
    }

    double calculateSalary() const override {
        return salesAmount_ * commissionRate_;
    }

    void display() const override {
        std::cout << "========== 兼职推销员信息 ==========" << std::endl;
        displayBasic();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "提成比例: " << (commissionRate_ * 100) << "%" << std::endl;
        std::cout << "本月销售额: " << salesAmount_ << std::endl;
        std::cout << "当月工资: " << calculateSalary() << std::endl;
        std::cout << "=====================================" << std::endl;
    }

    void inputSpecificInfo() override {
        std::cout << "提成比例(如0.05表示5%): ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);
        try { commissionRate_ = std::stod(s); } catch (...) { commissionRate_ = 0.0; }

        std::cout << "本月销售额: ";
        std::cout.flush();
        std::getline(std::cin, s);
        try { salesAmount_ = std::stod(s); } catch (...) { salesAmount_ = 0.0; }
    }

    std::string toCSV() const override {
        std::ostringstream oss;
        oss << basicToCSV() << ","
            << std::fixed << std::setprecision(4) << commissionRate_ << ","
            << "0,"  // hours 占位
            << std::fixed << std::setprecision(2) << salesAmount_;
        return oss.str();
    }

    void parseCSV(const std::vector<std::string>& cols) override {
        // cols[6] = commissionRate, cols[8] = salesAmount
        if (cols.size() > 6) {
            try { commissionRate_ = std::stod(cols[6]); } catch (...) { commissionRate_ = 0; }
        }
        if (cols.size() > 8) {
            try { salesAmount_ = std::stod(cols[8]); } catch (...) { salesAmount_ = 0; }
        }
    }
};

#endif // PARTTIMESALESPERSON_H
