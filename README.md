# Transformada Discreta de Fourrier
Implementação da Transformada Discreta de Fourrier em C para pic 18F4550. Abordagem: otimização do tempo de execução.

### Autores:

* **José João Silva** - [jjoaosilva](https://github.com/jjoaosilva/)
* **Mario Matheus**   - [MarioMatheus](https://github.com/MarioMatheus/)

## Sobre
Esse trabalho foi desenvolvimento na disciplica de Sistema Embarcados 1 do curso de Engenharia de computação do Instituto de educação, ciência e tecnologia do Ceará(IFCE).

A ideia era escolher um algoritmo que atendesse aos requisitos mínimos necessários para a disciplina. O algoritmo escolhido foi a Transformada Discreta de Fourrier(DFT) com 512 pontos de entrada. Em resumo, a DFT decompõe um sinal temporal no domínio da frequência e tal transformada é dada por: 

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/equacao.PNG?raw=true)

Vale salientar que o objetivo não é otimizar a DFT no seu formato de cálculo e sim manter a implementação do cálculo do algoritmo original e modificar funções visando diminuir o tempo de execução.

## Como foi feita
Utilizando a ferramenta Matlab, foi aquisitada a amostra de teste da DFT: Senoide, 60 Hz, pico 1, taxa de amostragem: 1 KHz. Com isso, podemos aplicar a DFT nesse sinal e obter uma resposta confiável para poder comparar com as do nossos testes.

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/sinal%2BFFT.jpg?raw=true)

## Implementações

### Retas


### Python

### VHDL

#### A entrada do componente possui 25 bits: 

#### A saída do componente possui 16 bits: 

#### Para usar

### Possíveis dúvidas