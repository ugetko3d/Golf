#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QCheckBox>
#include "ui_Golf.h"

#include <sqlite3.h>

#include <iostream>
#include <vector>

#include "Player.h"
#include "Course.h"
#include "Hole.h"
#include "Scorecard.h"

class Golf : public QMainWindow
{
    Q_OBJECT

public:
    Golf(QWidget *parent = Q_NULLPTR);

private:
    static int callback(void* count, int argc, char** argv, char** azColName);
    static int fetchAllCallback(void* data, int argc, char** argv, char** azColName);

    enum class Type { PLAYER, COURSE, SCORECARD, HOLE };

    void openDatabase();
    void closeDatabase();
    void executeQuery();
    void createDatabaseTables();
    void setupGUI(QWidget* parent);
    void fetchAll();
    void fetchItems(Type t, const std::vector<std::string>& columns, void* data);

    // Qt variables
    Ui::GolfClass ui;
    QMessageBox* m_messageBox;

    // sqlite3 variables
    sqlite3* m_db;
    int m_rc;
    char* m_errMsg = 0;
    int m_count = 0;
    std::string m_sqlQuery;



private slots:
    void onButtonRegScorecardClicked();
    void onSelectScorecardClicked();
    void onButtonRegGolferClicked();
    void onButtonRegCourseClicked();
};
