# Huawei-GaussDB-Database-Challenge
第三届华为云GaussDB数据库挑战赛季军
##### 比赛链接: https://competition.huaweicloud.com/information/1000041405/introduction

### 作品内容
实现了一个小型SQL引擎，能够高效实现单表查询，两表连接，基本能够跑过赛题要求的所有SQL语句，实际通过测试用例97.67%，总耗时1172ms，在复赛中排名第三。


### 具体功能
①表的建立；②数据的插入，支持int和text两种数据类型；③表的查询，可以是单表查询，两表连接查询。支持多个查询条件，以and，or连接。查询条件可以是等值，不等值，比较运算，包括对text之间的比较。被查询的列名可以是*，column，table.column三种形式，可以重复。


### 作品特点
1. 与读入有关的部分有一定鲁棒性
2. 比较高效，能够根据表的规模选取合适的查询算法，在O(n*logn)时间内完成连接查询
3. 代码可扩展性好，易于增加更多功能模块


### 不足之处
为了比赛而写的代码，其架构并不适用于实际应用，例如将读入的数据，无论是int还是text，均以string形式存储在二维表中，因而消耗了更多的存储空间。


### 使用说明
源码只有一个gaussdb.cpp，可以直接编译运行。与sqlite等数据库使用方法相同，直接输入建表，插入，查询的SQL语句即可，但只支持赛题要求的SQL语句，且不涉及磁盘读写。提供了一份测试用例，基本覆盖了赛题要求的全部功能，可以直接复制进程序进行测试，并生成了一份两万行的额外的随机数据，用以测试大数据量情况下的性能。
