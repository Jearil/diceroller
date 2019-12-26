#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float average(int (*f)(void), int runs) {
  int total = 0;
  for(int i = 0; i < runs; i++) {
    total += f();
  }
  return (float)total / (float)runs;
}

int roll(int numToRoll, int target, int explode) {
  int numHit = 0;
  for(int i = 0; i < numToRoll; i++) {
    int roll = rand() % 6 + 1;
    if (roll >= target) {
      numHit++;
      if (roll >= explode) {
        numHit++;
      }
    }
  }
  return numHit;
}

int simulate(int numAttacks, int toHit, int toWound, int explode) {
  int hits = roll(numAttacks, toHit, explode);
  int wounds = roll(hits, toWound, 7);
  return wounds;
}

int normalHits() {
  return simulate(21, 3, 4, 6);
}

int greatBlade() {
  int normal = simulate(19, 3, 4, 6);
  normal += simulate(2, 3, 3, 7);
  return normal;
}

int bladeLeader() {
  int normal = simulate(18, 3, 4, 6);
  normal += simulate(3, 3, 3, 7);
  return normal;
}

int buffHits() {
  return simulate(21, 2, 4, 6);
}

int buffGreatBlade() {
  int normal = simulate(19, 2, 4, 6);
  normal += simulate(2, 2, 3, 7);
  return normal;
}

int buffBladeLeader() {
  int normal = simulate(18, 2, 4, 6);
  normal += simulate(3, 2, 3, 7);
  return normal;
}

int explode() {
  return simulate(21, 3, 4, 5);
}

int explodeGreatBlade() {
  int normal = simulate(19, 3, 4, 5);
  normal += simulate(2, 3, 3, 7);
  return normal;
}

int explodeBladeLeader() {
  int normal = simulate(18, 3, 4, 5);
  normal += simulate(3, 3, 3, 7);
  return normal;
}

void runSimulation(int numSimulate) {
  printf("Running %d simulations\n\n", numSimulate);

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

  runSimulation(1000000);
}

