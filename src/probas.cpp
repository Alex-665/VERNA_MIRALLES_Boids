#include "probas.hpp"
#include "boid.hpp"
#include "flock.hpp"

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

void computeEvolution(Flock &flock, const p6::Context &ctx, float &birth_time, float &death_time, parameters &p)
{
  if((int)(ctx.time()) % (int)(birth_time) == 0 &&  ((int)(ctx.time() - ctx.delta_time()) % (int)birth_time) != 0)
  {
    Boid tmp;
    flock.add_boid(tmp);
    p._boids_number ++;
    birth_time = exponentialLaw(0.1f);
  }
  if((int)(ctx.time()) % (int)(death_time) == 0 &&  ((int)(ctx.time() - ctx.delta_time()) % (int)death_time) != 0)
  {
    flock.remove_boid();
    p._boids_number --;
    death_time = exponentialLaw(0.09f);
  }
}