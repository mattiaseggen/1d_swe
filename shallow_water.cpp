#include <fstream>
#include <iostream>

double dt = 0.05;
double dx = 0.025;
int T = static_cast<int> (1.0 / dt);
int X = static_cast<int> (1.0 / dx);

const double density = 997.0;
const double gravity = 9.81;

double *height[2] = { nullptr, nullptr };
double *momentum[2] = { nullptr, nullptr };
double *bed_elevation = nullptr;

#define H(x, t) height[0][((t) + 1) * (X + 2) + (x) + 1]
// #define H_next(x, t) height[1][((t) + 1) * (X + 2) + (x) + 1]
#define HU(x, t) momentum[0][((t) + 1) * (X + 2) + (x) + 1]
// #define HU_next(x, t) momentum[1][((t) + 1) * (X + 2) + (x) + 1]

void time_step (int t);
void boundary_condition (int t);
void domain_init ();
void plot ();

int
main ()
{
  // Shallow water equations 1D:
  //     conservation of height
  //     conservation of momentum

  domain_init ();

  for (int t = -1; t < T - 1; t++)
    {
      time_step (t);
      boundary_condition (t);

      //      double *tmp = height[0];
      //      height[0] = height[1];
      //      height[1] = tmp;
      //      tmp = momentum[0];
      //      momentum[0] = momentum[1];
      //      momentum[1] = tmp;
    }

  plot ();

  return 0;
}

void
time_step (int t)
{
  for (int x = 0; x < X; x++)
    {
      H (x, t + 1)
          = H (x, t) - dt * (HU (x + 1, t) - HU (x - 1, t)) / (2 * dx);
    }

  for (int x = 0; x < X; x++)
    {
      HU (x, t + 1) = HU (x, t)
                      - dt
                            * (((HU (x + 1, t) - HU (x - 1, t)) / (2 * dx))
                               + 0.5 * gravity
                                     * ((H (x + 1, t) * H (x + 1, t)
                                         - H (x - 1, t) * H (x - 1, t))
                                        / (2 * dx)));
    }
}

void
boundary_condition (int t)
{
  H (-1, t) = 0;
  H (X, t) = 0;
  HU (-1, t) = 0;
  HU (X, t) = 0;
}

void
domain_init ()
{
  height[0] = new double[(T + 2) * (X + 2)]();
  height[1] = new double[(T + 2) * (X + 2)]();
  momentum[0] = new double[(T + 2) * (X + 2)]();
  momentum[1] = new double[(T + 2) * (X + 2)]();
  bed_elevation = new double[(T + 2) * (X + 2)]();

  // Initial condition
  for (int i = 0; i < X; i++)
    {
      if (i <= 20)
        {
          H (i, -1) = 10;
        }
      else
        {
          H (i, -1) = 2;
        }
    }
}

void
plot ()
{
  std::string filename = "wave.dat";

  std::ofstream data (filename);
  if (!data)
    {
      std::cerr << "Error opening file for writing." << std::endl;
      exit (EXIT_FAILURE);
    }

  data.precision (3);
  data << std::fixed;

  for (int t = -1; t < T; t++)
    {
      for (int x = -1; x <= X; x++)
        {
          data << x << ' ' << t << ' ' << H (x, t) << std::endl;
        }
    }

  data.close ();
}
