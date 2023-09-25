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
            <th>Layer 1</th>
            <th>Layer 2</th>
            <th>Layer 3</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=4>L1 Name</td>
            <td rowspan=2>L2 Name A</td>
            <td>L3 Name A</td>
        </tr>
        <tr>
            <td>L3 Name B</td>
        </tr>
        <tr>
            <td rowspan=2>L2 Name B</td>
            <td>L3 Name C</td>
        </tr>
        <tr>
            <td>L3 Name D</td>
        </tr>
    </tbody>
</table>

| Parte | Função | Descrição |
| --- | --- | --- |
| LCD | ```void lcd_init(void)``` | Inicia o display |
|       | ```void lcd_command(unsigned char cmd)``` | Envia comandos ao display |
|       | ```void lcd_data(unsigned char data)``` | Envia dados em ASCII para ser exibido no display |
|       | ```void lcd_print(char *str)``` | Envia uma string ao display |
|       | ```void lcd_putValue(unsigned char value)``` | Usado internamente para inserir valores no display |
| DELAY | ```void delay_us(uint16_t t)``` | Delay em microssegundos |
|         | ```void delay_ms(uint16_t t) ``` | Delay em milissegundos |
| Conversão | ```void binary_to_bcd(uint8_t t)``` | Converte de binário para bcd |
|             | ```void bcd_to_ascii(void)``` | Converte de bcd para ascii |
| Manipulação de bit | ```char bitTst(char port, char bit)``` | Testa um bit de determinada PORT através da operação AND e deslocamento de bits |
| Controle de ciclos | ```void cycle_50(void)``` | Essas funções atualizam o LCD de acordo com a informação da porcentagem do duty cicle da função void check_cycle e envia ao registrador TIM3 o delay correspondente ao ciclo (void)
|                      | ```void cycle_25(void)``` |
|                      | ```void cycle_75(void)``` | |
| Funções de checagem | `````` |
| Buzzer | `````` |
| Principal | `````` |
