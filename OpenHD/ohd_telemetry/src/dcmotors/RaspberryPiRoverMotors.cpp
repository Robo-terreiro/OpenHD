/******************************************************************************
 * OpenHD
 *
 * Licensed under the GNU General Public License (GPL) Version 3.
 *
 * This software is provided "as-is," without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose, and non-infringement. For details, see the
 * full license in the LICENSE file provided with this source code.
 *
 * Non-Military Use Only:
 * This software and its associated components are explicitly intended for
 * civilian and non-military purposes. Use in any military or defense
 * applications is strictly prohibited unless explicitly and individually
 * licensed otherwise by the OpenHD Team.
 *
 * Contributors:
 * A full list of contributors can be found at the OpenHD GitHub repository:
 * https://github.com/OpenHD
 *
 * © OpenHD, All Rights Reserved.
 ******************************************************************************/
#include "RaspberryPiRoverMotors.h"

namespace openhd::telemetry::rpi {
    RaspberryPiRoverMotors::RaspberryPiRoverMotors(int in1, int in2, int in3, int in4, int pwm)
        : m_in1(in1), m_in2(in2), m_in3(in3), m_in4(in4), m_pwm(pwm) {
        gpioTerminate();
        gpioInitialise();
        // Usa um loop para inicializar os pinos
        for (int pin : {m_in1, m_in2, m_in3, m_in4, m_pwm}) {
            gpioSetMode(pin, PI_OUTPUT);
        }
    }

    RaspberryPiRoverMotors::~RaspberryPiRoverMotors() {
        stop(); // Reutiliza a função stop para desligar tudo
        gpioTerminate();
    }

    void RaspberryPiRoverMotors::set_speed(int speed) {
        gpioPWM(m_pwm, speed);
    }

    // Função unificada para definir a direção de um motor
    void RaspberryPiRoverMotors::set_direction(int forward_pin, int backward_pin, bool forward) {
        gpioWrite(forward_pin, forward ? 1 : 0);
        gpioWrite(backward_pin, forward ? 0 : 1);
    }

    void RaspberryPiRoverMotors::set_direction_motor_A(bool forward) {
        set_direction(m_in1, m_in2, forward);
    }

    void RaspberryPiRoverMotors::set_direction_motor_B(bool forward) {
        set_direction(m_in3, m_in4, forward);
    }

    void RaspberryPiRoverMotors::stop() {
        gpioWrite(m_in1, 0);
        gpioWrite(m_in2, 0);
        gpioWrite(m_in3, 0);
        gpioWrite(m_in4, 0);
        gpioPWM(m_pwm, 0);
    }

    int RaspberryPiRoverMotors::mapp(int speed) {
        const int min_in = 1000;
        const int max_in = 1999;
        const int max_out = 254;
        return (speed - min_in) * max_out / (max_in - min_in);
    }
}