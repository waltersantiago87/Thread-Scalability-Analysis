# Além do Paralelismo

## Uma Análise Experimental da Escalabilidade de Threads na Multiplicação de Matrizes

Pesquisa experimental desenvolvida na disciplina de Infraestrutura de Software, no curso de Ciência da Computação da CESAR School.

## Autores

- Walter Maia
- Rafael Borba

**Professor:** Ronierison Maciel

## Objetivo

Investigar como a quantidade de threads, a dimensão das matrizes e os padrões de acesso à memória influenciam o desempenho, a escalabilidade e a eficiência da multiplicação de matrizes.

## Artigo científico

[Acessar o artigo completo (PDF)](docs/artigo.pdf)

## Metodologia

Foram implementadas quatro versões do algoritmo:

- IJK sequencial
- IKJ sequencial
- IJK paralelo
- IKJ paralelo

Os experimentos consideraram:

- Matrizes de dimensões 128, 256, 512 e 1024.
- Equipes de 1, 2, 4, 6, 8, 10, 12, 16, 24 e 32 threads.
- 88 configurações experimentais.
- 10 repetições por configuração.
- 880 medições de desempenho.

## Principais resultados

Para matrizes de dimensão 1024:

| Implementação | Tempo mediano |
|---|---:|
| IJK sequencial | 19,7639 s |
| IJK paralelo (8 threads) | 0,9069 s |
| IKJ sequencial | 0,5323 s |
| IKJ paralelo (8 threads) | 0,1093 s |

A reorganização dos laços proporcionou um ganho sequencial de aproximadamente 37,13 vezes.

A combinação da reorganização dos laços com o paralelismo proporcionou um ganho de aproximadamente 180,8 vezes em relação à implementação IJK sequencial.

Os resultados demonstram que aumentar a quantidade de threads não garante, isoladamente, maior desempenho.

## Gráficos

- [Escalabilidade IJK](figures/speedup_ijk.pdf)
- [Escalabilidade IKJ](figures/speedup_ikj.pdf)

## Dados experimentais

- [Dados brutos](results/raw_data.csv)
- [Resultados estatísticos](results/summary.csv)
- [Ambiente experimental](docs/environment.txt)

## Tecnologias

- C
- OpenMP 4.5
- GCC
- Python 3
- Matplotlib
- Ubuntu 22.04 (WSL 2)

## Estrutura do repositório

| Diretório | Descrição |
|---|---|
| src/ | Código-fonte dos algoritmos |
| tests/ | Testes de validação |
| scripts/ | Automação e análise experimental |
| results/ | Dados brutos e estatísticas |
| figures/ | Gráficos de escalabilidade |
| docs/ | Artigo e documentação |

## Compilação

```bash
make
```

## Execução

```bash
./bin/matrix seq-ijk 512 1
./bin/matrix seq-ikj 512 1
./bin/matrix par-ijk 512 8
./bin/matrix par-ikj 512 8
```

## Reprodução dos experimentos

```bash
python3 scripts/benchmark.py
```

O script de coleta protege os dados existentes contra sobrescrita. Para realizar uma nova coleta, preserve o arquivo raw_data.csv original em outro local.

## Reprodução da análise estatística

```bash
python3 scripts/analyze.py
```

A análise utiliza os dados existentes e gera automaticamente as estatísticas e os gráficos de escalabilidade.

## Limitações

Os resultados estão associados ao ambiente experimental utilizado. Não foram realizadas medições diretas de eventos de cache, controle de frequência do processador ou testes em diferentes plataformas.

## Status

Pesquisa acadêmica concluída.
