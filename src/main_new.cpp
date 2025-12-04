/**
 * 企业人力管理系统 - 主程序
 * 
 * 采用面向对象设计：
 * - Employee (抽象基类)
 *   ├── Manager (经理 - 固定月薪)
 *   ├── PartTimeTechnician (兼职技术人员 - 按工时计薪)
 *   ├── SalesManager (销售经理 - 固定月薪 + 销售提成)
 *   └── PartTimeSalesperson (兼职推销员 - 按销售提成)
 * - EmployeeManager (员工管理类 - CRUD/统计/持久化)
 * 
 * 数据持久化：CSV 文件
 */

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "EmployeeManager.h"

void showMenu() {
    std::cout << "\n╔══════════════════════════════════════╗\n"
              << "║       企业人力管理系统               ║\n"
              << "╠══════════════════════════════════════╣\n"
              << "║  1. 添加员工                         ║\n"
              << "║  2. 删除员工                         ║\n"
              << "║  3. 查找员工                         ║\n"
              << "║  4. 修改员工信息                     ║\n"
              << "║  5. 显示所有员工                     ║\n"
              << "║  6. 工资统计及占比                   ║\n"
              << "║  7. 全员提级                         ║\n"
              << "║  8. 业绩排名                         ║\n"
              << "║  0. 退出系统                         ║\n"
              << "╚══════════════════════════════════════╝\n"
              << "请选择(0-8): ";
    std::cout.flush();
}

int main() {
#ifdef _WIN32
    // 设置 Windows 控制台使用 UTF-8 编码
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // 设置 I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(&std::cout);

    // 确定数据文件路径
    std::string csvPath = "data/employees.csv";
    {
        std::ifstream test(csvPath);
        if (!test.is_open()) {
            csvPath = "../data/employees.csv";
        }
    }

    std::cout << "╔══════════════════════════════════════╗\n"
              << "║     欢迎使用企业人力管理系统         ║\n"
              << "╚══════════════════════════════════════╝\n";
    std::cout << "数据文件: " << csvPath << std::endl;

    // 创建员工管理器并加载数据
    EmployeeManager manager(csvPath);
    manager.load();

    // 主循环
    while (true) {
        showMenu();
        
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            manager.addEmployee();
        } else if (choice == "2") {
            manager.removeEmployee();
        } else if (choice == "3") {
            manager.findEmployee();
        } else if (choice == "4") {
            manager.updateEmployee();
        } else if (choice == "5") {
            manager.listAll();
        } else if (choice == "6") {
            manager.statistics();
        } else if (choice == "7") {
            manager.promoteAll();
        } else if (choice == "8") {
            manager.ranking();
        } else if (choice == "0" || choice == "q" || choice == "Q") {
            break;
        } else {
            std::cout << "无效选择，请重试。" << std::endl;
        }
    }

    std::cout << "\n感谢使用，再见！" << std::endl;

#ifdef _WIN32
    std::cout << "按回车键关闭窗口..." << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
#endif

    return 0;
}
