/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *     Christian Schulte, 2004
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#include "gecode/set.hh"
#include "examples/support.hh"

namespace {
  /// The airline employees
  typedef enum {
    Tom, David, Jeremy, Ron,
    Joe, Bill, Fred,
    Bob, Mario, Ed,
    Carol, Janet, Tracy,
    Marilyn, Carolyn, Cathy,
    Inez, Jean, Heather, Juliet
  } Employee;
  const int noOfEmployees = Juliet+1;

  /// A flight to schedule
  struct Flight {
    int staff;     ///< Overall number of cabin crew needed
    int stewards;  ///< How many stewards are required
    int hostesses; ///< How many hostesses are required
    int french;    ///< How many French speaking employees are required
    int spanish;   ///< How many Spanish speaking employees are required
    int german;    ///< How many German speaking employees are required
  };

  const char* employeeToName(Employee e);
  extern const int stewards[];
  extern const int noOfStewards;
  extern const int hostesses[];
  extern const int noOfHostesses;
  extern const int spanishSpeaking[];
  extern const int noOfSpanishSpeaking;
  extern const int frenchSpeaking[];
  extern const int noOfFrenchSpeaking;
  extern const int germanSpeaking[];
  extern const int noOfGermanSpeaking;
  extern const Flight requiredCrew[];
  extern const int noOfFlights;
}

/**
 * \brief %Example: Airline crew allocation
 *
 * Assign 20 flight attendants to 10 flights. Each flight needs a certain
 * number of cabin crew, and they have to speak certain languages.
 * Every cabin crew member has two flights off after an attended flight.
 *
 * \ingroup ExProblem
 *
 */
class Crew : public Example {
public:
  /// The crew for each flight
  SetVarArray flight;

  /// The actual model
  Crew(const Options&) :
    flight(this,noOfFlights,IntSet::empty,0,noOfEmployees-1)
  {
    IntSet stewardsDS(stewards,noOfStewards);
    IntSet hostessesDS(hostesses,noOfHostesses);
    IntSet spanishDS(spanishSpeaking, noOfSpanishSpeaking);
    IntSet frenchDS(frenchSpeaking, noOfFrenchSpeaking);
    IntSet germanDS(germanSpeaking, noOfGermanSpeaking);

    for (int i=0; i<noOfFlights; i++) {
      IntVarArray ia(this,5,0,noOfEmployees-1);
      SetVar team = flight[i];

      int N        = requiredCrew[i].staff;
      int NStew    = requiredCrew[i].stewards;
      int NHost    = requiredCrew[i].hostesses;
      int NFrench  = requiredCrew[i].french;
      int NSpanish = requiredCrew[i].spanish;
      int NGerman  = requiredCrew[i].german;

      cardinality(this, team,N,N);
      SetVar stewardsInFS(this);
      SetVar hostessesInFS(this);
      SetVar spanishInFS(this);
      SetVar frenchInFS(this);
      SetVar germanInFS(this);

      rel(this, team, SOT_INTER, stewardsDS, SRT_EQ, stewardsInFS);
      rel(this, team, SOT_INTER, hostessesDS, SRT_EQ, hostessesInFS);
      rel(this, team, SOT_INTER, spanishDS, SRT_EQ, spanishInFS);
      rel(this, team, SOT_INTER, frenchDS, SRT_EQ, frenchInFS);
      rel(this, team, SOT_INTER, germanDS, SRT_EQ, germanInFS);

      cardinality(this, stewardsInFS, ia[0]);
      cardinality(this, hostessesInFS, ia[1]);
      cardinality(this, spanishInFS, ia[2]);
      cardinality(this, frenchInFS, ia[3]);
      cardinality(this, germanInFS, ia[4]);

      rel(this, ia[0], IRT_GQ, NStew);
      rel(this, ia[1], IRT_GQ, NHost);
      rel(this, ia[2], IRT_GQ, NSpanish);
      rel(this, ia[3], IRT_GQ, NFrench);
      rel(this, ia[4], IRT_GQ, NGerman);

    }

    for (int i=0; i<noOfFlights-2; i++) {
      rel(this, flight[i], SRT_DISJ, flight[i+1]);
      rel(this, flight[i], SRT_DISJ, flight[i+2]);
    }
    rel(this, flight[noOfFlights-2], SRT_DISJ, flight[noOfFlights-1]);

    branch(this, flight, SET_VAR_NONE, SET_VAL_MIN);
  }

  /// Print solution
  virtual void
  print(std::ostream& os) {
    for (int i=0; i<noOfFlights; i++) {
      SetVarGlbValues d(flight[i]);

      os << "\tFlight " << i+1 << ":" << std::endl;
      os << "\t\tCrew\tStew.\tHost.\tFrench\tSpanish\tGerman"
         << std::endl << "\t";
      os << "\t" << requiredCrew[i].staff << "\t" << requiredCrew[i].stewards
         << "\t" << requiredCrew[i].hostesses << "\t"
         << requiredCrew[i].spanish
         << "\t" << requiredCrew[i].french << "\t" << requiredCrew[i].german 
         << std::endl;

      os << "\t\tSchedule:" << std::endl << "\t\t";
      for (;d();++d) {
        os << employeeToName(static_cast<Employee>(d.val())) << " ";
      }
      os << std::endl << std::endl;
    }
  }

  /// Constructor for cloning \a s
  Crew(bool share, Crew& s)
    : Example(share,s) {
    flight.update(this,share,s.flight);
  }
  /// Copy during cloning
  virtual
  Space *copy(bool share) {
    return new Crew(share,*this);
  }

};

/** \brief Main-function
 *  \relates Crew
 */
int
main(int argc, char* argv[]) {
  Options o("Crew");
  o.iterations(100);
  o.parse(argc,argv);
  Example::run<Crew,DFSE,Options>(o);
  return 0;
}

namespace {
  
  /// Return name of employee \a e as a string
  const char*
  employeeToName(Employee e) {
    switch(e) {
    case Tom : return "Tom";
    case David : return "David";
    case Jeremy: return "Jeremy";
    case Ron: return "Ron";
    case Joe: return "Joe";
    case Bill: return "Bill";
    case Fred: return "Fred";
    case Bob: return "Bob";
    case Mario: return "Mario";
    case Ed: return "Ed";
    case Carol: return "Carol";
    case Janet: return "Janet";
    case Tracy: return "Tracy";
    case Marilyn: return "Marilyn";
    case Carolyn: return "Carolyn";
    case Cathy: return "Cathy";
    case Inez: return "Inez";
    case Jean: return "Jean";
    case Heather: return "Heather";
    case Juliet: return "Juliet";
    default: GECODE_NEVER; return "";
    }
  }

  // these have to be sorted!
  /// The stewards
  const int stewards[] =
    {Tom, David, Jeremy, Ron, Joe, Bill, Fred, Bob, Mario, Ed};
  /// The number of stewards
  const int noOfStewards = sizeof(stewards) / sizeof(int);
  /// The hostesses
  const int hostesses[] =
    { Carol, Janet, Tracy, Marilyn, Carolyn, Cathy, Inez,
      Jean, Heather, Juliet };
  /// The number of hostesses
  const int noOfHostesses = sizeof(hostesses) / sizeof(int);
  /// The French speaking employees
  const int frenchSpeaking[] =
    { Bill, Inez, Jean, Juliet };
  /// The number of French speaking employees
  const int noOfFrenchSpeaking = sizeof(frenchSpeaking) / sizeof(int);
  /// The German speaking employees
  const int germanSpeaking[] =
    { Tom, Jeremy, Mario, Cathy, Juliet };
  /// The number of German speaking employees
  const int noOfGermanSpeaking = sizeof(germanSpeaking) / sizeof(int);
  /// The Spanish speaking employees
  const int spanishSpeaking[] =
    { Joe, Bill, Fred, Mario, Marilyn, Inez, Heather };
  /// The number of Spanish speaking employees
  const int noOfSpanishSpeaking = sizeof(spanishSpeaking) / sizeof(int);

  /// The flights to schedule
  const Flight requiredCrew[] =
    { {4,1,1,1,1,1},
      {5,1,1,1,1,1},
      {5,1,1,1,1,1},
      {6,2,2,1,1,1},
      {7,3,3,1,1,1},
      {4,1,1,1,1,1},
      {5,1,1,1,1,1},
      {6,1,1,1,1,1},
      {6,2,2,1,1,1},
      {7,3,3,1,1,1} };
  
  /// The number of flights to schedule
  const int noOfFlights = sizeof(requiredCrew) / sizeof(Flight);
}

// STATISTICS: example-any

