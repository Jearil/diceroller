#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <locale.h>

double average(long (*f)(uint*), int runs, uint thread) {
  long total = 0;
  for(long i = 0; i < runs; i++) {
    total += f(&thread);
  }
  return (double)total / (double)runs;
}

long roll(long numToRoll, long target, long explode, uint* thread) {
  long numHit = 0;
  for(long i = 0; i < numToRoll; i++) {
    long roll = rand_r(thread) % 6 + 1;
    if (roll >= target) {
      numHit++;
      if (roll >= explode) {
        numHit++;
      }
    }
  }
  return numHit;
}

long simulate(long numAttacks, long toHit, long toWound, long explode, uint *thread) {
  long hits = roll(numAttacks, toHit, explode, thread);
  long wounds = roll(hits, toWound, 7, thread);
  return wounds;
}

long normalHits(uint *thread) {
  return simulate(21, 3, 4, 6, thread);
}

long greatBlade(uint *thread) {
  long normal = simulate(19, 3, 4, 6, thread);
  normal += simulate(2, 3, 3, 7, thread);
  return normal;
}

long bladeLeader(uint *thread) {
  long normal = simulate(18, 3, 4, 6, thread);
  normal += simulate(3, 3, 3, 7, thread);
  return normal;
}

long buffHits(uint *thread) {
  return simulate(21, 2, 4, 6, thread);
}

long buffGreatBlade(uint *thread) {
  long normal = simulate(19, 2, 4, 6, thread);
  normal += simulate(2, 2, 3, 7, thread);
  return normal;
}

long buffBladeLeader(uint *thread) {
  long normal = simulate(18, 2, 4, 6, thread);
  normal += simulate(3, 2, 3, 7, thread);
  return normal;
}

long explode(uint *thread) {
  return simulate(21, 3, 4, 5, thread);
}

long explodeGreatBlade(uint *thread) {
  long normal = simulate(19, 3, 4, 5, thread);
  normal += simulate(2, 3, 3, 7, thread);
  return normal;
}

long explodeBladeLeader(uint *thread) {
  long normal = simulate(18, 3, 4, 5, thread);
  normal += simulate(3, 3, 3, 7, thread);
  return normal;
}

struct AvgArgs {
  int thread;
  char *type;
  long (*f)(uint*);
  long numSimulate;
};

void *startThread(void *args) {
  struct AvgArgs *avg = (struct AvgArgs *)args;
  double total = average(avg->f, avg->numSimulate, avg->thread);
  printf(avg->type, total);
  return 0;
}


void runSimulation(long numSimulate) {
  setlocale(LC_NUMERIC, "");
  printf("Running %'ld simulations\n\n", numSimulate);

  pthread_t threads[9];
  struct AvgArgs args[] = {
    {rand(), "no buffs                   : %f\n", normalHits, numSimulate},
    {rand(), "no buffs, greatblade       : %f\n", greatBlade, numSimulate},
    {rand(), "no buffs, greatblade leader: %f\n", bladeLeader, numSimulate},
    {rand(), "hit buff                   : %f\n", buffHits, numSimulate},
    {rand(), "hit buff, greatblade       : %f\n", buffGreatBlade, numSimulate},
    {rand(), "hit buff, greatblade leader: %f\n", buffBladeLeader, numSimulate},
    {rand(), "exp buff                   : %f\n", explode, numSimulate},
    {rand(), "exp buff, greatblade       : %f\n", explodeGreatBlade, numSimulate},
    {rand(), "exp buff, greatblade leader: %f\n", explodeBladeLeader, numSimulate}
  };

  for(int i = 0; i < 9; i++) {
    pthread_create(&threads[i], NULL, startThread, (void *)&args[i]);
  }
  for(int i = 0; i < 9; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main(void) {
  srand(time(0));

  runSimulation(1000000);

  exit(0);
}

