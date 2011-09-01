#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

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
	quint8 get_mro();
	void set_mro(quint8);
	int get_fontsize();

private:
	Ui::Preferences *ui;

	//QString font_string(const QFont &);

public slots:
	//void show_font_dialog();
	//void set_font(const QFont &);
	void save_preferences();

signals:
	void preferences_changed();
};

#endif // PREFERENCES_H
