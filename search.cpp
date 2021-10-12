#include "search.h"
#include "ui_search.h"

search::search(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::search)
{
    ui->setupUi(this);
    ui->busyGif->setHidden(true);
    ui->resultsBox->setHidden(true);
    btnGoBack = new QPushButton();
    btnHardGoBack = new QPushButton();
    btnGoForward = new QPushButton();
    btnHardGoForward = new QPushButton();
    searchStatus = new QLabel();
    searchStatus->setStyleSheet("color:#212121");

    btnGoBack->setIcon(QIcon::fromTheme("draw-arrow-back"));
    btnGoBack->setGeometry(QRect(10, 220, 31, 30));
    btnGoBack->setMinimumSize(QSize(31, 30));
    btnGoBack->setMaximumSize(QSize(31, 30));

    btnGoForward->setIcon(QIcon::fromTheme("draw-arrow-forward"));
    btnGoForward->setGeometry(QRect(10, 220, 31, 30));
    btnGoForward->setMinimumSize(QSize(31, 30));
    btnGoForward->setMaximumSize(QSize(31, 30));

    btnHardGoBack->setIcon(QIcon::fromTheme("media-seek-backward"));
    btnHardGoBack->setGeometry(QRect(10, 220, 31, 30));
    btnHardGoBack->setMinimumSize(QSize(31, 30));
    btnHardGoBack->setMaximumSize(QSize(31, 30));

    btnHardGoForward->setIcon(QIcon::fromTheme("media-seek-forward"));
    btnHardGoForward->setGeometry(QRect(10, 220, 31, 30));
    btnHardGoForward->setMinimumSize(QSize(31, 30));
    btnHardGoForward->setMaximumSize(QSize(31, 30));

    statusBar()->addWidget(btnGoBack);
    statusBar()->addWidget(btnHardGoBack);
    statusBar()->addWidget(searchStatus->window());
    statusBar()->addWidget(btnHardGoForward);
    statusBar()->addWidget(btnGoForward);

    connect(btnGoBack, SIGNAL(clicked()), this, SLOT(backPage()));
    connect(btnGoForward, SIGNAL(clicked()), this, SLOT(nextPage()));
    connect(btnHardGoBack, SIGNAL(clicked()), this, SLOT(firstPage()));
    connect(btnHardGoForward, SIGNAL(clicked()), this, SLOT(lastPage()));

    Statusvisibility(false);
    UpdateStatus();

}

search::~search()
{delete ui;}

bool search::AreYouOnline(){
    QTcpSocket* sock = new QTcpSocket(this);
    sock->connectToHost("google.com", 80);
    bool connected = sock->waitForConnected(30000);
    if (!connected){
        sock->abort();
        return false;
    }
    sock->close();
    return true;
}

void search::on_SearchQuery_textChanged(const QString &query)
{
    if(query == nullptr){
        UpdateStatus();
        Statusvisibility(false);
        ui->Intro->setHidden(false);
        ui->resultsBox->setHidden(true);
        ui->resultsBox->setWidget(nullptr);
    }else{
        if(AreYouOnline()){
            ui->busyGif->setHidden(false);
            QUrl url("https://m58222sh95-dsn.algolia.net/1/indexes/packagist");
            QUrlQuery params;
            params.addQueryItem("query", query);
            params.addQueryItem("page", "0");
            url.setQuery(params);
            QNetworkRequest request(url);
            request.setRawHeader("x-algolia-agent", "Algolia for vanilla JavaScript (lite) 3.27.0;instantsearch.js 2.7.4;JS Helper 2.26.0");
            request.setRawHeader("x-algolia-application-id", "M58222SH95");
            request.setRawHeader("x-algolia-api-key", "5ae4d03c98685bd7364c2e0fd819af05");
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            manager->get(request);
            connect(manager, &QNetworkAccessManager::finished, this, &search::searched);
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle("Network Conection !");
            messageBox.setText("You're Offline.");
            messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
            messageBox.exec();
        }
    }
}

void search::searched(QNetworkReply *reply)
{
    ui->busyGif->setHidden(true);
    QByteArray result = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(result);
    QJsonObject JON = doc.object();
    QJsonArray hits = JON.value("hits").toArray();
    QWidget *tba = new QWidget;
    tba->setProperty("class", "blank");
    QVBoxLayout *VBoxArea = new QVBoxLayout(tba);
    if(hits.count() >= 1){
        for (int i=0;i<hits.count();i++) {
            QJsonObject index = hits[i].toObject();
            QGroupBox *boxResult;
            QVBoxLayout *gridLayout;
            QLabel *labelResult;
            QPushButton *btnCommand, *btnHomePage;
            QHBoxLayout *buttons;
            boxResult = new QGroupBox(VBoxArea->widget());
            boxResult->setGeometry(QRect(10, 10, 781, 101));
            boxResult->setMinimumSize(QSize(0, 111));
            boxResult->setMaximumSize(QSize(16777215, 171));;
            boxResult->setProperty("class", "resp-box");
            VBoxArea->addWidget(boxResult);
            gridLayout = new QVBoxLayout(boxResult);
            labelResult = new QLabel(boxResult);
            labelResult->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
            labelResult->setWordWrap(true);
            gridLayout->addWidget(labelResult);
            btnCommand = new QPushButton(boxResult);
            btnHomePage = new QPushButton(boxResult);
            buttons = new QHBoxLayout();
            buttons->addWidget(btnCommand);
            buttons->addWidget(btnHomePage);
            gridLayout->addLayout(buttons);

            btnCommand->setMinimumSize(QSize(0, 35));
            btnCommand->setMaximumSize(QSize(16777215, 35));
            btnHomePage->setMinimumSize(QSize(0, 35));
            btnHomePage->setMaximumSize(QSize(16777215, 35));

            btnCommand->setProperty("class", "resp-btn");
            btnHomePage->setProperty("class", "resp-btn");
            btnCommand->setObjectName("button-" + QString::number(i) + "-command");
            btnHomePage->setObjectName("button-" + QString::number(i) + "-homepage");
            btnCommand->setProperty("ObjectID", index.value("name").toString());
            btnHomePage->setProperty("HomePage", index.value("repository").toString());
            btnCommand->setCursor(QCursor(Qt::PointingHandCursor));
            btnHomePage->setCursor(QCursor(Qt::PointingHandCursor));
            boxResult->setTitle(index.value("name").toString());
            labelResult->setText(index.value("description").toString());
            btnCommand->setText("$ composer require " + index.value("name").toString());
            btnHomePage->setText("open repository");
            connect(btnCommand, SIGNAL(clicked()), this, SLOT(copyCommand()));
            connect(btnHomePage, SIGNAL(clicked()), this, SLOT(openRepository()));
        }
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("No Results");
        messageBox.setText("you're search for `" + JON.value("query").toString() + "` has no results!, you can try again with some difrent words.");
        messageBox.exec();
        //ui->SearchQuery->clear();
        return;
    }
    ui->resultsBox->setHidden(false);
    ui->Intro->setHidden(true);
    this->currentQuery= JON.value("query").toString();
    this->currentPage = JON.value("page").toInt();
    this->totalPages  = JON.value("nbPages").toInt()-1;
    ui->resultsBox->setWidget(tba);
    Statusvisibility(true);
    UpdateStatus();
}

void search::nextPage()
{
    if(canGoForward){
        if(AreYouOnline()){
            ui->busyGif->setHidden(false);
            QUrl url("https://m58222sh95-dsn.algolia.net/1/indexes/packagist");
            QUrlQuery params;
            params.addQueryItem("query", currentQuery);
            params.addQueryItem("page", QString::number(currentPage+1));
            url.setQuery(params);
            QNetworkRequest request(url);
            request.setRawHeader("x-algolia-agent", "Algolia for vanilla JavaScript (lite) 3.27.0;instantsearch.js 2.7.4;JS Helper 2.26.0");
            request.setRawHeader("x-algolia-application-id", "M58222SH95");
            request.setRawHeader("x-algolia-api-key", "5ae4d03c98685bd7364c2e0fd819af05");
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            manager->get(request);
            connect(manager, &QNetworkAccessManager::finished, this, &search::searched);
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle("Network Conection !");
            messageBox.setText("You're Offline.");
            messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
            messageBox.exec();
        }
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("cant go forward");
        messageBox.setText("You're Offline.");
        messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
        messageBox.exec();
    }
}

void search::backPage()
{
    if(canGoBack){
        if(AreYouOnline()){
            ui->busyGif->setHidden(false);
            QUrl url("https://m58222sh95-dsn.algolia.net/1/indexes/packagist");
            QUrlQuery params;
            params.addQueryItem("query", currentQuery);
            params.addQueryItem("page", QString::number(currentPage-1));
            url.setQuery(params);
            QNetworkRequest request(url);
            request.setRawHeader("x-algolia-agent", "Algolia for vanilla JavaScript (lite) 3.27.0;instantsearch.js 2.7.4;JS Helper 2.26.0");
            request.setRawHeader("x-algolia-application-id", "M58222SH95");
            request.setRawHeader("x-algolia-api-key", "5ae4d03c98685bd7364c2e0fd819af05");
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            manager->get(request);
            connect(manager, &QNetworkAccessManager::finished, this, &search::searched);
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle("Network Conection !");
            messageBox.setText("You're Offline.");
            messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
            messageBox.exec();
        }
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("cant go back");
        messageBox.setText("You're Offline.");
        messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
        messageBox.exec();
    }
}

void search::firstPage()
{
    if(canGoBack){
        if(AreYouOnline()){
            ui->busyGif->setHidden(false);
            QUrl url("https://m58222sh95-dsn.algolia.net/1/indexes/packagist");
            QUrlQuery params;
            params.addQueryItem("query", currentQuery);
            params.addQueryItem("page", "0");
            url.setQuery(params);
            QNetworkRequest request(url);
            request.setRawHeader("x-algolia-agent", "Algolia for vanilla JavaScript (lite) 3.27.0;instantsearch.js 2.7.4;JS Helper 2.26.0");
            request.setRawHeader("x-algolia-application-id", "M58222SH95");
            request.setRawHeader("x-algolia-api-key", "5ae4d03c98685bd7364c2e0fd819af05");
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            manager->get(request);
            connect(manager, &QNetworkAccessManager::finished, this, &search::searched);
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle("Network Conection !");
            messageBox.setText("You're Offline.");
            messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
            messageBox.exec();
        }
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("cant go back");
        messageBox.setText("You're Offline.");
        messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
        messageBox.exec();
    }
}

void search::lastPage()
{
    if(canGoForward){
        if(AreYouOnline()){
            ui->busyGif->setHidden(false);
            QUrl url("https://m58222sh95-dsn.algolia.net/1/indexes/packagist");
            QUrlQuery params;
            params.addQueryItem("query", currentQuery);
            params.addQueryItem("page", QString::number(totalPages));
            url.setQuery(params);
            QNetworkRequest request(url);
            request.setRawHeader("x-algolia-agent", "Algolia for vanilla JavaScript (lite) 3.27.0;instantsearch.js 2.7.4;JS Helper 2.26.0");
            request.setRawHeader("x-algolia-application-id", "M58222SH95");
            request.setRawHeader("x-algolia-api-key", "5ae4d03c98685bd7364c2e0fd819af05");
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            manager->get(request);
            connect(manager, &QNetworkAccessManager::finished, this, &search::searched);
        }else{
            QMessageBox messageBox;
            messageBox.setWindowTitle("Network Conection !");
            messageBox.setText("You're Offline.");
            messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
            messageBox.exec();
        }
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("cant go forward");
        messageBox.setText("You're Offline.");
        messageBox.setDetailedText("if you're ot offline thne please turn off your vpn or check you're network connection.");
        messageBox.exec();
    }
}

void search::copyCommand()
{
    QClipboard *clipboard = QApplication::clipboard();
    QPushButton* btnSend = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
    QString ObjectID = btnSend->property("ObjectID").toString();
    clipboard->setText("composer require " + ObjectID);
}

void search::openRepository()
{
    QPushButton* btnSend = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
    QString HomePage = btnSend->property("HomePage").toString();
    QDesktopServices::openUrl(QUrl(HomePage));
}

void search::Statusvisibility(bool toBe)
{
    if(toBe) statusBar()->show();
    else statusBar()->hide();
}

void search::UpdateStatus()
{
    if(currentPage == 0 && totalPages == 0){
        searchStatus->setText(" write somting to search ");
    }else{
        searchStatus->setText(" page " + QString::number(currentPage) + " of " + QString::number(totalPages) + " ");
    }
    if(totalPages > currentPage){
        if(currentPage == 0){
            this->canGoBack = false;
            this->canGoForward = true;

            btnGoBack->setDisabled(true);
            btnHardGoBack->setDisabled(true);

            btnGoForward->setEnabled(true);
            btnHardGoForward->setEnabled(true);
        }else{
            this->canGoBack = true;
            this->canGoForward = true;

            btnGoBack->setEnabled(true);
            btnHardGoBack->setEnabled(true);

            btnGoForward->setEnabled(true);
            btnHardGoForward->setEnabled(true);
        }
    }else if(totalPages == currentPage){
        if(currentPage == 0){
            this->canGoBack = false;
            this->canGoForward = false;

            btnGoBack->setDisabled(true);
            btnHardGoBack->setDisabled(true);

            btnGoForward->setEnabled(false);
            btnHardGoForward->setEnabled(false);
        }else{
            this->canGoBack = true;
            this->canGoForward = false;

            btnGoBack->setEnabled(true);
            btnHardGoBack->setEnabled(true);

            btnGoForward->setEnabled(false);
            btnHardGoForward->setEnabled(false);
        }
    }
}


