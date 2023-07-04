#ifndef __SHFASTIO_H__
#define __SHFASTIO_H__
#include <Arduino.h>

class FastDigitalPin {
private:
    uint8_t pin = -1;
	bool pinIsValid = false;
public:

	bool isValid() {
		return true;
	}

	void begin(int pin) {
		if (pin < 0) {
			pinIsValid = false;
		}
		else {
			pinIsValid = true;
            this->pin = pin;
        }
	}

	int digitalReadPin()
	{
		return digitalRead(this->pin);
	}
};

#endif