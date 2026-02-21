// $Id: RunAction.hh 22 2009-12-22 12:36:46Z schaelic $
/**
 * @file   RunAction.hh
 *
 * @date   17 Dec 2009
 * @author adotti
 */

#ifndef RUNACTION_HH
#define RUNACTION_HH 1

/*!
 * \file
 * \brief User's Run Action
 */
#include "G4UserRunAction.hh"

class G4Run;

/*!
 * \brief User's RunAction class
 * This class implements the Run Action
 * The relevant method is \sa BeginOfRunAction and \sa EndOfRunAction
 * This class controls the saving facility (\sa RootSaver class), since
 * the handling of ROOT TTree is done at run level (each run one TTree)
 */
class RunAction : public G4UserRunAction
{
public:
  //! constructor
  RunAction();
  //! destructor
  virtual ~RunAction();
  //! Called at the beginning of each run
  virtual void BeginOfRunAction(const G4Run*);
  //! Called at the end of each run
  virtual void EndOfRunAction(const G4Run*);
};

#endif /* RUNACTION_HH */
