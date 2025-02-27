#include "pico/stdlib.h"
#include <stdlib.h>
#include <time.h>


// Definição dos pinos dos LEDs
#define led_pin_red 13
#define led_pin_green 12
#define led_pin_blue 11

// Definição dos pinos dos botões
#define button_pin_a 5
#define button_pin_b 6
#define button_joystick 22

// Definição do pino do buzzer
#define buzzer_pin 10

// Definição das frequências para o buzzer
#define FREQ_VERMELHO 262 
#define FREQ_AZUL 294      
#define FREQ_VERDE 330     
#define FREQ_ACERTO 440    
#define FREQ_ERRO 150      

void som(uint freq, uint duration) {
    for (int i = 0; i < (duration * freq / 1000); i++) {
        gpio_put(buzzer_pin, 1);
        sleep_us(500000 / freq); 
        gpio_put(buzzer_pin, 0);
        sleep_us(500000 / freq); 
    }
}

// Função para exibir um LED com base no número aleatório
void exibir_led(int led_number) {
    switch (led_number) {
        case 0: // Vermelho
            gpio_put(led_pin_red, 1);
            som(FREQ_VERMELHO, 500); // Toca o som vermelho
            sleep_ms(500);
            gpio_put(led_pin_red, 0);
            break;
        case 1: // Azul
            gpio_put(led_pin_blue, 1);
            som(FREQ_AZUL, 500); // Toca o som azul
            sleep_ms(500);
            gpio_put(led_pin_blue, 0);
            break;
        case 2: // Verde
            gpio_put(led_pin_green, 1);
            som(FREQ_VERDE, 500); // Toca o som verde
            sleep_ms(500);
            gpio_put(led_pin_green, 0);
            break;
    }
    sleep_ms(500); // Pausa entre as sequências
}

void inicializacao_dos_leds_e_botoes() {
    // Inicializando LEDs
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
    gpio_init(led_pin_green);
    gpio_set_dir(led_pin_green, GPIO_OUT);

    // Inicializando botões
    gpio_init(button_pin_a);
    gpio_set_dir(button_pin_a, GPIO_IN);
    gpio_pull_up(button_pin_a); // Habilita o resistor de pull-up interno

    gpio_init(button_pin_b);
    gpio_set_dir(button_pin_b, GPIO_IN);
    gpio_pull_up(button_pin_b); // Habilita o resistor de pull-up interno

    gpio_init(button_joystick);
    gpio_set_dir(button_joystick, GPIO_IN);
    gpio_pull_up(button_joystick); // Habilita o resistor de pull-up interno

    // Configuração do buzzer
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);
}

int main() {
    stdio_init_all();
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    inicializacao_dos_leds_e_botoes();

    int sequencia_length = 3; // Comprimento inicial da sequência

    while (true) {
        int sequencia[sequencia_length]; // Armazena a sequência de LEDs
        for (int i = 0; i < sequencia_length; i++) {
            sequencia[i] = rand() % 3; // Gera um número aleatório entre 0 e 2 (vermelho, verde, azul)
            exibir_led(sequencia[i]);
        }

        int tentativa[sequencia_length];
        for (int i = 0; i < sequencia_length; i++) {
            while (true) {
                if (gpio_get(button_pin_a) == 0) {
                    tentativa[i] = 0; // Vermelho
                    exibir_led(0);
                    break;
                } else if (gpio_get(button_pin_b) == 0) {
                    tentativa[i] = 1; // Azul
                    exibir_led(1);
                    break;
                } else if (gpio_get(button_joystick) == 0) {
                    tentativa[i] = 2; // Verde
                    exibir_led(2);
                    break;
                }
                sleep_ms(50);
            }
            sleep_ms(200);
        }

        bool acertou = true;
        for (int i = 0; i < sequencia_length; i++) {
            if (sequencia[i] != tentativa[i]) {
                acertou = false;
                break;
            }
        }

        if (acertou) {
            som(FREQ_ACERTO, 500); // Som de acerto
            sequencia_length++;     // Aumenta o comprimento da sequência
        } else {
            som(FREQ_ERRO, 500); // Som de erro
            sequencia_length = 3; // Reseta o comprimento da sequência
        }

        sleep_ms(1000);
    }
    return 0; 
}