#pragma once

/**
 * Full kernel for Bio problem:
 * both Laplace & Yukawa kernels
 */

#include "LaplaceSphericalBEM.hpp"
#include "YukawaCartesianBEM.hpp"
#include <utility>

class BioBEM
{
 public:
  unsigned K;
  static constexpr unsigned dimension = LaplaceSphericalBEM::dimension;
  //! Point type
  typedef LaplaceSphericalBEM::point_type point_type;
  //! Source type
  typedef LaplaceSphericalBEM::source_type source_type;
  //! Target type;
  typedef LaplaceSphericalBEM::target_type target_type;
  //! Charge type: phi and dphi/dn
  typedef std::pair<double,double> charge_type;
  //! kernel result_type
  typedef std::pair<double,double> kernel_value_type;
  //! blah.
  typedef std::pair<double,double> result_type;
  //! Multipole expansion
  typedef std::pair<LaplaceSphericalBEM::multipole_type, YukawaCartesianBEM::multipole_type> multipole_type;
  //! Local expansion
  typedef std::pair<LaplaceSphericalBEM::local_type, YukawaCartesianBEM::local_type> local_type;
  //! Panel
  typedef LaplaceSphericalBEM::Panel panel_type;

  //! default constructor
  BioBEM(int p, double kappa, unsigned k=3) : KL(p,k), KY(p,kappa,k) {
    BEMConfig::Init();
    auto Config = BEMConfig::Instance();
    Config->setK(K);
  }

  /** Initialise a multipole expansion */
  void init_multipole(multipole_type& M, const point_type& extents, unsigned level) const {
    KL.init_multipole(M.first, extents, level);
    KY.init_multipole(M.second, extents, level);
  }

  /** Initialise a local expansion */
  void init_local(local_type& L, const point_type& extents, unsigned level) const {
    KL.init_local(L.first, extents, level);
    KY.init_local(L.second, extents, level);
  }

  /** Kernel evaluation */
  kernel_value_type operator()(const source_type& t, const target_type& s) const {
    // code here
    (void) t;
    (void) s;
    return kernel_value_type(std::make_pair(0.,0.));
  }

  /** Kernel P2M */
  void P2M(const source_type& source, const charge_type& charge, const point_type& center,
           multipole_type& M, unsigned p) const {
    (void) source;
    (void) charge;
    (void) center;
    (void) M;
    (void) p;
  }

  /** Kernel M2M -- no real differences here
   *  - Call subject kernels
   */
  void M2M(const multipole_type& Msource,
                 multipole_type& Mtarget,
           const point_type& translation, unsigned p) const {
    KL.M2M(Msource.first, Mtarget.first, translation, p);
    KY.M2M(Msource.second, Mtarget.second, translation, p);
  }

  /** M2P - only stub for now */
  template <typename PointIter, typename ResultIter>
  void M2P(const multipole_type& M, const point_type& center,
           PointIter t_begin, PointIter t_end,
           ResultIter r_begin, unsigned p) const {
    (void) M;
    (void) center;
    (void) t_begin;
    (void) t_end;
    (void) r_begin;
    (void) p;
  }

  /** M2L - call subject kernel */
  void M2L(const multipole_type& Msource,
                 local_type& Ltarget,
           const point_type& translation, unsigned p) const {
    KL.M2L(Msource.first, Ltarget.first, translation, p);
    KY.M2L(Msource.second, Ltarget.second, translation, p);
  };

  /** L2L */
  void L2L(const local_type& Lsource,
                 local_type& Ltarget,
           const point_type& translation, unsigned p) const {
    KL.L2L(Lsource.first, Ltarget.first, translation, p);
    KY.L2L(Lsource.second, Ltarget.second, translation, p);
  }

  /** L2P - only stub for now */
  template <typename PointIter, typename ResultIter>
  void L2P(const local_type& L, const point_type& center,
           PointIter t_begin, PointIter t_end,
           ResultIter r_begin, unsigned p) const {
    (void) L;
    (void) center;
    (void) t_begin;
    (void) t_end;
    (void) r_begin;
    (void) p;
  }

 private:
  //! Laplace BEM kernel
  const LaplaceSphericalBEM KL;
  //! Yukawa BEM kernel
  const YukawaCartesianBEM KY;
};
