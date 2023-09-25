# teclado-musical-duas-oitavas
Projeto de teclado musical com duas oitavas desenvolvido em C com teste em placa de treinamento Redpill (embarcada). O teclado musical projetado é capaz de fornecer duas oitavas musicais em diferentes frequências.

Ao carregar o projeto na placa, o teclado musical funcionará da seguinte forma: o buzzer irá emitir um som gerado a partir de um sinal quadrado, que será controlado pela chave SW3 para controle do “timbre do teclado”, e modulado pelas frequências para conseguir gerar as notas musicais. As notas serão feitas a partir das chaves de acionamento da placa, da chave SW5 até a chave SW17. As notas não podem ser combinadas para formar acordes e quando nenhuma chave estiver sendo pressionada nenhum som será emitido.

Na placa também há um LCD que irá mostrar qual oitava musical estará sendo utilizada, em sua primeira linha, e o valor do ciclo de trabalho atual (que pode variar de 25%, 50% e 75%), na sua segunda linha. O projeto também conta com um potenciômetro que irá regular o efeito de bending.

## <img src="https://img.icons8.com/external-soft-fill-juicy-fish/30/external-chip-computer-hardware-soft-fill-soft-fill-juicy-fish.png"/> Componentes Utilizados na Placa

Para a construção de nosso código, utilizamos os seguintes componentes referenciados novamente na seção de demonstração:

<img src="https://uploaddeimagens.com.br/images/004/617/792/full/teclado01.png?1695600447" width=40% height=40%>

Em relação à pinagem utilizada, consideremos a figura, retirada do apêndice dos laboratórios de ELTD13, ministrados pelo Prof. Gustavo Colletta:

<img src="https://uploaddeimagens.com.br/images/004/617/794/full/teclado02.png?1695600597" width=38% height=38%>

● `SW1: PB12`, `SW2: PB13`, `SW3: PB14`, `POT: PB1`, `Buzzer: PB0`, `LCD4: PA8`, `LCD5: PA6`, `LCD6: PA5`, `LCD7: PA11`, `LCDEN: PA12`, `LCDRS: PA15`, `SW5: PB5`, `SW6: PB4`, `SW7: PB3`, `SW8: PA3`, `SW9: PA4`, `SW10: PB8`, `SW11: PB9`, `SW12: PB11`, `SW13: PB10`, `SW14: PA7`, `SW15: PC15`, `SW16: PC14`, `SW17: PC13`

Para a checagem dos registradores e bits ao decorrer do projeto, foi utilizado o [manual de referência da ST](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf).

## <img src="https://img.icons8.com/color/30/code.png"/> O Código

Para a construção do código, utilizamos o ambiente Keil uVision. A seguir será apresentada cada função utilizada e sua lógica detalhada a seguir.

<table>
  <thead>
    <tr>
      <th> Parte
      </th>
      <th> 
        <strong> Função
        </strong>
      </th>
      <th> 
        <strong> Descrição
        </strong>
      </th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td colspan="1" rowspan="5">LCD
      </td>
      <td>
        void lcd_init(void)
      </td>
      <td>
        Inicia o display
      </td>
    </tr>
    <tr>
      <td>
          void lcd_command(unsigned char cmd)
      </td>
      <td>
          Envia comandos ao display
      </td>
    </tr>
    <tr>
      <td>
          void lcd_data(unsigned char data)
      </td>
      <td>
          Envia dados em ASCII para ser exibido no display
      </td>
    </tr>
    <tr>
      <td>
          void lcd_print(char *str)
      </td>
      <td>
          Envia uma string ao display
      </td>
    </tr>
    </tr>
      <td>
          void lcd_putValue(unsigned char value)
      </td>
      <td>
         Usado internamente para inserir valores no display
      </td>
    </tr>
    </tr>
  <tr>
      <td colspan="1" rowspan="2">DELAY
      </td>
      <td>
        void delay_us(uint16_t t)
      </td>
      <td>
        Delay em microssegundos
      </td>
  </tr>
  <tr>
      <td>
        void delay_ms(uint16_t t)
      </td>
      <td>
        Delay em milissegundos
      </td>
  </tr>
  <tr>
      <td colspan="1" rowspan="2">Conversão
      </td>
      <td>
        void binary_to_bcd(uint8_t t)
      </td>
      <td>
        Converte de binário para bcd
      </td>
  </tr>
  <tr>
      <td>
        void bcd_to_ascii(void)
      </td>
      <td>
        Converte de bcd para ascii
      </td>
  </tr>
  <tr>
      <td>Manipulação de bit
      </td>
      <td>
        char bitTst(char port, char bit)
      </td>
      <td>
        Testa um bit de determinada PORT através da operação AND e deslocamento de bits
      </td>
  </tr>
  <tr>
      <td colspan="1" rowspan="3">Controle de ciclos
      </td>
      <td> 
        void cycle_50(void)
      </td>
      <td> 
        Essas funções atualizam o LCD de acordo com a informação da porcentagem do duty cicle da função void check_cycle e envia ao registrador TIM3 o delay correspondente ao ciclo (void)
      </td>
  </tr>
  <tr>
      <td>
        void cycle_25(void)
      </td>
      <td>
      </td>
  </tr>
  <tr>
      <td>
        void cycle_75(void)
      </td>
      <td>
      </td>
  </tr>
  <tr>
      <td colspan="1" rowspan="4">Funções de checagem
      </td>
      <td>
        void check_cycle(void)
      </td>
      <td>
        Verifica a porcentagem que o duty cicle deverá operar para enviar o timbre ao buzzer
      </td>
  </tr>
  <tr>
      <td>
        void check_octave(void)
      </td>
      <td>
        Verifica, por meio da função char bitTst(char port, char bit), qual oitava foi selecionada no teclado
      </td>
  </tr>
  <tr>
      <td>
        void check_pot(void)
      </td>
      <td>
        Recebe, através da entrada ADC, o valor do potenciômetro, salvando-o na variável bending. Para uma maior precisão, a fim de obter uma suavidade no movimento, o valor é convertido em uma escala de log
      </td>
  </tr>
  <tr>
      <td>
        void check_keyboard_key(void)
      </td>
      <td>
        Verifica qual tecla foi pressionada usando a função char bitTst(char port, char bit). De acordo com a tecla pressionada e com a oitava selecionada, é tocado a notachamando a função void buzzer(uint16_t note)
      </td>
  </tr>
  <tr>
      <td>Buzzer
      </td>
      <td>
        void buzzer(uint16_t note)
      </td>
      <td>
        Além de configurar o TIMER 3 para ativar o buzzer, essa função realiza um cálculo para o prescaler, com base na frequência da nota recebida, setando-o no registrador do timer
      </td>
  </tr>
  <tr>
      <td>Principal
      </td>
      <td>
        int main()
      </td>
      <td>
        Função principal com todas as chamadas de funções secundárias e inicialização dos registradores utilizados
      </td>
  </tr>
 </tbody>
</table>

### Definições
No início do projeto, incluímos a biblioteca “stm32f10x.h” devido ao microcontrolador utilizado ser o STM32F103C8. Também definimos os pinos utilizados para os comandos específicos do LCD, a fim de facilitar o processo do uso do componente ao longo do código e algumas variáveis responsáveis e importantes para a lógica de nossa solução.

```bash
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
```

Também temos as definições das variáveis globais, onde:
- `bcd_cents, bcd_tens e bcd_unity`: valores que guardam a centena, dezena e unidade da conversão BCD;
- `ascii_cents, ascii_tens, ascii_unity`: valores que guardam a centena, dezena e unidade da conversão ASCII para serem enviados corretamente ao LCD.

```bash
/*--------------------------------GLOBAL VARIABLES--------------------------------*/
 static uint8_t bcd_cents = 0; //valor das centenas do BCD
 static uint8_t bcd_tens = 0; //valor das dezenas do BCD
 static uint8_t bcd_unity = 0; //valor das unidades do BCD
 static uint8_t ascii_cents = 0; //valor das centenas do ASCII
 static uint8_t ascii_tens = 0; //valor das dezenas do ASCII
 static uint8_t ascii_unity = 0; //valor das unidades do ASCII
```

Seguindo, temos as definições das outras variáveis que utilizamos no programa, inclusive as matrizes usadas para guardarem as notas e as chaves para o teclado musical com:
- `timbre`: guarda o valor para selecionar o duty cycle
- `octaves`: guarda valor da oitava selecionada
- `cycle`: guarda o ciclo em execução
- `silence`: flag para verificar se não há alguma chave de nota selecionada
- `bending`: guarda valor para controlar buzzer pelo potenciômetro

Também temos duas matrizes:
- `notes[][]`: matriz com a primeira linha sendo todas as frequências da primeira oitava e com a segunda linha sendo todas as frequências relativas à segunda oitava
- `keyboard_key`: matriz com a primeira linha sendo a porta referente à chave e a segunda linha representando o pino correto para cada uma

```bash
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
```

### Protótipos das funções
Como boas práticas de programação, todas as funções foram instanciadas no início, com suas implementações após a estrutura principal (main).

```bash
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
```

### Função void check_pot(void)
Na função check_pot(), temos como primeiro comando “ADC1->SQR3 = 9;”, que atribui o valor 9 ao registrador ADC1->SQR3, de modo a defini-lo como primeiro da lista de conversão. O comando “ADC1->CR2 = 1;” refere-se ao início da conversão e, espera-se que seja concluída (while((ADC1->SR & (1 << 1)) == 0);) para o efeito de bending ser implementado pelo valor de “ADC1->DR”.

```bash
/*-----------------------------------OTHER FUNCTIONS-----------------------------------*/
void check_pot(void){
		ADC1->SQR3 = 9; //Canal 1 como entrada
		ADC1->CR2 = 1; //ADON = 1 (inicia a conversão)
		while((ADC1->SR & (1 << 1)) == 0); //Espera até EOC flag
		bending = ADC1->DR ; //Salva o valor na variavel
}
```

### Função void check_octave(void)
É uma função que checa a oitava em execução no momento atual do programa. Para isso, é testada qual das chaves (SW1 - Pino PB12 ou SW2 - Pino PB13) está selecionada. Caso seja a primeira, a variável de controle de oitavas recebe 0 (primeira oitava), é posicionado o cursor LCD no começo da primeira linha (instrução “lcd_command(_LCD_CURSOR_FIRST_LINE_BEGINNING);”) e escrito no LCD “Oitava 1”. Caso seja selecionada a segunda chave, a variável de controle de oitavas recebe 1 (segunda oitava), é posicionado o cursor LCD no começo da primeira linha (instrução “lcd_command(_LCD_CURSOR_FIRST_LINE_BEGINNING);”) e escrito no LCD “Oitava 2”.

```bash
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
```

### Função void check_cycle(void)
O duty cycle é verificado por essa função, a partir do pino PB14. Caso a variável timbre assuma 0, através de um switch..case, chama-se a função cycle_25(), que configura o ciclo de 25%. Caso seja igual a 1, chama-se a função cycle_50(), que configura o ciclo de 50% e, caso seja igual a 2, chama-se a função cycle_75(), que configura o ciclo de 75%.

```bash
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
```

### Função void buzzer(uint16_t note)
A função buzzer recebe a frequência da nota selecionada e ela é do tipo uint16_t, pois não é necessário um valor maior que isso pela faixa de valores. O primeiro comando desta função (“TIM3->CCER |= (1 << 8);”) seta nível lógico ‘1’ no oitavo bit do registrador “TIM3->CCER”, correspondentemente a ativar o timer 3. A segunda instrução configura o prescaler do timer 3, a partir do valor de contagem definido anteriormente. O cálculo está explicitado na seção de Cálculos do Projeto deste documento.

```bash
void buzzer(uint16_t note){
	 TIM3->CCER |= (1 << 8); //Ativa o TIM3
		
	 // f_onda = f_clk/((PSC+1)*(ARR+1))
	 // Para f_onda = note -> PSC = [f_clk/(note*(ARR+1))] - 1
	 TIM3->PSC = (72000000/(note*TIM3->ARR))-1; //Seta prescale com base no ARR já definido e nota musical
}
```

### Função void check_keyboard_key(void)
É responsável por verificar a nota pressionada e chamar a função buzzer passando a frequência correspondente. O for() é o loop que percorre os elementos da matriz das para as chaves das notas do teclado. Para testar a nota é utilizada a função bitTst() e, quando um elemento da primeira linha com as portas GPIOs é igual ao de mesma posição na segunda linha, quer dizer que uma nota é pressionada e, ao depender da oitava, passa para o buzzer a frequência da nota com a adição do efeito de bending a partir da primeira linha da matriz notes (referente à primeira oitava) ou da segunda linha da matriz notes (referente à segunda oitava). Caso nenhuma chave referente a uma nota é selecionada, o timer 3 é desligado, ou seja, o buzzer é desabilitado.

```bash
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
```

### Funções void cycle_25(void), void cycle_50(void) e void cycle_75(void)
São as funções responsáveis por escreverem no LCD o ciclo de trabalho no momento e configurar o delay do buzzer conforme a contagem escolhida (ARR). Para escrever no LCD, o cursor é posicionado na segunda linha, no começo, pois na primeira consta a oitava selecionada já.

```bash
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
```

### Função char bitTst(char port, char bit)
Função para testar se o bit desejado e da porta desejada está em nível lógico ‘1’, que retorna “!((GPIOx->IDR) & (1 << bit))”, ou seja, retorna 0 ou 1 dependendo do teste realizado para a porta A, B ou C.

```bash
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
```

### Funções void binary_to_bcd(uint8_t t) e void bcd_to_ascii(void)
São funções que convertem de binário para BCD e de BCD para ASCII, respectivamente, colocando nas variáveis de centena, dezena e unidade os valores convertidos para serem enviados para o LCD.

```bash
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
```

### Funções relacionadas ao LCD
- `lcd_putvalue()`: interpreta as informações transmitidas pelas funções `lcd_command() e lcd_data()`
- `lcd_command()`: envia configurações pré-definidas para o LCD, como posicionar o curso no LCD na primeira linha (0x80)
- `lcd_data()`: envia dados para o LCD
- `lcd_print()`: imprime na tela do LCD uma string
- `lcd_intit()`: inicialização do LCD

```bash
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
```

### Funções void delay_us(uint16_t t) e void delay_ms(uint16_t t)
São funções que têm for() aninhados para a contagem de tempo. A primeira seria em us e a segunda, em ms.

```bash
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
```

### Função int main()
Inicialmente, na função main, temos algumas configurações importantes de alguns registradores, como habilitar o clock dos GPIO's, TIMER e ADC. Após isso, o display LCD é iniciado e definições importantes são realizadas para um estado inicial de todo o sistema. Finalmente temos o loop infinito, onde uma série de checagens são feitas a fim de detectar uma eventual mudança nas configurações do sistema. Todas as funções podem ser conferidas na tabela 1 e a explicação detalhada de cada uma delas consta nas subseções anteriores.

```bash
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
```

## <img src="https://img.icons8.com/nolan/30/math.png"/> Cálculos do Projeto
Para a garantia de funcionamento do nosso código, precisamos realizar alguns cálculos. Para isso, o valor do registrador associado duty cycle e frequência da onda da nota devem ser parâmetros calculados a fim de se obter valores ótimos e corretos para o funcionamento de nossa solução. Dessa forma, é preciso que o registro do timer relacionado ao delay do sinal de PWM enviado e de PSC, ligado ao prescaler setado do timer sejam configurados com esses valores calculados. Através de várias simulações feitas pelo grupo, encontramos como um valor de contagem máximo (ARR) 45, de maneira que fossem claras as comparações entre os ciclos e oitavas, além do comportamento de bending. Assim, para o valor do registrador associado ao duty cycle, considerando o timer 3 usado, temos:

$𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 = 𝑑𝑢𝑡𝑦_𝑐𝑦𝑐𝑙𝑒   *   (𝐴𝑅𝑅 + 1)$

$𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 = 𝑑𝑢𝑡𝑦_𝑐𝑦𝑐𝑙𝑒   *   (46)$

Para cada duty cycle:

$𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 (25\%) = 0,25 * (46) = 11,5 −> 𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 (25\%) = 12$

$𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 (50\%) = 0,50 * (46) = 23$

$𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 (75\%) = 0,75 * (46) = 34,5 −> 𝑇𝐼𝑀3 −> 𝐶𝐶𝑅3 (25\%) = 34$

Para o valor do registrador correspondente ao prescaler, utilizamos a seguinte equação, sabendo que a frequência de clock fclk é igual a 72MHz:

$$ 𝑓𝑜𝑛𝑑𝑎 = {𝑓𝑐𝑙𝑜𝑐k \over {(𝑇𝐼𝑀3−>𝑃𝑆𝐶+1) x (𝑇𝐼𝑀3−>𝐴𝑅𝑅 + 1)}} $$

Isolando TIM3->PSC, temos:

$$ 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = {𝑓𝑐𝑙𝑜𝑐k \over {𝑓𝑜𝑛𝑑𝑎 x (𝑇𝐼𝑀3−>𝐴𝑅𝑅 + 1)}} - 1 $$

$$ 𝑇𝐼𝑀3 −> 𝑃SC = {72000000Hz \over {𝑓𝑜𝑛𝑑𝑎 x 46}} - 1 $$

Como a frequência da nota muda (fonda), há um valor de prescale associado a cada uma dessas frequências, cálculo este realizado dentro da função do buzzer, que pode ser conferida anteriormente. Para cada frequência, já considerando o resultado sem casas decimais:

### Oitava 1
- C (fonda = 132Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 11857
- C# (fonda = 140Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 11179
- D (fonda = 148Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 10575
- D# (fonda = 157Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 9968
- E (fonda = 166Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 9428
- F (fonda = 176Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 8892
- F# (fonda = 187Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 8369
- G (fonda = 198Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 7904
- G# (fonda = 209Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 7488
- A (fonda = 222Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 7050
- A# (fonda = 235Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 6659
- B (fonda = 249Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 6285
- C (fonda = 264Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 5928

### Oitava 2
- C (fonda = 264Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 5928
- C# (fonda = 280Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 5589
- D (fonda = 296Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 5287
- D# (fonda = 314Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 4984
- E (fonda = 332Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 4714
- F (fonda = 352Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 4446
- F# (fonda = 374Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 4184
- G (fonda = 396z): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 3952
- G# (fonda = 418Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 3744
- A (fonda = 444Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 3524
- A# (fonda = 470Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 3329
- B (fonda = 498Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 3142
- C (fonda = 528Hz): 𝑇𝐼𝑀3 −> 𝑃𝑆𝐶 = 2963

## <img src="ttps://img.icons8.com/external-phatplus-lineal-color-phatplus/30/external-testing-cloud-computing-phatplus-lineal-color-phatplus.png"/> Demonstração

Para verificar o funcionamento do código, foram feitos testes na placa Redpill conectada à placa Bluepill. Também foi utilizado um potenciômetro como um efeito de bending, que aumenta a frequência da nota, um buzzer para emitir o som e o LCD para mostrar qual oitava está selecionada e o valor do ciclo de trabalho atual (Duty Cycle). A pinagem utilizada ficou da seguinte forma:

| Tecla | Função |
| --- | --- |
| SW1 | Seleciona 1ª oitava |
| SW2 | Seleciona 2ª oitava |
| SW3 | Seleciona Duty Cycle |
| POT | Bending |

| Tecla | Nota Musical |
| --- | --- |
| SW5 | C |
| SW13 | C# |
| SW6 | D |
| SW14 | D# |
| SW7 | E |
| SW8 | F |
| SW15 | F# |
| SW9 | G |
| SW16 | G# |
| SW10 | A |
| SW17 | A# |
| SW11 | B |
| SW12 | C |

O primeiro exemplo foi a música “Cai cai balão" e foi utilizado o duty cycle de 25% com a 1ª oitava selecionada. Já o segundo exemplo é a música “Back In Black” da banda AC/DC e foi utilizado o duty cycle de 75%, foi mantido a 1ª oitava e alterado o timbre. 

O Link do vídeo de demonstrações: https://youtu.be/gAyJ-6XzcmM

## <img src="https://img.icons8.com/external-tal-revivo-fresh-tal-revivo/30/external-thinking-a-best-idea-to-be-executed-for-class-school-fresh-tal-revivo.png"/> Time

`Danilo Bitencourt Medeiros do Nascimento`
`Igor José Marinho Vieira`
`Sabrina Danielly Lacerda`
`Stéfany Coura Coimbra`
