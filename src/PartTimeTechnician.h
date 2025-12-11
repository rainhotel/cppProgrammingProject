#ifndef PARTTIMETECHNICIAN_H
#define PARTTIMETECHNICIAN_H

#include "Employee.h"

/**
 * 兼职技术人员类 (PartTimeTechnician)
 * - 继承自 Employee
 * - 特有属性：时薪 (hourlyRate_)、本月工时 (hoursWorked_)
 * - 薪资计算：时薪 × 工时
 */
class PartTimeTechnician : public Employee {
private:
    double hourlyRate_;   // 时薪
    double hoursWorked_;  // 本月工作小时数

public:
    PartTimeTechnician() : Employee(), hourlyRate_(0.0), hoursWorked_(0.0) {}
    
    PartTimeTechnician(int id, const std::string& name, const std::string& gender,
                       int level, double hourlyRate, double hoursWorked)
        : Employee(id, name, gender, level), 
          hourlyRate_(hourlyRate), hoursWorked_(hoursWorked) {}

    // ========== 访问器 ==========
    double getHourlyRate() const { return hourlyRate_; }
    void setHourlyRate(double rate) { hourlyRate_ = rate; }
    
    double getHoursWorked() const { return hoursWorked_; }
    void setHoursWorked(double hours) { hoursWorked_ = hours; }

    // ========== 实现纯虚函数 ==========
    
    std::string getRoleName() const override {
        return "PartTimeTech";
    }

    double calculateSalary() const override {
        return hourlyRate_ * hoursWorked_;
    }

    void display() const override {
        std::cout << "========== 兼职技术人员信息 ==========" << std::endl;
        displayBasic();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "时薪: " << hourlyRate_ << std::endl;
        std::cout << "本月工时: " << hoursWorked_ << " 小时" << std::endl;
        std::cout << "当月工资: " << calculateSalary() << std::endl;
        std::cout << "======================================" << std::endl;
    }

    void inputSpecificInfo() override {
        std::cout << "时薪: ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);
        try { hourlyRate_ = std::stod(s); } catch (...) { hourlyRate_ = 0.0; }

        std::cout << "本月工时(小时): ";
        std::cout.flush();
        std::getline(std::cin, s);
        try { hoursWorked_ = std::stod(s); } catch (...) { hoursWorked_ = 0.0; }
    }

    std::string toCSV() const override {
        std::ostringstream oss;
        oss << basicToCSV() << ","
            << std::fixed << std::setprecision(2) << hourlyRate_ << ","
            << std::fixed << std::setprecision(2) << hoursWorked_ << ","
            << "0";  // sales 占位
        return oss.str();
    }

    void parseCSV(const std::vector<std::string>& cols) override {
        // cols[6] = hourlyRate, cols[7] = hoursWorked
        if (cols.size() > 6) {
            try { hourlyRate_ = std::stod(cols[6]); } catch (...) { hourlyRate_ = 0; }
        }
        if (cols.size() > 7) {
            try { hoursWorked_ = std::stod(cols[7]); } catch (...) { hoursWorked_ = 0; }
        }
    }
};

#endif // PARTTIMETECHNICIAN_H
