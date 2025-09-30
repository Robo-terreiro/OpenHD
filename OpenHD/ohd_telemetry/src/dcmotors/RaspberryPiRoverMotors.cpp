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
    RaspberryPiRoverMotors::RaspberryPiRoverMotors(int brk_A, int brk_B, int motor_A, int motor_B, int pwm)
        : m_brk_A(brk_A), m_brk_B(brk_B), m_motor_A(motor_A), m_motor_B(motor_B), m_pwm(pwm) {
        gpioTerminate();
        gpioInitialise();
        // Usa um loop para inicializar os pinos
        for (int pin : {m_brk_A, m_brk_B, m_motor_A, m_motor_B, m_pwm}) {
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
    void RaspberryPiRoverMotors::set_direction(int forward_pin, bool forward) {
        gpioWrite(forward_pin, forward ? 1 : 0);
    }

    void RaspberryPiRoverMotors::set_direction_motor_A(bool forward) {
        set_direction(m_motor_A, forward);
    }

    void RaspberryPiRoverMotors::set_direction_motor_B(bool forward) {
        set_direction(m_motor_B, forward);
    }

    void RaspberryPiRoverMotors::stop() {
        gpioWrite(m_brk_A, 1);
        gpioWrite(m_brk_B, 1);
        // gpioWrite(m_motor_A, 0);
        // gpioWrite(m_motor_B, 0);
        gpioPWM(m_pwm, 0);
    }

    int RaspberryPiRoverMotors::mapp(int speed) {
        const int min_in = 1000;
        const int max_in = 1999;
        const int max_out = 254;
        return (speed - min_in) * max_out / (max_in - min_in);
    }
}