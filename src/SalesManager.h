#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include "Employee.h"

/**
 * 销售经理类 (SalesManager)
 * - 继承自 Employee
 * - 特有属性：固定月薪 (fixedSalary_)、提成比例 (commissionRate_)、本月销售额 (salesAmount_)
 * - 薪资计算：固定月薪 + 销售额 × 提成比例
 */
class SalesManager : public Employee {
private:
    double fixedSalary_;     // 固定月薪
    double commissionRate_;  // 提成比例 (0.0 ~ 1.0)
    double salesAmount_;     // 本月销售额

public:
    SalesManager() : Employee(), fixedSalary_(0.0), commissionRate_(0.0), salesAmount_(0.0) {}
    
    SalesManager(int id, const std::string& name, const std::string& gender,
                 int level, double fixedSalary, double commissionRate, double salesAmount)
        : Employee(id, name, gender, level),
          fixedSalary_(fixedSalary), commissionRate_(commissionRate), salesAmount_(salesAmount) {}

    // ========== 访问器 ==========
    double getFixedSalary() const { return fixedSalary_; }
    void setFixedSalary(double salary) { fixedSalary_ = salary; }
    
    double getCommissionRate() const { return commissionRate_; }
    void setCommissionRate(double rate) { commissionRate_ = rate; }
    
    double getSalesAmount() const { return salesAmount_; }
    void setSalesAmount(double amount) { salesAmount_ = amount; }

    // ========== 实现纯虚函数 ==========
    
    std::string getRoleName() const override {
        return "SalesManager";
    }

    double calculateSalary() const override {
        return fixedSalary_ + salesAmount_ * commissionRate_;
    }

    void display() const override {
        std::cout << "========== 销售经理信息 ==========" << std::endl;
        displayBasic();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "固定月薪: " << fixedSalary_ << std::endl;
        std::cout << "提成比例: " << (commissionRate_ * 100) << "%" << std::endl;
        std::cout << "本月销售额: " << salesAmount_ << std::endl;
        std::cout << "当月工资: " << calculateSalary() << std::endl;
        std::cout << "==================================" << std::endl;
    }

    void inputSpecificInfo() override {
        std::string s;
        
        std::cout << "固定月薪: ";
        std::cout.flush();
        std::getline(std::cin, s);
        try { fixedSalary_ = std::stod(s); } catch (...) { fixedSalary_ = 0.0; }

        std::cout << "提成比例(如0.05表示5%): ";
        std::cout.flush();
        std::getline(std::cin, s);
        try { commissionRate_ = std::stod(s); } catch (...) { commissionRate_ = 0.0; }

        std::cout << "本月销售额: ";
        std::cout.flush();
        std::getline(std::cin, s);
        try { salesAmount_ = std::stod(s); } catch (...) { salesAmount_ = 0.0; }
    }

    std::string toCSV() const override {
        // 格式: id,name,role,level,gender,fixedSalary,commissionRate,salesAmount
        // 为兼容统一格式，这里用: baseSalary=fixedSalary, hours=commissionRate, sales=salesAmount
        std::ostringstream oss;
        oss << basicToCSV() << ","
            << std::fixed << std::setprecision(2) << fixedSalary_ << ","
            << std::fixed << std::setprecision(4) << commissionRate_ << ","
            << std::fixed << std::setprecision(2) << salesAmount_;
        return oss.str();
    }

    void parseCSV(const std::vector<std::string>& cols) override {
        // cols[6] = fixedSalary, cols[7] = commissionRate, cols[8] = salesAmount
        if (cols.size() > 6) {
            try { fixedSalary_ = std::stod(cols[6]); } catch (...) { fixedSalary_ = 0; }
        }
        if (cols.size() > 7) {
            try { commissionRate_ = std::stod(cols[7]); } catch (...) { commissionRate_ = 0; }
        }
        if (cols.size() > 8) {
            try { salesAmount_ = std::stod(cols[8]); } catch (...) { salesAmount_ = 0; }
        }
    }
};

#endif // SALESMANAGER_H
