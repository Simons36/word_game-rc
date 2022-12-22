# word_game-rc
Projeto de Redes e Computadores 2022/2023

Nota: as imagens recebidas pelo cliente encontram-se na pasta
client/source/image, o scoreboard em client/source/scoreboard
e o ficheiro do state em client/source/state.

Para alterar o tempo que o cliente espera pela resposta do servidor,
no ficheiro network.h, alterar a constante TIME_WAIT_RESPONSE para o
valor desejado em segundos.

Para desativar o timer e tentativas de reenviar a mensagem ao servidor,
alterar a constante TIMER_ACTIVE para FALSE.

script 07 funciona com o servidor em valgrind
script 20 funciona, mas no fim cliente da double free; com valgrind(no cliente)
isto não acontece
script 23 funciona com valgrind no servidor