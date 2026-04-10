#ifndef MISSILE_LANCHER_H_
#define MISSILE_LANCHER_H_

#include "../IGun.h"


class MissileLancher : public IGun {
	// •`‰æ
	virtual void draw() const override;
	// ”­ŽË
	virtual void fire() override;
};

#endif