#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QException>

#define PROJECT_MAGIC 0xB144

class Project
{
public:
    static void write_project(QString savefile, QString datafile, QString structdata);
    static void open_project(QString projectfile, QString *datafile, QString *structdata);

    class ProjectFileException : public QException
    {
    public:
        QString message;

        ProjectFileException() : QException () {}
        void set_message(QString m);
        void raise() const;
        ProjectFileException *clone() const { return new ProjectFileException(*this); }
    };
};

#endif // PROJECT_H
