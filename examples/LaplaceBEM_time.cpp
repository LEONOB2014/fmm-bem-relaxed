/** @file serialBEM.cpp
 * @brief Testing and debugging script for FMM-BEM applications
 */


#include "FMM_plan.hpp"
#include "DirectMatvec.hpp"
#include "LaplaceSphericalBEM.hpp"
#include "Triangulation.hpp"

#include "GMRES.hpp"
#include "fmgmres.hpp"

#include "timing.hpp"

enum SolveType { INITIAL_SOLVE = 1,
                 GMRES_DIAGONAL = 2,
                 INNER_OUTER = 4 };

struct ProblemOptions
{
  typedef enum { PHI_SET, DPHIDN_SET } BoundaryCondition;
  BoundaryCondition bc_ = PHI_SET;
  double value_ = 1.;
  int recursions = 4;

  ProblemOptions() : bc_(PHI_SET), value_(1.) {};
  ProblemOptions(double value) : bc_(PHI_SET), value_(value) {};
  ProblemOptions(BoundaryCondition bc, double value) : bc_(bc), value_(value) {};

  double getValue() { return value_; };
  BoundaryCondition getBC() { return bc_; };
};

void printHelpAndExit()
{
  printf("serialBEM : FMM-BEM for Potential problems\n");
  printf("\nUsage: ./serialBEM <options>\n\n");
  printf("Options:\n");
  printf("-theta <double> : Set MAC theta for treecode evaluators\n");
  printf("-eval {FMM,TREE} : Choose either FMM or treecode evaluator\n");
  printf("-p <double> : Number of terms in the Multipole / Local expansions\n");
  printf("-k {1,3,4,7} : Number of Gauss integration points used per panel\n");
  printf("-lazy_eval : enable 'lazy' evaluator\n");
  printf("-ncrit <int> : Maximum # of particles per Octree box\n");
  printf("-recursions <int> : number of recursive subdivisions to create a sphere - # panels = 2*4^recursions\n");
  printf("-help : print this message\n");
  std::exit(0);
}

template <typename SourceType, typename ChargeType>
void initialiseSphere(std::vector<SourceType>& panels,
                      std::vector<ChargeType>&  charges,
                      unsigned recursions = 4)
{
  (void) charges;
  create_unit_sphere(panels, recursions);
}

int main(int argc, char **argv)
{
  int numPanels= 1000, recursions = 4, p = 5, k = 3;
  FMMOptions opts = get_options(argc,argv);
  // opts.set_max_per_box(10);
  SolverOptions solver_options;
  bool second_kind = false;

  // use lazy evaluator by default
  // opts.lazy_evaluation = true;

  // parse command line args
  // check if no arguments given
  if (argc == 1) printHelpAndExit();
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i],"-N") == 0) {
      i++;
      numPanels = atoi(argv[i]);
    } else if (strcmp(argv[i],"-p") == 0) {
      i++;
      p = atoi(argv[i]);
      solver_options.max_p = p;
    } else if (strcmp(argv[i],"-k") == 0) {
      i++;
      k = atoi(argv[i]);
    } else if (strcmp(argv[i],"-recursions") == 0) {
      i++;
      recursions = atoi(argv[i]);
    } else if (strcmp(argv[i],"-second_kind") == 0) {
      second_kind = true;
    } else if (strcmp(argv[i],"-fixed_p") == 0) {
      solver_options.variable_p = false;
    } else if (strcmp(argv[i],"-solver_tol") == 0) {
      i++;
      solver_options.residual = (double)atof(argv[i]);
    } else if (strcmp(argv[i],"-help") == 0) {
      printHelpAndExit();
    } else {
      printf("[W]: Unknown command line arg: \"%s\"\n",argv[i]);
      // printHelpAndExit();
    }
  }
  double tic, toc;
  // Init the FMM Kernel
  typedef LaplaceSphericalBEM kernel_type;
  kernel_type K(p,k);

  // useful typedefs
  typedef kernel_type::point_type  point_type;
  typedef kernel_type::source_type source_type;
  typedef kernel_type::target_type target_type;
  typedef kernel_type::charge_type charge_type;
  typedef kernel_type::result_type result_type;

  // Init points and charges
  std::vector<source_type> panels(numPanels);
  std::vector<charge_type> charges(numPanels);
  initialiseSphere(panels, charges, recursions); //, ProblemOptions());

  // run case solving for Phi (instead of dPhi/dn)
  if (second_kind)
    for (auto& it : panels) it.switch_BC();

  // set constant Phi || dPhi/dn for each panel
  charges.resize(panels.size());
  charges = std::vector<charge_type>(panels.size(),1.);

  // Build the FMM structure
  FMM_plan<kernel_type> plan = FMM_plan<kernel_type>(K, panels, opts);

  // single evaluation
  std::vector<charge_type> c_temp(panels.size(),1.);
  std::vector<result_type> r_temp(panels.size(),0.);
  tic = get_time();
  r_temp = plan.execute(c_temp,p);
  toc = get_time();

  printf("# panels: %d, t: %.4e\n",(int)panels.size(), toc-tic);

}

