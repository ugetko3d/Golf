#include "Golf.h"

Golf::Golf(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_messageBox = new QMessageBox(parent);

    setupGUI(parent);

    createDatabaseTables();

    fetchAll();

    connect(ui.btn_reg_golfer, &QPushButton::released, this, &Golf::onButtonRegGolferClicked);
    connect(ui.btn_reg_course, &QPushButton::released, this, &Golf::onButtonRegCourseClicked);
    connect(ui.btn_reg_scorecard, &QPushButton::released, this, &Golf::onButtonRegScorecardClicked);
    connect(ui.list_show_scorecards, &QListWidget::itemSelectionChanged, this, &Golf::onSelectScorecardClicked);
}

void Golf::onButtonRegScorecardClicked()
{
    // Check if input is valid
    if (ui.cb_sel_golfer->currentIndex() == -1 || ui.cb_sel_course->currentIndex() == -1)
    {
        m_messageBox->setText("ERROR: player or course cannot be empty!");
        m_messageBox->show();
        return;
    }

    std::string golferName = ui.cb_sel_golfer->currentText().toStdString();
    std::string courseName = ui.cb_sel_course->currentText().toStdString();

    QDate date = ui.date_sel_time->date();
    std::string dateString = date.toString().toStdString();

    std::string score;
    QSpinBox* sb;
    for (int i = 0; i < 18; i++)
    {
        sb = dynamic_cast<QSpinBox*>(ui.tbl_sel_strokes->cellWidget(i, 0));
        score += std::to_string(sb->value());
    }

    // Insert scorecard to database
    openDatabase();

    m_sqlQuery = "INSERT INTO scorecard VALUES ('" + golferName + "', '" + courseName + "', '" + dateString.c_str() + "', '" + score + "')";
    executeQuery();

    closeDatabase();

    // Add scorecard to list
    std::string tmpStr;
    tmpStr += dateString;
    tmpStr += " : ";
    tmpStr += golferName;
    tmpStr += " : ";
    tmpStr += courseName;
    QString str = QString::fromUtf8(tmpStr);

    ui.list_show_scorecards->addItem(str);
}

void Golf::onSelectScorecardClicked()
{
    std::string scorecard = ui.list_show_scorecards->currentItem()->text().toStdString();
    std::string delimiter = " : ";

    std::vector<std::string> strings;
    int pos = 0;

    while ((pos = scorecard.find(" : ")) != std::string::npos) {
        strings.emplace_back(scorecard.substr(0, pos));
        scorecard.erase(0, pos + delimiter.length());
    }

    std::string player_name = strings.at(1);
    std::string course_name = scorecard;
    std::string time = strings.at(0);

    std::vector<std::string> columns;
    columns.emplace_back(player_name);
    columns.emplace_back(course_name);
    columns.emplace_back(time);

    std::string score;
    m_sqlQuery = "SELECT * FROM scorecard WHERE player_name = ? AND course_name = ? AND time = ?;";
    fetchItems(Type::SCORECARD, columns, &score);
 
    for (int i = 0; i < 18; i++)
    {
        ui.tbl_show_strokes->setItem(i, 0, new QTableWidgetItem);
        ui.tbl_show_strokes->item(i, 0)->setText(QString(score[i]));
    }
       
}

void Golf::onButtonRegGolferClicked()
{
    // Check if input is valid
    std::string name = ui.txt_reg_golferName->toPlainText().toStdString();

    if (name.empty())
    {
        m_messageBox->setText("ERROR: player name cannot be empty!");
        m_messageBox->show();
        return;
    }

    int hcp = ui.spin_reg_handicap->value();

    openDatabase();

    // Update max id
    m_sqlQuery = "SELECT MAX(player_id) FROM player;";
    executeQuery();

    Player player(m_count, name, hcp);

    // Insert player to database
    m_sqlQuery = "INSERT INTO player VALUES ('" + std::to_string(player.getId()) + "', '" + player.getName() + "', '" + std::to_string(player.getHandicap()) + "')";
    executeQuery();

    if (m_rc)
    {
        closeDatabase();
        return;
    }

    closeDatabase();

    // Add player to combobox
    ui.cb_sel_golfer->addItem(player.getName().c_str());
}

void Golf::onButtonRegCourseClicked()
{
    // Check if input is valid
    std::string name = ui.txt_reg_courseName->toPlainText().toStdString();

    if (name.empty())
    {
        m_messageBox->setText("ERROR: course name cannot be empty!");
        m_messageBox->show();
        return;
    }

    openDatabase();

    // Update max id
    m_sqlQuery = "SELECT MAX(course_id) FROM course;";
    executeQuery();

    Course course(m_count, name);

    // Insert course to database
    m_sqlQuery = "INSERT INTO course VALUES ('" + std::to_string(course.getId()) + "', '" + course.getName() + "')";
    executeQuery();

    if (m_rc)
    {
        closeDatabase();
        return;
    }

    // Update max id
    m_sqlQuery = "SELECT MAX(hole_id) FROM hole;";
    executeQuery();

    // Insert holes to database
    QSpinBox* sb;
    QCheckBox* cb;
    for (int i = 0; i < 18; i++)
    {
        Hole hole(m_count + i, i + 1);

        // Get par value
        sb = dynamic_cast<QSpinBox*>(ui.tbl_reg_holes->cellWidget(i, 0));
        hole.setPar(sb->value());

        // Get handicap value
        sb = dynamic_cast<QSpinBox*>(ui.tbl_reg_holes->cellWidget(i, 1));
        hole.setHandicap(sb->value());

        // Get length value
        sb = dynamic_cast<QSpinBox*>(ui.tbl_reg_holes->cellWidget(i, 2));
        hole.setLength(sb->value());

        // Get water value
        cb = dynamic_cast<QCheckBox*>(ui.tbl_reg_holes->cellWidget(i, 3));
        hole.setIsWater(cb->isChecked());

        // Get fairway bunker value
        cb = dynamic_cast<QCheckBox*>(ui.tbl_reg_holes->cellWidget(i, 4));
        hole.setIsFairwayBunker(cb->isChecked());

        // Get green bunker value
        cb = dynamic_cast<QCheckBox*>(ui.tbl_reg_holes->cellWidget(i, 5));
        hole.setIsGreenBunker(cb->isChecked());

        m_sqlQuery = "INSERT INTO hole VALUES ('" + std::to_string(hole.getId()) + "', '" + std::to_string(hole.getHoleNr()) + "', \
                                               '" + std::to_string(hole.getPar()) + "', '" + std::to_string(hole.getHandicap()) + "', \
                                               '" + std::to_string(hole.getLength()) + "', '" + std::to_string(hole.getIsWater()) + "', \
                                               '" + std::to_string(hole.getIsFairwayBunker()) + "', '" + std::to_string(hole.getIsGreenBunker()) + "', \
                                               '" + std::to_string(course.getId()) + "')";
        executeQuery();
    }

    closeDatabase();

    // Add course to combobox
    ui.cb_sel_course->addItem(course.getName().c_str());
}

void Golf::executeQuery()
{  
    m_rc = sqlite3_exec(m_db, m_sqlQuery.c_str(), callback, &m_count, &m_errMsg);
    if (m_rc)
    {
        m_messageBox->setText("SQL Query failed:\n" + QString(sqlite3_errmsg(m_db)));
        m_messageBox->show();
    }
}

void Golf::openDatabase()
{
    m_rc = sqlite3_open("golf.sqlite3", &m_db);
    if (m_rc)
    {
        m_messageBox->setText("Can't open database:\n" + QString(sqlite3_errmsg(m_db)));
        m_messageBox->show();
    }
}

void Golf::closeDatabase()
{
    m_rc = sqlite3_close(m_db);
    if (m_rc)
    {
        m_messageBox->setText("Can't close database:\n" + QString(sqlite3_errmsg(m_db)));
        m_messageBox->show();
    }
}

int Golf::callback(void* count, int argc, char** argv, char** azColName)
{
    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    int* c = (int*) count;
    if (argv[0] == nullptr)
        *c = 1;
    else
        *c = atoi(argv[0]) + 1;

    // Return successful
    return 0;
}

void Golf::createDatabaseTables()
{
    openDatabase();

    m_sqlQuery = "CREATE TABLE IF NOT EXISTS course ("  \
        "course_id INT     PRIMARY KEY NOT NULL," \
        "course_name TEXT              NOT NULL UNIQUE);";
    executeQuery();

    m_sqlQuery = "CREATE TABLE IF NOT EXISTS hole ("  \
        "hole_id INT PRIMARY KEY    NOT NULL," \
        "number INT                NOT NULL," \
        "par INT                    NOT NULL," \
        "hole_hcp INT               NOT NULL," \
        "length INT                 NOT NULL," \
        "water BIT                  NOT NULL," \
        "fairway_bunker BIT         NOT NULL," \
        "green_bunker BIT           NOT NULL," \
        "course_id INT              NOT NULL," \
        "FOREIGN KEY (course_id) REFERENCES course (course_id));";
    executeQuery();

    m_sqlQuery = "CREATE TABLE IF NOT EXISTS player ("  \
        "player_id INT   PRIMARY KEY   NOT NULL," \
        "player_name TEXT              NOT NULL UNIQUE," \
        "player_hcp INT                NOT NULL);";
    executeQuery();

    m_sqlQuery = "CREATE TABLE IF NOT EXISTS scorecard ("  \
        "player_name TEXT                    NOT NULL," \
        "course_name TEXT               NOT NULL," \
        "time TEXT               NOT NULL," \
        "score TEXT               NOT NULL," \
        "FOREIGN KEY (player_name) REFERENCES player (player_name)," \
        "FOREIGN KEY (course_name) REFERENCES course (course_name));";
    executeQuery();

    closeDatabase();
}

void Golf::setupGUI(QWidget* parent)
{
    // Add spinboxes to register scorecard table
    for (int i = 0; i < 18; i++)
    {
        QSpinBox* sb = new QSpinBox(parent);
        sb->setMinimum(1);
        sb->setMaximum(9);
        ui.tbl_sel_strokes->setCellWidget(i, 0, sb);
    }

    // Add necessary widgets to register course table
    QSpinBox* sb = new QSpinBox(parent);
    QCheckBox* cb = new QCheckBox(parent);

    for (int i = 0; i < 18; i++)
    {
        // Par spinbox
        sb = new QSpinBox(parent);
        sb->setMinimum(3);
        sb->setMaximum(5);
        sb->setValue(4);
        ui.tbl_reg_holes->setCellWidget(i, 0, sb);

        // Handicap spinbox
        sb = new QSpinBox(parent);
        sb->setMinimum(1);
        sb->setMaximum(54);
        sb->setValue(30);
        ui.tbl_reg_holes->setCellWidget(i, 1, sb);

        // Length spinbox
        sb = new QSpinBox(parent);
        sb->setMinimum(1);
        sb->setMaximum(999);
        sb->setValue(350);
        ui.tbl_reg_holes->setCellWidget(i, 2, sb);

        // Water checkbox
        cb = new QCheckBox(parent);
        cb->setChecked(false);
        ui.tbl_reg_holes->setCellWidget(i, 3, cb);

        // Fairway bunker checkbox
        cb = new QCheckBox(parent);
        cb->setChecked(true);
        ui.tbl_reg_holes->setCellWidget(i, 4, cb);

        // Green bunker checkbox
        cb = new QCheckBox(parent);
        cb->setChecked(true);
        ui.tbl_reg_holes->setCellWidget(i, 5, cb);

    }

    ui.tbl_reg_holes->horizontalHeader()->setVisible(true);
    ui.tbl_reg_holes->verticalHeader()->setVisible(true);
    ui.tbl_reg_holes->setColumnWidth(0, 30);
    ui.tbl_reg_holes->setColumnWidth(1, 60);
    ui.tbl_reg_holes->setColumnWidth(2, 60);
    ui.tbl_reg_holes->setColumnWidth(3, 60);

    // Set default tab to first one
    ui.tabWidget->setCurrentIndex(0);
}

void Golf::fetchAll()
{
    openDatabase();

    // Fetch players
    std::vector<Player> players;
    m_sqlQuery = "SELECT * FROM player;";
    m_rc = sqlite3_exec(m_db, m_sqlQuery.c_str(), fetchAllCallback, &players, &m_errMsg);
    for (Player player : players)
        ui.cb_sel_golfer->addItem(player.getName().c_str());

    // Fetch courses
    std::vector<Course> courses;
    m_sqlQuery = "SELECT * FROM course;";
    m_rc = sqlite3_exec(m_db, m_sqlQuery.c_str(), fetchAllCallback, &courses, &m_errMsg);
    for (Course course : courses)
        ui.cb_sel_course->addItem(course.getName().c_str());

    // Fetch scorecards
    std::vector<std::string> strings;
    m_sqlQuery = "SELECT * FROM scorecard;";
    m_rc = sqlite3_exec(m_db, m_sqlQuery.c_str(), fetchAllCallback, &strings, &m_errMsg);
    for (std::string s : strings)
    {
        QString str = QString::fromUtf8(s);
        ui.list_show_scorecards->addItem(str);
    }
       
    closeDatabase();
}

int Golf::fetchAllCallback(void* data, int argc, char** argv, char** azColName)
{
    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    if (azColName[0] != nullptr && strcmp(azColName[0], "player_id") == 0)
    {
        std::vector<Player>* players = static_cast<std::vector<Player>*>(data);
        int id = (int) atoi(argv[0]);
        std::string name = argv[1];
        int hcp = (int)atoi(argv[2]);

        players->emplace_back(Player(id, name, hcp));
    }
    else if (azColName[0] != nullptr && strcmp(azColName[0], "course_id") == 0)
    {
        std::vector<Course>* courses = static_cast<std::vector<Course>*>(data);
        int id = (int)atoi(argv[0]);
        std::string name = argv[1];

        courses->emplace_back(Course(id, name));
    }
    else if (azColName[0] != nullptr && strcmp(azColName[0], "player_name") == 0)
    {
        std::vector<std::string>* strings = static_cast<std::vector<std::string>*>(data);

        std::string tmpStr;
        tmpStr += argv[2];
        tmpStr += " : ";
        tmpStr += argv[0];
        tmpStr += " : ";
        tmpStr += argv[1];

        strings->emplace_back(tmpStr);
    }
    
    return 0;
}

void Golf::fetchItems(Type t, const std::vector<std::string>& columns, void* data)
{
    // Fetch from database
    openDatabase();   

    sqlite3_stmt* stmt = nullptr;
    m_rc = sqlite3_prepare_v2(m_db, m_sqlQuery.c_str(), -1, &stmt, nullptr);
    if (m_rc)
    {
        m_messageBox->setText("Can't prepare SQL statement");
        m_messageBox->show();
    }

    if (t == Type::SCORECARD)
    {
        sqlite3_bind_text(stmt, 1, columns.at(0).c_str(), strlen(columns.at(0).c_str()), nullptr);
        sqlite3_bind_text(stmt, 2, columns.at(1).c_str(), strlen(columns.at(1).c_str()), nullptr);
        sqlite3_bind_text(stmt, 3, columns.at(2).c_str(), strlen(columns.at(2).c_str()), nullptr);
    } 
    else 
        sqlite3_bind_text(stmt, 1, columns.at(0).c_str(), strlen(columns.at(0).c_str()), nullptr);

    do
    {
        m_rc = sqlite3_step(stmt);
        switch (m_rc)
        {
            /** No more data */
        case SQLITE_DONE:
            break;

            /** New data */
        case SQLITE_ROW:
        {
            if (t == Type::PLAYER)
            {
                std::vector<Player>* players = static_cast<std::vector<Player>*>(data);
                players->emplace_back(Player(sqlite3_column_int(stmt, 0), (const char*)sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2)));
                
            }
            else if (t == Type::COURSE)
            {
                std::vector<Course>* courses = static_cast<std::vector<Course>*>(data);
                courses->emplace_back(Course(sqlite3_column_int(stmt, 0), (const char*)sqlite3_column_text(stmt, 1)));
            }
            else if (t == Type::SCORECARD)
            {
                std::string* score = static_cast<std::string*>(data);
                *score = (const char*) sqlite3_column_text(stmt, 3);
            }

        }
        break;

        default:
            break;
        }
    } while (m_rc == SQLITE_ROW);

    m_rc = sqlite3_finalize(stmt);

    closeDatabase();
}
