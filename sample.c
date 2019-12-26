#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <locale.h>

double average(long (*f)(void), int runs) {
  long total = 0;
  for(long i = 0; i < runs; i++) {
    total += f();
  }
  return (double)total / (double)runs;
}

long roll(long numToRoll, long target, long explode) {
  long numHit = 0;
  for(long i = 0; i < numToRoll; i++) {
    long roll = rand() % 6 + 1;
    if (roll >= target) {
      numHit++;
      if (roll >= explode) {
        numHit++;
      }
    }
  }
  return numHit;
}

long simulate(long numAttacks, long toHit, long toWound, long explode) {
  long hits = roll(numAttacks, toHit, explode);
  long wounds = roll(hits, toWound, 7);
  return wounds;
}

long normalHits() {
  return simulate(21, 3, 4, 6);
}

long greatBlade() {
  long normal = simulate(19, 3, 4, 6);
  normal += simulate(2, 3, 3, 7);
  return normal;
}

long bladeLeader() {
  long normal = simulate(18, 3, 4, 6);
  normal += simulate(3, 3, 3, 7);
  return normal;
}

long buffHits() {
  return simulate(21, 2, 4, 6);
}

long buffGreatBlade() {
  long normal = simulate(19, 2, 4, 6);
  normal += simulate(2, 2, 3, 7);
  return normal;
}

long buffBladeLeader() {
  long normal = simulate(18, 2, 4, 6);
  normal += simulate(3, 2, 3, 7);
  return normal;
}

long explode() {
  return simulate(21, 3, 4, 5);
}

long explodeGreatBlade() {
  long normal = simulate(19, 3, 4, 5);
  normal += simulate(2, 3, 3, 7);
  return normal;
}

long explodeBladeLeader() {
  long normal = simulate(18, 3, 4, 5);
  normal += simulate(3, 3, 3, 7);
  return normal;
}

struct AvgArgs {
  char *type;
  long (*f)(void);
  long numSimulate;
};

void *startThread(void *args) {
  struct AvgArgs *avg = (struct AvgArgs *)args;
  double total = average(avg->f, avg->numSimulate);
  printf(avg->type, total);
  return 0;
}


void runSimulation(long numSimulate) {
  setlocale(LC_NUMERIC, "");
  printf("Running %'ld simulations\n\n", numSimulate);

  pthread_t threads[9];
  struct AvgArgs args[] = {
    {"no buffs                   : %f\n", normalHits, numSimulate},
    {"no buffs, greatblade       : %f\n", greatBlade, numSimulate},
    {"no buffs, greatblade leader: %f\n", bladeLeader, numSimulate},
    {"hit buff                   : %f\n", buffHits, numSimulate},
    {"hit buff, greatblade       : %f\n", buffGreatBlade, numSimulate},
    {"hit buff, greatblade leader: %f\n", buffBladeLeader, numSimulate},
    {"exp buff                   : %f\n", explode, numSimulate},
    {"exp buff, greatblade       : %f\n", explodeGreatBlade, numSimulate},
    {"exp buff, greatblade leader: %f\n", explodeBladeLeader, numSimulate}
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

  runSimulation(10000000);

  exit(0);
}

