# ProjetoThreads
Vale lembrar que foi feito para o sistema operacional windows.
Para testar, eu recomendo executar o arquivo "TrabalhoAtualizado.cpp", pois ele tem otimizações e correção na definição na prioridade das threads.
Recomendo que verifique quantas threads seu processador possui, para assim conseguir tirar maior proveito do código.
Aqui está um exemplo de código que você pode copiar e colar quando executar o algoritmo:

1
10000 10000
2
1
3
4
25000000
5
4
7
6
8
6
9
10

Observações sobre as linhas:
Na linha 10, é o tamanho da matriz que irá ser usada, eu recomendo um tamanho entre 1000x1000 até 10000x10000.
Na linha 12, é a seed para a geração de números aleatórios a serem preenchidos na matriz, você pode escolher qualquer número.
Na linha 15, é o tamanho do macrobloco que será utilizado, o tamanho da matriz para cada threads, recomendo utilizar (tamanho X da matrix * tamanho Y)/(Numero de threads)
Na linha 17, é o total de threads que serão utilizadas.
Na linha 19, é a prioridade da thread, vai desde o número 1 ao 7, maiores informações estão no código, CUIDADO ao utilizar o número 7, pois devido a prioridade, seu computador irá dedicar mais tempos de CPU ao código, causando travamentos. Após executado, a situação irá se normalizar.
As linhas não citadas têm suas funções explicadas ao executar o código. 
