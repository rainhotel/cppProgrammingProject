#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

/**
 * 员工基类 (抽象类)
 * - 包含所有员工的公共属性：编号、姓名、性别、级别
 * - 定义纯虚函数：计算月薪、显示信息、获取角色名、序列化/反序列化
 */
class Employee {
protected:
    int id_;
    std::string name_;
    std::string gender_;  // "男" 或 "女"
    int level_;

public:
    Employee() : id_(0), level_(1) {}
    Employee(int id, const std::string& name, const std::string& gender, int level)
        : id_(id), name_(name), gender_(gender), level_(level) {}
    
    virtual ~Employee() = default;

    // ========== 访问器 ==========
    int getId() const { return id_; }
    void setId(int id) { id_ = id; }
    
    std::string getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    std::string getGender() const { return gender_; }
    void setGender(const std::string& gender) { gender_ = gender; }
    
    int getLevel() const { return level_; }
    void setLevel(int level) { level_ = level; }

    // ========== 纯虚函数（派生类必须实现） ==========
    
    // 获取角色类型名称
    virtual std::string getRoleName() const = 0;
    
    // 计算当月薪资
    virtual double calculateSalary() const = 0;
    
    // 显示员工详细信息
    virtual void display() const = 0;
    
    // 从控制台输入该类型员工特有的属性
    virtual void inputSpecificInfo() = 0;
    
    // 序列化为 CSV 行（不含换行）
    virtual std::string toCSV() const = 0;
    
    // 从 CSV 列向量解析特有属性（公共属性由工厂处理）
    virtual void parseCSV(const std::vector<std::string>& cols) = 0;

    // ========== 通用方法 ==========
    
    // 提升级别
    void promote(int increment = 1) {
        level_ += increment;
        std::cout << name_ << " 级别提升到 " << level_ << std::endl;
    }

    // 输入公共属性（姓名、性别）
    void inputBasicInfo() {
        std::cout << "姓名: ";
        std::cout.flush();
        std::getline(std::cin, name_);
        
        while (true) {
            std::cout << "性别(男/女): ";
            std::cout.flush();
            std::getline(std::cin, gender_);
            if (gender_ == "男" || gender_ == "女") break;
            std::cout << "输入错误，性别只能为 男 或 女，请重试。" << std::endl;
        }
        
        std::cout << "级别(整数, 默认1): ";
        std::cout.flush();
        std::string levelStr;
        std::getline(std::cin, levelStr);
        try {
            level_ = std::stoi(levelStr);
        } catch (...) {
            level_ = 1;
        }
    }

protected:
    // 辅助：生成公共属性的 CSV 部分
    std::string basicToCSV() const {
        std::ostringstream oss;
        oss << id_ << "," << name_ << "," << getRoleName() << ","
            << level_ << "," << gender_;
        return oss.str();
    }

    // 辅助：显示公共属性
    void displayBasic() const {
        std::cout << "编号: " << id_ << "\n"
                  << "姓名: " << name_ << "\n"
                  << "性别: " << gender_ << "\n"
                  << "级别: " << level_ << "\n"
                  << "岗位: " << getRoleName() << "\n";
    }
};

#endif // EMPLOYEE_H
