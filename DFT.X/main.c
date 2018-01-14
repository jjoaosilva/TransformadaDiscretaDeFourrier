/*
 * @file main.c
 * @brief Codigo teste de uma transformada rapida de fourrier para PIC 18F4550.  
 *        Algoritmo de DFT retirado de: https://www.nayuki.io/page/free-small-fft-in-multiple-languages
 * @par
 * COPYRIGHT: (c) 2017 IFCE - Engenharia de computa��o
 * 
 */

////// CLOCK DE 48MHz,CLOCK EXTERNO
//// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (Divide by 5 (20 MHz oscillator input))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)
// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
//------------------------------------------------------------------------------

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)//

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototipos de funcoes privadas */
static void dft(const float *inreal, const float *inimag, bool inverse, int n); // Funcao que realiza calculo da DFT.
bool float_to_char(char *ptr,float myFloat);                                    // Converte um numero float para uma string.
void serial_tx(char *ptr);                                                      // Realiza tranmissao serial de dados.

/*!
 * AmostrasR: Armazena os valores reais de entrada. Valor de teste: Senoide, 60 Hz, pico 1, taxa de amostragem: 1 KHz.
 * AmostrasI: Armazena os valores imaginarios de entrada: Valor de teste: zerado.  
 */
const float amostrasR[512] = {0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, -0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, 0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328, -0.982287, -0.982287, -0.844328, -0.587785, -0.248690, 0.125333, 0.481754, 0.770513, 0.951057, 0.998027, 0.904827, 0.684547, 0.368125, 0.000000, -0.368125, -0.684547, -0.904827, -0.998027, -0.951057, -0.770513, -0.481754, -0.125333, 0.248690, 0.587785, 0.844328, 0.982287, 0.982287, 0.844328, 0.587785, 0.248690, -0.125333, -0.481754, -0.770513, -0.951057, -0.998027, -0.904827, -0.684547, -0.368125, -0.000000, 0.368125, 0.684547, 0.904827, 0.998027, 0.951057, 0.770513, 0.481754, 0.125333, -0.248690, -0.587785, -0.844328};
const float amostrasI[512] = {0.0};

int count = 0;                     //variavel para armazenar a contagem do timer
int tempoDecorrido = 0;            //variavel para armazenar o tempo decorrido em segundos
char tempo[10];

char msg[9];                       // Variavel que ir� armazenar os dados float no formator string.
char inicio[]     = "INICIO\n";    // String de sinaliza��o.
char fim[]        = "  FIM\n";     // String de sinaliza��o. 
char tabulacao[]  = "   ";         // String de formata��o.
char quebraLinha[] = " \n";        // String de formata��o

void interrupt ISR(){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0; //limpa a flag
        count++;
        if(count==188){
            tempoDecorrido++;
            count = 0;
        }
    }
}

float senoL(double num){
    int flag = 1, cont = 1;
    if(num < 0){
        flag = -1;
        num *= -1;
    }
    while(num > 6){
        num = num - 6;
        cont++;
    }
    if      (num >= 0  && num <= 0.5) return  (0.958800*num + 0.000000)*flag*cont;
    else if (num > 0.5 && num <= 1  ) return  (0.724200*num + 0.117300)*flag*cont;
    else if (num > 1   && num <= 1.5) return  (0.312000*num + 0.529500)*flag*cont;
    else if (num > 1.5 && num <= 2  ) return  (-0.176400*num + 1.262100)*flag*cont;
    else if (num > 2   && num <= 2.5) return  (-0.621600*num + 2.152500)*flag*cont;
    else if (num > 2.5 && num <= 4  ) return  (-0.903533*num + 2.857333)*flag*cont;
    else if (num > 4   && num <= 4.5) return  (-0.441400*num + 1.008800)*flag*cont;
    else if (num > 4.5 && num <= 5  ) return  (0.037200*num - 1.144900)*flag*cont;
    else if (num > 5   && num <= 5.5) return  (0.506800*num - 3.492900)*flag*cont;
    else if (num > 5.5 && num <= 6  ) return  (0.852200*num - 5.392600)*flag*cont;
    else return 100000;
}

float cossenoL(double num){
    int flag = 1, cont = 1;
    if(num < 0){
        flag = -1;
        num *= -1;
    }
    while(num > 6){
        num = num - 6;
        cont++;
    }
    if      (num >= 0  && num <= 0.5) return  (-0.244800*num + 1.000000)*flag;
    else if (num > 0.5 && num <= 1  ) return  (-0.674600*num + 1.214900)*flag;
    else if (num > 1   && num <= 2.5) return  (-0.894267*num + 1.434567)*flag;
    else if (num > 2.5 && num <= 3  ) return  (-0.377800*num + 0.143400)*flag;
    else if (num > 3   && num <= 3.5) return  (0.107000*num - 1.311000)*flag;
    else if (num > 3.5 && num <= 4  ) return  (0.565800*num - 2.916800)*flag;
    else if (num > 4   && num <= 5.5) return  (0.908200*num - 4.286400)*flag;
    else if (num > 5.5 && num <= 6  ) return  (10.493200*num - 61.99900)*flag;
    else return 100000;
}

void main(void) {
    
    TRISC = 0x00;            // Configura PORTC como saida.
    
    TXSTA   = 0b00100100;      // Configura��o da transmissao serial: observar datasheet para mais informa��es.
    RCSTA   = 0b10000000;      // Configura��o da recep��o serial: observar datasheet para mais informa��es.
    BAUDCON = 0b00000000;      // Configura��o da baudrate serial: observar datasheet para mais informa��es.
    SPBRG   = 207;             // Setado com esse valor para baudrate de 14400 bps.
 
    INTCONbits.GIE  = 1;   //habilita todas as interrupcoes globais
    INTCONbits.PEIE = 1;   //habilita interrupcao periferica
    INTCONbits.TMR0IE = 1; //habilita interrupcao de timer
    INTCONbits.TMR0IF = 0; //limpa a flag de overflow do timer
    ei();
    //configuracao do timer
    T0CONbits.T0CS = 0;   //seleciona clock interno
    T0CONbits.T0SE = 0;   //selecao da transicao(low to high)
    T0CONbits.PSA  = 0;   //prescaler habilitado
    //selecao do prescaler 1:256
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 1;
    /    int n = 1 << 9;          // determina o numero de pontos da DFT: 1 << 9 == 512.    

    serial_tx(&inicio);  
    dft(&amostrasR, &amostrasI, false, n); // Chamada da fun��o princial.
    serial_tx(&fim);           // Print de format��o.
    
    sprintf(&tempo, "%d", tempoDecorrido);   
    serial_tx(&tempo);
    while(1){}
}

/*!
 * @brief Algoritmo para realiza��o do calculo da transformada de fourrier de forma discreta.
 *        As saidas geradas pela DFT sao transmitidas via serial no formato: "ParteReal   ParteImaginario \n".
 * @param[in] const float *inreal: vetor com os dados reais. 
 * @param[in] const float *inimag: vetor com os dados imaginarios.
 * @param[in] bool inverse: true para transformada direta; false para transformada inversa.
 * @param[in] int n: numero de pontos da DFT.
 * @return void
 */
static void dft(const float *inreal, const float *inimag, bool inverse, int n) {
	int k, t;
    float coef = (inverse ? 2 : -2) * M_PI;     // 2pi caso seja transformada direta, -2pi caso seja transformada inversa.
    char respostaReal[10], respostaImag[10];    // Vetores usados para transmiss�o dos valores de sa�da em string.
    float sumreal = 0;
    float sumimag = 0;   
    float angle = 0;
    
	for (k = 0; k < (n/2)+1; k++) {
        sumreal = 0;
        sumimag = 0;
		for (t = 0; t < n; t++) {
			angle = coef * ((long long)t * k % n) / n;
/* 			sumreal += inreal[t]*cos(angle) - inimag[t]*sin(angle);  // Calculos da DFT com seno e cosseno da Math.
			sumimag += inreal[t]*sin(angle) + inimag[t]*cos(angle);  // Calculos da DFT com seno e cosseno da Math. */
			sumreal += inreal[t]*cossenoL(angle) - inimag[t]*senoL(angle);  // Calculos da DFT com seno e cosseno linearizado.
			sumimag += inreal[t]*senoL(angle) + inimag[t]*cossenoL(angle);  // Calculos da DFT com seno e cosseno linearizado.
		}
        float_to_char(&respostaReal, sumreal);                       // Convers�o do valor float para char.
        float_to_char(&respostaImag, sumimag);                       // Convers�o do valor float para char.
        
        serial_tx(&respostaReal);                                    // Transmissao do valor Real.
        serial_tx(&tabulacao);                                       // Transmissao de formata��o.
        serial_tx(&respostaImag);                                    // Transmissao do valor imaginario.
        serial_tx(&quebraLinha);                                     // Transmissao de formata��o.   
	}
}

/*!
 * @brief Fun��o que realiza a conver��o de numeros float para string.
 * @param[in] char *ptr: ponteiro para onde a resposta sera salva. 
 * @param[in] float myFloat: valor float a ser convertido.
 * @return true para sucesso; false para error.
 */
bool float_to_char(char *ptr, float myFloat){
	int ret = sprintf(ptr, "%.4f", myFloat);    // para mudar o numero de casas decimais basta remover o .4 ou modificar para .n.
    
	if (ret < 0) return false;
	return true;
}

/*!
 * @brief Fun��o que realiza a transmiss�o de dados via serial.
 * @param[in] char *ptr: ponteiro para a string a ser transmitida.
 * @return void
 */
void serial_tx(char *ptr){
    while(*ptr) { 
        TXREG = *ptr++;                // Registrador para onde o byte a ser transmitido � colocado.
        while(!TXSTAbits.TRMT);        // Aguarda a transmiss�o ser finalizada.
    }
}