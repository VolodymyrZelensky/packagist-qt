#ifndef SEARCH_H
#define SEARCH_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QUrl>
#include <QClipboard>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class search; }
QT_END_NAMESPACE

class search : public QMainWindow
{
    Q_OBJECT

public:
    search(QWidget *parent = nullptr);
    ~search();

private slots:
    void on_SearchQuery_textChanged(const QString &query);
    void searched(QNetworkReply *reply);
    void nextPage();
    void backPage();
    void firstPage();
    void lastPage();
    void copyCommand();
    void openRepository();

private:
    QPushButton *btnGoBack;
    QPushButton *btnGoForward;
    QPushButton *btnHardGoBack;
    QPushButton *btnHardGoForward;
    QLabel *searchStatus;
    void Statusvisibility(bool toBe);
    void UpdateStatus();
    QString currentQuery;
    int totalPages =0;
    int currentPage =0;
    bool canGoBack =false;
    bool canGoForward=false;
    bool AreYouOnline();
    Ui::search *ui;
};
#endif // SEARCH_H
