/**
 * Mandelbulber v2, a 3D fractal generator       ,=#MKNmMMKmmßMNWy,
 *                                             ,B" ]L,,p%%%,,,§;, "K
 * Copyright (C) 2016 Krzysztof Marczak        §R-==%w["'~5]m%=L.=~5N
 *                                        ,=mm=§M ]=4 yJKA"/-Nsaj  "Bw,==,,
 * This file is part of Mandelbulber.    §R.r= jw",M  Km .mM  FW ",§=ß., ,TN
 *                                     ,4R =%["w[N=7]J '"5=],""]]M,w,-; T=]M
 * Mandelbulber is free software:     §R.ß~-Q/M=,=5"v"]=Qf,'§"M= =,M.§ Rz]M"Kw
 * you can redistribute it and/or     §w "xDY.J ' -"m=====WeC=\ ""%""y=%"]"" §
 * modify it under the terms of the    "§M=M =D=4"N #"%==A%p M§ M6  R' #"=~.4M
 * GNU General Public License as        §W =, ][T"]C  §  § '§ e===~ U  !§[Z ]N
 * published by the                    4M",,Jm=,"=e~  §  §  j]]""N  BmM"py=ßM
 * Free Software Foundation,          ]§ T,M=& 'YmMMpM9MMM%=w=,,=MT]M m§;'§,
 * either version 3 of the License,    TWw [.j"5=~N[=§%=%W,T ]R,"=="Y[LFT ]N
 * or (at your option)                   TW=,-#"%=;[  =Q:["V""  ],,M.m == ]N
 * any later version.                      J§"mr"] ,=,," =="""J]= M"M"]==ß"
 *                                          §= "=C=4 §"eM "=B:m|4"]#F,§~
 * Mandelbulber is distributed in            "9w=,,]w em%wJ '"~" ,=,,ß"
 * the hope that it will be useful,                 . "K=  ,=RMMMßM"""
 * but WITHOUT ANY WARRANTY;                            .'''
 * without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * ###########################################################################
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com)
 *
 * TODO: description
 */

#include "dock_animation.h"

#include "../src/automated_widgets.hpp"
#include "../src/file_image.hpp"
#include "ui_dock_animation.h"
#include "audio_selector.h"

cDockAnimation::cDockAnimation(QWidget *parent) : QWidget(parent), ui(new Ui::cDockAnimation)
{
	ui->setupUi(this);
	automatedWidgets = new cAutomatedWidgets(this);
	automatedWidgets->ConnectSignalsForSlidersInWindow(this);
	ConnectSignals();
}

cDockAnimation::~cDockAnimation()
{
	delete ui;
}

void cDockAnimation::DisableEXR()
{
	qobject_cast<QStandardItemModel *>(ui->comboBox_keyframe_animation_image_type->model())
		->item(ImageFileSave::IMAGE_FILE_TYPE_EXR)
		->setEnabled(false);
	qobject_cast<QStandardItemModel *>(ui->comboBox_flight_animation_image_type->model())
		->item(ImageFileSave::IMAGE_FILE_TYPE_EXR)
		->setEnabled(false);
}

void cDockAnimation::DisableTIFF()
{
	qobject_cast<QStandardItemModel *>(ui->comboBox_keyframe_animation_image_type->model())
		->item(ImageFileSave::IMAGE_FILE_TYPE_TIFF)
		->setEnabled(false);
	qobject_cast<QStandardItemModel *>(ui->comboBox_flight_animation_image_type->model())
		->item(ImageFileSave::IMAGE_FILE_TYPE_TIFF)
		->setEnabled(false);
}

Ui::cDockAnimation *cDockAnimation::GetUi()
{
	return ui;
}

void cDockAnimation::ConnectSignals()
{
	connect(ui->pushButton_load_audio, SIGNAL(clicked()), this, SLOT(slotLoadAudio()));
}

void cDockAnimation::slotLoadAudio()
{
	cAudioSelector *audioSelectorDialog = new cAudioSelector(this);
	audioSelectorDialog->setWindowFlags(Qt::Dialog);
	audioSelectorDialog->show();
}
