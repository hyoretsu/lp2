// Faça um programa em C/C++ que implemente uma solução para o problema do buffer compartilhado. Com um buffer compartilhado de 1024 chars, o Programa deverá implementar duas funções:

// 	- produtor() - gera um caractere alfanumérico aleatório e o deposita no buffer; exibe o valor e a posição no buffer na tela;	dorme 50ms; repete;
// 	- consumidor() - captura um dos valores disponíveis no buffer compartilhado e armazena em um arquivo; dorme um tempo aleatório entre 30ms e 90ms; repete;

// A função produtor deverá ser realizada por uma thread, assim como a função consumidor.

// Use semáforos (biblioteca semaphore.h) para sincronizar o acesso ao buffer compartilhado, de forma que todos os caracteres gerados pelos produtores sejam consumidos uma vez, evitando qualquer problema ao ler ou escrever no buffer. Quando o buffer for consumido 4 vezes (ou seja, quando os produtores tiverem gerado 4096 caracteres e estes forem consumidos) finalize o programa, salvando o conteúdo do arquivo. Comente o código explicando o uso dos semáforos.
