#ifndef BAZOOKA_LANCHER_H_
#define BAZOOKA_LANCHER_H_

#include "../IGun.h"


class BazookaLauncher : public IGun {
	// •`‰æ
	virtual void draw() const override;
	// ”­ŽË
	virtual void fire() override;
};
#endif