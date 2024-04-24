#include "probas.hpp"

double rand01() {
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

int poissonLaw(float lambda)
{
  int nb_essais = 0;
  float sum = 0;
  while(sum < 1)
  {
    sum += exponentialLaw(lambda);
    nb_essais ++;
  }
  return nb_essais - 1;
}
