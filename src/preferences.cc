#include "preferences.h"
#include "ui_preferences.h"
#include "hexwidget.h"

#include <QFontDialog>
#include <QString>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
	parent_font = parent->font();

	ui->setupUi(this);
	ui->bytes_per_column_spinbox->setValue(((HexWidget*)parent)->get_bytes_per_column());
	ui->font_button->setText(font_string(parent_font));

	connect(ui->font_button, SIGNAL(clicked()), this, SLOT(show_font_dialog()));
	connect(ui->buttonBox, SIGNAL(accepted()),
			this, SLOT(save_preferences()));
}

QString Preferences::font_string(const QFont & f)
{
	QString fnt = f.family() + " [";
	fnt.append(QString("%1").arg(f.pointSize())).append("]");
	return fnt;
}

void Preferences::save_preferences()
{
	emit preferences_changed();
}

void Preferences::show_font_dialog()
{
	QFontDialog *font_dialog = new QFontDialog(parent_font, this);

	connect(font_dialog, SIGNAL(fontSelected(const QFont &)),
			this, SLOT(set_font(const QFont &)));

	font_dialog->show();
}

void Preferences::set_font(const QFont & f)
{
	ui->font_button->setText(font_string(f));
	pref_font = f;
}

int Preferences::get_bpc()
{
	return ui->bytes_per_column_spinbox->value();
}

QFont Preferences::get_font()
{
	return this->pref_font;
}

Preferences::~Preferences()
{
    delete ui;
}
