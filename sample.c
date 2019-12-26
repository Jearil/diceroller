#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct AvgArgs {
  long (*f)(void);
  long numSimulate;
};

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



void runSimulation(long numSimulate) {
  printf("Running %ld simulations\n\n", numSimulate);

  printf("no buffs                   : %f\n", average(normalHits, numSimulate));
  printf("no buffs, greatblade       : %f\n", average(greatBlade, numSimulate));
  printf("no buffs, greatblade leader: %f\n", average(bladeLeader, numSimulate));
  printf("hit buff                   : %f\n", average(buffHits, numSimulate));
  printf("hit buff, greatblade       : %f\n", average(buffGreatBlade, numSimulate));
  printf("hit buff, greatblade leader: %f\n", average(buffBladeLeader, numSimulate));
  printf("exp buff                   : %f\n", average(explode, numSimulate));
  printf("exp buff, greatblade       : %f\n", average(explodeGreatBlade, numSimulate));
  printf("exp buff, greatblade leader: %f\n", average(explodeBladeLeader, numSimulate));
}

int main(void) {
  srand(time(0));

  runSimulation(1000000000);
}

