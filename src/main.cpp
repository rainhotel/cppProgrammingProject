#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif

// 简化实现：所有逻辑集中于一个源文件，CSV 持久化
// 字段：id,name,role,level,gender,baseSalary,hours,sales

enum class Role {
    Manager,            // 固定月薪
    PartTimeTech,       // 兼职技术，按工时
    SalesManager,       // 固定 + 提成
    PartTimeSales       // 兼职推销员，按销售额提成
};

struct Employee {
    int id;
    std::string name;
    Role role;
    int level;            // 级别，整数
    std::string gender;   // "男" 或 "女"
    double baseSalary;    // 基础薪资（经理/销售经理固定薪资；技术的时薪；销售提成基数）
    double hours;         // 技术工时
    double sales;         // 当月销售额

    double monthlyPay() const {
        // 简单规则（可根据需要调整）：
        // Manager: baseSalary
        // PartTimeTech: baseSalary(时薪) * hours
        // PartTimeSales: sales * commission(用 baseSalary 作为提成比例，0.0~1.0)
        // SalesManager: baseSalary + sales * commission(用 baseSalary 作为提成比例，0.0~1.0)
        switch (role) {
            case Role::Manager:
                return baseSalary;
            case Role::PartTimeTech:
                return baseSalary * hours;
            case Role::PartTimeSales:
                return sales * baseSalary; // 这里 baseSalary 表示提成比例
            case Role::SalesManager:
                return baseSalary + sales * (baseSalary * 0.2); // 销售经理额外提成，这里用 20% 的比例
            default:
                return 0.0;
        }
    }
};

static std::string roleToString(Role r) {
    switch (r) {
        case Role::Manager: return "Manager";
        case Role::PartTimeTech: return "PartTimeTech";
        case Role::SalesManager: return "SalesManager";
        case Role::PartTimeSales: return "PartTimeSales";
    }
    return "Unknown";
}

static Role roleFromString(const std::string &s) {
    if (s == "Manager") return Role::Manager;
    if (s == "PartTimeTech") return Role::PartTimeTech;
    if (s == "SalesManager") return Role::SalesManager;
    if (s == "PartTimeSales") return Role::PartTimeSales;
    // 默认 Manager
    return Role::Manager;
}

struct Repository {
    std::string csvPath;
    int nextId = 1;
    std::vector<Employee> items;

    void load() {
        items.clear();
        std::ifstream in(csvPath);
        if (!in.is_open()) return;
        std::string line;
        // 跳过可能的表头
        bool firstLine = true;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> cols;
            while (std::getline(ss, token, ',')) cols.push_back(token);
            if (firstLine && cols.size() && cols[0] == "id") { firstLine = false; continue; }
            firstLine = false;
            if (cols.size() < 8) continue;
            Employee e;
            e.id = std::stoi(cols[0]);
            e.name = cols[1];
            e.role = roleFromString(cols[2]);
            e.level = std::stoi(cols[3]);
            e.gender = cols[4];
            e.baseSalary = std::stod(cols[5]);
            e.hours = std::stod(cols[6]);
            e.sales = std::stod(cols[7]);
            items.push_back(e);
            nextId = std::max(nextId, e.id + 1);
        }
    }

    void save() const {
        std::ofstream out(csvPath);
        out << "id,name,role,level,gender,baseSalary,hours,sales\n";
        for (const auto &e : items) {
            out << e.id << ','
                << e.name << ','
                << roleToString(e.role) << ','
                << e.level << ','
                << e.gender << ','
                << std::fixed << std::setprecision(2) << e.baseSalary << ','
                << std::fixed << std::setprecision(2) << e.hours << ','
                << std::fixed << std::setprecision(2) << e.sales << '\n';
        }
    }

    int add(Employee e) {
        e.id = nextId++;
        items.push_back(e);
        save();
        return e.id;
    }

    bool removeById(int id) {
        auto it = std::remove_if(items.begin(), items.end(), [&](const Employee &x){ return x.id == id; });
        if (it == items.end()) return false;
        items.erase(it, items.end());
        save();
        return true;
    }

    std::vector<Employee*> findByName(const std::string &name) {
        std::vector<Employee*> res;
        for (auto &e : items) if (e.name == name) res.push_back(&e);
        return res;
    }

    Employee* findById(int id) {
        for (auto &e : items) if (e.id == id) return &e;
        return nullptr;
    }
};

static bool inputGender(std::string &gender) {
    std::string g;
    std::cout << "性别(男/女): ";
    std::getline(std::cin, g);
    if (g == "男" || g == "女") { gender = g; return true; }
    std::cout << "输入错误，性别只能为 男 或 女，请重试。\n";
    return false;
}

static Role inputRole() {
    while (true) {
        std::cout << "角色: 1.经理 2.兼职技术 3.销售经理 4.兼职推销\n选择(1-4): ";
        std::string s; std::getline(std::cin, s);
        if (s == "1") return Role::Manager;
        if (s == "2") return Role::PartTimeTech;
        if (s == "3") return Role::SalesManager;
        if (s == "4") return Role::PartTimeSales;
        std::cout << "无效选择，请重试。\n";
    }
}

static void printEmployee(const Employee &e) {
    std::cout << "#" << e.id << " " << e.name
              << " | role=" << roleToString(e.role)
              << " | level=" << e.level
              << " | gender=" << e.gender
              << " | baseSalary=" << e.baseSalary
              << " | hours=" << e.hours
              << " | sales=" << e.sales
              << " | monthlyPay=" << std::fixed << std::setprecision(2) << e.monthlyPay()
              << "\n";
}

static void listAll(Repository &repo) {
    std::cout << "当前员工列表(" << repo.items.size() << "):\n";
    for (const auto &e : repo.items) printEmployee(e);
}

static void addEmployee(Repository &repo) {
    Employee e{};
    std::cout << "姓名: "; std::getline(std::cin, e.name);
    e.role = inputRole();
    std::string gender;
    while (!inputGender(gender)) {}
    e.gender = gender;
    std::cout << "级别(整数): ";
    {
        std::string s; std::getline(std::cin, s);
        try { e.level = std::stoi(s); } catch(...) { e.level = 1; }
    }
    std::cout << "基础薪资(经理/销售经理固定薪资；技术时薪；销售提成比例0~1): ";
    {
        std::string s; std::getline(std::cin, s);
        try { e.baseSalary = std::stod(s); } catch(...) { e.baseSalary = 0.0; }
    }
    std::cout << "本月工时(技术岗位有效): ";
    { std::string s; std::getline(std::cin, s); try { e.hours = std::stod(s); } catch(...) { e.hours = 0.0; } }
    std::cout << "本月销售额(销售岗位有效): ";
    { std::string s; std::getline(std::cin, s); try { e.sales = std::stod(s); } catch(...) { e.sales = 0.0; } }
    int id = repo.add(e);
    std::cout << "已添加，编号为: " << id << "\n";
}

static void removeEmployee(Repository &repo) {
    std::cout << "输入要删除的编号: ";
    std::string s; std::getline(std::cin, s);
    int id = -1; try { id = std::stoi(s); } catch(...) {}
    if (id <= 0) { std::cout << "编号无效。\n"; return; }
    if (repo.removeById(id)) std::cout << "已删除。\n"; else std::cout << "未找到。\n";
}

static void findEmployee(Repository &repo) {
    std::cout << "按 1.姓名 2.编号 检索: ";
    std::string s; std::getline(std::cin, s);
    if (s == "1") {
        std::cout << "输入姓名: ";
        std::string name; std::getline(std::cin, name);
        auto res = repo.findByName(name);
        if (res.empty()) std::cout << "未检索到相应信息。\n";
        for (auto *p : res) printEmployee(*p);
    } else if (s == "2") {
        std::cout << "输入编号: ";
        std::string idStr; std::getline(std::cin, idStr);
        int id = -1; try { id = std::stoi(idStr); } catch(...) {}
        auto p = repo.findById(id);
        if (!p) std::cout << "未检索到相应信息。\n"; else printEmployee(*p);
    } else {
        std::cout << "无效选择。\n";
    }
}

static void promoteAll(Repository &repo) {
    for (auto &e : repo.items) e.level += 1;
    repo.save();
    std::cout << "所有人员已提升级别(+1)。\n";
}

static void stats(Repository &repo) {
    double total = 0.0;
    double m=0.0, t=0.0, sm=0.0, s=0.0;
    for (const auto &e : repo.items) {
        double pay = e.monthlyPay();
        total += pay;
        switch (e.role) {
            case Role::Manager: m += pay; break;
            case Role::PartTimeTech: t += pay; break;
            case Role::SalesManager: sm += pay; break;
            case Role::PartTimeSales: s += pay; break;
        }
    }
    std::cout << "总工资: " << std::fixed << std::setprecision(2) << total << "\n";
    if (total > 0) {
        std::cout << "占比(经理/技术/销售经理/推销): "
                  << m/total*100 << "% / "
                  << t/total*100 << "% / "
                  << sm/total*100 << "% / "
                  << s/total*100 << "%\n";
    }
}

static void ranking(Repository &repo) {
    std::vector<Employee> arr = repo.items;
    std::sort(arr.begin(), arr.end(), [](const Employee &a, const Employee &b){ return a.monthlyPay() > b.monthlyPay(); });
    std::cout << "个人业绩(月薪)排名: \n";
    for (const auto &e : arr) printEmployee(e);
}

static void menu() {
    std::cout << "\n企业人力管理系统\n"
                 "1. 添加人员\n"
                 "2. 删除人员\n"
                 "3. 检索/显示人员\n"
                 "4. 列出全部\n"
                 "5. 统计工资及占比\n"
                 "6. 全员提级\n"
                 "7. 业绩(月薪)排名\n"
                 "0. 退出\n"
                 "请选择: ";
    std::cout.flush();
}

int main() {
#ifdef _WIN32
    // 设置 Windows 控制台使用 UTF-8，解决中文乱码
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(&std::cout);
    std::string csvPath = "../data/employees.csv"; // 相对可执行文件位置
    // 为在源码目录运行方便，若相对路径不存在，尝试仓库根下 data 路径
    {
        std::ifstream test(csvPath);
        if (!test.is_open()) csvPath = "data/employees.csv";
    }

    Repository repo{csvPath};
    repo.load();
    std::cout << "数据文件: " << csvPath << "\n";

    while (true) {
        menu();
        std::string choice; std::getline(std::cin, choice);
        if (choice == "1") addEmployee(repo);
        else if (choice == "2") removeEmployee(repo);
        else if (choice == "3") findEmployee(repo);
        else if (choice == "4") listAll(repo);
        else if (choice == "5") stats(repo);
        else if (choice == "6") promoteAll(repo);
        else if (choice == "7") ranking(repo);
        else if (choice == "0" || choice == "q" || choice == "Q") break;
        else std::cout << "无效选择，请重试。\n";
    }

    std::cout << "已退出。\n";
#ifdef _WIN32
    std::cout << "按回车键关闭窗口..." << std::endl;
    std::string dummy; std::getline(std::cin, dummy);
#endif
    return 0;
}
