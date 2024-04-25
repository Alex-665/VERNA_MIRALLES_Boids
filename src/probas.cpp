#include "probas.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include "glm/ext/quaternion_exponential.hpp"

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

void computeEvolution(Flock &flock, const p6::Context &ctx, float &birth_time, float &death_time, int &n)
{
  if((int)(ctx.time()) % (int)(birth_time) == 0 &&  ((int)(ctx.time() - ctx.delta_time()) % (int)birth_time) != 0)
  {
    Boid tmp;
    flock.add_boid(tmp);
    n++;
    birth_time = exponentialLaw(0.1f); 
    if (birth_time < 2) birth_time += 2.f; //To avoid dividing by 0 and taking a modulo 1
    std::cout << "birth time : " <<birth_time << "\n";
  }
  if((int)(ctx.time()) % (int)(death_time) == 0 &&  ((int)(ctx.time() - ctx.delta_time()) % (int)death_time) != 0)
  {
    flock.remove_boid();
    n--;
    death_time = exponentialLaw(0.09f);
    if(death_time < 2) death_time += 2.f; //To avoid dividing by 0 and taking a modulo 1
    std::cout << "death time " << death_time << "\n";
    }
}
double beta_law(float a, float b)
{
  float u = rand01();
  return glm::pow(u, a-1) * glm::pow(1-u, b-1);
}
