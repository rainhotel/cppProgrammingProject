#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Employee.h"
#include "Manager.h"
#include "PartTimeTechnician.h"
#include "PartTimeSalesperson.h"
#include "SalesManager.h"

/**
 * 员工管理类 (EmployeeManager)
 * - 管理所有员工对象（使用智能指针）
 * - 提供 CRUD、检索、统计、排名、持久化等功能
 */
class EmployeeManager {
private:
    std::vector<std::unique_ptr<Employee>> employees_;
    int nextId_;
    std::string csvPath_;

public:
    explicit EmployeeManager(const std::string& csvPath) 
        : nextId_(1), csvPath_(csvPath) {}

    // 从 CSV 文件加载
    void load() {
        employees_.clear();
        nextId_ = 1;

        std::ifstream in(csvPath_);
        if (!in.is_open()) {
            std::cout << "数据文件不存在，将创建新文件: " << csvPath_ << std::endl;
            return;
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            
            // 解析 CSV 行
            std::vector<std::string> cols;
            std::stringstream ss(line);
            std::string token;
            while (std::getline(ss, token, ',')) {
                cols.push_back(token);
            }

            // 跳过表头
            if (isHeader && cols.size() > 0 && cols[0] == "id") {
                isHeader = false;
                continue;
            }
            isHeader = false;

            if (cols.size() < 5) continue;

            // 解析公共属性
            int id = 0;
            try { id = std::stoi(cols[0]); } catch (...) { continue; }
            std::string name = cols[1];
            std::string role = cols[2];
            int level = 1;
            try { level = std::stoi(cols[3]); } catch (...) {}
            std::string gender = cols[4];
            std::string birthday = (cols.size() > 5) ? cols[5] : "";

            // 根据角色创建对应的派生类对象
            std::unique_ptr<Employee> emp = createEmployeeByRole(role);
            if (!emp) continue;

            emp->setId(id);
            emp->setName(name);
            emp->setGender(gender);
            emp->setLevel(level);
            emp->setBirthday(birthday);
            emp->parseCSV(cols);

            employees_.push_back(std::move(emp));
            nextId_ = std::max(nextId_, id + 1);
        }

        std::cout << "已加载 " << employees_.size() << " 条员工记录。" << std::endl;
    }

    // 保存到 CSV 文件
    void save() const {
        std::ofstream out(csvPath_);
        if (!out.is_open()) {
            std::cout << "无法写入文件: " << csvPath_ << std::endl;
            return;
        }

        // 写入表头
        out << "id,name,role,level,gender,birthday,param1,param2,param3\n";
        
        for (const auto& emp : employees_) {
            out << emp->toCSV() << "\n";
        }

        std::cout << "数据已保存。" << std::endl;
    }


    static std::unique_ptr<Employee> createEmployeeByRole(const std::string& role) {
        if (role == "Manager") {
            return std::make_unique<Manager>();
        } else if (role == "PartTimeTech") {
            return std::make_unique<PartTimeTechnician>();
        } else if (role == "PartTimeSales") {
            return std::make_unique<PartTimeSalesperson>();
        } else if (role == "SalesManager") {
            return std::make_unique<SalesManager>();
        }
        return nullptr;
    }

    static std::unique_ptr<Employee> createEmployeeByChoice(int choice) {
        switch (choice) {
            case 1: return std::make_unique<Manager>();
            case 2: return std::make_unique<PartTimeTechnician>();
            case 3: return std::make_unique<SalesManager>();
            case 4: return std::make_unique<PartTimeSalesperson>();
            default: return nullptr;
        }
    }

    // ========== CRUD 操作 ==========

    // 添加员工
    void addEmployee() {
        std::cout << "\n选择员工类型:\n"
                  << "1. 经理 (固定月薪)\n"
                  << "2. 兼职技术人员 (按工时计薪)\n"
                  << "3. 销售经理 (固定月薪 + 销售提成)\n"
                  << "4. 兼职推销员 (按销售提成)\n"
                  << "请选择(1-4): ";
        std::cout.flush();

        std::string s;
        std::getline(std::cin, s);
        int choice = 0;
        try { choice = std::stoi(s); } catch (...) {}

        auto emp = createEmployeeByChoice(choice);
        if (!emp) {
            std::cout << "无效选择。" << std::endl;
            return;
        }

        emp->setId(nextId_++);
        emp->inputBasicInfo();
        emp->inputSpecificInfo();

        std::cout << "\n已添加员工，编号: " << emp->getId() << std::endl;
        emp->display();

        employees_.push_back(std::move(emp));
        save();
    }

    // 删除员工
    void removeEmployee() {
        std::cout << "输入要删除的员工编号: ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);
        int id = 0;
        try { id = std::stoi(s); } catch (...) {}

        auto it = std::remove_if(employees_.begin(), employees_.end(),
            [id](const std::unique_ptr<Employee>& e) { return e->getId() == id; });

        if (it != employees_.end()) {
            employees_.erase(it, employees_.end());
            save();
            std::cout << "已删除编号为 " << id << " 的员工。" << std::endl;
        } else {
            std::cout << "未找到编号为 " << id << " 的员工。" << std::endl;
        }
    }

    // 查找员工
    void findEmployee() {
        std::cout << "按 1.姓名 2.编号 检索: ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);

        if (s == "1") {
            std::cout << "输入姓名: ";
            std::cout.flush();
            std::string name;
            std::getline(std::cin, name);

            bool found = false;
            for (const auto& emp : employees_) {
                if (emp->getName() == name) {
                    emp->display();
                    found = true;
                }
            }
            if (!found) {
                std::cout << "未检索到姓名为 \"" << name << "\" 的员工。" << std::endl;
            }
        } else if (s == "2") {
            std::cout << "输入编号: ";
            std::cout.flush();
            std::getline(std::cin, s);
            int id = 0;
            try { id = std::stoi(s); } catch (...) {}

            bool found = false;
            for (const auto& emp : employees_) {
                if (emp->getId() == id) {
                    emp->display();
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "未检索到编号为 " << id << " 的员工。" << std::endl;
            }
        } else {
            std::cout << "无效选择。" << std::endl;
        }
    }

    // 修改员工信息
    void updateEmployee() {
        std::cout << "输入要修改的员工编号: ";
        std::cout.flush();
        std::string s;
        std::getline(std::cin, s);
        int id = 0;
        try { id = std::stoi(s); } catch (...) {}

        Employee* target = nullptr;
        for (auto& emp : employees_) {
            if (emp->getId() == id) {
                target = emp.get();
                break;
            }
        }

        if (!target) {
            std::cout << "未找到编号为 " << id << " 的员工。" << std::endl;
            return;
        }

        std::cout << "当前信息:" << std::endl;
        target->display();

        std::cout << "\n重新输入信息（按回车保留原值暂不支持，将覆盖）:\n";
        target->inputBasicInfo();
        target->inputSpecificInfo();

        save();
        std::cout << "修改完成。" << std::endl;
        target->display();
    }

    // 列出所有员工
    void listAll() const {
        if (employees_.empty()) {
            std::cout << "当前没有员工记录。" << std::endl;
            return;
        }

        std::cout << "\n========== 全部员工列表 (" << employees_.size() << " 人) ==========" << std::endl;
        for (const auto& emp : employees_) {
            emp->display();
        }
    }

    // ========== 统计与分析 ==========

    // 统计工资及占比
    void statistics() const {
        if (employees_.empty()) {
            std::cout << "当前没有员工记录。" << std::endl;
            return;
        }

        double total = 0.0;
        double managerTotal = 0.0;
        double techTotal = 0.0;
        double salesManagerTotal = 0.0;
        double salespersonTotal = 0.0;

        int managerCount = 0, techCount = 0, salesManagerCount = 0, salespersonCount = 0;

        for (const auto& emp : employees_) {
            double salary = emp->calculateSalary();
            total += salary;

            std::string role = emp->getRoleName();
            if (role == "Manager") {
                managerTotal += salary;
                managerCount++;
            } else if (role == "PartTimeTech") {
                techTotal += salary;
                techCount++;
            } else if (role == "SalesManager") {
                salesManagerTotal += salary;
                salesManagerCount++;
            } else if (role == "PartTimeSales") {
                salespersonTotal += salary;
                salespersonCount++;
            }
        }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n========== 工资统计 ==========" << std::endl;
        std::cout << "员工总数: " << employees_.size() << " 人" << std::endl;
        std::cout << "工资总额: " << total << " 元" << std::endl;
        std::cout << std::endl;

        std::cout << "各类员工统计:" << std::endl;
        std::cout << "  经理: " << managerCount << " 人, 工资合计 " << managerTotal 
                  << " 元, 占比 " << (total > 0 ? managerTotal / total * 100 : 0) << "%" << std::endl;
        std::cout << "  兼职技术: " << techCount << " 人, 工资合计 " << techTotal 
                  << " 元, 占比 " << (total > 0 ? techTotal / total * 100 : 0) << "%" << std::endl;
        std::cout << "  销售经理: " << salesManagerCount << " 人, 工资合计 " << salesManagerTotal 
                  << " 元, 占比 " << (total > 0 ? salesManagerTotal / total * 100 : 0) << "%" << std::endl;
        std::cout << "  兼职推销: " << salespersonCount << " 人, 工资合计 " << salespersonTotal 
                  << " 元, 占比 " << (total > 0 ? salespersonTotal / total * 100 : 0) << "%" << std::endl;
        std::cout << "==============================" << std::endl;
    }

    // 全员提级
    void promoteAll() {
        if (employees_.empty()) {
            std::cout << "当前没有员工记录。" << std::endl;
            return;
        }

        for (auto& emp : employees_) {
            emp->promote(1);
        }
        save();
        std::cout << "全员已提升一级。" << std::endl;
    }

    // 业绩排名（按月薪）
    void ranking() const {
        if (employees_.empty()) {
            std::cout << "当前没有员工记录。" << std::endl;
            return;
        }

        // 创建索引并排序
        std::vector<size_t> indices(employees_.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }

        std::sort(indices.begin(), indices.end(), [this](size_t a, size_t b) {
            return employees_[a]->calculateSalary() > employees_[b]->calculateSalary();
        });

        std::cout << "\n========== 业绩排名 (按月薪) ==========" << std::endl;
        int rank = 1;
        for (size_t idx : indices) {
            const auto& emp = employees_[idx];
            std::cout << "第 " << rank++ << " 名: "
                      << emp->getName() << " (" << emp->getRoleName() << ") - "
                      << std::fixed << std::setprecision(2) << emp->calculateSalary() << " 元" << std::endl;
        }
        std::cout << "=======================================" << std::endl;
    }

    // 生日提醒功能
    void birthdayReminder() const {
        if (employees_.empty()) {
            std::cout << "当前没有员工记录。" << std::endl;
            return;
        }

        // 获取当前日期
        std::cout << "请输入当前日期(格式YYYY-MM-DD, 如2025-12-11): ";
        std::cout.flush();
        std::string currentDate;
        std::getline(std::cin, currentDate);

        if (!Employee::isValidDate(currentDate)) {
            std::cout << "日期格式错误。" << std::endl;
            return;
        }

        std::string currentMonthDay = currentDate.substr(5, 5); // MM-DD
        int currentMonth = 0, currentDay = 0;
        try {
            currentMonth = std::stoi(currentDate.substr(5, 2));
            currentDay = std::stoi(currentDate.substr(8, 2));
        } catch (...) {
            std::cout << "日期解析错误。" << std::endl;
            return;
        }

        std::cout << "\n请输入提醒天数范围(如7表示未来7天内): ";
        std::cout.flush();
        std::string daysStr;
        std::getline(std::cin, daysStr);
        int reminderDays = 7;
        try { reminderDays = std::stoi(daysStr); } catch (...) {}

        std::vector<const Employee*> upcomingBirthdays;

        for (const auto& emp : employees_) {
            std::string birthday = emp->getBirthday();
            if (birthday.empty() || birthday.length() < 10) continue;

            std::string birthMonthDay = birthday.substr(5, 5); // MM-DD
            int birthMonth = 0, birthDay = 0;
            try {
                birthMonth = std::stoi(birthday.substr(5, 2));
                birthDay = std::stoi(birthday.substr(8, 2));
            } catch (...) {
                continue;
            }

            // 简化逻辑：只比较月和日，计算天数差
            int currentDayOfYear = currentMonth * 31 + currentDay;
            int birthDayOfYear = birthMonth * 31 + birthDay;
            
            // 如果生日已过今年，算作明年
            if (birthDayOfYear < currentDayOfYear) {
                birthDayOfYear += 365;
            }

            int daysUntilBirthday = birthDayOfYear - currentDayOfYear;
            
            if (daysUntilBirthday >= 0 && daysUntilBirthday <= reminderDays) {
                upcomingBirthdays.push_back(emp.get());
            }
        }

        if (upcomingBirthdays.empty()) {
            std::cout << "\n未来 " << reminderDays << " 天内没有员工生日。" << std::endl;
        } else {
            std::cout << "\n========== 生日提醒 (未来" << reminderDays << "天内) ==========" << std::endl;
            for (const auto* emp : upcomingBirthdays) {
                std::string birthday = emp->getBirthday();
                std::string birthMonthDay = birthday.substr(5, 5);
                std::cout << "员工: " << emp->getName() 
                          << " (编号: " << emp->getId() << ")"
                          << " | 生日: " << birthMonthDay 
                          << " | 岗位: " << emp->getRoleName() << std::endl;
            }
            std::cout << "======================================" << std::endl;
        }
    }

    // ========== 辅助 ==========
    
    size_t count() const { return employees_.size(); }
    const std::string& getDataPath() const { return csvPath_; }
};

#endif // EMPLOYEEMANAGER_H
