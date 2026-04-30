#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    chargerFichier();

    QStringList titres;
    titres << "Nom" << "Prénom" << "Age" << "Classe";

    ui->tableWidget->setHorizontalHeaderLabels(titres);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sauvegarder(QString nom, QString prenom, int age, QString classe)
{
    QFile fichier("eleves.txt");

    if(fichier.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&fichier);
        out << nom << ";" << prenom << ";" << age << ";" << classe << "\n";
        fichier.close();
    }
}

void MainWindow::chargerFichier()
{
    QFile fichier("eleves.txt");

    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fichier);

        while(!in.atEnd())
        {
            QString ligne = in.readLine();
            QStringList data = ligne.split(";");

            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            for(int i=0;i<4;i++)
                ui->tableWidget->setItem(row,i,new QTableWidgetItem(data[i]));
        }

        fichier.close();
    }
}

void MainWindow::on_pushButtonAjouter_clicked()
{
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();
    int age = ui->spinBoxAge->value();
    QString classe = ui->comboBoxClasse->currentText();

    int ligne = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(ligne);

    ui->tableWidget->setItem(ligne,0,new QTableWidgetItem(nom));
    ui->tableWidget->setItem(ligne,1,new QTableWidgetItem(prenom));
    ui->tableWidget->setItem(ligne,2,new QTableWidgetItem(QString::number(age)));
    ui->tableWidget->setItem(ligne,3,new QTableWidgetItem(classe));

    sauvegarder(nom, prenom, age, classe);
}

void MainWindow::sauvegarderTout()
{
    QFile fichier("eleves.txt");

    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&fichier);

        int lignes = ui->tableWidget->rowCount();

        for(int i = 0; i < lignes; i++)
        {
            QString nom = ui->tableWidget->item(i,0)->text();
            QString prenom = ui->tableWidget->item(i,1)->text();
            QString age = ui->tableWidget->item(i,2)->text();
            QString classe = ui->tableWidget->item(i,3)->text();

            out << nom << ";" << prenom << ";" << age << ";" << classe << "\n";
        }

        fichier.close();
    }
}


/*void MainWindow::on_pushButtonSupprimer_clicked()
{
    int ligne = ui->tableWidget->currentRow();

    if(ligne >= 0)
    {
        ui->tableWidget->removeRow(ligne);
        sauvegarderTout();
    }
}*/

void MainWindow::on_pushButtonSupprimer_clicked()
{
    int ligne = ui->tableWidget->currentRow();

    if(ligne >= 0)
    {
        derniereLigne.clear();

        for(int i=0; i<4; i++)
        {
            derniereLigne << ui->tableWidget->item(ligne,i)->text();
        }

        dernierePosition = ligne;

        ui->tableWidget->removeRow(ligne);

        sauvegarderTout();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier &&
        event->key() == Qt::Key_C)
    {
        if(!derniereLigne.isEmpty())
        {
            ui->tableWidget->insertRow(dernierePosition);

            for(int i=0; i<4; i++)
            {
                ui->tableWidget->setItem(
                    dernierePosition,
                    i,
                    new QTableWidgetItem(derniereLigne[i])
                    );
            }

            sauvegarderTout();

            derniereLigne.clear();
        }
    }

    QMainWindow::keyPressEvent(event);
}


void MainWindow::on_pushButton_Modif_clicked()
{
        int ligne = ui->tableWidget->currentRow();

        if(ligne >= 0)
        {
            ui->tableWidget->setItem(ligne,0,new QTableWidgetItem(ui->lineEditNom->text()));
            ui->tableWidget->setItem(ligne,1,new QTableWidgetItem(ui->lineEditPrenom->text()));
            ui->tableWidget->setItem(ligne,2,new QTableWidgetItem(QString::number(ui->spinBoxAge->value())));
            ui->tableWidget->setItem(ligne,3,new QTableWidgetItem(ui->comboBoxClasse->currentText()));

            sauvegarderTout();
        }
}

