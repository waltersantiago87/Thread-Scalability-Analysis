# Experimentos preliminares

Data: 26/11/2026
Versao do codigo: 0db15a6

Foram realizadas oito medicoes exploratorias,
considerando duas dimensoes de matrizes e quatro
modalidades de execucao.

Os resultados estao armazenados em
results/preliminary.csv.

## Observacoes

- Os checksums coincidiram entre as implementacoes.
- A ordem IKJ apresentou menor tempo de execucao.
- Foi observado um ganho aparentemente superlinear
  na implementacao IJK para a dimensao 1024.

## Limitacoes

- Apenas uma execucao por configuracao.
- Ambiente virtualizado WSL 2.
- Ausencia de controle rigoroso da ordem dos testes.
- Ausencia de validacao elemento por elemento.
- Regioes OpenMP serializadas na versao sequencial.

Os resultados sao exploratorios e nao constituem
conclusoes definitivas da pesquisa.
