#include "project.h"
#include <QDataStream>
#include <QFile>
#include <QIODevice>

void Project::write_project(QString savefile, QString datafile, QString structdata)
{
    QFile file(savefile);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out << static_cast<quint16>(PROJECT_MAGIC) << datafile << structdata;

    file.close();
}

void Project::open_project(QString projectfile, QString *datafile, QString *structdata)
{
    QFile file(projectfile);
    file.open(QIODevice::ReadOnly);
    QDataStream out(&file);
    quint16 magic;

    out >> magic;
    if (magic != PROJECT_MAGIC) {
        auto ex = Project::ProjectFileException();
        ex.set_message("Invalid BinGrok file!");
        throw ex;
    }
    out >> *datafile;
    out >> *structdata;
}

void Project::ProjectFileException::raise() const
{
    throw *this;
}

void Project::ProjectFileException::set_message(QString m)
{
    message = m;
}
