# Transformada Discreta de Fourrier
Implementação da Transformada Discreta de Fourrier em C para PIC18F4550. Abordagem: otimização do tempo de execução.

### Autores:

* **José João Silva** - [jjoaosilva](https://github.com/jjoaosilva/)
* **Mario Matheus**   - [MarioMatheus](https://github.com/MarioMatheus/)

## Sobre
Esse trabalho foi desenvolvimento na disciplica de Sistema Embarcados 1 do curso de Engenharia de computação do Instituto de educação, ciência e tecnologia do Ceará(IFCE).

A ideia era escolher um algoritmo que atendesse aos requisitos mínimos necessários para a disciplina. O algoritmo escolhido foi a Transformada Discreta de Fourrier(DFT) com 512 pontos de entrada. Em resumo, a DFT decompõe um sinal temporal no domínio da frequência e tal transformada é dada por: 

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/equacao.PNG?raw=true)

Vale salientar que o objetivo não é otimizar a DFT no seu formato de cálculo e sim manter a implementação do cálculo do algoritmo original e modificar funções visando diminuir o tempo de execução.

## Base de dados confiável
Utilizando a ferramenta Matlab, foi aquisitada a amostra de teste da DFT: Senoide, 60 Hz, pico 1, taxa de amostragem: 1 KHz. Com isso, podemos aplicar a DFT nesse sinal e obter uma resposta confiável para poder comparar com as do nossos testes.

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/sinal%2BFFT.jpg?raw=true)

A partir daí, nossa DFT foi testada em um PIC18F4550 normalmente sem nenhuma otimização. Podemos observar sua saída e a a diferença entre ela e a do Matlab a seguir:

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/DFT%2BerroSemL.jpg?raw=true)

Podemos observar um erro máximo de 0.000045 

## Otimização

### Número de iterações

Para situar você, o numero de itereações realizadas pela DFT é de ordem n^2, ou seja, com 512 pontos, temos 512x512 = 262144 iterações. Porém, após a realização do cálculo, se 'jogarmos' as saídas direto no Matlab, podemos observar o seguinte comportamento:  

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/exemplo.jpg?raw=true)

Ou seja, a saída da DFT, que deve ser semelhante a da Transformada Rápida de Fourrier(FFT) apresentada a cima, é espelhada e para afeito de análise, precisamos apenas de metade desses pontos. Com isso, podemos, em invez de realizar 262144 iterações, realizar apenas 512 itereções. O que diminui pela metade o tempo de processamento.

### Porque Linezarizar?
Mesmo com o corte de itereações, o processo ainda era lento. Se você observar o cálculo da DFT, verá que dentro de cada iteração é realizado o cálculo de de 2 senos e 2 cossenos, ou seja, 4 operações trigonométricas por iteração. Sabendo que esse tipo de cálculo possui um elevado custo de processamento ao controlador, foi dada a ideia de linearização do seno e cosseno. 

Vale salientar que isso tudo não foi um tiro no escuro: foram substituídas as funções seno e cosseno por equações do primeiro grau quaisquer e foi póssivel observar visualmente que as saídas do PIC estavam sendo geradas mais rapidamente.

### Linearização da função Seno
Utilizando a ferramenta Matlab, a função seno foi linearizada dessa forma:

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/Linearizacao/LinearizacaoSeno/senoL.png?raw=true)

As equações podem ser vistas aqui: [seno](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/Linearizacao/LinearizacaoSeno/Equacoes.txt)

### Linearização da função Cosseno
Utilizando a ferramenta Matlab, a função cosseno foi linearizada dessa forma:

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/Linearizacao/LinearizacaoCosseno/cosseno.jpg?raw=true)

As equações podem ser vistas aqui: [cosseno](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/Linearizacao/LinearizacaoCosseno/equacoes.txt)

### Testes Após linearização

#### Foram realizados dois testes: 
* DFT apenas com Seno Linearizado
* DFT com Seno e Cosseno Linearizado

#### DFT apenas com Seno Linearizado

Utilizando apenas a função seno linearizada e jé comparando com a DFT do Matlab, temos: 

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/DFT%2BerroComLS.jpg?raw=true)

Podemos observar um erro máximo de 0.035 

#### DFT com Seno e Cosseno Linearizado

Utilizando as duas funções linearizadas e jé comparando com a DFT do Matlab, temos: 

![alt text](https://github.com/jjoaosilva/TransformadaDiscretaDeFourrier/blob/master/imgs/DFT%2BerroComLSC.jpg?raw=true)

Podemos observar um erro máximo de 0.3

### Resultados Finais

* No primeiro teste, sem nenhuma modificação do algoritmo original, uma DFT com 512 pontos era executada em 19 minutos e 50 segundos em um PIC18F4550 a 48MHz

* Após a diminuição do número de interações, temos que uma DFT com 512 pontos era executada em 10 minutos e 16 segundos em um PIC18F4550 a 48MHz

* Após a linezarização da função seno, temos que uma DFT com 512 pontos era executada em 7 minutos e 07 segundos em um PIC18F4550 a 48MHz

* Após a linezarização da função seno e cosseno, temos que uma DFT com 512 pontos era executada em 3 minutos e 15 segundos em um PIC18F4550 a 48MHz

-------- | Tempos   | Erro
-------- | -------- | --------
Original | 00:19:50 | 0.000045
CorteI   | 00:10:16 | 0.000045
LS       | 00:07:07 | 0.035
LSC      | 00:03:15 | 0.3

#### Onde:
* CorteI - Corte de iterações
* LS     - Linearização da função seno
* LSC    - Linearização da função seno e cosseno

* Por fim, concluímos que após uma redução de 16 minutos e 35 segundos, o erro também aumentou de quase zero para 0.3. Qual abordagem usar, vai depender do seu problema: você quer tempo ou precisão?

### Caso procure

* A pasta DFT.X é um projeto do MplabX
* Na pasta Linezarizazao temos as funções linezarizadas com suas retas nas pastas LinearizacaoCosseno e LinearizacaoSeno. Já na pasta testeDektop temos testes dessas funções em python e C para desktop.


License
O projeto é licenciado pela MIT License - veja a [LICENSE.md](LICENSE) para mais detalhes