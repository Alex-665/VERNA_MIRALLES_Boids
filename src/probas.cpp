#include "probas.hpp"
#include "boid.hpp"

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

void computeEvolution(Flock &flock, p6::Context ctx, float &elapsed_time, float &birth_time, float death_time)
{
  if((int)(elapsed_time) % (int)(birth_time) == 0 &&  ((int)(elapsed_time - ctx.delta_time()) % 5) != 0)
  {
    Boid tmp;
    flock.add_boid(tmp);
    birth_time = exponentialLaw(0.1f);
  }
  if((int)(elapsed_time) % (int)(death_time) == 0 &&  ((int)(elapsed_time - ctx.delta_time()) % 5) != 0)
  {
    flock.remove_boid();
    death_time = exponentialLaw(0.09f);
  }
}
