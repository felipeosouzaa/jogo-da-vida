
# Jogo da Vida (Conway's Game of Life)

Implementação em **C** do autômato celular criado por John Conway.  
O programa lê um tabuleiro inicial a partir de um arquivo de entrada, simula várias gerações e salva os resultados em um arquivo de saída.

## 🚀 Funcionalidades
- Leitura do tabuleiro a partir de arquivo.
- Simulação de múltiplas gerações.
- Regras do Jogo da Vida implementadas (sobrevivência, morte e nascimento).
- Geração de arquivo de saída com as transições.

## 📂 Estrutura
- `jogodavida.c` → código-fonte principal.

## ▶️ Como executar
```bash
gcc jogodavida.c -o jogo
./jogo
Será solicitado o nome do arquivo de entrada.
O programa criará automaticamente o arquivo de saída com as gerações simuladas.
