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
	ui->setupUi(this);
	ui->bytes_per_column_spinbox->setValue(static_cast<HexWidget*>(parent)->get_bytes_per_column());
	ui->fontsize_spinbox->setValue(parent->font().pointSize());
    ui->display_textview_checkbox->setChecked(static_cast<HexWidget*>(parent)->get_display_textview());

	//ui->font_button->setText(font_string(parent_font));
	//connect(ui->font_button, SIGNAL(clicked()), this, SLOT(show_font_dialog()));

	connect(ui->buttonBox, SIGNAL(accepted()),
			this, SLOT(save_preferences()));
}

void Preferences::save_preferences()
{
	emit preferences_changed();
}

int Preferences::get_bpc()
{
	return ui->bytes_per_column_spinbox->value();
}

int Preferences::get_fontsize()
{
	return ui->fontsize_spinbox->value();
}

void Preferences::set_mro(quint8 mro)
{
	ui->max_recently_open_spinbox->setValue(mro);
}

quint8 Preferences::get_mro()
{
	return ui->max_recently_open_spinbox->value();
}

Preferences::~Preferences()
{
    delete ui;
}
