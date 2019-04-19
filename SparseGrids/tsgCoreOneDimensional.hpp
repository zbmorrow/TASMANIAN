/*
 * Copyright (c) 2017, Miroslav Stoyanov
 *
 * This file is part of
 * Toolkit for Adaptive Stochastic Modeling And Non-Intrusive ApproximatioN: TASMANIAN
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * UT-BATTELLE, LLC AND THE UNITED STATES GOVERNMENT MAKE NO REPRESENTATIONS AND DISCLAIM ALL WARRANTIES, BOTH EXPRESSED AND IMPLIED.
 * THERE ARE NO EXPRESS OR IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE SOFTWARE WILL NOT INFRINGE ANY PATENT,
 * COPYRIGHT, TRADEMARK, OR OTHER PROPRIETARY RIGHTS, OR THAT THE SOFTWARE WILL ACCOMPLISH THE INTENDED RESULTS OR THAT THE SOFTWARE OR ITS USE WILL NOT RESULT IN INJURY OR DAMAGE.
 * THE USER ASSUMES RESPONSIBILITY FOR ALL LIABILITIES, PENALTIES, FINES, CLAIMS, CAUSES OF ACTION, AND COSTS AND EXPENSES, CAUSED BY, RESULTING FROM OR ARISING OUT OF,
 * IN WHOLE OR IN PART THE USE, STORAGE OR DISPOSAL OF THE SOFTWARE.
 */

#ifndef __TSG_CORE_ONE_DIMENSIONAL_HPP
#define __TSG_CORE_ONE_DIMENSIONAL_HPP

#include "tsgLinearSolvers.hpp"

//! \internal
//! \file tsgCoreOneDimensional.hpp
//! \brief Algorithms and meta-data for the Gauuss, Chebyshev and custom rules.
//! \author Miroslav Stoyanov
//! \ingroup TasmanianCoreOneDimensional
//!
//! Contains core information about one dimensional rules, custom tabulated,
//! Chebyshev and Gaussian rules. Also, generic I/O data.

/*!
 * \internal
 * \ingroup TasmanianSG
 * \addtogroup TasmanianCoreOneDimensional Core meta-data for Gauuss, Chebyshev and custom rules
 *
 * \par Core One Dimensional Rules
 * Contains information about all one dimensional rules, number of nodes, exactness, IO meta-data.
 * The CustomTabulated class for managing a user provided rule is also included,
 * as well as methods for generating Gaussian and Chebyshev nodes and weights.
 */

namespace TasGrid{

//! \brief Class providing manipulation of custom tabulated rules, file I/O and structured access to the points, weights, and meta-data.
//! \ingroup TasmanianCoreOneDimensional
class CustomTabulated{
public:
    //! \brief Default constructor, create an empty table, need to read from file before any other data can be accessed.
    CustomTabulated();
    //! \brief Destructor, clear the rule.
    ~CustomTabulated();

    //! \brief Write to an already open ASCII/binary file, used in conjunction with \b GlobalGrid::write()
    template<bool useAscii> void write(std::ostream &os) const;
    //! \brief Read from an already open ASCII/binary file, used in conjunction with \b GlobalGrid::read()
    template<bool useAscii> void read(std::istream &is);

    //! \brief Read from a custom user provided ASCII file, see the file-format section.
    void read(const char* filename);

    //! \brief Returns the number of loaded levels.
    int getNumLevels() const;
    //! \brief Returns the number of points associated with the selected \b level.
    int getNumPoints(int level) const;
    //! \brief Return the exactness of the interpolation rule at \b level, usually one less than the number of points.
    int getIExact(int level) const;
    //! \brief Return the exactness of the integration/quadrature rule at \b level, provided by the user in the custom file.
    int getQExact(int level) const;

    //! \brief Get the points \b x and quadrature weights \b w associated with the rule at the \b level.
    void getWeightsNodes(int level, std::vector<double> &w, std::vector<double> &x) const;
    //! \brief Returns the user provided human readable description string.
    const char* getDescription() const;

protected:
    //! \brief Clear the number of levels.
    void reset();

private:
    int num_levels;
    std::vector<int> num_nodes;
    std::vector<int> precision;

    std::vector<std::vector<double>> nodes;
    std::vector<std::vector<double>> weights;
    std::string description;
};

//! \internal
//! \brief Metadata for one dimensional rules, number of points, exactness, I/O, etc.
//! \ingroup TasmanianCoreOneDimensional
namespace OneDimensionalMeta{
    //! \brief Return the number of points for the \b rule at the \b level, includes all global rules.
    int getNumPoints(int level, TypeOneDRule rule);
    //! \brief Return the exactness of the interpolation \b rule at the \b level, includes all global rules.
    int getIExact(int level, TypeOneDRule rule);
    //! \brief Return the exactness of the integration/quadrature \b rule at the \b level, includes all global rules.
    int getQExact(int level, TypeOneDRule rule);

    //! \brief Return \b True if the \b rule does not have nested nodes, e.g., gauss-legendre.
    bool isNonNested(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule is nested and has single-node growth, fit for \b GridSequence, e.g., leja.
    bool isSequence(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule has basis with Lagrange polynomials with global support, fit for \b GridGlobal, e.g., clenshaw-curtis.
    bool isGlobal(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule grows by one point per level.
    bool isSingleNodeGrowth(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule has polynomial basis with local support, fit for \b GridLocalPolynomial, e.g., localp.
    bool isLocalPolynomial(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule has wavelet basis with local support.
    bool isWavelet(TypeOneDRule rule);
    //! \brief Return \b True if the \b rule has trigonometric basis.
    bool isFourier(TypeOneDRule rule);

    //! \brief Map the enumerate to a human readable string, used in \b printStats().
    const char* getHumanString(TypeOneDRule rule);

    //! \brief Identifies the general contour, linear \b type_level, log-corrected \b type_curved, or hyperbolic \b type_hyperbolic.

    //! The types of index selection are divided into contour type and selection type.
    //! All types are combination between the two, i.e., type_iptotal uses interpolation polynomial selection and linear contour.
    //! This function reduces the type to one of the three level contours, which simplifies the if-statements in many places.
    //! Note: the tensor rules are a special case and are not considered in this function.
    inline TypeDepth getControurType(TypeDepth type){
        if ((type == type_level) || (type == type_iptotal) || (type == type_qptotal)){
            return type_level;
        }else if ((type == type_curved) || (type == type_ipcurved) || (type == type_qpcurved)){
            return type_curved;
        }else{
            return type_hyperbolic;
        }
    }

    //! \brief Returns true if the \b type indicates exactness with respect to raw levels.
    inline bool isExactLevel(TypeDepth type){
        return (type == type_level) || (type == type_curved) || (type == type_hyperbolic) || (type == type_tensor);
    }

    //! \brief Returns true if the \b type indicates exactness with respect to interpolation.
    inline bool isExactInterpolation(TypeDepth type){
        return (type == type_iptotal) || (type == type_ipcurved) || (type == type_iphyperbolic) || (type == type_iptensor);
    }

    //! \brief Returns true if the \b type indicates exactness with respect to integration.
    inline bool isExactQuadrature(TypeDepth type){
        return (type == type_qptotal) || (type == type_qpcurved) || (type == type_qphyperbolic) || (type == type_qptensor);
    }

    //! \brief Identifies the selection type, level \b type_level, interpolation \b type_iptotal, or quadrature \b type_qptotal.

    //! Similar to \b getControurType(), return what selection is considered, simple levels or interpolation/quadrature polynomial space.
    inline TypeDepth getSelectionType(TypeDepth type){
        if ((type == type_level) || (type == type_curved) || (type == type_hyperbolic)){
            return type_level;
        }else if ((type == type_iptotal) || (type == type_ipcurved) || (type == type_iphyperbolic)){
            return type_iptotal;
        }else{
            return type_qptotal;
        }
    }

    //! \brief Return \b True if the multi-index selection type has log-correction term (need to use floating point indexing).
    inline bool isTypeCurved(TypeDepth type){ return (getControurType(type) == type_curved); }

    //! \brief Map the string to the enumerate hierarchical refinement strategy, used in command line and Python.
    TypeRefinement getIOTypeRefinementString(const char *name);
    //! \brief Creates a map with \b std::string rule names (used by C/Python/CLI) mapped to \b TypeRefinement enums.
    std::map<std::string, TypeRefinement> getStringToRefinementMap();
    //! \brief Map the integer to the enumerate hierarchical refinement strategy, used in Fortran.
    TypeRefinement getIOTypeRefinementInt(int ref);
}

//! \internal
//! \brief Contains algorithms for generating Gauss,Chebyshev, and Fourier nodes and weights.
//! \ingroup TasmanianCoreOneDimensional
namespace OneDimensionalNodes{
    // non-nested rules
    //! \brief Generate Gauss-Legendre weights \b w and points \b x for (input) number of points \b m.
    void getGaussLegendre(int m, std::vector<double> &w, std::vector<double> &x);
    //! \brief Generate Chebyshev weights \b w and points \b x for (input) number of points \b m.
    void getChebyshev(int m, std::vector<double> &w, std::vector<double> &x);
    //! \brief Generate Gauss-Chebyshev type 1 weights \b w and points \b x for (input) number of points \b m.
    void getGaussChebyshev1(int m, std::vector<double> &w, std::vector<double> &x);
    //! \brief Generate Gauss-Chebyshev type 2 weights \b w and points \b x for (input) number of points \b m.
    void getGaussChebyshev2(int m, std::vector<double> &w, std::vector<double> &x);
    //! \brief Generate Gauss-Jacobi weights \b w and points \b x for (input) number of points \b m, using parameters \b alpha and \b beta.
    void getGaussJacobi(int m, std::vector<double> &w, std::vector<double> &x, double alpha, double beta);
    //! \brief Generate Gauss-Hermite weights \b w and points \b x for (input) number of points \b m, using parameters \b alpha
    void getGaussHermite(int m, std::vector<double> &w, std::vector<double> &x, double alpha);
    //! \brief Generate Gauss-Laguerre weights \b w and points \b x for (input) number of points \b m, using parameters \b alpha
    void getGaussLaguerre(int m, std::vector<double> &w, std::vector<double> &x, double alpha);

    // nested rules
    //! \brief Generate Clenshaw-Curtis \b nodes for the \b level.
    void getClenshawCurtisNodes(int level, std::vector<double> &nodes);
    //! \brief Return the Clenshaw-Curtis weight for the \b level and node indexed by \b point.
    double getClenshawCurtisWeight(int level, int point);

    //! \brief Generate Clenshaw-Curtis zero-boundary condition \b nodes for the \b level.
    void getClenshawCurtisNodesZero(int level, std::vector<double> &nodes); // assuming zero boundary
    //! \brief Return the Clenshaw-Curtis zero-boundary condition weight for the \b level and node indexed by \b point.
    double getClenshawCurtisWeightZero(int level, int point); // assuming zero boundary

    //! \brief Generate Fejer type 2 \b nodes for the \b level.
    void getFejer2Nodes(int level, std::vector<double> &nodes);
    //! \brief Return the Fejer type 2 weight for the \b level and node indexed by \b point.
    double getFejer2Weight(int level, int point);

    //! \brief Generate the first \b n R-Leja \b nodes, starting with 1, -1, 0, ...
    void getRLeja(int n, std::vector<double> &nodes);
    //! \brief Generate the first \b n R-Leja \b nodes, starting with 0, 1, -1, ...
    void getRLejaCentered(int n, std::vector<double> &nodes);
    //! \brief Generate the first \b n R-Leja \b nodes, starting with -0.5, 0.5, ...
    void getRLejaShifted(int n, std::vector<double> &nodes);

    //! \brief Generate the Fourier \b nodes for the given \b level, uniformly distributed points with right-most point omitted due to periodicity.
    void getFourierNodes(int level, std::vector<double> &nodes);
}

}

#endif
