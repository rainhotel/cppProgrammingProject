# 企业人力管理系统 (C++)

功能覆盖课程设计要求：
- 人员信息的读取/增加/删除/保存（CSV 持久化）。
- 按姓名或编号检索、显示。
- 统计总工资与各类占比；个人业绩(月薪)排名。
- 全员提级；错误输入提示（性别校验等）。
- 角色与薪资：
  - 经理：固定月薪(`baseSalary`)
  - 兼职技术：时薪(`baseSalary`) * 工时(`hours`)
  - 兼职推销员：销售额(`sales`) * 提成比例(`baseSalary`，0~1)
  - 销售经理：固定月薪(`baseSalary`) + 销售额 * (提成比例的20%)

> 提示：提成比例/权重可按需求在代码中调整。

## 项目结构
- `src/main.cpp`：控制台程序与核心逻辑（为教学简化集中于单文件）。
- `data/employees.csv`：CSV 数据文件。

## 构建与运行（Windows / PowerShell）
请确保已安装 C++ 编译器（例如 `g++`/MinGW 或 VS 的 `cl`）。

使用 g++（MinGW 示例）：
```powershell
cd "D:\moniC\project\cppClassDesign\cppProgrammingProject"
mkdir build
# 编译
g++ -std=c++17 -O2 -o build\hr.exe src\main.cpp
# 运行（可在仓库根运行，程序自动尝试 data 路径）
./build/hr.exe
```

使用 MSVC `cl`：
```powershell
cd "D:\moniC\project\cppClassDesign\cppProgrammingProject"
mkdir build
cl /EHsc /std:c++17 /Fe:build\hr.exe src\main.cpp
./build/hr.exe
```

## CSV 格式
表头：
```
id,name,role,level,gender,baseSalary,hours,sales
```
- `role` 枚举：`Manager|PartTimeTech|SalesManager|PartTimeSales`
- `gender`：`男|女`

示例行：
```
1,张三,Manager,3,男,15000,0,0
2,李四,PartTimeTech,1,女,80,160,0
3,王五,PartTimeSales,2,男,0.05,0,200000
4,赵六,SalesManager,4,女,12000,0,500000
```

## 使用说明
- 程序启动后显示菜单；按提示输入。
- 新增人员时需按岗位输入工时/销售额；性别仅允许“男/女”。
- 删除人员按编号；检索支持姓名/编号。
- “统计工资及占比”按各类岗位合计占比；“排名”按当月薪资排序。
- “全员提级”统一 `level += 1`。

## 备注
- 若需改用 JSON/SQLite 存储，可在后续迭代替换持久化层。
- 如需生日提醒、转正提醒、用户管理等扩展，可在 CSV 增加字段并在菜单增设功能。
