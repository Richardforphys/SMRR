// $Id: CrosstalkGenerator.hh 28 2010-01-12 10:24:06Z adotti $

#ifndef CROSSTALKGENERATOR_HH_
#define CROSSTALKGENERATOR_HH_

/**
 * @file   CrosstalkGenerator.hh
 *
 * @date   11 Dec 2009
 * @author adotti
 *
 * @brief Define crosstalk simulation.
 */


#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"

/*! \brief Crosstalk generator
 *
 * This class simulates the crosstalk between strips.
 * In the default implementation the crosstalks is applied in first neighbour
 * apporximation.
 * For a strip i the charge is modified to be:
 * Q'(i) = (1-2*f)*Q(i) + f*Q(i+1) + f*Q(i-1)
 */
class CrosstalkGenerator
{
public:
	/*! \brief Default constructor
	 *
	 * Creates a Crosstalk generator object
	 * @param xtalk : the fraction of charge leaking
	 * @param dimension : the number of elements (Si strips)
	 */
	CrosstalkGenerator(const double& xtalk , const int& dimension);
	//! Default destructor
	virtual ~CrosstalkGenerator() {};
	/*! \brief Simulate crosstalk
	 *
	 * The crosstalk is applied to the input.
	 * Note that the input vector has to be ordered.
	 */
	virtual CLHEP::HepVector operator()( const CLHEP::HepVector& input ) const { return xtalk*input; }
private:
	//! crosstalk parameter for first neighbours
	double firstNearXtalk;
	//! parameter defining the number of elements
	int dimension;
	//! corsstalk matrix representation
	CLHEP::HepSymMatrix xtalk;
	/*! \brief Initializes \sa xtalk
	 *
	 * This method can be overwritten to simulate
	 * more complex crosstalk patterns.
	 */
	virtual void Init();
};

#endif /* CROSSTALKGENERATOR_HH_ */
