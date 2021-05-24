#include <iostream>
#include "sqlite/sqlite3.h"
#include <vector>
#include <string>

using namespace std;

int id = -1;

static int callback(void* data, int argc, char** argv, char** azColName) {
    ::id = atoi(argv[0]);
    return 0;
}

vector <int> LinkId;

static int callback1(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(azColName[i], "LinkId")) {
            ::LinkId.push_back(atoi(argv[i]));
        }
    }
    return 0;
}


static int callback2(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; ++i) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


int main(int argc, char** argv){
    string sql1;
    cin >> sql1;
    sqlite3* DB;
    string sql("select * from Words where Word ='" + sql1 + "'");
    int exit = 0;
    exit = sqlite3_open("Searching.db", &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messaggeError);
    if (::id == -1) {
        string sql2("INSERT INTO Words (Word) VALUES('" + sql1 + "')");
        exit = sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messaggeError);
    }
    else {
        int i = ::id;
        auto id1 = std::to_string(i);
        string sql3("Select * from Connections where WordId="+ id1);
        int exit_code = sqlite3_exec(DB, sql3.c_str(), callback1, 0, &messaggeError);
        for (int i = 0; i < LinkId.size(); ++i) {
            auto id2 = std::to_string(LinkId[i]);
            string sql4("Select Link from Link Where Id=" + id2);
            int exit_code = sqlite3_exec(DB, sql4.c_str(), callback2, 0, &messaggeError);
        }
    }
    sqlite3_close(DB);
    return (0);
}