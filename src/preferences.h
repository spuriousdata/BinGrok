#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QFont>

namespace Ui {
    class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();
	int get_bpc();
	QFont get_font();
	quint8 get_mro();
	void set_mro(quint8);

private:
    Ui::Preferences *ui;
	QFont parent_font;
	QFont pref_font;

	QString font_string(const QFont &);

public slots:
	void show_font_dialog();
	void set_font(const QFont &);
	void save_preferences();

signals:
	void preferences_changed();
};

#endif // PREFERENCES_H
