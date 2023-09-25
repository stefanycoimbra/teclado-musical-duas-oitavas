/*------------------------------------LIBRARIES------------------------------------*/
# include "stm32f10x.h"

 /*-------------------------------LCD PIN DEFINITIONS-------------------------------*/
# define LCD_RS 15
# define LCD_EN 12

/*-----------------------------------LCD COMMANDS-----------------------------------*/
#define _LCD_CLEAR 1
#define _LCD_HOME 2
#define _LCD_SHIFT_CURSOR_TO_LEFT 4
#define _LCD_SHIFT_CURSOR_TO_RIGHT 6
#define _LCD_CURSOR_FIRST_LINE_BEGINNING 128
#define _LCD_CURSOR_SECOND_LINE_BEGINNING 192
#define _LCD_DISPLAY_ON_CURSOR_BLINKING 14

/*--------------------------------GLOBAL VARIABLES--------------------------------*/
 static uint8_t bcd_cents = 0; //valor das centenas do BCD
 static uint8_t bcd_tens = 0; //valor das dezenas do BCD
 static uint8_t bcd_unity = 0; //valor das unidades do BCD
 static uint8_t ascii_cents = 0; //valor das centenas do ASCII
 static uint8_t ascii_tens = 0; //valor das dezenas do ASCII
 static uint8_t ascii_unity = 0; //valor das unidades do ASCII
 
 //Outras variáveis
 static uint8_t timbre = 0;
 static uint8_t octaves = 0;
 static uint8_t cycle = 0;
 static uint8_t silence = 0;
 static uint32_t bending = 0;
 
 //Na seguinte matriz, cada linha representa uma oitava
 static uint16_t notes [2][13]={{132,140,148,157,166,176,187,198,209,222,235,249,264},
																{264,280,296,314,332,352,374,396,418,444,470,498,528}};

/*Na seguibte matriz, as teclas foram ordenadas de forma que a primeira linha represente seus GPIOs
	e a segunda linha representa o pino do botão*/
 static uint8_t keyboard_key [2][13]={{'B','B','B','A','B','A','C','A','C','B','C','B','B'},
																			{5,10,4,7,3,3,15,4,14,8,13,9,11}};

 /*------------------------------FUNCTION PROTOTYPES------------------------------*/
 //LCD FUNCTIONS
 void lcd_init(void); //Inicia o display
 void lcd_command(unsigned char cmd); //Envia comandos
 void lcd_data(unsigned char data); //Envia dados em ASCII
 void lcd_print(char *str); //Envia strings
 void lcd_putValue(unsigned char value); //Usada por outras funções para inserção de valores no LCD
 
 //DELAY FUNCTIONS
 void delay_us(uint16_t t); //Atraso em microssegundos
 void delay_ms(uint16_t t); //Atraso em milissegundos
 
 //CONVERSION FUNCTIONS
 void binary_to_bcd(uint8_t t); //Converte de binario para BCD
 void bcd_to_ascii(void); //Converte de BCD para ASCII
 
 //BIT MANIPULATION FUNCTION
 char bitTst(char port, char bit); //Testa individualmente cada bit
 
 //CYCLE CONTROL FUNCTIONS
 void cycle_50(void);
 void cycle_25(void);
 void cycle_75(void);
 
 //CHECK FUNCTIONS
 void check_cycle(void);
 void check_octave(void);
 void check_pot(void);
 void check_keyboard_key(void);
 
 //BUZZER FUNCTION
 void buzzer(uint16_t note);

/*----------------------------------MAIN FUNCTION----------------------------------*/
int main(){
	 // GPIO setup
	 RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	 AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE ;
	 RCC->APB2ENR |= 0xFC | (1 << 9) | (1 << 14);
	 RCC->APB1ENR |= (1 << 1);//Habilita o clock do timer3
	
	 GPIOB->CRH = 0x44444444 ; //Configura todas as chaves como entradas
	 GPIOB->CRL = 0x4444440B ; //Configura buzzer como saída alternada
	 
	 GPIOC->CRL = 0x44444444 ; //Configura chaves sw15, sw16, sw17 como entradas
	 
	 GPIOA->CRL = 0x43344333 ; //Configura PA7/PA4/PA3 como entradas
	 GPIOA->CRH = 0x33333333 ; //Configura saídas do LCD 
	 
	 //Configurações do TIM3
	 TIM3->CCMR2 = 0x0060;
	 TIM3->ARR = 46 - 1;			// Define ARR como 45
	 TIM3->CR1 = 1;
	
	 //Configuração inicial do potenciômetro
	 ADC1->CR2 = 1; //Set ADON (power up)
   ADC1->SMPR2 = 1 << 27; //Set SMP1 (001)m , canal 9
   delay_us(1); //Espera 1 us para garantir que o módulo ADC esteja estável
	 
	 //Inicialização do LCD
	 lcd_init();
	 delay_ms(100);
   lcd_command(_LCD_CURSOR_FIRST_LINE_BEGINNING);
	 lcd_print("Oitava 1");
	 cycle_25();

	 for(;;) {
		  check_octave(); //Verifica qual oitava
		  check_cycle(); //Verifica qual ciclo
		  check_pot(); //Verifica qual o valor do potênciometro
		  check_keyboard_key(); //Verifica a tecla e aciona o buzzer
   }
}

/*-----------------------------------OTHER FUNCTIONS-----------------------------------*/
void check_pot(void){
		ADC1->SQR3 = 9; //Canal 1 como entrada
		ADC1->CR2 = 1; //ADON = 1 (inicia a conversão)
		while((ADC1->SR & (1 << 1)) == 0); //Espera até EOC flag
		bending = ADC1->DR ; //Salva o valor na variavel
}	

void check_octave(void){
	 /*As seguintes condições verificam se o botão foi pressionado, caso sim, é atualizado o valor da
	   variável e o lcd*/
	if(bitTst('B', 12) == 1){
		octaves = 0;
		lcd_command(_LCD_CURSOR_FIRST_LINE_BEGINNING);
		lcd_print("Oitava 1");
	}
  if(bitTst('B', 13) == 1){
		octaves = 1;
		lcd_command(_LCD_CURSOR_FIRST_LINE_BEGINNING);
		lcd_print("Oitava 2");
	}
}

void check_cycle(void){
	if (bitTst('B', 14) == 1){
		timbre++;
		timbre = timbre % 3; //Mantem entre 0 e 2
		switch(timbre){ //Seleciona a função de acordo com o correspondente
			case 0:
				cycle_25();
				break;
			case 1:
				cycle_50();
				break;
			case 2:
				cycle_75();
				break;
		}
		delay_ms(100);
	}
}

void buzzer(uint16_t note){
	 TIM3->CCER |= (1 << 8); //Ativa o TIM3
		
	 // f_onda = f_clk/((PSC+1)*(ARR+1))
	 // Para f_onda = note -> PSC = [f_clk/(note*(ARR+1))] - 1
	 TIM3->PSC = (72000000/(note*TIM3->ARR))-1; //Seta prescale com base no ARR já definido e nota musical
}	

void check_keyboard_key(void){
	for(int i = 0; i < 13; i++){ //Percorre a matriz pegando os GPIO e os pinos
		if(bitTst(keyboard_key[0][i], keyboard_key[1][i]) == 1){ 
			if(octaves == 1){
				buzzer((uint16_t)((notes[1][i]) + bending)); //Toca a nota da oitava selecionada com a adição do valor de bending
			}
			if(octaves == 0){
				buzzer((uint16_t)((notes[0][i]) + bending)); //Toca a nota da oitava selecionada com a adição do valor de bending
			}
			silence = 1;
		}	
	}
	//Caso nenhuma nota seja tocada o TIM3 será desabilitado
	if(silence == 0){
		TIM3->CCER &= ~(1 << 8);
	}
	silence = 0;
}	

void cycle_25(void){
	cycle = 25; //Atualiza a variavel
	
	//TIME3->CCR3 = (TIM3->ARR+1)*0.25
	TIM3->CCR3 = (TIM3->ARR+1)*0.25;
	lcd_command(_LCD_CURSOR_SECOND_LINE_BEGINNING); //Força o cursor para o começo
	lcd_print("Ciclo: 25%");	//Atualiza o lcd
}

void cycle_50(void){
	cycle = 50; //Atualiza a variavel
	
	//TIME3->CCR3 = (TIM3->ARR+1)*0.50
	TIM3->CCR3 = (TIM3->ARR+1)*0.50;
	lcd_command(_LCD_CURSOR_SECOND_LINE_BEGINNING); //Força o cursor para o começo
	lcd_print("Ciclo: 50%");	//Atualiza o lcd
}

void cycle_75(void){
 	cycle = 75; //Atualiza a variavel
	
	//TIME3->CCR3 = (TIM3->ARR+1)*0.75
	TIM3->CCR3 = (TIM3->ARR+1)*0.75;
	lcd_command(_LCD_CURSOR_SECOND_LINE_BEGINNING); //Força o cursor para o começo
	lcd_print("Ciclo: 75%");	//Atualiza o lcd
}	

char bitTst(char port, char bit){
	 //Realiza o bitshift de acordo com a GPIO necessária
	 switch(port){
		 case('A'):
			return !((GPIOA->IDR) & (1 << bit));
		 case('B'):
			return !((GPIOB->IDR) & (1 << bit));
		 case('C'):
			return !((GPIOC->IDR) & (1 << bit));
	 }
		return ' ';
}

//Converte número binário para BCD
void binary_to_bcd(uint8_t t){
	 static uint8_t aux = 0;
	 bcd_cents = t / 100;
	 aux = t % 100;
	 bcd_tens = aux / 10;
	 bcd_unity = aux % 10;
}
 
//Converte BCD para ASCII
void bcd_to_ascii(void){
	 ascii_cents = bcd_cents | 0x30;
	 ascii_tens = bcd_tens | 0x30;
	 ascii_unity = bcd_unity | 0x30;
}

void lcd_putValue(unsigned char value){
	 uint16_t aux; //Variável para ajudar a ser construída a saída
	 aux = 0x00; //Limpa aux
	 GPIOA->BRR = (1 << 5) | (1 << 6) | (1 << 8) | (1 << 11); //Limpa linhas de dodos
	 aux = value & 0xF0 ;
	 aux = aux >> 4;
	 GPIOA->BSRR = (uint32_t)(((aux & 0x08) << 8) | ((aux & 0x04) << 3) | ((aux & 0x02) << 5) | ((aux & 0x01) << 8));
	 GPIOA->ODR |= (1 << LCD_EN); //EN = 1 para H - para - L pulse
	 delay_ms(3); //Make EN pulse wider
	 GPIOA->ODR &= (unsigned int)(~(1 << LCD_EN)); //EN = 0 para H - para - L pulse
	 delay_ms(1); //Espera 1ms
	 GPIOA->BRR = (1 << 5) | (1 << 6) | (1 << 8) | (1 << 11); //Limpa linhas de dodos
	 aux = 0x00 ; //Limpa aux
	 aux = value & 0x0F ;
	 GPIOA->BSRR = (uint32_t)(((aux & 0x08) << 8) | ((aux & 0x0004) << 3) | ((aux & 0x0002) << 5) | ((aux & 0x0001) << 8));
	 GPIOA->ODR |= (1 << LCD_EN); //EN = 1 para H - para - L pulse
	 delay_ms(3); //Make EN pulse wider
	 GPIOA->ODR &= (unsigned int)(~(1 << LCD_EN)); //EN = 0 para H - para - L pulse
	 delay_ms(1); //Wait
}

void lcd_command(unsigned char cmd){
	 GPIOA->ODR &= (unsigned int)(~(1 << LCD_RS)); //RS = 0 para comando
	 lcd_putValue(cmd);
}


void lcd_data(unsigned char data){
	 GPIOA->ODR |= (1 << LCD_RS); //RS = 1 para dados
	 lcd_putValue(data);
}

void lcd_print(char *str){
	 unsigned char i = 0;

	 while(str[i] != 0){ //Enquanto não é o final da string
		 lcd_data(str[i]); //Mostra str[i] no LCD
		 i++;
	 }
}

//Inicializa o LCD
void lcd_init(){
	 delay_ms (15);
	 GPIOA->ODR &= (unsigned int)(~(1 << LCD_EN)); //LCD_EN = 0
	 delay_ms(3); //Wait 3 ms
	 lcd_command(0x33); //Lcd init
	 delay_ms(5);
	 lcd_command(0x32); //Lcd init
	 delay_us(3000);
	 lcd_command(0x28); //4 - bit mode , 1 line and 5x8 charactere set
	 delay_ms(3);
	 lcd_command(_LCD_DISPLAY_ON_CURSOR_BLINKING); //Display on , cursor blinking
	 delay_ms(3);
	 lcd_command(_LCD_CLEAR); //Display clear
	 delay_ms(3);
	 lcd_command(_LCD_SHIFT_CURSOR_TO_RIGHT); //Move right
	 delay_ms(3);
}

//Delay, em us
void delay_us(uint16_t t){
 volatile unsigned long l = 0;
	 for(uint16_t i = 0; i < t; i++){
		 for (l = 0; l < 6; l++);
	 }
}

//Delay, em ms
void delay_ms(uint16_t t){
 volatile unsigned long l = 0;
	 for(uint16_t i = 0; i < t; i++){
		 for(l = 0; l < 6000; l++);
	 }
}