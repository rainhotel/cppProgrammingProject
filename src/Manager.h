#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

/**
 * 经理类 (Manager)
 * - 继承自 Employee
 * - 特有属性：固定月薪 (fixedSalary_)
 * - 薪资计算：直接返回固定月薪
 */
class Manager : public Employee {
private:
    double fixedSalary_;  // 固定月薪

public:
    Manager() : Employee(), fixedSalary_(0.0) {}
    
    Manager(int id, const std::string& name, const std::string& gender, 
            int level, double fixedSalary)
        : Employee(id, name, gender, level), fixedSalary_(fixedSalary) {}

    // ========== 访问器 ==========
    double getFixedSalary() const { return fixedSalary_; }
    void setFixedSalary(double salary) { fixedSalary_ = salary; }

    // ========== 实现纯虚函数 ==========
    
    std::string getRoleName() const override {
        return "Manager";
    }

    double calculateSalary() const override {
        return fixedSalary_;
    }

    void display() const override {
        std::cout << "========== 经理信息 ==========" << std::endl;
        displayBasic();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "固定月薪: " << fixedSalary_ << std::endl;
        std::cout << "当月工资: " << calculateSalary() << std::endl;
        std::cout << "==============================" << std::endl;
    }

    void inputSpecificInfo() override {
        std::cout << "固定月薪: ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);
        try {
            fixedSalary_ = std::stod(s);
        } catch (...) {
            fixedSalary_ = 0.0;
        }
    }

    std::string toCSV() const override {
        std::ostringstream oss;
        oss << basicToCSV() << ","
            << std::fixed << std::setprecision(2) << fixedSalary_
            << ",0,0";  // hours, sales 占位
        return oss.str();
    }

    void parseCSV(const std::vector<std::string>& cols) override {
        // cols[6] = fixedSalary (baseSalary 列)
        if (cols.size() > 6) {
            try { fixedSalary_ = std::stod(cols[6]); } catch (...) { fixedSalary_ = 0; }
        }
    }
};

#endif // MANAGER_H
