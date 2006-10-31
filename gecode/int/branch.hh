/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2002
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  See the file "LICENSE" for information on usage and
 *  redistribution of this file, and for a
 *     DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef __GECODE_INT_BRANCH_HH__
#define __GECODE_INT_BRANCH_HH__

#include "gecode/int.hh"

/**
 * \namespace Gecode::Int::Branch
 * \brief Integer branchings
 */

namespace Gecode { namespace Int { namespace Branch {

  /*
   * Value selection classes
   *
   */

  /**
   * \brief Class for selecting minimum value
   *
   * All value selection classes require
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValMin {
  public:
    /// Return minimum value of view \a x
    int val(const Space* home, View x) const;
    /// Tell \f$x=n\f$ (\a a = 0) or \f$x\neq n\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, int n);
  };

  /**
   * \brief Class for selecting maximum value
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValMed {
  public:
    /// Return maximum value of view \a x
    int val(const Space* home, View x) const;
    /// Tell \f$x=n\f$ (\a a = 0) or \f$x\neq n\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, int n);
  };

  /**
   * \brief Class for selecting median value
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValMax {
  public:
    /// Return median value of view \a x
    int val(const Space* home, View x) const;
    /// Tell \f$x=n\f$ (\a a = 0) or \f$x\neq n\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, int n);
  };

  /**
   * \brief Class for splitting domain (lower half first)
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValSplitMin {
  public:
    /// Return minimum value of view \a x
    int val(const Space* home, View x) const;
    /// Tell \f$x\leq n\f$ (\a a = 0) or \f$x >n\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, int n);
  };

  /**
   * \brief Class for splitting domain (upper half first)
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValSplitMax {
  public:
    /// Return minimum value of view \a x
    int val(const Space* home, View x) const;
    /// Tell \f$x>n\f$ (\a a = 0) or \f$x\leq n\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, int n);
  };

  /// Create branchings for a given view selection strategy \a ViewSel
  template<class View, class SelView>
  static void
  create(Space* home, ViewArray<View>&, BvalSel);

  /// For Boolean branchings not needing a value
  class NoValue {};

  /**
   * \brief Class for trying zero and then one
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValZeroOne {
  public:
    /// Return no value of view \a x
    NoValue val(const Space* home, View x) const;
    /// Tell \f$x=0\f$ (\a a = 0) or \f$x=1\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, NoValue n);
  };

  /**
   * \brief Class for trying one and then zero
   *
   * Requires
   * \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelVal
   */
  template<class View>
  class ValOneZero {
  public:
    /// Return no value of view \a x
    NoValue val(const Space* home, View x) const;
    /// Tell \f$x=1\f$ (\a a = 0) or \f$x=0\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, View x, NoValue n);
  };


  /*
   * Variable selection classes
   *
   */

  /**
   * \brief View selection class for first variable
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByNone {
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest min
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByMinMin {
  protected:
    /// So-far smallest minimum
    int min;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest min
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByMinMax {
  protected:
    /// So-far largest minimum
    int min;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest max
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByMaxMin {
  protected:
    /// So-far smallest maximum
    int max;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest max
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByMaxMax {
  protected:
    /// So-far largest maximum
    int max;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest size
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class BySizeMin {
  protected:
    /// So-far smallest size
    unsigned int size;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest size
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class BySizeMax {
  protected:
    /// So-far largest size
    unsigned int size;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest degree (and smallest size in case of ties)
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByDegreeMin {
  protected:
    /// So-far smallest degree
    unsigned int degree;
    /// So-far smallest size for degree
    unsigned int size;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest degree (and smallest size in case of ties)
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByDegreeMax {
  protected:
    /// So-far largest degree
    unsigned int degree;
    /// So-far smallest size for degree
    unsigned int size;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest degree
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByDegreeMinNoTies {
  protected:
    /// So-far smallest degree
    unsigned int degree;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest degree
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByDegreeMaxNoTies {
  protected:
    /// So-far largest degree
    unsigned int degree;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest min-regret
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByRegretMinMin {
  protected:
    /// So-far smallest regret
    unsigned int regret;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest min-regret
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByRegretMinMax {
  protected:
    /// So-far largest regret
    unsigned int regret;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with smallest max-regret
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByRegretMaxMin {
  protected:
    /// So-far smallest regret
    unsigned int regret;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

  /**
   * \brief View selection class for view with largest max-regret
   *
   * Requires \code #include "gecode/int/branch.hh" \endcode
   * \ingroup FuncIntSelView
   */
  template<class View>
  class ByRegretMaxMax {
  protected:
    /// So-far largest regret
    unsigned int regret;
  public:
    /// Intialize with view \a x
    ViewSelStatus init(const Space* home, View x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space* home, View x);
  };

}}}

#include "gecode/int/branch/select-val.icc"
#include "gecode/int/branch/select-view.icc"

#endif

// STATISTICS: int-branch

