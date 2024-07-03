gcc -c flecs.c -o flecs.o -I./ -lWs2_32 -std=gnu99

ar rvs flecs.a flecs.o 
