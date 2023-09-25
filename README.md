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
```
